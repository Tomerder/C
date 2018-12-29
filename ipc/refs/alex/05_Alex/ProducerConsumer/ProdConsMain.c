/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-15    
    Last modified date:		2013-09-15
    Description: producer-consumer - main thread
***************************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "GenericQueue.h"
#include "Producer.h"
#include "Consumer.h"


/*######################################################################*/
/*						DEFINITIONS	& FOREWARD DEC.						*/
/*######################################################################*/

#define CHECK0(X,S,G)		if( (X) != 0 ) { fprintf(stderr,S); fprintf(stderr,": %d\n", __LINE__); goto G; }
#define CHECKNOT0(X,S,G)	if( !(X) ) { fprintf(stderr,S); fprintf(stderr,": %d\n", __LINE__); goto G; }

#define MAX_ITEMS		100

#define NITEMS_PROD		5
#define QUEUE_CAPAC		5
#define NPROD			2
#define NCONS			2
#define WORKPROD		100000
#define WORKCONS		100000

#define THOUSAND		1000


typedef struct Options_t
{
	int		m_nItemsProd;	/* # of items per producer */
	int		m_qCapacity;	/* capacity of queue */
	int		m_nProducers;
	int		m_nConsumers;
	int		m_pWork;		/* time for producer work (msec) */
	int		m_cWork;		/* time for consumer work (msec) */
} Options;


typedef struct Params_t
{
	Producer*	m_prod;
	Consumer*	m_cons;
	int			m_toProduce;
	int			m_work;
} Params;


void*	ProducerThread	(void* _prod);
void*	ConsumerThread	(void* _cons);

void 	OptHandler	(int _argc, char *_argv[], Options *_options);


/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char *argv[])
{
	Producer*	producer	= NULL;
	Consumer*	consumer	= NULL;
	Queue*		queue		= NULL;
	pthread_t *prodThreads = NULL, *consThreads = NULL;
	
	pthread_mutex_t 	mutex;
	pthread_mutexattr_t mutexAttr;
	pthread_cond_t 		cond;
	
	Options 	options;
	Params 		paramsProd, paramsCons;
	int 		i, result;
	

	/****** initialize ******/
	
	/* runtime options */
	OptHandler(argc, argv, &options);
	
	/* threads */
	prodThreads = (pthread_t*)malloc( options.m_nProducers * sizeof(pthread_t) );
	CHECKNOT0(prodThreads, "malloc1", MALLOC_PROD_FAILED);
	
	consThreads = (pthread_t*)malloc( options.m_nConsumers * sizeof(pthread_t) );
	CHECKNOT0(prodThreads, "malloc2", MALLOC_CONS_FAILED);
	
	/* queue */
	queue = QueueCreate( options.m_qCapacity );
	CHECKNOT0(queue, "QueueCreate", QUEUE_FAILED);
	
	/* mutex */
	pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_ERRORCHECK);
	
	result = pthread_mutex_init(&mutex, &mutexAttr);
	CHECK0(result, "mutex_init", MUTEX_FAILED);
	
	pthread_mutexattr_destroy(&mutexAttr);
	
	
	/* cond. var. */
	result = pthread_cond_init(&cond, 0);
	CHECK0(result, "cond_init", COND_FAILED);
	
	/* producer, consumer structs */
	producer = ProducerInit(queue, &mutex, &cond, options.m_qCapacity);
	CHECKNOT0(producer, "ProducerInit", PROD_INIT_FAILED);
	
	consumer = ConsumerInit(queue, &mutex, &cond, options.m_qCapacity);
	CHECKNOT0(consumer, "ConsumerInit", CONS_INIT_FAILED);
	
	/* producer, consumer thread parameters */
	
	paramsProd.m_prod		= producer;
	paramsProd.m_cons		= NULL;
	paramsProd.m_toProduce	= options.m_nItemsProd;
	paramsProd.m_work		= options.m_pWork;
	
	paramsCons.m_prod		= NULL;
	paramsCons.m_cons		= consumer;
	paramsCons.m_toProduce	= 0;
	paramsCons.m_work		= options.m_cWork;
	
	/* random # gen. */
	srand(time(0));
	
	
	/****** actual work starts here ******/

	/* create producers */
	for(i=0; i < options.m_nProducers; ++i)
	{
		pthread_create(prodThreads+i, 0, ProducerThread, &paramsProd);
	}
	
	printf("producers created\n");
	usleep(WORKPROD);
	
	
	/* create consumers */
	for(i=0; i < options.m_nConsumers; ++i)
	{
		pthread_create(consThreads+i, 0, ConsumerThread, &paramsCons);
	}
	
	printf("consumers created\n");
	
	/* join producers */
	for(i=0; i < options.m_nProducers; ++i)
	{
		pthread_join(prodThreads[i], 0);
	}
	
	/* raise done flag for consumers */
	 ConsumerDone(consumer); 
	
	printf("all producers done\n");
	
	/* join consumers */
	for(i=0; i < options.m_nConsumers; ++i)
	{
		pthread_join(consThreads[i], 0);
	}
	
	printf("Items produced: %d, items consumed: %d\n", ProducerGetTotal(producer), ConsumerGetTotal(consumer));
	
	/*** free everything ***/
	
	free(prodThreads);
	free(consThreads);
	QueueDestroy(queue);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	ProducerDestroy(producer);
	ConsumerDestroy(consumer);

	return 0;
	

/*** error handling ***/

CONS_INIT_FAILED:
	ProducerDestroy(producer);

PROD_INIT_FAILED:
	pthread_cond_destroy(&cond);

COND_FAILED:
	pthread_mutex_destroy(&mutex);

MUTEX_FAILED:
	QueueDestroy(queue);

QUEUE_FAILED:
	free(consThreads);

MALLOC_CONS_FAILED:
	free(prodThreads);

MALLOC_PROD_FAILED:
	return -1;
}
/*######################################################################*/


/*######################################################################*/
/*							Producer Thread Function					*/
/*######################################################################*/
void*	ProducerThread(void* _params)
{
	Producer* producer = ((Params*)_params)->m_prod;
	int total = ((Params*)_params)->m_toProduce;
	int work = ((Params*)_params)->m_work;
	int nProduced = 0;
	char items[MAX_ITEMS] = {0};
	int i;
	
	/* initizlize items */
	for(i=0; i<total; ++i) {
		items[i] = 'A'+i;
	}
	
	/* produce items */
	while( nProduced < total )
	{
		Produce(producer, (void*)&items[nProduced]);	
		printf("Producer produced: %c\n", items[nProduced]);
		++nProduced;
		
		usleep( rand()%work );
/*		usleep(work);*/
	}
	
	pthread_exit(0);
}
/*######################################################################*/


/*######################################################################*/
/*							Consumer Thread Function					*/
/*######################################################################*/
void*	ConsumerThread(void* _params)
{
	Consumer* consumer = ((Params*)_params)->m_cons;
	int work = ((Params*)_params)->m_work;
	char items[MAX_ITEMS] = {0};
	void* temp = NULL;
	int i = 0;
	
	while( 1 /*NULL != (temp = Consume(consumer)) */ )
	{
		temp = Consume(consumer);
		
printf("consThread: empty: %d, done: %d\n", ConsumerQueueIsEmpty(consumer), ConsumerIsDone(consumer) );
		
		if( ConsumerIsDone(consumer) && ConsumerQueueIsEmpty(consumer) ) {	/*** temp */
			break;
		}
		
		if( (int)temp > 0 && (int)temp < 256)
		{
			printf("ConsumerThread error\n");		
			pthread_exit(0);
		}
		
		items[i] = *(char*)temp;
		printf("Consumer consumed: %c\n", items[i]);
		++i;
		
		usleep( rand()%work );
/*		usleep(work);*/
	}
	
	pthread_exit(0);
	return NULL;
}
/*######################################################################*/



/*######################################################################*/
/* 							Options Handler								 */
/*######################################################################*/
void OptHandler(int _argc, char *_argv[], Options *_options)
{
	int opt;
	
	/* initialize all options to default */
	
	_options->m_nItemsProd	= NITEMS_PROD;	
	_options->m_qCapacity	= QUEUE_CAPAC;	
	_options->m_nProducers	= NPROD;
	_options->m_nConsumers	= NCONS;
	_options->m_pWork		= WORKPROD;	
	_options->m_cWork		= WORKCONS;	
		
	/* read flags */
	while ((opt = getopt(_argc, _argv, "i:q:p:c:s:t:")) != -1)
	{
		switch(opt)
		{
		case 'i':
			_options->m_nItemsProd = atoi(optarg);
			break;
	
		case 'q':
			_options->m_qCapacity = atoi(optarg);
			break;
		
		case 'p':
			_options->m_nProducers = atoi(optarg);
			break;
		
		case 'c':
			_options->m_nConsumers = atoi(optarg);
			break;
		
		case 's':
			_options->m_pWork = atoi(optarg)*THOUSAND;
			break;
		
		case 't':
			_options->m_cWork = atoi(optarg)*THOUSAND;
			break;
		
		default:
			printf("Usage: -i [int] -q [int] -p [int] -c [int] -s [msec] -t [msec]\n");
			exit(1);
		}
	}
}
/*######################################################################*/



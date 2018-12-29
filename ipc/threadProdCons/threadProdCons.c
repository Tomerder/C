/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 12.02.14                                                                                              
*    LAST MODIFIED: 12.02.14                                                                                  
*    DESCRIPTION: Thread - producer consumer 
*****************************************************************************************************************/
#define _XOPEN_SOURCE 500
/*-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

/*-----------------------------------------------------------------*/

#include "queue.h"

/*-----------------------------------------------------------------*/

enum{FALSE , TRUE};

enum{OK_E, ALOC_ERR , SEM_INIT_ERR , THREAD_JOIN_ERR};

#define MSG_SIZE 64

#define TXT_SND "message num. %d => sender tid %u"

#define DEFAULT_N_OF_PROD 3
#define DEFAULT_N_OF_CONS 3
#define DEFAULT_BOX_SIZE 10
#define DEFAULT_N_OF_LOOPS 10
#define DEFAULT_WORK_TIME 100000
#define MICROSEC_MUL_RES 1000

#define CHKERR(ERR) if(ERR != OK_E) { printf("ERROR : %d\n",ERR); return ERR; }

enum{NOT_SHAREABLE, SHAREABLE};

/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_numOfProd;	
	int m_numOfCons;
	int m_boxSize;
	int m_numOfLoops;
	int m_workTime;
}Args;

/*-----------------------------------------------------------------*/

void GetArgs(int argc ,char** argv);

int Init(Queue** _queue, pthread_t** _threadsProd, pthread_t** _threadsCons);

void Clean(Queue* _queue, pthread_t* _threadsProd, pthread_t* _threadsCons);

int StartWork(Queue* _queue, pthread_t* _threadsProd, pthread_t* _threadsCons);

int WaitFinishWork(Queue* _queue, pthread_t* _threadsProd, pthread_t* _threadsCons);

void* cons(void* _parm);
void* prod(void* _parm);

/*-----------------------------------------------------------------*/

Args args = {DEFAULT_N_OF_PROD, DEFAULT_N_OF_CONS , DEFAULT_BOX_SIZE, DEFAULT_N_OF_LOOPS, DEFAULT_WORK_TIME} ;

/*-----------------------------------------------------------------*/
/*
typedef struct ThreadParams{
	Queue* m_queue;
}ThreadParams;
*/
/*-----------------------------------------------------------------*/

pthread_mutex_t mutex;

sem_t emptySem;  
sem_t fullSem;  

int flagProdsFinished = FALSE;
int isFirstConsFinished = FALSE;

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int err;
	Queue* queue = NULL;
	pthread_t *threadsProd = NULL , *threadsCons = NULL;
	
	GetArgs(argc ,argv);
	
	/*init*/
	err = Init(&queue, &threadsProd , &threadsCons );
	CHKERR(err);
    
    /*start prods and cons*/
    err = StartWork(queue ,threadsProd, threadsCons);
    CHKERR(err);
    
    /*wait for threads to finish the job*/
    err = WaitFinishWork(queue ,threadsProd, threadsCons);
    CHKERR(err);
    
    /*clean*/
    Clean(queue, threadsProd, threadsCons);	
		
	printf("PROG ENDED\n");
	
	return OK_E;
}

/*-----------------------------------------------------------------*/

int StartWork(Queue* _queue, pthread_t* _threadsProd, pthread_t* _threadsCons)
{
	int i;
	
	srand (time(NULL));
	
	/*start prods*/
	for(i=0; i < args.m_numOfProd; i++){
		 pthread_create(&_threadsProd[i], NULL, prod, (void*)_queue);   
	}
	
	/*start cons*/
	for(i=0; i < args.m_numOfCons; i++){
		 pthread_create(&_threadsCons[i], NULL, cons, (void*)_queue);   
	}

	return OK_E;
}

/*-----------------------------------------------------------------*/

int WaitFinishWork(Queue* _queue, pthread_t* _threadsProd, pthread_t* _threadsCons)
{
	int i , numOfReadMsgs;
	
	/*wait for prods to finish writing*/
	for(i=0; i < args.m_numOfProd; i++){
		if(pthread_join(_threadsProd[i], NULL)) {
			return THREAD_JOIN_ERR;
		}	
	}
	
	flagProdsFinished = TRUE;
	
	/*wait for prods to finish writing*/
	for(i=0; i < args.m_numOfCons; i++){
		if(pthread_join(_threadsCons[i], (void**)&numOfReadMsgs)) {
			return THREAD_JOIN_ERR;
		}	
		
		printf("thread %u => read %d msgs\n", (unsigned)_threadsCons[i] , numOfReadMsgs);
	}

	return OK_E;
}

/*-----------------------------------------------------------------*/

int Init(Queue** _queue, pthread_t** _threadsProd, pthread_t** _threadsCons)
{
	int err;

	/*create queue*/
	*_queue = QueueCreate(args.m_boxSize);	
	if (*_queue == NULL){
		return ALOC_ERR;
	}
	
	/*init arr of prods*/
	*_threadsProd = malloc( args.m_numOfProd * sizeof(_threadsProd) );
	if (*_threadsProd == NULL){
		return ALOC_ERR;
	}
		
	/*init arr of cons*/
	*_threadsCons = malloc( args.m_numOfCons * sizeof(_threadsProd) );
	if (*_threadsCons == NULL){
		return ALOC_ERR;
	}
	
	/*init sems*/
	err = sem_init(&fullSem, NOT_SHAREABLE, args.m_boxSize);
    if(err){
    	return SEM_INIT_ERR;
    } 
    
    err = sem_init(&emptySem, NOT_SHAREABLE, 0);
    if(err){
    	return SEM_INIT_ERR;
    } 
    
     /* initialize mutex */
    pthread_mutex_init(&mutex, NULL);                 
		
	return OK_E;
}

/*-----------------------------------------------------------------*/

void Clean(Queue* _queue, pthread_t* _threadsProd, pthread_t* _threadsCons)
{
	QueueDestroy(_queue);
	
	free(_threadsProd);
	
	free(_threadsCons);
	
	pthread_mutex_destroy(&mutex);
	
	sem_destroy(&fullSem);

	sem_destroy(&emptySem);
}

/*-----------------------------------------------------------------*/

void* prod(void* _parm)
{
	int i;
	Queue* queue = (Queue*)_parm;
	char* msg;
	unsigned tid = (unsigned)pthread_self();
	int numOfLoops = rand() % args.m_numOfLoops + 1;   
	
	printf("thread %u => writing %d msgs\n\n", tid , numOfLoops);	
	
	for(i=0; i < numOfLoops; i++)
	{
		/*work*/
		usleep(args.m_workTime); 
		
		/*allocate msg on heap and write msg*/
		msg = malloc(sizeof(char) * MSG_SIZE);
		sprintf( msg , TXT_SND , i + 1 , tid);                   
        printf("writing: %s\n\n",  msg);	
        
        /*------------------------*/
        sem_wait(&fullSem);
        pthread_mutex_lock(&mutex);
        /*------------------------*/
        /*push to queue*/
        QueueInsert(queue, msg);
        /*------------------------*/
        pthread_mutex_unlock(&mutex);
        sem_post(&emptySem);
        /*------------------------*/              
	}
	
	return (void*)OK_E;
}

/*-----------------------------------------------------------------*/

void* cons(void* _parm)
{
	int numOfReadMsgs = 0;
	Queue* queue = (Queue*)_parm;
	char* msg;
	unsigned int tid = (unsigned int)pthread_self();
	
	while(TRUE)
	{
		/*first consumer exit point*/
		if(flagProdsFinished && IsQueueEmpty(queue)){
			isFirstConsFinished = TRUE;
			sem_post(&emptySem);        /*let next consumer exit*/
			return (void*)numOfReadMsgs;
		}
	
		/*------------------------*/
        sem_wait(&emptySem);
        pthread_mutex_lock(&mutex);
        /*------------------------*/
        /*push to queue*/
        QueueRemove(queue, (void**)&msg);
        /*------------------------*/
        pthread_mutex_unlock(&mutex);
        sem_post(&fullSem);
        /*------------------------*/       
	
		/*all other consumers exit point*/
		if(isFirstConsFinished){
			sem_post(&emptySem);        /*let next consumer exit*/
			return (void*)numOfReadMsgs;
		}
				
		++numOfReadMsgs;
		printf("reading: %s => receiver : %u\n\n",  msg , tid);	
		free(msg);
	
		/*work*/
		usleep(args.m_workTime); 
	}
	
	return (void*)OK_E;
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "p:c:n:l:s:")) != -1)
	{
		switch(argType)
		{
			case 'p':
				args.m_numOfProd = atoi(optarg);
				break;	
			case 'c':
				args.m_numOfCons = atoi(optarg);
				break;	
			case 'n':
				args.m_boxSize = atoi(optarg) ;
				break;	
			case 'l':
				args.m_numOfLoops = atoi(optarg);
				break;					
			case 's':
				args.m_workTime = atoi(optarg) * MICROSEC_MUL_RES;
				break;	
			default:				
				break;
		}
	}	
}

/*-----------------------------------------------------------------*/

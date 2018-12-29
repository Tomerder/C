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
#include <time.h>
#include <assert.h>

/*-----------------------------------------------------------------*/

#include "prod.h"

/*-----------------------------------------------------------------*/

enum{FALSE , TRUE};

#define WORK_TIME 100000

#define CHKERR(ERR) if(ERR != OK_E) { printf("ERROR\n"); return ERR; }

/*-----------------------------------------------------------------*/

struct Produce_t{
	int m_maxQueueSize;	
	int m_exitFlag;
	int m_numOfProd;	
	Queue* m_queue;
	pthread_mutex_t* m_mutex;
	pthread_cond_t* m_cond;
};

/*-----------------------------------------------------------------*/

Produce_t* ProduceInit(Queue* _queue, pthread_mutex_t* _mutex, pthread_cond_t* _cond, size_t _maxQueueSize)
{
	Produce_t* prod = NULL;

	assert(_queue);
	assert(_mutex);		
	assert(_cond);
	assert(_maxQueueSize);		

	prod = malloc(sizeof(Produce_t));               
	if(prod == NULL){
		return NULL;
	} 	
	
	prod->m_maxQueueSize = _maxQueueSize;
	prod->m_queue = _queue;
	prod->m_mutex = _mutex;		
	prod->m_cond = _cond;
	prod->m_exitFlag = FALSE;
	prod->m_numOfProd = 0;
		
	return prod;
}

/*-----------------------------------------------------------------*/

void ProduceDestroy(Produce_t* _produce)
{
	free(_produce);
}

/*-----------------------------------------------------------------*/

void* Produce(Produce_t* _produce, void* _item)
{
	int err;

	/*usleep(WORK_TIME);*/
	_produce->m_numOfProd++;

    /*--------------------------------------*/
	/*lock*/
	pthread_mutex_lock(_produce->m_mutex);
		
	while ( IsQueueFull(_produce->m_queue) && !(_produce->m_exitFlag)) 
	{
		/*wait for consumer*/
		err = pthread_cond_wait(_produce->m_cond, _produce->m_mutex);
		CHKERR((void*)err);
	}	
	
	/*insert item to queue*/
	QueueInsert(_produce->m_queue, _item);
	
	/*printf("INSERT : %s\n", (char*)_item);*/

	/*unlock*/
	pthread_mutex_unlock(_produce->m_mutex);
    /*--------------------------------------*/
      
   	/*signal to consumer*/
    err = pthread_cond_signal( _produce->m_cond); 
    CHKERR((void*)err);
    
	_produce->m_numOfProd--;

	return (void*)OK_E;
}

/*-----------------------------------------------------------------*/

Queue* ProdGetQueue(Produce_t* _produce)
{
	return _produce->m_queue;
}


pthread_mutex_t* ProdGetMutex(Produce_t* _produce)
{
	return _produce->m_mutex;
}

/*-----------------------------------------------------------------*/

int ProdExit(Produce_t* _produce)
{
	_produce->m_exitFlag = TRUE;
	return pthread_cond_signal( _produce->m_cond);
}

/*-----------------------------------------------------------------*/

int ProdGetNum(Produce_t* _produce)
{
	return _produce->m_numOfProd;
}

/*-----------------------------------------------------------------*/





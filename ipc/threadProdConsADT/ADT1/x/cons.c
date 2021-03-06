/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 12.02.14                                                                                              
*    LAST MODIFIED: 12.02.14                                                                                  
*    DESCRIPTION: Thread - consumer consumer 
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

#include "cons.h"

/*-----------------------------------------------------------------*/

enum{FALSE , TRUE};

#define WORK_TIME 100000

#define CHKERR(ERR) if(ERR != OK_E) { printf("ERROR\n"); return ERR; }

/*-----------------------------------------------------------------*/

struct Consume_t{
	int m_maxQueueSize;	
	int m_exitFlag;
	int m_numOfCons;	
	Queue* m_queue;
	pthread_mutex_t* m_mutex;
	pthread_cond_t* m_cond;
};

/*-----------------------------------------------------------------*/

Consume_t* ConsumeInit(Queue* _queue, pthread_mutex_t* _mutex, pthread_cond_t* _cond, size_t _maxQueueSize)
{
	Consume_t* cons = NULL;

	assert(_queue);
	assert(_mutex);		
	assert(_cond);
	assert(_maxQueueSize);		

	cons = malloc(sizeof(Consume_t));               
	if(cons == NULL){
		return NULL;
	} 	
	
	cons->m_maxQueueSize = _maxQueueSize;
	cons->m_queue = _queue;
	cons->m_mutex = _mutex;		
	cons->m_cond = _cond;
	cons->m_exitFlag = FALSE;
	cons->m_numOfCons = 0;
	
	return cons;
}

/*-----------------------------------------------------------------*/

void ConsumeDestroy(Consume_t* _consume)
{
	free(_consume);
}

/*-----------------------------------------------------------------*/

void* Consume(Consume_t* _consume)
{
	int err;
	void* removedItem;
    
	_consume->m_numOfCons++;

    /*--------------------------------------*/
	/*lock*/
	pthread_mutex_lock(_consume->m_mutex);
		
	while ( IsQueueEmpty(_consume->m_queue) && !(_consume->m_exitFlag) ) 
	{
		/*wait for consumer*/
		err = pthread_cond_wait(_consume->m_cond, _consume->m_mutex);
		CHKERR((void*)err);
	}	
	
	/*remove item from queue*/
	QueueRemove(_consume->m_queue, &removedItem);
	
	/*printf("REMOVE : %s\n", (char*)removedItem);*/

	/*unlock*/
	pthread_mutex_unlock(_consume->m_mutex);
    /*--------------------------------------*/
    
   	/*signal to producer*/
    err = pthread_cond_signal( _consume->m_cond); 
   	CHKERR((void*)err);
    
	/*usleep(WORK_TIME);*/
	_consume->m_numOfCons--;

	return removedItem;
}

/*-----------------------------------------------------------------*/

pthread_mutex_t* ConsGetMutex(Consume_t* _consume)
{
	return _consume->m_mutex;
}

/*-----------------------------------------------------------------*/

int ConsExit(Consume_t* _consume)
{
	_consume->m_exitFlag = TRUE;
	return pthread_cond_signal( _consume->m_cond);
}

/*-----------------------------------------------------------------*/

int ConsGetNum(Consume_t* _consume)
{
	return _consume->m_numOfCons;
}

/*-----------------------------------------------------------------*/

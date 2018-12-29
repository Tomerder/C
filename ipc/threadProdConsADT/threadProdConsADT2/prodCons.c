/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 12.02.14                                                                                              
*    LAST MODIFIED: 12.02.14                                                                                  
*    DESCRIPTION: Thread - producer consumer 
*****************************************************************************************************************/
#ifndef _XOPEN_SOURCE 
#define _XOPEN_SOURCE 500
#endif
/*-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>

/*-----------------------------------------------------------------*/

#include "prodCons.h"

/*-----------------------------------------------------------------*/

enum{FALSE , TRUE};

#define WORK_TIME 100000

#define CHKERR(ERR) if(ERR != OK_E) { printf("ERROR\n"); return ERR; }

/*-----------------------------------------------------------------*/

struct ProdCons{
	int m_exitFlag;
	int m_numOfProd;
	int m_numOfCons;		
	Queue* m_queue;
	pthread_mutex_t m_mutexProd;
	pthread_mutex_t m_mutexCons;
	pthread_cond_t m_condProd;
	pthread_cond_t m_condCons;
};

/*-----------------------------------------------------------------*/

ProdCons*	PCCreate (int _boxSize)
{
	ProdCons* prodCons = NULL;

	assert(_boxSize);		

	prodCons = malloc(sizeof(ProdCons));               
	if(prodCons == NULL){
		return NULL;
	} 	
	
	/*create queue*/
	prodCons->m_queue = QueueCreate(_boxSize);	
	if (prodCons->m_queue == NULL){
		free(prodCons);
		return NULL;
	}
	
	/* initialize mutex */
    pthread_mutex_init( &(prodCons->m_mutexProd), NULL); 
	pthread_mutex_init( &(prodCons->m_mutexCons), NULL); 
    
    /* initialize cond var */
    pthread_cond_init( &(prodCons->m_condProd) , NULL);
    pthread_cond_init( &(prodCons->m_condCons) , NULL);
	
	prodCons->m_exitFlag = FALSE;
	prodCons->m_numOfProd = 0;
	prodCons->m_numOfCons = 0;
		
	return prodCons;
}

/*-----------------------------------------------------------------*/

void PCDestroy(ProdCons* _prodCons)
{
	QueueDestroy( _prodCons->m_queue );

	pthread_mutex_destroy( &_prodCons->m_mutexProd);  
	pthread_mutex_destroy( &_prodCons->m_mutexCons);  

	free(_prodCons);
}

/*-----------------------------------------------------------------*/

void* PCProduceItem(ProdCons* _prodCons, void* _item)
{
	int err;
	int broadcastConsFlag = FALSE;

	_prodCons->m_numOfProd++;

    /*--------------------------------------*/
	/*lock*/
	err = pthread_mutex_lock( &_prodCons->m_mutexProd);
	CHKERR((void*)err);
		
	while ( IsQueueFull(_prodCons->m_queue) && !(_prodCons->m_exitFlag)) 
	{
		/*wait for cons to remove first item from full box*/
		err = pthread_cond_wait(&_prodCons->m_condProd, &_prodCons->m_mutexProd);
	}	
	
	/*if insert item to empty box -> broadcast to cons*/
	if(	IsQueueEmpty( _prodCons->m_queue)){
		broadcastConsFlag = TRUE;
	}
	
	/*insert item to queue*/
	QueueInsert( _prodCons->m_queue, _item);
	
	/*printf("INSERT : %s\n", (char*)_item);*/

	/*unlock*/
	err = pthread_mutex_unlock( &_prodCons->m_mutexProd);
	CHKERR((void*)err);
    /*--------------------------------------*/
     
     
    /*if item was inserted to empty box -> broadcast to cons*/  
    if(broadcastConsFlag){
    	err = pthread_cond_broadcast( &_prodCons->m_condCons); 
    	CHKERR((void*)err);
    }   
      
	_prodCons->m_numOfProd--;

	return (void*)OK_E;
}

/*-----------------------------------------------------------------*/

void* PCConsumeItem(ProdCons* _prodCons)
{
	Item itemRemoved;
	int err;
	int broadcastProdFlag = FALSE;

	_prodCons->m_numOfCons++;

    /*--------------------------------------*/
	/*lock*/
	err = pthread_mutex_lock(&_prodCons->m_mutexCons);
	CHKERR((void*)err);
		
	while ( IsQueueEmpty(_prodCons->m_queue) && !(_prodCons->m_exitFlag)) 
	{
		/*wait for prod to insert first item to empty box*/
		err = pthread_cond_wait( &_prodCons->m_condCons, &_prodCons->m_mutexCons);
	}	
	
	/*if remove item from full box -> broadcast to prod*/
	if(	IsQueueFull(_prodCons->m_queue)){
		broadcastProdFlag = TRUE;
	}
	
	/*remove item from queue*/
	QueueRemove( _prodCons->m_queue, &itemRemoved);
	
	/*printf("REMOVE : %s\n", (char*)itemRemoved);*/

	/*unlock*/
	err = pthread_mutex_unlock( &_prodCons->m_mutexCons);
	CHKERR((void*)err);
    /*--------------------------------------*/
     
     
    /*if item was inserted to empty box -> broadcast to cons*/  
    if(broadcastProdFlag){
    	err = pthread_cond_broadcast( &_prodCons->m_condProd); 
    	CHKERR((void*)err);
    }   
      
	_prodCons->m_numOfCons--;

	return itemRemoved;
}

/*-----------------------------------------------------------------*/

void PCSetShutdownMode(ProdCons* _prodCons)
{
	_prodCons->m_exitFlag = TRUE;
	pthread_cond_broadcast( &_prodCons->m_condProd);
	pthread_cond_broadcast( &_prodCons->m_condCons);
}

/*-----------------------------------------------------------------*/




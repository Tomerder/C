/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 16.02.14                                                                                              
*    LAST MODIFIED: 16.02.14                                                                                  
*    DESCRIPTION: Barrier
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

#include "barrier.h"

/*-----------------------------------------------------------------*/

enum{FALSE , TRUE};

#define WORK_TIME 100000

#define CHKERR(ERR , RET) if(ERR != OK_E) { printf("ERROR :%d\n", RET); return RET; }

#define DEBUG FALSE

#define PRINT_D(MSG)  if(DEBUG) printf(MSG);
#define PRINT_N_D(MSG,N)  if(DEBUG) printf(MSG,N);
#define PRINT_U_D(MSG,U)  if(DEBUG) printf(MSG,U);
#define PRINT_UN_D(MSG,U,N)   if(DEBUG) printf(MSG,U,N);

/*-----------------------------------------------------------------*/

struct Barrier{
	int m_capacity;
	int m_curNumOfWaiting;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
	pthread_mutex_t m_lock;
};

/*-----------------------------------------------------------------*/

Barrier* BarrierInit(int _capacity)
{
	Barrier* barrier = NULL;

	assert(_capacity);		

	barrier = malloc(sizeof(Barrier));               
	if(barrier == NULL){
		return NULL;
	} 	
	
	/* initialize mutex */
    pthread_mutex_init( &(barrier->m_mutex), NULL); 
    pthread_mutex_init( &(barrier->m_lock), NULL); 
	
    /* initialize cond var */
    pthread_cond_init( &(barrier->m_cond) , NULL);
	
	barrier->m_capacity = _capacity;
	barrier->m_curNumOfWaiting = 0;
		
	return barrier;
}

/*-----------------------------------------------------------------*/

void BarrierDestroy(Barrier* _bar)
{
	pthread_mutex_destroy( &_bar->m_mutex);
	pthread_mutex_destroy( &_bar->m_lock);  
  

	free(_bar);
}

/*-----------------------------------------------------------------*/

int BarrierWait(Barrier* _bar)
{
	int err;
	unsigned tid = pthread_self(); 

	PRINT_U_D("%u => barrier entered \n", tid);

	/*--------------------------------------*/
	/*lock - protecting _bar->m_curNumOfWaiting */
	err = pthread_mutex_lock( &_bar->m_lock);
	CHKERR(err, LOCK_ERR);
	
	_bar->m_curNumOfWaiting++;	

	PRINT_UN_D("%u => barrier entered :%d\n", tid, _bar->m_curNumOfWaiting);
	
	/*check if barrier should be released => broadcast*/  
    if(_bar->m_curNumOfWaiting == _bar->m_capacity){
		/*broadcast*/		
		PRINT_U_D("%u => BROADCAST\n", tid);
    	err = pthread_cond_broadcast( &_bar->m_cond); 
    	CHKERR(err, BROADCAST_ERR);
 		
		/*zeros counter*/
 		_bar->m_curNumOfWaiting = 0;	

		/*unlock*/
		err = pthread_mutex_unlock( &_bar->m_lock);
		CHKERR(err, LOCK_ERR);

    	return BARRIER_OPENED;
    }   

	/*unlock*/
	err = pthread_mutex_unlock( &_bar->m_lock);
	CHKERR(err, LOCK_ERR);
	/*--------------------------------------*/
	
	
	PRINT_U_D("%u => start barrier lock\n", tid);
    /*--------------------------------------*/
	/*lock - barrier*/
	err = pthread_mutex_lock( &_bar->m_mutex);
	CHKERR(err, LOCK_ERR);
	
	PRINT_U_D("%u => locked\n",tid);
			
	/*if ( _bar->m_curNumOfWaiting != _bar->m_capacity) */
	{
		PRINT_U_D("%u => while barrier not full\n",tid);
		/*wait for barrier to fill*/
		err = pthread_cond_wait(&_bar->m_cond, &_bar->m_mutex);
		if(err){
			pthread_mutex_unlock( &_bar->m_mutex);
			CHKERR(err, WAIT_ERR);
		}
	}	
	
	/*unlock - barrier*/
	err = pthread_mutex_unlock( &_bar->m_mutex);
	CHKERR(err, UNLOCK_ERR);
    /*--------------------------------------*/    
    PRINT_U_D("%u => exit barrier lock\n", tid); 


	return OK_E;
}

/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/




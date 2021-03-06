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

enum{ENTER , EXIT};

enum{FALSE , TRUE};

#define WORK_TIME 100000

#define CHKERR(ERR , RET) if(ERR != OK_E) { printf("ERROR :%d\n", RET); return RET; }

#define DEBUG TRUE

#define PRINT_D(MSG)  if(DEBUG) printf(MSG);
#define PRINT_N_D(MSG,N)  if(DEBUG) printf(MSG,N);
#define PRINT_U_D(MSG,U)  if(DEBUG) printf(MSG,U);
#define PRINT_UN_D(MSG,U,N)   if(DEBUG) printf(MSG,U,N);
#define PRINT_UNN_D(MSG,U,N,N2)   if(DEBUG) printf(MSG,U,N,N2);

/*-----------------------------------------------------------------*/

struct Barrier{
	pthread_mutex_t m_mutexEnter;
	pthread_cond_t m_condEnter;
	pthread_mutex_t m_mutexExit;
	pthread_cond_t m_condExit;
	int m_capacity;
	int m_countIn;
	int m_countOut;
	int m_openGate;	
	int m_curColor;	
	int m_totalCount;	
};

/*-----------------------------------------------------------------*/

static int EnterGate(Barrier* _bar);
static int ExitGate(Barrier* _bar);

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
    pthread_mutex_init( &(barrier->m_mutexEnter), NULL); 
    pthread_mutex_init( &(barrier->m_mutexExit), NULL); 
	
    /* initialize cond var */
    pthread_cond_init( &(barrier->m_condEnter) , NULL);
    pthread_cond_init( &(barrier->m_condExit) , NULL);
	
	barrier->m_capacity = _capacity;	
	barrier->m_countIn = 0;
	barrier->m_countOut = 0;	
	barrier->m_openGate = ENTER;	
	barrier->m_curColor = 0;
	barrier->m_totalCount = 0;
		
	return barrier;
}

/*-----------------------------------------------------------------*/

void BarrierDestroy(Barrier* _bar)
{
	pthread_mutex_destroy( &(_bar->m_mutexEnter));
	pthread_mutex_destroy( &(_bar->m_mutexExit));  
  
  	pthread_cond_destroy( &(_bar->m_condEnter));
  	pthread_cond_destroy( &(_bar->m_condExit));

	free(_bar);
}

/*-----------------------------------------------------------------*/

int BarrierWait(Barrier* _bar, CallBack _CallBack, void* _param)
{
	int err;
	
	/*enter gate*/
	err = EnterGate(_bar);
	CHKERR(err, err);

	/*callback*/
	if(_CallBack){ 
		_CallBack(_param);
	}
	
	/*exit gate*/
	err = ExitGate(_bar);
	CHKERR(err, err);
 	
	return OK_E;
}

/*-----------------------------------------------------------------*/

static int EnterGate(Barrier* _bar)
{
	int err;
	unsigned tid = pthread_self(); 
	int color;

	PRINT_U_D("%u => ENTER EnterGate\n", tid);
    /*---------------------------*/
	/*lock - EnterGate*/
	err = pthread_mutex_lock( &_bar->m_mutexEnter);
	CHKERR(err, LOCK_ERR);
	
	/*current color*/
	color = _bar->m_totalCount / _bar->m_capacity; 
	_bar->m_totalCount++; 
		
	/*enter ++*/
	_bar->m_countIn++;	
	PRINT_UNN_D("%u => EnterGate - count :%d , color :%d********************\n", tid, _bar->m_countIn, color);			
		
	/*if barrier should be released => broadcast + open enter gate + close exit gate*/  	
	if(_bar->m_countIn == _bar->m_capacity){
		/*allow enter*/
		_bar->m_countIn = 0;
		_bar->m_openGate = EXIT;
		/*broadcast*/		
		PRINT_U_D("%u => BROADCAST EnterGate\n", tid);
    	err = pthread_cond_broadcast( &_bar->m_condEnter); 
    	CHKERR(err, BROADCAST_ERR);
	}
	else
	{	
		/*if color != current color => keep waiting */ 				
		while(_bar->m_openGate == ENTER)  /* || color > _bar->m_curColor) */
		{		
			/*wait for barrier to fill*/
			PRINT_U_D("%u => wait EnterGate\n",tid);
			err = pthread_cond_wait(&_bar->m_condEnter, &_bar->m_mutexEnter);
			if(err){
				pthread_mutex_unlock( &_bar->m_mutexEnter);
				CHKERR(err, WAIT_ERR);
			}	
		}		
	}		
			
	/*unlock - barrier*/
	err = pthread_mutex_unlock( &_bar->m_mutexEnter);
	CHKERR(err, UNLOCK_ERR);
    /*---------------------------*/  
    PRINT_UN_D("%u => EXIT EnterGate => color :%d ####################\n", tid ,color); 
    
    return OK_E;
}


/*-----------------------------------------------------------------*/

static int ExitGate(Barrier* _bar)
{
	int err;
	unsigned tid = pthread_self(); 

	PRINT_U_D("%u => ENTER ExitGate\n", tid);
    /*---------------------------*/
	/*lock - barrier*/
	err = pthread_mutex_lock( &_bar->m_mutexExit);
	CHKERR(err, LOCK_ERR);
		
	/*exit ++*/
	_bar->m_countOut++;	
	PRINT_UN_D("%u => ExitGate - count :%d\n", tid, _bar->m_countOut);			
		
	/*if barrier should be released => broadcast + open exit gate + close enter gate*/  	
	if(_bar->m_countOut == _bar->m_capacity){
		/*allow Exit*/
		_bar->m_countOut = 0;
		_bar->m_openGate = ENTER;
		/*broadcast*/		
		PRINT_U_D("%u => BROADCAST ExitGate\n", tid);
    	err = pthread_cond_broadcast( &_bar->m_condExit); 
    	CHKERR(err, BROADCAST_ERR);
		/*to next color*/
		_bar->m_curColor++; 
		PRINT_U_D("color changed : %d &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n", _bar->m_curColor );
	}
	else
	{					
		while(_bar->m_openGate == EXIT){
			/*wait for barrier to fill*/
			PRINT_U_D("%u => wait ExitGate\n",tid);
			err = pthread_cond_wait(&_bar->m_condExit, &_bar->m_mutexExit);
			if(err){
				pthread_mutex_unlock( &_bar->m_mutexExit);
				CHKERR(err, WAIT_ERR);
			}				
		}		
	}		
		
	/*unlock - barrier*/
	err = pthread_mutex_unlock( &_bar->m_mutexExit);
	CHKERR(err, UNLOCK_ERR);
    /*---------------------------*/
    PRINT_U_D("%u => EXIT ExitGate\n", tid); 
    
    return OK_E;
}

/*-----------------------------------------------------------------*/




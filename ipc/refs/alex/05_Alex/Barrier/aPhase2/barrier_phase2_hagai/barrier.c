/**************************************************************************************
    Author: Hagai Moshe
    Creation date :      
    Last modified date:  
    Description : .
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>        /*-pthread*/
#include "barrier.h"



#define CHECK_INIT(X) 	if (X){							\
							return(0); 		            \
						} 

#define CHECK(X) 		if (X){ 		                \
							return(X); 		            \
						} 

#define VERBOSE(X) 		if(options.m_verbose){	    	\
							printf("%s\n", (X));    	\
						}



struct Barrier_t
{
	pthread_mutex_t		m_mutex;
	pthread_cond_t		m_condVarIn;
	pthread_cond_t		m_condVarOut;
	int					m_gatesLock;/*a key that control both of the gates*/
	int 				m_countIn;	/*num Of Threads currently on wait */
	int 				m_countOut;	/*num Of Threads currently on wait */	
	int 				m_capacity;	/*max num Of Threads on wait*/
};/*Barrier_t*/



static int				BarrierGateOut(Barrier* _barrier);
static int				BarrierGateIn(Barrier* _barrier);


Barrier*		BarrierInit( size_t _numOfThreads )
{
	int ret;
	Barrier* barrier;
	
	barrier = (Barrier*)calloc(1,sizeof(Barrier));
	CHECK_INIT(!barrier);
	
	barrier->m_capacity		= _numOfThreads;
	barrier->m_countIn 		= 0 ;
	barrier->m_countOut 	= 0 ;
		/*gateslock is set to open gateOut and close gateIn */
	barrier->m_gatesLock	= 0 ;
	
	ret = pthread_mutex_init(&barrier->m_mutex,NULL);
	CHECK_INIT(ret);
	
	ret = pthread_cond_init(&barrier->m_condVarIn,NULL);
	CHECK_INIT(ret);
	ret = pthread_cond_init(&barrier->m_condVarOut,NULL);
	CHECK_INIT(ret);

	return barrier;
}/*BarrierInit*/




void			BarrierDestroy(Barrier* _barrier)
{
	free(_barrier);

}/*BarrierDestroy*/




int				BarrierWait(Barrier* _barrier, CallBack _CallBack , void* _params)
{
	int ret;
		
		/*first gate*/
	ret = BarrierGateIn( _barrier);
	CHECK(ret);	
	
		/*CallBack*/


		/*second gate*/
	ret = BarrierGateOut( _barrier);
	CHECK(ret);

	return 0;
}/*BarrierWait*/






static int				BarrierGateIn(Barrier* _barrier)
{
	int ret;
	
    ret = pthread_mutex_lock(&_barrier->m_mutex);	
    CHECK(ret);
		/* keep count of num Of Threads currently on wait */
	++ _barrier->m_countIn;
	
    	/*if max num Of Threads has been reached awakening all threads, else thread is put to wait*/
	if(_barrier->m_countIn == _barrier->m_capacity ){
		ret =  pthread_cond_broadcast(&_barrier->m_condVarIn);
		CHECK(ret);
		/*reset barrier */
		_barrier->m_countIn = 0;
		/*opening gateIn and closing gateOut*/
		++ _barrier->m_gatesLock;
	}
	else{
		while( ! _barrier->m_gatesLock ){
		    ret = pthread_cond_wait (&_barrier->m_condVarIn, &_barrier->m_mutex); 
		    CHECK(ret);
		}
	}

    ret = pthread_mutex_unlock(&_barrier->m_mutex);
	CHECK(ret);
	return 0;
}/*BarrierGateIn*/







static int				BarrierGateOut(Barrier* _barrier)
{
	int ret;
	
    ret = pthread_mutex_lock(&_barrier->m_mutex);	
    CHECK(ret);
		/* keep count of num Of Threads currently on wait */
	++ _barrier->m_countOut;
	
    	/*if max num Of Threads has been reached awakening all threads, else thread is put to wait*/
	if(_barrier->m_countOut == _barrier->m_capacity ){
		ret =  pthread_cond_broadcast(&_barrier->m_condVarOut);
		CHECK(ret);
		/*reset barrier */
		_barrier->m_countOut = 0;
		/*closing gateIn and opening gateOut*/
		-- _barrier->m_gatesLock;		
	}
	else{
		while( _barrier->m_gatesLock ){
		    ret = pthread_cond_wait (&_barrier->m_condVarOut, &_barrier->m_mutex); 
		    CHECK(ret);
		}
	}

    ret = pthread_mutex_unlock(&_barrier->m_mutex);
	CHECK(ret);
	return 0;
}/*BarrierGateOut*/











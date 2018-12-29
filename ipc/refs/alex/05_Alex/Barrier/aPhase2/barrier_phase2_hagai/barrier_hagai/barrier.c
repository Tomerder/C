/**************************************************************************************
    Author: Hagai Moshe
    Creation date :      
    Last modified date:  
    Description : .
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         /*getopt*/
#include <string.h>
#include <pthread.h>        /*-pthread*/
#include "barrier.h"



#define CHECK_INIT(X) if (X){							\
							return(0); 		            \
						} 

#define CHECK(X) 		if (X){ 		                \
							printf("%d\n", __LINE__); return(X); 		            \
						} 

#define VERBOSE(X) 		if(options.m_verbose){	    	\
							printf("%s\n", (X));    	\
						}



struct Barrier_t
{
	pthread_mutex_t		m_mutex;
	pthread_cond_t		m_condVar1;
	int 				m_count;	/*num Of Threads currently on wait */
	int 				m_capacity;	/*max num Of Threads on wait*/
};/*Barrier_t*/







Barrier*		BarrierInit( size_t _numOfThreads )
{
	int ret;
	Barrier* barrier;
	
	barrier = (Barrier*)calloc(1,sizeof(Barrier));
	CHECK_INIT(!barrier);
	
	barrier->m_count 		= 0 ;
	barrier->m_capacity		= _numOfThreads;
	
	ret = pthread_mutex_init(&barrier->m_mutex,NULL);
	CHECK_INIT(ret);
	
	ret = pthread_cond_init(&barrier->m_condVar1,NULL);
	CHECK_INIT(ret);

	return barrier;
}/*BarrierInit*/




void			BarrierDestroy(Barrier* _barrier)
{
	free(_barrier);

}/*BarrierDestroy*/




int				BarrierWait(Barrier* _barrier)
{
	int ret;
	
    ret = pthread_mutex_lock(&_barrier->m_mutex);	
    CHECK(ret);
		/* keep count of num Of Threads currently on wait */
	++ _barrier->m_count;
	
    	/*if max num Of Threads has been reached awakening all threads, else thread is put to wait*/
	if(_barrier->m_count == _barrier->m_capacity ){
		ret =  pthread_cond_broadcast(&_barrier->m_condVar1);
		CHECK(ret);
		/*reset barrier */
		_barrier->m_count = 0;
	}
	else{
		while( _barrier->m_count ){
		    ret = pthread_cond_wait (&_barrier->m_condVar1, &_barrier->m_mutex); 
		    CHECK(ret);
		}
	}

    ret = pthread_mutex_unlock(&_barrier->m_mutex);
	CHECK(ret);
	return 0;
}/*BarrierWait*/





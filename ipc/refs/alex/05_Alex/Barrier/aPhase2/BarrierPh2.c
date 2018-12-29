/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-13    
    Last modified date:		2013-09-13
    Description: 	conditional variables
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

/*#include <unistd.h>*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#include "BarrierPh2.h"


struct Barrier_t
{
	int 			m_counter;
	int 			m_threshold;
	pthread_mutex_t	m_mutex;
	pthread_cond_t	m_cond;
};



/*######################################################################*/
/*							API FUNCTIONS								*/
/*######################################################################*/


/*######################################################################*/
/*							BARRIER INIT								*/
/*######################################################################*/
Barrier* BarrierInit(size_t _capacity)
{
	Barrier* bar;
	
	bar = (Barrier*)malloc(sizeof(Barrier));
	if(!bar) {
		goto ALLOC_FAIL;
	}
	
	bar->m_threshold = _capacity;
	bar->m_counter	 = 0;
	
	if( 0 != pthread_mutex_init(&bar->m_mutex, 0) )
	{
		perror("pthread_mutex_init");
		goto MUTEX_FAIL;
	}
	
	if( 0 != pthread_cond_init(&bar->m_cond, 0) )
	{
		perror("pthread_cond_init");
		goto COND_FAIL;
	}
	
	return bar;

COND_FAIL:
	pthread_mutex_destroy(&bar->m_mutex);

MUTEX_FAIL:
	free(bar);
	
ALLOC_FAIL:
	return NULL;
	
}
/*######################################################################*/



/*######################################################################*/
/*							BARRIER DESTROY								*/
/*######################################################################*/
void BarrierDestroy(Barrier* _barrier)
{
	assert(_barrier);
	
	pthread_mutex_destroy(&_barrier->m_mutex);
	pthread_cond_destroy(&_barrier->m_cond);
	free(_barrier);
}
/*######################################################################*/



/*######################################################################*/
/*							BARRIER WAIT								*/
/*	wait until counter reaches threshold, then release & reset counter.	*/
/*	return 0 for success, -1 for error									*/
/*######################################################################*/
int BarrierWait(Barrier* _barrier, Func _callback, void* _params)
{
	/* lock mutex */
	if( 0 != pthread_mutex_lock(&_barrier->m_mutex) )
	{
		perror("pthread_mutex_lock");
		return -1;
	}
	++ _barrier->m_counter;
		
	/* if counter == threshold: release all threads */
	if( _barrier->m_threshold == _barrier->m_counter )
	{
		if( 0 != pthread_cond_broadcast(&_barrier->m_cond) )
		{
			perror("pthread_cond_broadcast");
			return -1;
		}
		
		/* reset counter & release waiting threads*/
		_barrier->m_counter = 0;
	}
	else {	/* else: wait */
		if(_callback) {
			_callback(_params);	/* execute function while waiting */
		}
		
		if( 0 != pthread_cond_wait(&_barrier->m_cond, &_barrier->m_mutex) )
		{
			perror("pthread_cond_wait");
			return -1;
		}
	}

	if( 0 != pthread_mutex_unlock(&_barrier->m_mutex) )
	{
		perror("pthread_mutex_unlock");
		return -1;
	}

	return 0;

}
/*######################################################################*/




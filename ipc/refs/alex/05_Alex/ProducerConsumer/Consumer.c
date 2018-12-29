/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-15    
    Last modified date:		2013-09-15
    Description: producer-consumer - consumer module
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#include "GenericQueue.h"
#include "Consumer.h"


/*######################################################################*/
/*							DEFINITIONS									*/
/*######################################################################*/

#define CHECK0(X,S,E,R)		if( (X) != 0 ) { fprintf(stderr,S); fprintf(stderr,"%s\n",E); return R; }
#define CHECK0_2(X,S,E)		if( (X) != 0 ) { fprintf(stderr,S); fprintf(stderr,"%s\n",E); return; }

#define QUEUE		(_consumer->m_queue)
#define COUNTER		(_consumer->m_counter)
#define	Q_NITEMS	(QueueCountItems(QUEUE))
#define CAPACITY	(_consumer->m_qCapacity)
#define LOCK		(_consumer->m_lock)
#define COND		(_consumer->m_cond)


struct Consumer_t
{
	Queue			*m_queue;
	pthread_mutex_t	*m_lock;
	pthread_cond_t	*m_cond;
	int				 m_qCapacity;
	int				 m_counter; /* # of consumed items */
	int				 m_done;
};



/*######################################################################*/
/*							API FUNCTIONS								*/
/*######################################################################*/


/*######################################################################*/
/* initialize consumer thread, return null for failure */
Consumer*	ConsumerInit	(Queue* _queue, pthread_mutex_t* _lock, pthread_cond_t *_cond, size_t _queueCapacity)
{
	Consumer *consumer = NULL;
	
	assert(_queue && _lock && _cond);
	
	consumer = (Consumer*)malloc(sizeof(Consumer));
	if(!consumer) {
		return NULL;
	}
	
	consumer->m_queue		= _queue;
	consumer->m_lock		= _lock;
	consumer->m_cond		= _cond;
	consumer->m_qCapacity	= _queueCapacity;
	consumer->m_counter 	= 0;
	consumer->m_done		= 0;
	
	return consumer;
}
/*######################################################################*/


/*######################################################################*/
/* free everything */
void		ConsumerDestroy	(Consumer *_consumer)
{
	assert(_consumer);
	free(_consumer);
}
/*######################################################################*/


/*######################################################################*/
/* take item from queue - protected */
void*		Consume			(Consumer *_consumer)
{
	int error;
	void* item = NULL;
	
	assert(_consumer);
	
	error = pthread_mutex_lock(LOCK);	/* lock */
	CHECK0(error, "Consume: mutex_lock - ", strerror(error), (void*)error);
	
	/* if queue empty - wait */
	while( Q_NITEMS == 0 )
	{
printf("consumer: Q empty, done: %d\n", _consumer->m_done);
		
		if( _consumer->m_done ) {
			return NULL;
		}
		
		error = pthread_cond_wait(COND, LOCK);
		CHECK0(error, "Consume: cond_wait - ", strerror(error), (void*)error);
	}
	
	/*** take item ***/
	
	++COUNTER;
	item = QueueRemove(QUEUE);
	
	error = pthread_mutex_unlock(LOCK);	/* unlock */
	CHECK0(error, "Consume: mutex_unlock - ", strerror(error), (void*)error);
	
	if(Q_NITEMS == CAPACITY-1)
	{	/* if queue was full - wakup producers */
		error = pthread_cond_broadcast(COND);
		CHECK0(error, "Consume: cond_broadcast - ", strerror(error), (void*)error);
	}
	
	return item;
}
/*######################################################################*/


/*######################################################################*/
/* exit when done: raise flag & wake all sleepers */
void		ConsumerDone	(Consumer *_consumer)
{
	int error;
	
	assert(_consumer);
	
	error = pthread_mutex_lock(LOCK);	/* lock */
	CHECK0_2(error, "Consume: mutex_lock - ", strerror(error));
	
	_consumer->m_done = 1;
	
	error = pthread_mutex_unlock(LOCK);	/* unlock */
	CHECK0_2(error, "Consume: mutex_unlock - ", strerror(error));
	
	error = pthread_cond_broadcast(COND);
	CHECK0_2(error, "ConsumerDone: cond_broadcast - ", strerror(error));
}
/*######################################################################*/


/*######################################################################*/
/* return 1 if done, 0 if not */
int			ConsumerIsDone	(Consumer *_consumer)
{
	return _consumer->m_done;
}
/*######################################################################*/


/*######################################################################*/
/* return 1 if queue is empty, 0 if not */
int		ConsumerQueueIsEmpty	(Consumer *_consumer)
{
	return QueueIsEmpty(QUEUE);
}
/*######################################################################*/


/*######################################################################*/
/* get # of items consumed */
int			ConsumerGetTotal (Consumer *_consumer)
{
	return COUNTER;
}
/*######################################################################*/



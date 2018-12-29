/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-15    
    Last modified date:		2013-09-15
    Description: producer-consumer - producer module
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#include "GenericQueue.h"
#include "Producer.h"


/*######################################################################*/
/*							DEFINITIONS									*/
/*######################################################################*/

#define CHECK0_2(X,S,E)		if( (X) != 0 ) { fprintf(stderr,S); fprintf(stderr,"%s\n",E); return; }


#define QUEUE		(_producer->m_queue)
#define COUNTER		(_producer->m_counter)
#define	Q_NITEMS	(QueueCountItems(QUEUE))
#define CAPACITY	(_producer->m_qCapacity)
#define LOCK		(_producer->m_lock)
#define COND		(_producer->m_cond)

struct Producer_t
{
	Queue			*m_queue;
	pthread_mutex_t	*m_lock;
	pthread_cond_t	*m_cond;
	int				 m_qCapacity;
	int				 m_counter;	/* # of sent items */
};



/*######################################################################*/
/*							API FUNCTIONS								*/
/*######################################################################*/


/*######################################################################*/
/* initialize producer thread, return null for failure */
Producer*	ProducerInit	(Queue* _queue, pthread_mutex_t* _lock, pthread_cond_t *_cond, size_t _queueCapacity)
{
	Producer *producer = NULL;
	
	assert(_queue && _lock && _cond && _queueCapacity);
	
	producer = (Producer*)malloc(sizeof(Producer));
	if(!producer) {
		return NULL;
	}
	
	producer->m_queue		= _queue;
	producer->m_lock		= _lock;
	producer->m_cond		= _cond;
	producer->m_qCapacity	= _queueCapacity;
	producer->m_counter 	= 0;
	
	return producer;
}
/*######################################################################*/


/*######################################################################*/
/* free everything */
void		ProducerDestroy	(Producer *_producer)
{
	assert(_producer);
	free(_producer);
}
/*######################################################################*/



/*######################################################################*/
/* add item to queue -protected */
void		Produce			(Producer *_producer, void* _item)
{
	int error;
	
	assert(_producer && _item);
	
	error = pthread_mutex_lock(LOCK);	/* lock */
	CHECK0_2(error, "Produce: mutex_lock - ", strerror(error));
	
	/* if queue full - wait */
	while(Q_NITEMS == CAPACITY)
	{
		error = pthread_cond_wait(COND, LOCK);
		CHECK0_2(error, "Produce: cond_wait - ", strerror(error));
	}
	
	/*** add item ***/
	
	QueueInsert(QUEUE, _item);
	++COUNTER;
	
	error = pthread_mutex_unlock(LOCK);	/* unlock */
	CHECK0_2(error, "Produce: mutex_unlock - ", strerror(error));
	
	if(Q_NITEMS == 1)
	{	/* if queue was empty - wakup consumers */
		error = pthread_cond_broadcast(COND);
		CHECK0_2(error, "Produce: cond_broadcast - ", strerror(error));
	}
}
/*######################################################################*/


/*######################################################################*/
/* get # of items produced */
int			ProducerGetTotal (Producer *_producer)
{
	return COUNTER;
}
/*######################################################################*/


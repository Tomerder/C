/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-15    
    Last modified date:		2013-09-15
    Description: header file for producer
***************************************************************************************/

#ifndef __PRODUCER_H__
#define __PRODUCER_H__

#include <pthread.h>
#include "GenericQueue.h"

typedef struct Producer_t Producer;

/* initialize producer thread, return null for failure */
Producer*	ProducerInit	(Queue* _queue, pthread_mutex_t* _lock,
								pthread_cond_t *_cond, size_t _queueCapacity);

/* free everything */
void		ProducerDestroy	(Producer *_producer);

/* add item to queue */
void		Produce			(Producer *_producer, void* _item);

/* get # of items produced */
int			ProducerGetTotal (Producer *_producer);


#endif /* __PRODUCER_H__ */

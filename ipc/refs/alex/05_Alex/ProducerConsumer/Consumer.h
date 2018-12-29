/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-15    
    Last modified date:		2013-09-15
    Description: header file for consumer
***************************************************************************************/

#ifndef __CONSUMER_H__
#define __CONSUMER_H__

#include <pthread.h>
#include "GenericQueue.h"

typedef struct Consumer_t Consumer;

/* initialize consumer thread, return null for failure */
Consumer*	ConsumerInit	(Queue* _queue, pthread_mutex_t* _lock,
								pthread_cond_t *_cond, size_t _queueCapacity);

/* free everything */
void	ConsumerDestroy	(Consumer *_consumer);

/* take item from queue */
void*	Consume			(Consumer *_consumer);

/* exit when done: raise flag & wake all sleepers */
void	ConsumerDone	(Consumer *_consumer);

/* return 1 if done, 0 if not */
int		ConsumerIsDone	(Consumer *_consumer);

/* return 1 if queue is empty, 0 if not */
int		ConsumerQueueIsEmpty	(Consumer *_consumer);

/* get # of items consumed */
int		ConsumerGetTotal (Consumer *_consumer);


#endif /* __CONSUMER_H__ */

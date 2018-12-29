/**************************************************************************************
    Author :	(Stav Ofer)
    Creation date :      5.9.10	(2013-07-24)
    Date last modified : 2013-09-15
    Description : Queue Implementation - based on generic linked list
    				- adapted for use in producer-consumer: added capacity & nItems fields,
    				and countItems, IsFull functions
***************************************************************************************/
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdlib.h>
#include <stdio.h>

#include "ADTDefs.h"
#include "LListGen.h"


typedef struct Queue Queue;

/* create a queue of given capacity. return NULL for failure */
Queue*      QueueCreate (size_t _capacity);

/* destroy a queue */
void        QueueDestroy(Queue*	_queue);

/* insert into tail of queue. return success/error messege */
ADTErr		QueueInsert (Queue*	_queue, void*  _item);

/* remove from head of queue, return removed data */
void*       QueueRemove (Queue*	_queue);

/* return 1 for empty queue, 0 otherwise */
int         QueueIsEmpty(Queue*	_queue);
/* return 1 for full queue, 0 otherwise */
int			QueueIsFull(Queue*	_queue);

/* return # of items */
size_t		QueueCountItems(Queue* _queue);

/* print - for testing */
void        QueuePrint  (Queue*	_queue, PrintFunc _printFunc);

#endif /*__QUEUE_H__*/

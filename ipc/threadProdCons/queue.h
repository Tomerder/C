/**************************************************************************************
    Author :				Alex Katz
    Creation date :      	10.1.12
    Date last modified :
    Description : 			Circular Queue Implementation. Using fixed length vector and
							data item stored in the vector.
***************************************************************************************/
#ifndef __QUEUE_H__
#define __QUEUE_H__
#include "err.h"

typedef void* Item;
typedef struct Queue Queue;
typedef void(*PrintFunc)(Item _item);

/*
**	size   - max number of elements in the queue
**	memory - where to allocate queue elements
*/

Queue*   QueueCreate (size_t _size);
void	 QueueDestroy(Queue* _queue);

ADTERR	 QueueInsert (Queue* _queue, Item _item);
ADTERR	 QueueRemove (Queue* _queue , Item* _item);

int		 IsQueueEmpty(Queue* _queue);
int		 IsQueueFull (Queue* _queue);

void 	 QueuePrint	 (Queue *_queue, PrintFunc _printFunc);

int QueueNitems (Queue* _queue);

#endif /*__QUEUE_H__*/

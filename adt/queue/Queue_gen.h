/**************************************************************************************
    Author :				Alex Katz
    Creation date :      	10.1.12
    Date last modified :
    Description : 			Circular Queue Implementation. Using fixed length vector and
							data item stored in the vector.
***************************************************************************************/
#ifndef __QUEUE_H_
#define __QUEUE_H_

typedef void* Item;
typedef struct Queue Queue;

/*
**	size   - max number of elements in the queue
**	memory - where to allocate queue elements
*/

Queue	*QueueCreate (size_t _size);
void	 QueueDestroy(Queue *_queue);

int		 QueueInsert (Queue *_queue, Item _item);
Item	 QueueRemove (Queue *_queue);

int		 IsQueueEmpty(Queue *_queue);
int		 IsQueueFull (Queue *_queue);

void 	 QueuePrint	 (Queue *_queue);

#endif /*__QUEUE_H_*/

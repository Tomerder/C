/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      2013-07-24
    Date last modified : 2013-07-24
    Description : Queue Implementation - based on generic linked list
    				- adapted for use in producer-consumer: added capacity & nItems fields,
    				and countItems, IsFull functions
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "LListGen.h"
#include "GenericQueue.h"


/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/

#define LIST(X)		(X)->m_list

typedef struct Node_t
{
	ListData		m_data;
	struct Node_t*	m_next;
	struct Node_t*	m_prev;
} Node;

struct List_t
{
	Node	m_head;
	Node	m_tail;
};


struct Queue
{
	List*	m_list;
	size_t	m_nItems;
	size_t	m_capacity;
};


/*----------------------------------------------------------*/




/*----------------------------------------------------------*/
/*						API functions						*/
/*----------------------------------------------------------*/



/* create a queue of given capacity. return NULL for failure */
Queue*      QueueCreate (size_t _capacity)
{
	Queue*	newQueue = NULL;
	List*	newList	 = NULL;
	
	newList = ListCreate();
	if(!newList) {
		return NULL;
	}
	
	newQueue = (Queue*)malloc(sizeof(Queue));
	if(!newQueue) {
		ListDestroy(newList);
		return NULL;
	}
	
	newQueue->m_list		= newList;
	newQueue->m_nItems		= 0;
	newQueue->m_capacity	= _capacity;
	
	return newQueue;
}
/*----------------------------------------------------------*/


/* destroy a queue */
void        QueueDestroy(Queue*	_queue)
{
	assert(_queue);
	assert(LIST(_queue));
	
	ListDestroy(LIST(_queue));
	free(_queue);
}
/*----------------------------------------------------------*/



/* insert into tail of queue. return success/error messege */
ADTErr   QueueInsert (Queue* _queue, void*  _item)
{
	int error;

	assert(_queue && _item);
	
	if(QueueIsFull(_queue)) {
		return ERR_OVERFLOW;
	}
	
	error = ListPushTail(LIST(_queue), (ListData)_item);
	++_queue->m_nItems;
	return error;
}
/*----------------------------------------------------------*/



/* remove from head of queue, return removed data */
/* for empty queue returns NULL */
void*       QueueRemove (Queue*	_queue)
{
	ListData data;
	
	assert(_queue);
	if(ListIsEmpty(LIST(_queue))) {
		return NULL;
	}
	
	data = ListPopHead(LIST(_queue));
	--_queue->m_nItems;
	return data;
}
/*----------------------------------------------------------*/



/* return 1 for empty queue, 0 otherwise */
int         QueueIsEmpty(Queue*	_queue)
{
	assert(_queue);
	
	return(0 == _queue->m_nItems);
}
/*----------------------------------------------------------*/


/* return 1 for full queue, 0 otherwise */
int		QueueIsFull(Queue* _queue)
{
	assert(_queue);
	
	return(_queue->m_capacity == _queue->m_nItems);
}
/*----------------------------------------------------------*/


/* return # of items */
size_t		QueueCountItems(Queue* _queue)
{
	return _queue->m_nItems;
}
/*----------------------------------------------------------*/





/* print - for testing */
void        QueuePrint  (Queue*	_queue, PrintFunc _printFunc)
{
	assert(_queue);
	
	ListPrint((void*)LIST(_queue), _printFunc);
}
/*----------------------------------------------------------*/




/*----------------------------------------------------------*/
/*					internal functions						*/
/*----------------------------------------------------------*/





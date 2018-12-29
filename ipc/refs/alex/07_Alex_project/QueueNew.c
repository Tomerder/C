/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-16    
    Last modified date:		2013-09-17
    Description:	functions for handling queues.
    				update 16/9: modified for use as part of message queue.
********************************************************************************/

/*#include <stdlib.h>*/
/*#include <string.h>*/
#include <stdio.h>
#include <assert.h>

#include "QueueNew.h"



/*######################################################################*/
/*					Definitions & Foreward Declarations					*/
/*######################################################################*/


struct Queue
{
	size_t	m_head;		/* index of current head of queue - first/oldest inserted item	*/
	size_t	m_tail;		/* index of current tail of queue - next empty place after last/newest	*/
	size_t	m_noItems;	/* number of items						*/
	size_t	m_size;		/* size of array - constant 			*/
	size_t	m_items[1];	/* (start of) array of items (offsets)	*/
};


enum Success
{
	SUCCESS = 0,
	FAIL = -1
};


/*######################################################################*/
/*								API Fucntions							*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* create queue of size _nPlaces, starting at given memory address */
/* return NULL for failure */
Queue_t*	QueueCreate		(void* _memory, size_t _nPlaces)
{
	Queue_t* queue = (Queue_t*)_memory;
	
	assert( _memory && _nPlaces);
	
	queue->m_head 		= 0;
	queue->m_tail 		= 0;
	queue->m_noItems 	= 0;
	queue->m_size 		= _nPlaces;
	
	return queue;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* destroy (empty) */
void	QueueDestroy	(Queue_t* _queue)
{
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* return required memory size for given _nPlaces */
size_t	QueueGetSize	(size_t _nPlaces)
{
	 return ( sizeof(Queue_t) + sizeof(size_t) * (_nPlaces - 1) );
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* insert to queue tail, return success (0) / fail (-1) */
int		QueueInsert		(Queue_t* _q, int _item)
{
	assert(_q);
	
	if( IsQueueFull(_q) ) {
		return FAIL;
	}
	
	_q->m_items[_q->m_tail] = _item;
	++_q->m_noItems;
	
	_q->m_tail = (_q->m_tail + 1) % (_q->m_size);		/* tail & head go in circles --> modulo */
	
	return SUCCESS;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* remove from queue head, return success (0) / fail (-1) */
int		QueueRemove		(Queue_t* _q, int* _item)
{
	assert(_q && _item);
	
	if( IsQueueEmpty(_q) ) {
		return FAIL;
	}
	
	*_item = _q->m_items[_q->m_head];
	--_q->m_noItems;
	
	_q->m_head = (_q->m_head + 1) % (_q->m_size);		/* tail & head go in circles --> modulo */
	
	return SUCCESS;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* IsEmpty, IsFull: return true (1) / false (0) */
/*----------------------------------------------------------------------*/
int		IsQueueEmpty	(Queue_t* _q)
{
	assert(_q);
	return ( 0 == _q->m_noItems );
}
/*----------------------------------------------------------------------*/
int		IsQueueFull		(Queue_t* _q)
{
	assert(_q);
	return ( _q->m_noItems == _q->m_size );
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* for testing purposes */
void	QueuePrint		(Queue_t* _q)
{
	int i;
	
	assert(_q);
	
	printf("Queue is: \n");
	if( 0 ==_q->m_noItems ) {
		printf("Empty!\n");
		return;
	}
	for( i = 0; i < _q->m_noItems; i++ ) {
		printf("%d ", _q->m_items[ (i+_q->m_head)%(_q->m_size) ] );
	}
	printf("\n");
}
/*----------------------------------------------------------------------*/


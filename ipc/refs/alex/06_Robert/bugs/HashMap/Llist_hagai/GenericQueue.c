/**************************************************************************************
    Author :
    Creation date :      5.9.10
    Date last modified :
    Description : Queue Implementation.
***************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "GenericQueue.h"
#include "DLLGen.h"

struct Node
{
	struct	Node* 	m_next;
	struct	Node* 	m_prev;
	Data			m_data;
};

struct List
{
	Node	m_first;
	Node	m_last;
};

struct Queue
{
	List	*m_list;
};



Queue*      QueueCreate (void)
{
	Queue *queue;

	queue = (Queue*)malloc(sizeof(Queue));
	if(!queue){
		return NULL;
	}	
	queue->m_list = ListCreate();
	if(!queue->m_list){
		free(queue);
		return NULL;	
	}
	
return queue;
}/*QueueCreate*/



void        QueueDestroy(Queue*	_queue)
{
	ListDestroy ( _queue->m_list );
}/*QueueDestroy*/



ADTErr    QueueInsert (Queue*	_queue, void*  _item)
{
	return ListPushHead( _queue->m_list, _item );
}/*QueueInsert*/



void*       QueueRemove (Queue*	_queue)
{
	return ListPopTail ( _queue->m_list );
}/*QueueRemove*/



int         QueueIsEmpty(Queue*	_queue)
{
	return ListIsEmpty	( _queue->m_list );
}/*QueueIsEmpty*/


void        QueuePrint  (Queue*	_queue, void(*_Print)(void*))
{
	ListPrint( _queue->m_list, _Print);	
}/*QueuePrint*/









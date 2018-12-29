#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

/*------------------------------------------------------------------------------------*/

#define GET_ITEMS(QUEUE)  (  (Item*)((char*)QUEUE + (sizeof(size_t) * 4)))

#define GET_ITEM(QUEUE, IND)  (  (GET_ITEMS(QUEUE)[IND]) )
/*------------------------------------------------------------------------------------*/

struct Queue{
	size_t m_size;
	size_t m_head;
	size_t m_tail;
	size_t m_nItems;  /*for knowing if queue is empty or full*/
	
	/*items arr is allocated with struct queue (one malloc) => makes queue self depended*/    
	/*Item*  m_data;*/
};

/*------------------------------------------------------------------------------------*/

Queue* QueueInit (char* _memAllocated, size_t _size)
{
	Queue* queue = (Queue*)(_memAllocated);
	
	queue->m_size = _size;	
	queue->m_head = 0;
	queue->m_tail = 0;
	queue->m_nItems = 0;
	
	return queue;
}

/*------------------------------------------------------------------------------------*/

int QueueNeededSize (int _nItems)
{
	return ( sizeof(Queue) + sizeof(Item) * _nItems );
}

/*------------------------------------------------------------------------------------*/

Queue* QueueCreate (size_t _size)
{
	Queue* queue;
	
	if(_size == 0){
			return NULL;    
	}

	/*allocate both queue and items*/
	queue = calloc (1, sizeof(Queue) + (sizeof(Item) * _size) );   
	if (queue == NULL){
		return NULL;
	}	

	queue->m_size = _size;	
	queue->m_head = 0;
	queue->m_tail = 0;
	queue->m_nItems = 0;

	return queue;
}

/*------------------------------------------------------------------------------------*/

void QueueDestroy (Queue* _queue)
{
	assert(_queue);

	free(_queue);
}

/*------------------------------------------------------------------------------------*/

ADTERR QueueInsert (Queue* _queue, Item _item)
{
	assert(_queue);
	
	/*check if queue is full*/
	if(_queue->m_nItems == _queue->m_size){
		return OVERFLOW;
	}

	 /*insert*/
	GET_ITEM(_queue, _queue->m_tail)  = _item;  
	_queue->m_tail = (_queue->m_tail + 1) % _queue->m_size;                 
	(_queue->m_nItems)++;

	return OK;
}

/*------------------------------------------------------------------------------------*/

ADTERR QueueRemove (Queue* _queue , Item* _item)
{
	assert(_queue);
	
	/*check if queue is empty*/
	if(_queue->m_nItems == 0){
		return UNDERFLOW;
	}

	/*remove*/
	*_item = GET_ITEM(_queue, _queue->m_head );  /*item to return*/
	_queue->m_head = (_queue->m_head + 1) % _queue->m_size;       /*head--*/                     
	(_queue->m_nItems)--;

	return OK;
}

/*------------------------------------------------------------------------------------*/

int IsQueueEmpty (Queue* _queue){
	assert(_queue);
	
	return (_queue->m_nItems == 0);	
}

/*------------------------------------------------------------------------------------*/

int IsQueueFull (Queue* _queue){
	assert(_queue);

	return (_queue->m_nItems == _queue->m_size);
}

/*------------------------------------------------------------------------------------*/

void QueuePrint (Queue* _queue , PrintFunc _printFunc) {
	int i=0 , itemsNum,size,curIndex;
	assert(_queue);
	assert(_printFunc);
	
	itemsNum = _queue->m_nItems;
	curIndex = _queue->m_head;
	size = _queue->m_size; 	

	printf("head=>tail :");
	while(i < itemsNum){
		_printFunc( GET_ITEMS(_queue) [curIndex] );
		printf(" , ");
		curIndex = (curIndex + 1) % size;
		i++;
	}  
	printf("\n\n");
}

/*------------------------------------------------------------------------------------*/





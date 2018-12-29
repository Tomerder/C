/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-19    
    Last modified date:		2013-07-24
    Description: double linked list functions.
    			 update 21/7: now for generic data using typedef void ListData.
********************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ADTDefs.h"
#include "LListGenFunc.h"


/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/

#define MAGIC 0xdeadbeef

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


static Node*	 	NodeCreate	(ListData _data, Node* _next, Node* _prev);
static ListData 	NodeDestroy	(Node* _node);
static void 		NodePrint	(Node *_node, List *_list, PrintFunc _printFunc);

/*----------------------------------------------------------*/



/*----------------------------------------------------------*/
/*				API functions - General						*/
/*----------------------------------------------------------*/



/* allocate place for list and create begin & end nodes */
List* ListCreate(void)
{
	/* allocate & check */
	List *tmpList = (List *)malloc(sizeof(List));				
	if( !tmpList ) {
		return NULL;
	}

	/* beginning node */
	tmpList->m_head.m_prev = NULL;								
	tmpList->m_head.m_next = &tmpList->m_tail;
	tmpList->m_head.m_data = (ListData)MAGIC;
	
	/* end node */
	tmpList->m_tail.m_prev = &tmpList->m_head;
	tmpList->m_tail.m_next = NULL;
	tmpList->m_tail.m_data = (ListData)MAGIC;

	return tmpList;
}
/*----------------------------------------------------------*/


/* free list */
void ListDestroy(List *_list)
{
	Node *tempNode;
	
	assert(_list);
	
	tempNode = _list->m_head.m_next;
	
	while(tempNode != &_list->m_tail)
	{
		tempNode = tempNode->m_next;
		ListPopHead(_list);
	}
	
	free(_list);
}
/*----------------------------------------------------------*/


/* insert at beginning */
ADTErr ListPushHead(List *_list, ListData _data)	
{
	Node *newNode; 
	
	assert(_list);
	
	/* allocate node & check success */
	newNode = NodeCreate(_data, _list->m_head.m_next, &_list->m_head);
	if( !newNode ) {		
		return ERR_ALLOCATION_FAILED;
	}
	
	/* change pointers in beginning node & in previously-first node */
	_list->m_head.m_next = newNode;							
	newNode->m_next->m_prev = newNode;

	return ERR_OK;
}
/*----------------------------------------------------------*/


/* insert at end */
ADTErr ListPushTail(List *_list, ListData _data)
{
	Node *newNode; 

	assert(_list);
	
	/* allocate node & check success */
	newNode = NodeCreate(_data, &_list->m_tail, _list->m_tail.m_prev);
	if( !newNode ) {		
		return ERR_ALLOCATION_FAILED;
	}
	
	/* change pointers in end node & in previously-last node */
	_list->m_tail.m_prev = newNode;						
	newNode->m_prev->m_next = newNode;

	return ERR_OK;
}
/*----------------------------------------------------------*/


/* remove from beginning */
ListData ListPopHead(List *_list)
{
	Node *toRemove;
	ListData headData;

	assert(_list);
	
	if (_list->m_head.m_next == &_list->m_tail) {
		return NULL;
	}

	toRemove = _list->m_head.m_next;
	
	/* change pointers in prev. & next nodes */
	_list->m_head.m_next = _list->m_head.m_next->m_next;			
	_list->m_head.m_next->m_prev = &_list->m_head;
	
	/* get data & free node */
	headData = NodeDestroy(toRemove);

	return headData;
}
/*----------------------------------------------------------*/


/* remove from end */
ListData ListPopTail(List *_list)
{
	Node *toRemove = NULL;	
	ListData tailData;	

	assert(_list);
	if (_list->m_head.m_next == &_list->m_tail) {
		return NULL;
	}

	toRemove = _list->m_tail.m_prev;

	/* change pointers in prev. & next nodes */
	 _list->m_tail.m_prev = toRemove->m_prev;			
	toRemove->m_prev->m_next = &_list->m_tail;

	/* get data & free node */
	tailData = NodeDestroy(toRemove);
	
	return tailData;
}	
/*----------------------------------------------------------*/


/* return # of items, return -1 for error */
size_t ListCountItems(List *_list)
{
	Node *tempNode;
	size_t count = 0;

	assert(_list);
	
	/* pointer to get around difference b/w beginning node & regular nodes */
	tempNode = _list->m_head.m_next;

	/* actual work done here */
	for(count=0; tempNode != &_list->m_tail; ++count, tempNode = tempNode->m_next);

	return count;
}
/*----------------------------------------------------------*/


/*return 1 if list is empty, 0 otherwise */
int ListIsEmpty(List *_list)
{
	assert(_list);

	return ( _list->m_head.m_next == &_list->m_tail );
}
/*----------------------------------------------------------*/


/* print list */
/* calls internal function NodePrint */
void ListPrint(List *_list, PrintFunc _printFunc)
{
	assert(_list);

	printf("\nList is:\n");
	
	/* if list is empty */
	if( _list->m_head.m_next == &_list->m_tail )					
	{
		printf("Empty!\n\n");
		return;
	}
	
	/* call NodePrint */
	NodePrint(_list->m_head.m_next, _list, _printFunc);						
	printf("\n");
}
/*----------------------------------------------------------*/





/*----------------------------------------------------------*/
/*				API functions - Iterators					*/
/*----------------------------------------------------------*/



/* get iterator to 1st node */
/* for empty list, returns ListEnd */
ListItr		ListBegin	(List *_list)
{
	assert(_list);
	return (ListItr)_list->m_head.m_next;	
}
/*----------------------------------------------------------*/


/* get iter to past-last (tail) node */
ListItr		ListEnd		(List *_list)
{
	assert(_list);
	return (ListItr)&(_list->m_tail);
}
/*----------------------------------------------------------*/


/* go to next node */
/*** going beyond boundaries results in UNDEFINED BEHAVIOR ***/
ListItr		ListNext	(ListItr _itr)
{
	Node* nextNode;
	
	nextNode = ((Node*)_itr)->m_next;
	return (ListItr)nextNode;
}
/*----------------------------------------------------------*/


/* go to previous node */
/*** going beyond boundaries results in UNDEFINED BEHAVIOR ***/
ListItr		ListPrev	(ListItr _itr)
{
	Node* prevNode;
	
	prevNode = ((Node*)_itr)->m_prev;	
	
	if( (ListData)MAGIC == prevNode->m_data) {
		return NULL;
	}
	
	return (ListItr)prevNode;
}
/*----------------------------------------------------------*/



/* get data */
/* getting data from tail - UNDEFINED BEHAVIOR */
ListData	ListGetData	(ListItr _itr)
{
	if( (ListData)MAGIC == ((Node*)_itr)->m_data ) {
		return NULL;
	}
	return ((Node*)_itr)->m_data;
}
/*----------------------------------------------------------*/



/* insert before _itr, returns itr to inserted data */
/* if failed, return original _itr */
ListItr		ListInsertBefore	(ListItr _itr, ListData _data)
{
	Node* tempNode;
	
	tempNode = malloc( sizeof(Node) );
	if(!tempNode) {
		return _itr;
	}
	
	tempNode->m_data = _data;	
	((Node*)_itr)->m_prev->m_next = tempNode;
	tempNode->m_next = (Node*)_itr;
	
	return (ListItr)tempNode;
}
/*----------------------------------------------------------*/



/* remove node & return data */
/* if failed - UNDEFINED BEHAVIOR */
ListData	ListRemove	(ListItr _itr)
{
	ListData data;
	
	data = ((Node*)_itr)->m_data;
	
	if( data == (void*)MAGIC ) {
		return NULL;
	}
	
	((Node*)_itr)->m_prev->m_next = ((Node*)_itr)->m_next;
	((Node*)_itr)->m_next->m_prev = ((Node*)_itr)->m_prev;
	
	free(_itr);
	return data;
}
/*----------------------------------------------------------*/



/* search for _data between _from and _to, not including _to */
/* return iterator of required data if found, _to if not */
/* if data duplication, return 1st result found */
ListItr ListFindFirst(ListItr _from, ListItr _to, CompFunc _cmpfunc, ListData _data)
{
	ListItr itr = _from;
	
	while(itr != _to && ((Node*)itr)->m_data != (void*)MAGIC)
	{
		if( 0 == _cmpfunc(((Node*)itr)->m_data, _data) ) {
			return itr;
		}
		itr = ListNext(itr);
	}
	return _to;
}
/*----------------------------------------------------------*/



/* apply _dofunc to all nodes between _from and _to (not including _to) */
/* return _to for success, the Itr where failed if failed */
ListItr ListForEach(ListItr _from, ListItr _to, DoFunc _dofunc, void* _params)
{
	ListItr itr = _from;	
	int error;
	
	while(itr != _to && ((Node*)itr)->m_data != (void*)MAGIC)
	{
		error = _dofunc(((Node*)itr) ->m_data, _params);
		if(error) {
			return itr;
		}
		itr = ListNext(itr);
	}
	return _to;
}
/*----------------------------------------------------------*/








/*----------------------------------------------------------*/
/*					internal functions						*/
/*----------------------------------------------------------*/


/* create a list node from data & ptrs */
/* return new node ar NULL for failure */
Node* NodeCreate(ListData _data, Node* _next, Node* _prev)
{
	Node* newNode = malloc(sizeof(Node));
	if(!newNode) {
		return NULL;
	}
	
	newNode->m_data = _data;
	newNode->m_next = _next;
	newNode->m_prev = _prev;
	
	return newNode;
}
/*----------------------------------------------------------*/


/* destroy a node, return data */
ListData NodeDestroy(Node* _node)
{
	ListData removedData;
	
	removedData = _node->m_data;
	free(_node);
	
	return removedData;
}
/*----------------------------------------------------------*/



/* print nodes recursively, for use in ListPrint */
void NodePrint(Node *_node, List *_list, PrintFunc _printFunc)
{
	assert(_list);
	
	/* stop condition: when current node points to end node */
	if( _node->m_next == &_list->m_tail ) {						
		_printFunc(_node->m_data);
		return;
	}

	/* recursion, printing on the way in */
	_printFunc(_node->m_data);
	NodePrint(_node->m_next, _list, _printFunc);
}
/*----------------------------------------------------------*/





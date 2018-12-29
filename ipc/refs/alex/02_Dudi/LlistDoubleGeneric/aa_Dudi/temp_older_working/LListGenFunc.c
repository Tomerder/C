/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-19    
    Last modified date:		2013-07-22
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

#define MAGIC 0xbdbdbdbd

struct Node_t
{
	ListData*	m_data;
	Node*		m_next;
	Node*		m_prev;
};

struct List_t
{
	Node	m_head;
	Node	m_tail;
};


static void 	NodeDestroy	(Node *_node, List *_list);
extern void 	ListPrint	(List *_list, PrintFunc _printFunc);
static void 	NodePrint	(Node *_node, List *_list, PrintFunc _printFunc);
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
	tmpList->m_head.m_data = (ListData*)MAGIC;
	
	/* end node */
	tmpList->m_tail.m_prev = &tmpList->m_head;
	tmpList->m_tail.m_next = NULL;
	tmpList->m_tail.m_data = (ListData*)MAGIC;

	return tmpList;
}
/*----------------------------------------------------------*/


/* free list */
/* calls NodeDestroy */
void ListDestroy(List *_list)
{
	assert(_list);

	/* if list is empty */
	if( ListIsEmpty(_list) ) {						
		free(_list);
		return;
	}
	
	/* otherwise free all nodes first by function NodeDestroy */
	NodeDestroy(_list->m_head.m_next, _list);				
	free(_list);
}
/*----------------------------------------------------------*/


/* insert at beginning */
ADTErr ListPushHead(List *_list, ListData* _data)	
{
	Node *tempNode; 
	
	assert(_list);
	
	/* allocate node & check success */
	tempNode = malloc( sizeof(Node) );
	if( !tempNode ) {		
		return ERR_ALLOCATION_FAILED;
	}
	
	/* insert data & pointers to node */
	tempNode->m_data = _data;
	tempNode->m_next = _list->m_head.m_next;
	tempNode->m_prev = &_list->m_head;

	/* change pointers in beginning node & in previously-first node */
	_list->m_head.m_next = tempNode;							
	tempNode->m_next->m_prev = tempNode;

	return ERR_OK;
}
/*----------------------------------------------------------*/


/* insert at end */
ADTErr ListPushTail(List *_list, ListData* _data)
{
	Node *tempNode; 

	assert(_list);
	
	/* allocate node & check success */
	tempNode = malloc( sizeof(Node) );
	if( !tempNode ) {		
		return ERR_ALLOCATION_FAILED;
	}
	
	/* insert data & pointers to node */
	tempNode->m_data = _data;
	tempNode->m_next = &_list->m_tail;
	tempNode->m_prev = _list->m_tail.m_prev;

	/* change pointers in end node & in previously-last node */
	_list->m_tail.m_prev = tempNode;						
	tempNode->m_prev->m_next = tempNode;

	return ERR_OK;
}
/*----------------------------------------------------------*/


/* remove from beginning */
ListData* ListPopHead(List *_list, ListData** _data)
{
	Node *toRemove; 

	assert(_list);	
	if (_list->m_head.m_next == &_list->m_tail) {
		return NULL;
	}

	toRemove = _list->m_head.m_next;
	*_data = toRemove->m_data;	
	
	/* change pointers in prev. & next nodes */
	 _list->m_head.m_next = toRemove->m_next;			
	toRemove->m_next->m_prev = &_list->m_head;

	return toRemove;
}
/*----------------------------------------------------------*/


/* remove from end */
ListData* ListPopTail(List *_list, ListData** _data)
{
	Node *toRemove; 			

	assert(_list);
	
	if (_list->m_head.m_next == &_list->m_tail) {
		return NULL;
	}

	toRemove = _list->m_tail.m_prev;
	*_data = toRemove->m_data;

	/* change pointers in prev. & next nodes */
	 _list->m_tail.m_prev = toRemove->m_prev;			
	toRemove->m_prev->m_next = &_list->m_tail;

	return toRemove;
}	
/*----------------------------------------------------------*/


/* return # of items, return -1 for error */
size_t ListCountItems(List *_list)
{
	Node *tempNode;
	size_t count;

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




/*----------------------------------------------------------*/
/*				API functions - Iterators					*/
/*----------------------------------------------------------*/



/* get iterator to 1st node */
/* for empty list, returns ListEnd */
ListItr		ListBegin	(List *_list)
{
	assert(_list);
	return _list->m_head.m_next;	
}
/*----------------------------------------------------------*/


/* get iter to past-last (tail) node */
ListItr		ListEnd		(List *_list)
{
	assert(_list);
	return &(_list->m_tail);
}
/*----------------------------------------------------------*/


/* go to next node */
/*** going beyond boundaries results in UNDEFINED BEHAVIOR ***/
ListItr		ListNext	(ListItr _itr)
{
	return _itr->m_next;
}
/*----------------------------------------------------------*/


/* go to previous node */
/*** going beyond boundaries results in UNDEFINED BEHAVIOR ***/
ListItr		ListPrev	(ListItr _itr)
{
	if( NULL == _itr->m_prev->m_prev) {
		return NULL;
	}
	return _itr->m_prev;
}
/*----------------------------------------------------------*/



/* get data */
/* getting data from tail - UNDEFINED BEHAVIOR */
ListData*	ListGetData	(ListItr _itr)
{
	if( (ListData*)MAGIC == _itr->m_data ) {
		return NULL;
	}
	return _itr->m_data;
}
/*----------------------------------------------------------*/



/* insert before _itr, returns itr to inserted data */
/* if failed, return original _itr */
ListItr		ListInsertBefore	(ListItr _itr, ListData* _data)
{
	Node* tempNode;
	
	tempNode = malloc( sizeof(Node) );
	if(!tempNode) {
		return _itr;
	}
	
	tempNode->m_data = _data;	
	_itr->m_prev->m_next = tempNode;
	tempNode->m_next = _itr;
	
	return tempNode;
}
/*----------------------------------------------------------*/



/* remove node & return data */
/* if failed - UNDEFINED BEHAVIOR */
ListData*	ListRemove	(ListItr _itr)
{
	ListData* data;
	
	if( _itr->m_data == (void*)MAGIC ) {
		return NULL;
	}
	
	data = _itr->m_data;
	
	_itr->m_prev->m_next = _itr->m_next;
	_itr->m_next->m_prev = _itr->m_prev;
	
	free(_itr);
	return data;
}
/*----------------------------------------------------------*/



/* search for _data between _from and _to, not including _to */
/* return iterator of required data if found, _to if not */
/* if data duplication, return 1st result found */
ListItr ListFindFirst(ListItr _from, ListItr _to, CompFunc _cmpfunc, ListData* _data)
{
	ListItr itr = _from;
	
	while(itr != _to && itr->m_data != (void*)MAGIC)
	{
		if( 0 == _cmpfunc(itr->m_data, _data) ) {
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
	
	while(itr != _to && itr->m_data != (void*)MAGIC)
	{
		error = _dofunc(itr->m_data, _params);
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

/* print list */
/* calls NodePrint */
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



/* free all nodes recursively */
/* for use in function ListFree */
void NodeDestroy(Node *_node, List *_list)
{	
	
	/* stop condition: when current node points to end node */
	if( _node->m_next == &_list->m_tail ) {						
		free(_node);
		return;
	}

	/* recursion, then free node on the way out */
	NodeDestroy(_node->m_next, _list);	
	free(_node);
}
/*----------------------------------------------------------*/




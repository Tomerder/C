/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-19    
    Last modified date:		2013-07-23
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

typedef struct Node_t
{
	Data	m_data;
	Node*	m_next;
	Node*	m_prev;
} Node;

typedef struct List_t
{
	Node	m_head;
	Node	m_tail;
} List;

struct Itr_t
{
	Node*	m_node;
};

static Node* 	NodeCreate		(Data _data, Node* _next, Node* _prev);
static Data 	NodeDestroy		(Node* _node);
static void 	ListDestroyImp	(Node *_node, List *_list);
static void 	NodePrint		(Node *_node, List *_list, PrintFunc _printFunc);
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
	tmpList->m_head.m_data = (Data)MAGIC;
	
	/* end node */
	tmpList->m_tail.m_prev = &tmpList->m_head;
	tmpList->m_tail.m_next = NULL;
	tmpList->m_tail.m_data = (Data)MAGIC;

	return tmpList;
}
/*----------------------------------------------------------*/


/* free list */
/* calls ListDestroyImp */
void ListDestroy(List *_list)
{
	assert(_list);

	/* if list is empty */
	if( ListIsEmpty(_list) ) {						
		free(_list);
		return;
	}
	
	/* otherwise free all nodes first by function NodeDestroy */
	ListDestroyImp(_list->m_head.m_next, _list);				
	free(_list);
}
/*----------------------------------------------------------*/


/* insert at beginning */
/* calls internal function: NodeCreate */
ADTErr ListPushHead(List *_list, Data _data)	
{
	Node *newNode = NULL; 
	
	assert(_list);
	
	/* create new node */
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
ADTErr ListPushTail(List *_list, Data _data)
{
	Node *newNode = NULL; 
	
	assert(_list);
	
	/* create new node */
	newNode = NodeCreate(_data, _list->m_head.m_next, &_list->m_head);
	if( !newNode ) {		
		return ERR_ALLOCATION_FAILED;
	}

	/* change pointers in end node & in previously-last node */
	_list->m_tail.m_prev = newNode;						
	newNode->m_prev->m_next = tempNode;

	return ERR_OK;
}
/*----------------------------------------------------------*/


/* remove from beginning, get data & free node */
ADTErr ListPopHead(List *_list, Data* _data)
{
	Node **toRemove = NULL; 

	assert(_list);
	
	if (_list->m_head.m_next == &_list->m_tail) {
		return ERR_EMPTY;
	}

	toRemove = &_list->m_head.m_next;
	
	/* change pointers in prev. & next nodes */
	 _list->m_head.m_next = (*toRemove)->m_next;			
	(*toRemove)->m_next->m_prev = &_list->m_head;
	
	/* get data & free node */
	*_data = NodeDestroy(*toRemove);

	return ERR_OK;
}
/*----------------------------------------------------------*/


/* remove from end, get data & free node  */
ADTErr ListPopTail(List *_list, Data* _data)
{
	Node **toRemove = NULL;			

	assert(_list);
	if (_list->m_head.m_next == &_list->m_tail) {
		return ERR_EMPTY;
	}

	toRemove = &_list->m_tail.m_prev;

	/* change pointers in prev. & next nodes */
	 _list->m_tail.m_prev = (*toRemove)->m_prev;			
	(*toRemove)->m_prev->m_next = &_list->m_tail;

	/* get data & free node */
	*_data = NodeDestroy(*toRemove);
	
	return ERR_OK;
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
	if( NULL == _itr->m_prev->m_data) {
		return NULL;
	}
	return _itr->m_prev;
}
/*----------------------------------------------------------*/



/* get data */
/* getting data from tail - UNDEFINED BEHAVIOR */
Data	ListGetData	(ListItr _itr)
{
	if( (Data)MAGIC == _itr->m_data ) {
		return NULL;
	}
	return _itr->m_data;
}
/*----------------------------------------------------------*/



/* insert before _itr, returns itr to inserted data */
/* if failed, return original _itr */
ListItr		ListInsertBefore	(ListItr _itr, Data _data)
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
Data	ListRemove	(ListItr _itr)
{
	Data data;
	
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
ListItr ListFindFirst(ListItr _from, ListItr _to, CompFunc _cmpfunc, Data _data)
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


/* create a list node from data & ptrs */
/* return new node ar NULL for failure */
Node* NodeCreate(Data _data, Node* _next, Node* _prev)
{
	Node* newNode = malloc(sizeof(Node));
	if(!newNode) {
		return NULL;
	}
	
	newNode->m_data = _data;
	newNode->m_next = _next;
	newNode->m_prev = prev;
	
	return newNode;
}
/*----------------------------------------------------------*/


/* destroy a node, return data */
Data NodeDestroy(Node* _node)
{
	Data removedData;
	
	removedData = _node->m_data;
	free(_node);
	
	return removedData
}
/*----------------------------------------------------------*/



/* free all nodes recursively */
/* for use in function ListFree */
void ListDestroyImp(Node *_node, List *_list)
{	
	
	/* stop condition: when current node points to end node */
	if( _node->m_next == &_list->m_tail ) {						
		free(_node);
		return;
	}

	/* recursion, then free node on the way out */
	ListDestroyImp(_node->m_next, _list);	
	free(_node);
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



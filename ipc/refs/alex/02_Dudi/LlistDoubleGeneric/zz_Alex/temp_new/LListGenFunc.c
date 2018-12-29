/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-19    
    Last modified date:		2013-07-23
    Description: double linked list functions.
    			 update 21/7: now for generic data using typedef void* Data.
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

typedef struct Node
{
	Data			m_data;
	struct Node*	m_next;
	struct Node*	m_prev;
} Node;

struct Itr_t
{
	Node*	m_node;
};

struct List_t
{
	Node			m_head;
	Node			m_tail;
	struct Itr_t	m_end;
};


static Node* 	NodeCreate		(Data _data, Node* _next, Node* _prev);
static Data 	NodeDestroy		(Node* _node);
static void 	NodePrint		(Node *_node, List *_list, PrintFunc _printFunc);

static Itr 		ItrCreate		(List* _list, Node* _node);

/*----------------------------------------------------------*/




/*----------------------------------------------------------*/
/*				API functions - General						*/
/*----------------------------------------------------------*/



/* allocate place for list and create begin & end nodes */
List* ListCreate(void)
{
	/* allocate & check */
	List *newList = (List *)malloc(sizeof(List));				
	if( !newList ) {
		return NULL;
	}

	/* head node */
	newList->m_head.m_prev = NULL;								
	newList->m_head.m_next = &newList->m_tail;
	newList->m_head.m_data = (Data)MAGIC;
	
	/* tail node */
	newList->m_tail.m_prev = &newList->m_head;
	newList->m_tail.m_next = NULL;
	newList->m_tail.m_data = (Data)MAGIC;
	
	/* end iterator */
	newList->m_end.m_node = &(newList->m_tail);

	return newList;
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
	newNode->m_prev->m_next = newNode;

	return ERR_OK;
}
/*----------------------------------------------------------*/


/* remove from beginning, get data & free node */
Data ListPopHead(List *_list)
{
	Node **toRemove = NULL; 
	Data headData;
	
	assert(_list);
	
	if (_list->m_head.m_next == &_list->m_tail) {
		return NULL;
	}

	toRemove = &_list->m_head.m_next;
	
	/* change pointers in prev. & next nodes */
	_list->m_head.m_next = _list->m_head.m_next->m_next;			
	_list->m_head.m_next->m_prev = &_list->m_head;
	
	/* get data & free node */
	headData = NodeDestroy(*toRemove);

	return headData;
}
/*----------------------------------------------------------*/


/* remove from end, get data & free node  */
Data ListPopTail(List *_list)
{
	Node **toRemove = NULL;	
	Data tailData;	

	assert(_list);
	if (_list->m_head.m_next == &_list->m_tail) {
		return NULL;
	}

	toRemove = &_list->m_tail.m_prev;

	/* change pointers in prev. & next nodes */
	 _list->m_tail.m_prev = (*toRemove)->m_prev;			
	(*toRemove)->m_prev->m_next = &_list->m_tail;

	/* get data & free node */
	tailData = NodeDestroy(*toRemove);
	
	return tailData;
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
Itr		ListBegin	(List *_list)
{
	Itr newItr;
	
	assert(_list);
	newItr = ItrCreate(_list, _list->m_head.m_next);
	if(!newItr) {
		return &_list->m_end;
	}
	
	return newItr;	
}
/*----------------------------------------------------------*/


/* get iter to past-last (tail) node */
Itr		ListEnd		(List *_list)
{
	Itr newItr;
	
	assert(_list);
	newItr = ItrCreate(_list, &(_list->m_tail));
	if(!newItr) {
		return &_list->m_end;
	}
	
	return newItr;	
}
/*----------------------------------------------------------*/


/* go to next node */
/*** going beyond boundaries results in UNDEFINED BEHAVIOR ***/
Itr		ListNext	(Itr _itr)
{
	_itr->m_node = _itr->m_node->m_next;
	
	return _itr;
}
/*----------------------------------------------------------*/


/* go to previous node */
/*** going beyond boundaries results in UNDEFINED BEHAVIOR ***/
Itr		ListPrev	(Itr _itr)
{
	if( NULL == _itr->m_node->m_prev->m_data) {
		return NULL;
	}
	
	_itr->m_node = _itr->m_node->m_prev;
	return _itr;
}
/*----------------------------------------------------------*/



/* get data */
/* getting data from tail - UNDEFINED BEHAVIOR */
Data	ListGetData	(Itr _itr)
{
	if( (Data)MAGIC == _itr->m_node->m_data ) {
		return NULL;
	}
	return _itr->m_node->m_data;
}
/*----------------------------------------------------------*/



/* insert before _itr, returns itr to inserted data */
/* if failed, return original _itr */
Itr		ListInsertBefore	(Itr _itr, Data _data)
{
	Node*	newNode;
	
	newNode = malloc( sizeof(Node) );
	if(!newNode) {
		return _itr;
	}
	
	newNode->m_data = _data;	
	_itr->m_node->m_prev = newNode;
	newNode->m_next = _itr->m_node;
	
	return ListPrev(_itr);
}
/*----------------------------------------------------------*/



/* remove node & return data */
/* if failed - UNDEFINED BEHAVIOR */
Data	ListRemove	(Itr _itr)
{
	Data removedData;
	
	if( _itr->m_node->m_data == (void*)MAGIC ) {
		return NULL;
	}
	
	removedData = _itr->m_node->m_data;
	
	_itr->m_node->m_prev->m_next = _itr->m_node->m_next;
	_itr->m_node->m_next->m_prev = _itr->m_node->m_prev;
	
	free(_itr);
	return removedData;
}
/*----------------------------------------------------------*/



/* search for _data between _from and _to, not including _to */
/* return iterator of required data if found, _to if not */
/* if data duplication, return 1st result found */
Itr ListFindFirst(Itr _from, Itr _to, CompFunc _cmpfunc, Data _data)
{
	Itr newItr = _from;
	
	while(newItr != _to && newItr->m_node->m_data != (void*)MAGIC)
	{
		if( 0 == _cmpfunc(newItr->m_node->m_data, _data) ) {
			return newItr;
		}
		newItr = ListNext(newItr);
	}
	return _to;
}
/*----------------------------------------------------------*/



/* apply _dofunc to all nodes between _from and _to (not including _to) */
/* return _to for success, the Itr where failed if failed */
Itr ListForEach(Itr _from, Itr _to, DoFunc _dofunc, void* _params)
{
	Itr newItr = _from;	
	int error;
	
	while(newItr != _to && newItr->m_node->m_data != (void*)MAGIC)
	{
		error = _dofunc(newItr->m_node->m_data, _params);
		if(error) {
			return newItr;
		}
		newItr = ListNext(newItr);
	}
	return _to;
}
/*----------------------------------------------------------*/



/* free an Itr */
void ItrDestroy(Itr _itr)
{
	if(_itr) {
		free(_itr);
	}
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
	newNode->m_prev = _prev;
	
	return newNode;
}
/*----------------------------------------------------------*/


/* destroy a node, return data */
Data NodeDestroy(Node* _node)
{
	Data removedData;
	
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








/* allocate an Itr & assign node. for failure return the list->m_end Itr */
Itr ItrCreate(List* _list, Node* _node)
{
	Itr newItr = malloc(sizeof(struct Itr_t));
	if(!newItr) {
		return &_list->m_end;
	}
	
	newItr->m_node = _node;
	return newItr;
}
/*----------------------------------------------------------*/







/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-24
    Date last modified :	2013-07-24
    Description : Stack Implementation - based on generic linked list
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ADTDefs.h"
#include "LListGenFunc.h"
#include "GenericStack.h"


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


struct Stack
{
	List*	m_list;
};



/*----------------------------------------------------------*/



/*----------------------------------------------------------*/
/*						API functions						*/
/*----------------------------------------------------------*/


/* create stack, return NULL for failure */
Stack*    StackCreate  (void)
{
	Stack*	newStack = NULL;
	List*	newList	 = NULL;
	
	newList = ListCreate();
	if(!newList) {
		return NULL;
	}
	
	newStack = (Stack*)malloc(sizeof(Stack));
	if(!newStack) {
		ListDestroy(newList);
		return NULL;
	}
	
	newStack->m_list = newList;
	return newStack;
}
/*----------------------------------------------------------*/


/* destroy stack */
void      StackDestroy (Stack *_stack)
{
	assert(_stack);
	assert(LIST(_stack));
	
	ListDestroy(LIST(_stack));
	free(_stack);
}
/*----------------------------------------------------------*/


/* push: add item to tail, return success/error messege */
ADTErr    StackPush    (Stack *_stack, void*  _item)
{
	int error;

	assert(_stack);
	
	error = ListPushTail(LIST(_stack), (ListData)_item);
	return error;
}
/*----------------------------------------------------------*/


/* pop: delete last item & return its data */
/* for empty stack returns NULL */
void*     StackPop     (Stack *_stack)
{
	ListData data;
	
	assert(_stack);
	if(ListIsEmpty(LIST(_stack))) {
		return NULL;
	}
	
	data = ListPopTail(LIST(_stack));
	return data;
}
/*----------------------------------------------------------*/


/* top: get data of last item */
/* for empty stack returns NULL */
void*     StackTop     (Stack *_stack)
{
	ListData data;
	
	assert(_stack);
	if(ListIsEmpty(LIST(_stack))) {
		return NULL;
	}
	
	data = ListGetData(ListPrev(ListEnd(LIST(_stack))));
	return data;
}
/*----------------------------------------------------------*/


/*return 1 if stack is empty, 0 otherwise */
int       StackIsEmpty (Stack *_stack)
{
	assert(_stack);
	
	return(ListIsEmpty(LIST(_stack)));
}
/*----------------------------------------------------------*/




/* print - for testing */
void        StackPrint  (Stack*	_stack, PrintFunc _printFunc)
{
	assert(_stack);
	
	ListPrint((void*)LIST(_stack), _printFunc);
}
/*----------------------------------------------------------*/






/*----------------------------------------------------------*/
/*					internal functions						*/
/*----------------------------------------------------------*/







/**************************************************************************************
    Author :
    Creation date :      1.9.10
    Date last modified :
    Description : Stack Implementation. 
***************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "GenericStack.h"
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

struct Stack
{
	List	*m_list;
};


Stack*		StackCreate (void)
{
	Stack *stack;

	stack = (Stack*)malloc(sizeof(Stack));
	if(!stack){
		return NULL;
	}	
	stack->m_list = ListCreate();
	if(!stack->m_list){
		free(stack);
		return NULL;	
	}
	
return stack;
}/*QueueCreate*/



void      StackDestroy (Stack *_stack)
{
	ListDestroy ( _stack->m_list );
}/*QueueDestroy*/


ADTErr    StackPush (Stack *_stack, void*  _item)
{
	return ListPushHead( _stack->m_list, _item);
}/*StackPush*/



void*     StackPop (Stack *_stack)
{
	return ListPopHead ( _stack->m_list);
}/*StackPop*/
		


void*     StackTop (Stack *_stack)
{
	Itr		itr;
	itr = ListBegin ( _stack->m_list);
	return	ListGetData(itr);
}/*StackTop*/



int       StackIsEmpty (Stack *_stack)
{
return ListIsEmpty ( _stack->m_list);
}


void      StackPrint (Stack* _stack, void(*_Print)(void*))
{
	ListPrint ( _stack->m_list, _Print);	
}








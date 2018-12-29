#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"
#include "list.h"


/*------------------------------------------------------------------------------------*/

struct Stack{
	List*  m_list;
};

/*------------------------------------------------------------------------------------*/

Stack* StackCreate(){
	Stack* stack;
	
	stack = malloc(sizeof(Stack));
	if(stack == NULL){
		return NULL;
	}
	
	stack->m_list = ListCreate();
	if(stack->m_list == NULL){
		free(stack);
		return NULL;
	}
	
	return stack;
}

/*------------------------------------------------------------------------------------*/


void StackDestroy (Stack* _stack){
	assert(_stack);

	ListDestroy(_stack->m_list);
	
	free(_stack);
}

/*------------------------------------------------------------------------------------*/

ADTERR StackPush (Stack* _stack, Item _item){
	assert(_stack);
	
	ListPushHead(_stack->m_list , _item);

	return OK;
}


/*------------------------------------------------------------------------------------*/

ADTERR StackPop (Stack* _stack , Item* _item){
	assert(_stack);
	
	/*check if queue is empty*/
	if(ListIsEmpty(_stack->m_list)){
		return UNDERFLOW;
	}

	/*remove*/
	ListPopHead(_stack->m_list , _item);

	return OK;
}

/*------------------------------------------------------------------------------------*/

int IsStackEmpty (Stack* _stack){
	assert(_stack);
	
	return (ListIsEmpty(_stack->m_list));	
}

/*------------------------------------------------------------------------------------*/

Item StackTop(Stack* _stack)
{
	return ListBegin(_stack->m_list);

}

/*------------------------------------------------------------------------------------*/


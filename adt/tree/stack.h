/**************************************************************************************
    Author :				Alex Katz
    Creation date :      	10.1.12
    Date last modified :
    Description : 			Circular Queue Implementation. Using fixed length vector and
							data item stored in the vector.
***************************************************************************************/
#ifndef __STACK_H__
#define __STACK_H__
#include "err.h"

typedef void* Item;
typedef struct Stack Stack;
typedef void(*PrintFunc)(Item _item);

/*
**	size   - max number of elements in the queue
**	memory - where to allocate queue elements
*/

Stack*   StackCreate ();
void	 StackDestroy(Stack* _queue);

ADTERR	 StackPush (Stack* _stack, Item _item);
ADTERR	 StackPop (Stack* _stack , Item* _item);

Item    StackTop (Stack* _stack);

int		 IsStackEmpty(Stack* _stack);


#endif /*__STACK_H__*/

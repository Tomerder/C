/**************************************************************************************
    Author :	(Stav Ofer)
    Creation date :      	1.9.10 (2013-07-24)
    Date last modified :	2013-07-24
    Description : Stack Implementation - based on generic linked list/
***************************************************************************************/
#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>
#include <stdio.h>

#include "ADTDefs.h"
#include "LListGen.h"


typedef struct Stack Stack;

/* create stack, return NULL for failure */
Stack*    StackCreate  (void);
/* destroy stack */
void      StackDestroy (Stack *_stack);

/* push: add item to tail, return success/error messege */
ADTErr    StackPush    (Stack *_stack, void*  _item);
/* pop: delete last item & return its data */
/* for empty stack returns NULL */
void*     StackPop     (Stack *_stack);
/* top: get data of last item */
/* for empty stack returns NULL */
void*     StackTop     (Stack *_stack);

/*return 1 if stack is empty, 0 otherwise */
int       StackIsEmpty (Stack *_stack);

/*  Unit-Test functions  */
void      StackPrint   (Stack* _stack, void(*_Print)(void*));

#endif /*__STACK_H__*/

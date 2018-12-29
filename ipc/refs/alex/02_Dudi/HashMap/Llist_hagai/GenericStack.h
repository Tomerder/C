/**************************************************************************************
    Author :
    Creation date :      1.9.10
    Date last modified :
    Description : Stack Implementation. 
***************************************************************************************/

#ifndef __STACK_H__
#define __STACK_H__
#include "ADTErr.h"

typedef struct Stack Stack;

Stack*    StackCreate  (void);
void      StackDestroy (Stack *_stack);
ADTErr    StackPush    (Stack *_stack, void*  _item);
void*     StackPop     (Stack *_stack);
void*     StackTop     (Stack *_stack);
int       StackIsEmpty (Stack *_stack);

/*  Unit-Test functions  */
void      StackPrint   (Stack* _stack, void(*_Print)(void*));
#endif /*__STACK_H__*/

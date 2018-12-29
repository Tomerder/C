/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-24
    Date last modified :	2013-07-24
    Description : Stack Implementation - based on generic linked list
    			  basic testing
***************************************************************************************/

#include <stdio.h>

#include "ADTDefs.h"
#include "LListGen.h"
#include "GenericStack.h"

static void 	PrintInt		(void *_item);

/*---------------------------------------------------------*/

int main()
{
	Stack* myStack;
	int arr[] = {1, 2, 3};
	int item, isEmpty, numItems = 3;
	int i, error = 0;
	
	printf("\ntesting started, printing only list & errors\n");
	
	myStack = StackCreate();
	if(!myStack) {
		printf("allocation failed, line %d\n", __LINE__-2);
		return 1;
	}
	
	for(i=0; i<numItems; ++i) {
		error = StackPush(myStack, (void*)&arr[i]);
		if(error) {
			printf("error, line %d\n", __LINE__-2);
		}
		
		item = *(int*)StackTop(myStack);
		if(item != arr[i]) {
			printf("error, line %d\n", __LINE__-2);
		}
	}
	
	StackPrint(myStack, PrintInt);
	
	isEmpty = StackIsEmpty(myStack);
	if(isEmpty) {
		printf("error, line %d\n", __LINE__-2);
	}
	
	for(i=numItems-1; i>0; --i) {
		item = *(int*)StackPop(myStack);
		if(item != arr[i]) {
			printf("error, line %d\n", __LINE__-2);
		}
	}
	
	isEmpty = StackIsEmpty(myStack);
	if(isEmpty) {
		printf("error, line %d\n", __LINE__-2);
	}
	
	StackPop(myStack);
	isEmpty = StackIsEmpty(myStack);
	if(!isEmpty) {
		printf("error, line %d\n", __LINE__-2);
	}
	
	printf("\ntesting finished\n\n");
	
	StackDestroy(myStack);
	return 0;
}
/*---------------------------------------------------------*/



/* print function: integer */

void PrintInt(void *_item)
{
	printf("%d ", *(int*)_item);
}
/*---------------------------------------------------------*/



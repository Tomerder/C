/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-24
    Date last modified :	2013-07-24
    Description : Queue Implementation - based on generic linked list
    			  basic testing
***************************************************************************************/

#include <stdio.h>

#include "ADTDefs.h"
#include "LListGen.h"
#include "GenericQueue.h"

static void 	PrintInt		(void *_item);

/*---------------------------------------------------------*/

int main()
{
	Queue* myQueue;
	int arr[] = {1, 2, 3};
	int item, isEmpty, numItems = 3;
	int i, error = 0;
	
	printf("\ntesting started, printing only list & errors\n");
	
	myQueue = QueueCreate();
	if(!myQueue) {
		printf("allocation failed, line %d\n", __LINE__-2);
		return 1;
	}
	
	for(i=0; i<numItems; ++i) {
		error = QueueInsert(myQueue, (void*)&arr[i]);
		if(error) {
			printf("error, line %d\n", __LINE__-2);
		}
	}
	
	
	QueuePrint(myQueue, PrintInt);
	
	isEmpty = QueueIsEmpty(myQueue);
	if(isEmpty) {
		printf("error, line %d\n", __LINE__-2);
	}
	
	for(i=0; i<numItems-1; ++i) {
		item = *(int*)QueueRemove(myQueue);
		if(item != arr[i]) {
			printf("error, line %d\n", __LINE__-2);
		}
	}
	
	isEmpty = QueueIsEmpty(myQueue);
	if(isEmpty) {
		printf("error, line %d\n", __LINE__-2);
	}
	
	QueueRemove(myQueue);
	isEmpty = QueueIsEmpty(myQueue);
	if(!isEmpty) {
		printf("error, line %d\n", __LINE__-2);
	}
	
	printf("\ntesting finished\n\n");
	
	QueueDestroy(myQueue);
	return 0;
}
/*---------------------------------------------------------*/



/* print function: integer */

void PrintInt(void *_item)
{
	printf("%d ", *(int*)_item);
}
/*---------------------------------------------------------*/





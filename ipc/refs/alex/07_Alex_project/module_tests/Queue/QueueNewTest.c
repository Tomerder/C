/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-17    
    Last modified date:		2013-09-17
    Description:	some tests for QueueNew module
********************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "QueueNew.h"



#define SIZE			 10

#define ERRPRINT(X)		fprintf(stderr, "error: %s, line: %d\n", X, __LINE__)


int main()
{
	Queue_t* myQueue;
	void* mem = NULL;
	int size;
	int i, error = 0, temp; 
	
	printf("\n\t****** Testing Started ******\n");
	
	size = QueueGetSize(SIZE);
	
	printf("size of 10-item queue: %d\n", size);
	mem = malloc(size);
	if(!mem)
	{
		ERRPRINT("malloc");
		return -1;
	}
	
	printf("\nMaking & initializing a queue\n");							/* initialize */
	
	myQueue = QueueCreate(mem, SIZE);
	
	if ( !myQueue ) {
		ERRPRINT("QueueCreate");
		return -1;
	}
	
	printf("\nchecking if queue is empty, or full: ");						/* isFull, isEmpty */
	if ( IsQueueEmpty(myQueue) ) {		
		printf("Empty.\n");
	} else if ( IsQueueFull(myQueue) ) {					
		printf("Full.\n");
	} else {
		printf("Neither.\n");
	}
	
	
	printf("inserting 3 items to queue:\n");								/* fill partly */
	for( i=1; i<4; i++ ) {
		printf("Inserting item #%d\n", i);
		error += QueueInsert( myQueue, i*10 );
		if ( error ) {
			ERRPRINT("QueueInsert");
			break;
		}
	}
	
	error = 0;
	
	printf("\nprinting the queue:\n");										/* print */
	QueuePrint(myQueue);

	printf("\nchecking if queue is empty, or full: ");						/* isFull, isEmpty */
	if ( IsQueueEmpty(myQueue) ) {		
		printf("Failure: queue is empty.\n");
	} else if ( IsQueueFull(myQueue) ) {					
		printf("Failure: queue is full.\n");
	} else {
		printf("Neither.\n");
	}


	printf("\nFilling queue to capacity, attempting to fill more\n");		/* fill to capacity & more */
	for( i=4; i<SIZE+2; i++ ) {
		printf("Inserting item #%d\n", i);
		error = QueueInsert( myQueue, i*10 );
		if ( error ) {
			if(i!=SIZE+1) {
				ERRPRINT("QueueInsert - overflow handling");
			}
			break;
		}
	}
	
	error = 0;
	
	
	printf("\nchecking IsQueueFull function: ");							/* isFull */
	if ( IsQueueFull(myQueue) == 1 ) {
		printf("Queue is indeed full\n");
	} else if ( IsQueueFull(myQueue) == 0 ) {
		printf("Failure: queue is not full\n");
	} else {
		printf("Failure: general error in function IsQueueFull\n");
	}
	
	QueuePrint(myQueue);													/* print */

	printf("\nRemoving 4 items\n");											/* remove partly */
	for( i=1; i<5; i++ ) {
		printf("removing item #%d.\t", i);
		error = QueueRemove( myQueue, &temp );
		if ( error ) {
			ERRPRINT("QueueRemove");
			break;
		}
		printf("Item removed was: %d\n", temp);
	}

	QueuePrint(myQueue);													/* print */
	
	error = 0;
	
	
	printf("\nRemoving items until queue is empty, attempting to remove more\n");
	for( i=5; i<SIZE+2; i++ ) {
		printf("removing item #%d.\t", i);
		error = QueueRemove( myQueue, &temp );
		if ( error ) {
			if(i!=SIZE+1) {
				ERRPRINT("QueueRemove - underflow handling");
			}
			break;
		}
		printf("Item removed was: %d\n", temp);
	}
	
	error = 0;
	
	printf("\nchecking IsQueueEmpty function: ");							/* isEmpty */
	error = IsQueueEmpty(myQueue);
	if ( error == 1 ) {
		printf("Queue is indeed empty\n");
	} else if ( error == 0 ) {
		printf("Failure: queue is not empty\n");
	} else {
		printf("Failure: general error in function IsQueueEmpty\n");
	}

	printf("\ntrying to print empty queue:\n");
	QueuePrint(myQueue);													/* print */
	
	error = 0;
	
	printf("\nTesting wrap-around capability:\nfilling the queue to capacity, then removing 4 items & adding 2\n");
	for( i=1; i<=SIZE; i++ ) {
		printf("Inserting item #%d\n", i);
		error = QueueInsert( myQueue, i*10 );
		if ( error ) {
			ERRPRINT("QueueInsert");
			break;
		}
	}
	
	error = 0;
	
	for( i=1; i<5; i++ ) {
		printf("removing item #%d.\t", i);
		error = QueueRemove( myQueue, &temp );
		if ( error ) {
			ERRPRINT("QueueRemove");
			break;
		}
		printf("Item removed was: %d\n", temp);
	}
	
	error = 0;
	
	for( i=11; i<13; i++ ) {
		printf("Inserting item #%d\n", i);
		error = QueueInsert( myQueue, i*10 );
		if ( error ) {
			ERRPRINT("QueueInsert");
			break;
		}
	}
	printf("\n");

	QueuePrint(myQueue);													/* print */
	
	error = 0;
	
	printf("\nRemoving 4 items and adding 5\n");
	for( i=5; i<9; i++ ) {
		printf("removing item #%d.\t", i);
		error = QueueRemove( myQueue, &temp );
		if ( error ) {
			ERRPRINT("QueueInsert");
			break;
		}
		printf("Item removed was: %d\n", temp);
	}
	
	error = 0;
	
	for( i=13; i<19; i++ ) {
		printf("Inserting item #%d\n", i);
		error = QueueInsert( myQueue, i*10 );
		if ( error ) {
			ERRPRINT("QueueInsert");
			break;
		}
	}
	printf("\n");

	QueuePrint(myQueue);		
	
	QueueDestroy(myQueue);
	
	free(mem);
	
	printf("\n\n\t****** Testing finished ******\n\n\n");		
		
	return 0;
}

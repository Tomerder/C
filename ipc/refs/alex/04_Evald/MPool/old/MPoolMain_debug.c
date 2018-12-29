/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-08    
    Last modified date:		2013-08-09
    Description: 
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "MPoolFunc.h"


/*######################################################################*/
/*				Definitions & static foreward declarations				*/
/*######################################################################*/


#define MEMSIZE		42

typedef unsigned int ui;

/* print integer value of each byte in memory chunk */
static void 	MemMapPrint	(MPool* _mPool);

/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/

int main()
{
	void* memChunk = NULL;	
	MPool *myPool = NULL;
	
	void* block1 = NULL;
	void* block2 = NULL;
	void* block3 = NULL;
	void* block4 = NULL;
/*	void* block5 = NULL;
*/	
	int size1 = 18;
	int size2 = 4;
	int size3 = 4;
	int size4 = 8;
/*	int size5;
*/
	
	/* get memory chunk */
	memChunk = malloc(sizeof(char) * MEMSIZE);
	if(!memChunk)
	{
		fprintf(stderr, "Error, malloc failed\n");
		return 1;
	}
	
	printf("memChunk allocated\n");
	
	/* initialize as memory pool */
	myPool = MPoolInit(memChunk, MEMSIZE);
	
	printf("pool initialized\n");
	printf("memChunk: %u\tmyPool: %u\t  size: %d\n", (ui)memChunk, (ui)myPool, *(int*)myPool);
	
	MemMapPrint(myPool);
	
	/* allocate 1st block */
	block1 = MPoolAlloc(myPool, size1);
	if(!block1)
	{
		printf("allocation failed, line %d\n", __LINE__-2);
		return 0;
	}
	
	printf("\nblock1 allocated");
	MemMapPrint(myPool);
	
	printf("block1: %u\tsize: %d\nnext: %u\tsize: %d\n", (ui)block1, *(int*)( (void*)( (ui)block1-4 ) ), (ui)( (void*) ( (ui)block1 + size1 ) ), *(int*)( (void*) ( (ui)block1 + size1 ) ) );
	

	/* allocate 2nd block */
	block2 = MPoolAlloc(myPool, size2);
	if(!block2)
	{
		printf("allocation failed, line %d\n", __LINE__-2);
		return 0;
	}
	printf("\nblock2 allocated");
	MemMapPrint(myPool);
	
	printf("block2: %u\tsize: %d\nnext: %u\tsize: %d\n", (ui)block2, *(int*)( (void*)( (ui)block2-4 ) ), (ui)( (void*) ( (ui)block2 + size2 ) ), *(int*)( (void*) ( (ui)block2 + size2 ) ) );
	
	
	/* allocate 3rd block */
	block3 = MPoolAlloc(myPool, size3);
	if(!block3)
	{
		printf("\nallocation failed, line %d\n", __LINE__-2);
	} else {
		printf("\nblock3 allocated");
		MemMapPrint(myPool);
	
		printf("block3: %u\tsize: %d\nnext: %u\tsize: %d\n", (ui)block3, *(int*)( (void*)( (ui)block3-4 ) ), (ui)( (void*) ( (ui)block3 + size3 ) ), *(int*)( (void*) ( (ui)block3 + size3 ) ) );
	}
	
	
	/* free block 1 */
	MPoolFree(myPool, block1);
	printf("\nblock1 freed\n");
	MemMapPrint(myPool);
	
	/* try again to allocate 3rd block */
	block3 = MPoolAlloc(myPool, size3);
	if(!block3)
	{
		printf("\nallocation failed, line %d\n", __LINE__-2);
	} else {
		printf("\nblock3 allocated");
		MemMapPrint(myPool);
	
		printf("block3: %u\tsize: %d\nnext: %u\tsize: %d\n", (ui)block3, *(int*)( (void*)( (ui)block3-4 ) ), (ui)( (void*) ( (ui)block3 + size3 ) ), *(int*)( (void*) ( (ui)block3 + size3 ) ) );
	}
	
	
	/* free block 2 - expected defrag */
	MPoolFree(myPool, block2);
	printf("\nblock2 freed\n");
	MemMapPrint(myPool);	
	
	
	/* allocate 4th block  - expected defrag */
	block4 = MPoolAlloc(myPool, size4);
	if(!block4)
	{
		printf("\nallocation failed, line %d\n", __LINE__-2);
	} else {
		printf("\nblock4 allocated");
		MemMapPrint(myPool);
	
		printf("block4: %u\tsize: %d\nnext: %u\tsize: %d\n\n", (ui)block4, *(int*)( (void*)( (ui)block4-4 ) ), (ui)( (void*) ( (ui)block4 + size4 ) ), *(int*)( (void*) ( (ui)block4 + size4 ) ) );
	}
	
	
	free(memChunk);
	return 0;
}




/*######################################################################*/
/*							internal functions							*/
/*######################################################################*/


/* print integer value of each byte in memory chunk */
void MemMapPrint(MPool* _mPool)
{
	int i;
	
	printf("\n");
	for(i=0; i< MEMSIZE; ++i)
	{
		printf("%2d ", (int)( *(char*)( (ui)_mPool + i ) ) );
	}
	printf("\n");
}




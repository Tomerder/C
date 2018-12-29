/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-09    
    Last modified date:		2013-08-10
    Description: 	test function for MPool unit test.
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "MPoolFunc.h"




/*######################################################################*/
/*					Test functions foreward declaration					*/
/*######################################################################*/

int MPoolTestInit();
int MPoolTestAlloc();
int MPoolTestFreeDefrag();


/*######################################################################*/
/*				Definitions & static foreward declaration				*/
/*######################################################################*/


typedef unsigned int ui;

/* print integer value of each byte in memory chunk */
static void 	MemMapPrint	(MPool* _mPool);

/* handling of intermediate errors, printing error messeges in debug mode */
static void 	ErrorCheckPrint	(int *_errTemp, int *_errTot, char *_text, int _number);
static void 	PrintDebug		(char *_text, int _number);


/*######################################################################*/
/*							TEST FUNCTIONS								*/
/*																		*/
/* 			return 0 for success, positive # for failure,				*/
/* 			-1 for error unrelated to tested function(s)				*/
/*######################################################################*/



/*######################################################################*/
/* 		Test initialization for sizes above, at, and below threshold 	*/
/*######################################################################*/
int MPoolTestInit()
{
	MPool *myPool = NULL;
	void* memChunk = NULL;	
	void* memNotBoundary = NULL;
	
	int sizes[4] = {20, 100, 1000, 12};
	int i;
	int errorTemp = 0, errorTot = 0;


	/*** "normal" sizes, minimal size in i=3 ***/
	
	for(i=0; i<4; ++i)
	{
		/* get memory chunk */
		memChunk = malloc(sizeof(char) * sizes[i]);
		if(!memChunk)
		{
			fprintf(stderr, "Error: malloc failed, %s: line %d, %d bytes\n", __FILE__, __LINE__-2, sizes[i]);
			return -1;
		}
		PrintDebug("memory chunk allocated for size of: ", sizes[i]);
	
		/* initialize as memory pool */
		myPool = MPoolInit(memChunk, sizes[i]);
		PrintDebug("pool initialized", -1);
		if(i==0 || i==3)	{	MemMapPrint(myPool);	}
		
		/* check initialization */
		errorTemp += ( (ui)memChunk != (ui)myPool );	/* myPool not at beginning of chunck */
		errorTemp += ( *(int*)myPool != sizes[i]-8 );	/* incorrect size in myPool */
		errorTemp += ( *(int*)( (ui)myPool + 4) != sizes[i]-8 );	/* incorrect size at block beginning */
		
		ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolInit, line ", __LINE__-2);
		
		free(memChunk);
	}
	
	
	/*** setting start point not at a boundary - expected to move to a boundary & decrease size ***/
	
	/* get memory chunk */
	memChunk = malloc(sizeof(char) * sizes[0]);
	if(!memChunk)
	{
		fprintf(stderr, "Error: malloc failed, %s: line %d, %d bytes\n", __FILE__, __LINE__-2, sizes[0]);
		return -1;
	}
	PrintDebug("memory chunk allocated for size of: ", sizes[0]);
	
	/* get ptr to next byte & use it for MPool */
	memNotBoundary = (void*)( (ui)memChunk + 1 );
	
	/* initialize as memory pool */
	myPool = MPoolInit(memNotBoundary, sizes[0]);
	
	/* check initialization */
	errorTemp += ( (ui)myPool - (ui)memNotBoundary != 3 );	/* myPool not where expected */
	errorTemp += ( *(int*)myPool != (sizes[0] - 8 - 3) );	/* incorrect size in myPool */
	errorTemp += ( *(int*)( (ui)myPool + 4) != sizes[0]-8 - 3 );	/* incorrect size at block beginning */
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolInit for starting not an a boundary, line ", __LINE__-6);
	
	MemMapPrint(myPool);
	PrintDebug(" ", -1);
	
	free(memChunk);
	return errorTot;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/* 						Test allocation w/o defrag 						*/
/*######################################################################*/
int MPoolTestAlloc()
{
	MPool *myPool = NULL;
	void* memChunk = NULL;	
	void* blocks[3] = {NULL};
	int size = 38;
	int errorTemp = 0, errorTot = 0;
	
	
	/* initialize pool */
	memChunk = malloc(sizeof(char) * size);
	if(!memChunk)
	{
		fprintf(stderr, "Error: malloc failed, %s: line %d, %d bytes\n", __FILE__, __LINE__-3, size);
		return -1;
	}
	
	myPool = MPoolInit(memChunk, size);
	if(!myPool)
	{
		PrintDebug("Error: initialization failed, line ", __LINE__-3);
		return -1;
	}
	PrintDebug(" ", -1);
	
	/*** attempt to allocate - too large ***/
	blocks[0] = MPoolAlloc(myPool, 32);
	errorTemp += ( blocks[0] != NULL);
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolAlloc, allocated too-large size, line ", __LINE__-2);
	
	
	/*** allocate - divisible by 4 ***/
	blocks[0] = MPoolAlloc(myPool, 8);
	errorTemp += (!blocks[0]);								/* allocation failed */
	errorTemp += ( (ui)blocks[0] != (ui)myPool + 8 );		/* address is not at 1st block */
	errorTemp += ( *(int*)(void*)( (ui)blocks[0] - 4 ) != -8 );	/* size is wrong */
	errorTemp += ( *(int*)(void*)( (ui)blocks[0] + 8 ) != 18 );	/* next block size is wrong */
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolAlloc, 1st allocation, line ", __LINE__-5);
	
	MemMapPrint(myPool);
	
	
	/*** allocate - non-divisible by 4 ***/
	blocks[1] = MPoolAlloc(myPool, 5);
	errorTemp += (!blocks[1]);									/* allocation failed */
	errorTemp += ( (ui)blocks[1] != (ui)blocks[0] + 12 );		/* address is not where should be */
	errorTemp += ( *(int*)(void*)( (ui)blocks[1] - 4 ) != -8 );	/* size is wrong */
	errorTemp += ( *(int*)(void*)( (ui)blocks[1] + 8 ) != 6 );	/* next block size is wrong */
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolAlloc, 2nd allocation, line ", __LINE__-5);
	
	MemMapPrint(myPool);
	
	/*** allocate - close to end so should take all remaining memory ***/
	blocks[2] = MPoolAlloc(myPool, 4);
	errorTemp += (!blocks[2]);									/* allocation failed */
	errorTemp += ( (ui)blocks[2] != (ui)blocks[1] + 12 );		/* address is not where should be */
	errorTemp += ( *(int*)(void*)( (ui)blocks[2] - 4 ) != -6 );	/* size is wrong */
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolAlloc, 3rd allocation, line ", __LINE__-4);
	
	MemMapPrint(myPool);
	PrintDebug(" ", -1);
	
	free(memChunk);
	return errorTot;
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/* 				Test free and allocation with defrag					*/
/*######################################################################*/
int MPoolTestFreeDefrag()
{
	MPool *myPool = NULL;
	void* memChunk = NULL;	
	void* blocks[4] = {NULL};
	void* newBlock = NULL;
	int size = 36, blocksize = 4;
	int i, errorTemp = 0, errorTot = 0;
	
	/* initialize pool */
	memChunk = malloc(sizeof(char) * size);
	if(!memChunk)
	{
		fprintf(stderr, "Error: malloc failed, %s: line %d, %d bytes\n", __FILE__, __LINE__-3, size);
		return -1;
	}
	
	myPool = MPoolInit(memChunk, size);
	if(!myPool)
	{
		PrintDebug("Error: initialization failed, line ", __LINE__-3);
		return -1;
	}
	
	/* allocate until full */
	for(i=0; i<4; ++i)
	{
		blocks[i] = MPoolAlloc(myPool, blocksize);
		if(!blocks[i])
		{
			PrintDebug("Error: MPoolAlloc failed, line ", __LINE__-3);
			return -1;
		}
	}
	PrintDebug(" ", -1);
	MemMapPrint(myPool);
	
	/*** free one block (i=1) ***/
	
	MPoolFree(myPool, blocks[1]);
	errorTemp += ( *(int*)(void*)( (ui)blocks[1] - 4 ) != 4 );	/* not marked as free */
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolFree, line ", __LINE__-2);
	MemMapPrint(myPool);
		
	/*** allocate again block ***/
	
	blocks[1] = MPoolAlloc(myPool, blocksize);
	errorTemp += ( blocks[1] == NULL );							/* re-allocation failed */
	errorTemp += ( *(int*)(void*)( (ui)blocks[1] - 4 ) != -4 );	/* not marked properly */
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolAlloc after Free, line ", __LINE__-3);
	MemMapPrint(myPool);
	
	/*** free another block ***/
	
	MPoolFree(myPool, blocks[2]);
	errorTemp += ( *(int*)(void*)( (ui)blocks[2] - 4 ) != 4 );	/* not marked as free */
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolFree, line ", __LINE__-2);
	MemMapPrint(myPool);
	
	/*** free block before free one - should defrag ***/
	
	MPoolFree(myPool, blocks[1]);
	errorTemp += ( *(int*)(void*)( (ui)blocks[1] - 4 ) != 12 );	/* error with defrag */
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolFree with defrag, line ", __LINE__-2);
	MemMapPrint(myPool);
	
	/*** free following block - not expecting defrag ***/
	
	MPoolFree(myPool, blocks[3]);
	errorTemp += ( *(int*)(void*)( (ui)blocks[3] - 4 ) != 4 );	/* not marked as free */
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolFree, line ", __LINE__-2);
	MemMapPrint(myPool);
	
	/*** allocate - expecting defrag ***/
	
	newBlock = MPoolAlloc(myPool, 8);
	errorTemp += ( newBlock == NULL );							/* re-allocation failed */
	errorTemp += ( *(int*)(void*)( (ui)newBlock - 4 ) != -8 );	/* not marked properly */
	errorTemp += ( *(int*)(void*)( (ui)newBlock + 8 ) != 8 );	/* error with defrag */
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: MPoolAlloc with defrag, line ", __LINE__-4);
	MemMapPrint(myPool);
	
	PrintDebug(" ", -1);
	
	free(memChunk);
	return errorTot;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							INTERNAL FUNCTIONS							*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* print integer value of each byte in memory chunk - debug mode only 	*/
/*----------------------------------------------------------------------*/
void MemMapPrint(MPool* _mPool)
{
#ifdef DEBUG
	int i;
	
	for(i=0; i< *(int*)_mPool + 8; ++i)
	{
		printf("%2d ", (int)( *(char*)( (ui)_mPool + i ) ) );
		if(! ((i+1)%4) )	{	printf("| ");	}
	}
	printf("\n");
#endif
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* for printing in debug mode */
/*----------------------------------------------------------------------*/
static void PrintDebug(char *_text, int _number)
{
#ifdef DEBUG
	printf("%s", _text);
	if( _number != -1 ) {
		printf("%d", _number);
	}
	printf("\n");
#endif
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* error handling for test functions */
/*----------------------------------------------------------------------*/
static void ErrorCheckPrint(int *_errTemp, int *_errTot, char *_text, int _number)
{
	if(*_errTemp)
	{
		PrintDebug(_text, _number);
		*_errTot += *_errTemp;
		*_errTemp = 0;
	}
}
/*----------------------------------------------------------------------*/


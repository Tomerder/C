/*************************************************************/
/** 	Project:	FreeList                 				**/
/** 	Author:		Tomer Dery								**/
/** 	Created:	28/1/2112								**/
/** 	Updated:	28/1/2112								**/
/*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

/*------------------------------------------------------------------------------------*/

#include "FreeList.h"

/*------------------------------------------------------------------------------------*/

enum {FALSE, TRUE};
enum {FAIL, PASS};

char* resultArr[2] = {"FAIL","PASS"};

#define SUCCESS 1

#define DEBUG 0

/*------------------------------------------------------------------------------------*/

void UniTest1(size_t blockSize, size_t numOfBlocks); /*check Create & destroy*/
void UniTest2(size_t blockSize, size_t numOfBlocks); /*check malloc*/
void UniTest3(size_t blockSize, size_t numOfBlocks); /*check free*/
void UniTest4(size_t blockSize, size_t numOfBlocks); /*check out of blocks to allocate - malloc on empty list*/
void UniTest5(size_t blockSize, size_t numOfBlocks); /*check free on empty list - add block to list*/

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	size_t blockSize=8;
	size_t numOfBlocks=5;
	
	printf("/*-----------------------------------------------------------*/\n");
	
	UniTest1(blockSize, numOfBlocks);

	UniTest2(blockSize, numOfBlocks);

	UniTest3(blockSize, numOfBlocks);

	UniTest4(blockSize, numOfBlocks);

	UniTest5(blockSize, numOfBlocks);
		
    return 0; 	
}

/*------------------------------------------------------------------------------------*/

void UniTest1(size_t _blockSize, size_t _numOfBlocks)
{
	int resultIndex = PASS;

	FreeList* freeList = FreeListCreate(_blockSize, _numOfBlocks);

	if(FreeListCount(freeList) != _numOfBlocks){
		resultIndex = FAIL;
	}

	printf("/*check Create & destroy*/\n");
	printf("UnitTest1: %s\n", resultArr[resultIndex]);
	printf("/*-----------------------------------------------------------*/\n");	
	
	FreeListDestroy(freeList);
}

/*------------------------------------------------------------------------------------*/

void UniTest2(size_t _blockSize, size_t _numOfBlocks)
{
	int resultIndex = PASS;

	FreeList* freeList = FreeListCreate(_blockSize, _numOfBlocks);

	FreeListAlloc(freeList);	

	if(FreeListCount(freeList) != _numOfBlocks - 1){
		resultIndex = FAIL;
	}

	printf("/*check malloc*/\n");
	printf("UnitTest2: %s\n", resultArr[resultIndex]);
	printf("/*-----------------------------------------------------------*/\n");	

	FreeListDestroy(freeList);
}

/*------------------------------------------------------------------------------------*/

void UniTest3(size_t _blockSize, size_t _numOfBlocks)
{
	int resultIndex = PASS;
	void* block = NULL;

	FreeList* freeList = FreeListCreate(_blockSize, _numOfBlocks);

	block = FreeListAlloc(freeList);	

	FreeListFree(freeList, block);

	if(FreeListCount(freeList) != _numOfBlocks){
		resultIndex = FAIL;
	}

	printf("/*check free*/\n");
	printf("UnitTest3: %s\n", resultArr[resultIndex]);
	printf("/*-----------------------------------------------------------*/\n");	

	FreeListDestroy(freeList);
}

/*------------------------------------------------------------------------------------*/

void UniTest4(size_t _blockSize, size_t _numOfBlocks)
{
	int i , resultIndex = PASS;
	void* block = NULL;

	FreeList* freeList = FreeListCreate(_blockSize, _numOfBlocks);

	for(i=0; i<_numOfBlocks; i++){
		block = FreeListAlloc(freeList);
		if(block == NULL){
			resultIndex = FAIL;
		}	
	}

	block = FreeListAlloc(freeList);
	if(block != NULL){
		resultIndex = FAIL;
	}	


	printf("/*check out of blocks to allocate - malloc on empty list*/\n");
	printf("UnitTest4: %s\n", resultArr[resultIndex]);
	printf("/*-----------------------------------------------------------*/\n");	

	FreeListDestroy(freeList);
}


/*------------------------------------------------------------------------------------*/

void UniTest5(size_t _blockSize, size_t _numOfBlocks)
{
	int i , resultIndex = PASS;
	void* block = NULL;

	FreeList* freeList = FreeListCreate(_blockSize, _numOfBlocks);

	for(i=0; i<_numOfBlocks; i++){
		block = FreeListAlloc(freeList);
	}

	if(FreeListCount(freeList) != 0){
		resultIndex = FAIL;
	}

	FreeListFree(freeList, block);

	if(FreeListCount(freeList) != 1){
		resultIndex = FAIL;
	}

	printf("/*check free on empty list - add block to list*/\n");
	printf("UnitTest5: %s\n", resultArr[resultIndex]);
	printf("/*-----------------------------------------------------------*/\n");	

	FreeListDestroy(freeList);
}

/*------------------------------------------------------------------------------------*/


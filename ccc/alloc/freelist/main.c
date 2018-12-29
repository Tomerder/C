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

#define NUM_OF_CHECKS 6

/*------------------------------------------------------------------------------------*/
void Test(size_t _blockSize, size_t _numOfBlocks);


void InteractiveTest(size_t _blockSize, size_t _numOfBlocks);

void UniTest1(size_t blockSize, size_t numOfBlocks); /*check Create & destroy*/
void UniTest2(size_t blockSize, size_t numOfBlocks); /*check malloc*/
void UniTest3(size_t blockSize, size_t numOfBlocks); /*check free*/
void UniTest4(size_t blockSize, size_t numOfBlocks); /*check out of blocks to allocate - malloc on empty list*/
void UniTest5(size_t blockSize, size_t numOfBlocks); /*check free on empty list - add block to list*/
void UniTest6(size_t blockSize, size_t numOfBlocks); /*check free*2 on empty list after malloc on empty list*/

typedef void(*TestFunc)(size_t, size_t);
/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	int i;
	size_t blockSize=8;
	size_t numOfBlocks=5;
	TestFunc UniTestToRun[NUM_OF_CHECKS] = {UniTest1, UniTest2, UniTest3, UniTest4, UniTest5, UniTest6};
	
	printf("/*-----------------------------------------------------------*/\n");
	
	for(i=0; i < NUM_OF_CHECKS; i++){
		UniTestToRun[i](blockSize, numOfBlocks);  
	}
	
	printf("/*-----------------------------------------------------------*/\n");
	
	/*InteractiveTest(blockSize, numOfBlocks);*/
		
	printf("/*-----------------------------------------------------------*/\n");	
    
    return 0; 	
}

/*------------------------------------------------------------------------------------*/

void InteractiveTest(size_t _blockSize, size_t _numOfBlocks)
{
	void* block1 = NULL;
	void* block2 = NULL;
	void* block3 = NULL;
	int blockToFree;
	int cont = 1;
	int ch;
	
	FreeList* freeList = FreeListCreate(_blockSize, _numOfBlocks);

	printf("Interactive Test\n");
	printf("count: %d\n", FreeListCount(freeList));
	
	while(cont) {
		printf("1 - malloc\n");
		printf("2 - free\n");
		printf("Please, choose option: ");
	
		scanf("%d",&ch);
		
		switch(ch) {
			case 1 : 	
						block3 = block2;
						block2 = block1;
						block1 = FreeListAlloc(freeList);
						if(block1 == NULL){
							printf("No more blocks to allocate\n");
						}
						printf("count: %d\n", FreeListCount(freeList));
						break;		
			case 2 :  
						printf("enter block to free 1=last 2=before 3= before before: ");
						scanf("%d", &blockToFree);
						switch(blockToFree) {
							case 1 : FreeListFree(freeList, block1); 
									 block1=block2; block2=block3; 
									 printf("count: %d\n", FreeListCount(freeList));   
									 break;	
							case 2 : FreeListFree(freeList, block2); 
									 block2=block3;  
									 printf("count: %d\n", FreeListCount(freeList));
									 break;	
							case 3 : FreeListFree(freeList, block3); 
									 printf("count: %d\n", FreeListCount(freeList));
									 break;	
							default: cont = 0;
						}
						break;	
			
			default: cont = 0;
		}
	}
	
	FreeListDestroy(freeList);

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

void UniTest6(size_t _blockSize, size_t _numOfBlocks)
{
	int i , resultIndex = PASS;
	void* block1 = NULL;
	void* block2 = NULL;
	void* blockNull = NULL;

	FreeList* freeList = FreeListCreate(_blockSize, _numOfBlocks);

	for(i=0; i<_numOfBlocks-1; i++){
		block1 = FreeListAlloc(freeList);
	}

	/*check 1 block is left*/
	if(FreeListCount(freeList) != 1){
		resultIndex = FAIL;
	}

	block2 = FreeListAlloc(freeList);

	/*check empty list*/
	blockNull = FreeListAlloc(freeList);
	if(blockNull != NULL){
		resultIndex = FAIL;
	}

	FreeListFree(freeList, block1);
	FreeListFree(freeList, block2);

	/*check 2 blocks on list*/
	if(FreeListCount(freeList) != 2){
		resultIndex = FAIL;
	}

	printf("/*check free*2 on empty list after malloc on empty list*/\n");
	printf("UnitTest6: %s\n", resultArr[resultIndex]);
	printf("/*-----------------------------------------------------------*/\n");	

	FreeListDestroy(freeList);
}


/*------------------------------------------------------------------------------------*/


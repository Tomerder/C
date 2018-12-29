/*************************************************************/
/** 	Project:	My Malloc.              				**/
/** 	Author:		Tomer Dery								**/
/** 	Created:	27/1/2112								**/
/** 	Updated:	27/1/2112								**/
/*************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "FreeList.h"
/*--------------------------------------------------------------------*/

#define PTR_SIZE sizeof(void*)
#define MIN_ELEM_SIZE PTR_SIZE*2


/*#define NEXT_PTR(PTR) ( PTR = *PTR))  )  */

#define PTR_VAL(PTR)  ( *(void**)PTR )
#define PTR_TO_NEXT(PTR) ( PTR = PTR_VAL(PTR)  )


enum{ FALSE , TRUE };

/*--------------------------------------------------------------------*/

struct FreeList{
	void** m_ptrLastFree;
	void* m_blocks;
};


/*--------------------------------------------------------------------*/

FreeList* FreeListCreate( size_t _elementSize, size_t _nElements)
{
	void* prevBlockAdr = NULL;
	void* adrToWrtTo = NULL;
	FreeList* freeList = NULL;
	int i;
	
	assert(_elementSize >= MIN_ELEM_SIZE);
	assert(_nElements > 0);

	/*alloc freeList struct*/
	freeList = malloc(sizeof(FreeList));
	if(freeList == NULL){
		return NULL;
	}
	
	/*alloc ptr to last free block*/
	freeList->m_ptrLastFree = malloc(sizeof(void*));
	if(freeList->m_ptrLastFree == NULL){
		free(freeList);
		return NULL;
	}

	/*alloc blocks space*/
	freeList->m_blocks = calloc(1, _elementSize * _nElements);
	if(freeList->m_blocks == NULL){
		free(freeList->m_ptrLastFree);
		free(freeList);
		return NULL;
	}

	/*fix free list block pointers*/
	for(i=1; i<_nElements; i++){
		/*start of every block will point to previous one*/
		prevBlockAdr = (char*)freeList->m_blocks + ( (i-1) * _elementSize) ; 
		adrToWrtTo = (char*)freeList->m_blocks + (i * _elementSize) ;

		memcpy( adrToWrtTo , &prevBlockAdr , sizeof(void*) );		
	}
	
	/*set ptr to last free block*/
	*(freeList->m_ptrLastFree) = (char*)freeList->m_blocks + (_elementSize * (_nElements-1));
	
	return freeList;
}

/*--------------------------------------------------------------------*/

void FreeListDestroy( FreeList* _freeList)
{
	assert(_freeList);

	free(_freeList->m_blocks);
	free(_freeList->m_ptrLastFree);
	free(_freeList);
}

/*--------------------------------------------------------------------*/

void* FreeListAlloc( FreeList* _freeList)
{
	void* ptrToRet = NULL;

	assert(_freeList);

	ptrToRet = *_freeList->m_ptrLastFree;

	if(ptrToRet == NULL){
		return NULL;      /*no more blocks to alocate*/
	}

	/*set last as next of cur last*/
	PTR_TO_NEXT(*_freeList->m_ptrLastFree);

	return ptrToRet;
}

/*--------------------------------------------------------------------*/

void FreeListFree( FreeList* _freeList, void* _address)
{
	void* curLastFree = NULL; 
	
	assert(_freeList);
	assert(_address);

	curLastFree = *_freeList->m_ptrLastFree;

	/*link cur last as next of new last*/
	PTR_VAL(_address) = curLastFree;
	
	/*set new last*/
	*_freeList->m_ptrLastFree = _address;

}

/*--------------------------------------------------------------------*/

size_t FreeListCount( FreeList* _freeList)
{
	size_t count=0;	
	void* nextPtr = NULL;	

	assert(_freeList);

	nextPtr = *_freeList->m_ptrLastFree;
	while(nextPtr){
		/*printf("ptr : %x\n", (unsigned)nextPtr);*/	
	
		++count;
		PTR_TO_NEXT(nextPtr);
	}	

	/*printf("------------------\n");*/

	return count;
}

/*--------------------------------------------------------------------*/




















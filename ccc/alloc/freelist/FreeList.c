/*************************************************************/
/** 	Project:	Free List               				**/
/** 	Author:		Tomer Dery								**/
/** 	Created:	27/1/2112								**/
/** 	Updated:	27/1/2112								**/
/*************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "FreeList.h"
/*--------------------------------------------------------------------*/

enum{ FALSE , TRUE };

#define PTR_SIZE sizeof(void*)
#define MIN_ELEM_SIZE PTR_SIZE*2


#define START_OF_BLOCKS(FREE_LIST)  ( (char*)FREE_LIST + PTR_SIZE )


#define PTR_VAL(PTR)  ( *(int*)(PTR) )

#define PTR_ADVANCE_2_NEXT(FREE_LIST, OFFSET) ( OFFSET = PTR_VAL(START_OF_BLOCKS(FREE_LIST) + OFFSET))   


#define BASE_OFFSET_2_PTR(FREE_LIST, OFFSET)  ( (void*) (START_OF_BLOCKS(FREE_LIST) + OFFSET) )

#define PTR_2_OFFSET(FREE_LIST, PTR)  ( (int) ( (char*)PTR - START_OF_BLOCKS(FREE_LIST)) )

/*--------------------------------------------------------------------*/

struct FreeList{
	/*offset from base to last free block*/
	int m_ptrLastFree;
};

/*--------------------------------------------------------------------*/

FreeList* FreeListCreate( size_t _elementSize, size_t _nElements)
{
	size_t prevBlockOffsetFromBase = 0;
	void* adrToWrtTo = NULL;
	FreeList* freeList = NULL;
	int i;
	
	assert(_elementSize >= MIN_ELEM_SIZE);
	assert(_nElements > 0);

	/*alloc freeList struct + blocks space  =>  single allocate = self depend*/
	freeList = calloc(1, sizeof(FreeList) + _elementSize * _nElements);
	if(freeList == NULL){
		return NULL;
	}
	
	/*fix free list block pointers*/
	for(i=1; i<_nElements; i++){
		/*start of every block will point to previous one*/
		prevBlockOffsetFromBase = ( (i-1) * _elementSize)  ; 
		adrToWrtTo = START_OF_BLOCKS(freeList) + (i * _elementSize) ;
	
		PTR_VAL(adrToWrtTo) = prevBlockOffsetFromBase;
	}
	
	/*set ptr to last free block*/
	freeList->m_ptrLastFree = (_elementSize * (_nElements-1));
	
    /* mark end of list => -1 */
	PTR_VAL( START_OF_BLOCKS(freeList) ) = -1;
	

	return freeList;
}

/*--------------------------------------------------------------------*/

void FreeListDestroy( FreeList* _freeList)
{
	assert(_freeList);

	free(_freeList);
}

/*--------------------------------------------------------------------*/

void* FreeListAlloc( FreeList* _freeList)
{
	void* ptrToRet = NULL;

	assert(_freeList);

	if(_freeList->m_ptrLastFree == -1){  
		return NULL;   /*no blocks to allocate*/
	}

	ptrToRet = BASE_OFFSET_2_PTR(_freeList , _freeList->m_ptrLastFree);

	/*set lastPtr as next of cur lastPtr*/
	PTR_ADVANCE_2_NEXT(_freeList ,_freeList->m_ptrLastFree);
	

	return ptrToRet;
}

/*--------------------------------------------------------------------*/

void FreeListFree( FreeList* _freeList, void* _address)
{
	size_t curLastFree = 0; 
	
	assert(_freeList);
	assert(_address);

	curLastFree = _freeList->m_ptrLastFree;

	/*link cur last as next of new last*/
	PTR_VAL(_address) = curLastFree;

	_freeList->m_ptrLastFree = PTR_2_OFFSET(_freeList, _address) ;

}

/*--------------------------------------------------------------------*/

size_t FreeListCount( FreeList* _freeList)
{
	size_t count=0;	
	int nextPtr = 0;	

	assert(_freeList);

	nextPtr = _freeList->m_ptrLastFree;
		
	while(nextPtr >=0){
		/*printf("ptr : %d\n", (unsigned)nextPtr);	*/

		++count;		
		PTR_ADVANCE_2_NEXT(_freeList, nextPtr);
	}	

	/*printf("------------------\n");*/

	return count;
}

/*--------------------------------------------------------------------*/




















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

#include "MyMalloc.h"
/*--------------------------------------------------------------------*/

enum{ FALSE , TRUE };

typedef enum{ FREE , ALLOCATED } Status;


typedef struct MetaData{
	unsigned int m_size : 31;
	unsigned int m_status : 1;
}MetaData;

/*--------------------------------------------------------------------*/

#define ALIGNMENT_SIZE sizeof(MetaData)
#define MIN_INIT_SIZE  (ALIGNMENT_SIZE * 4)

#define ALIGN_SIZE_DOWN(SIZE) (SIZE - SIZE % ALIGNMENT_SIZE)
#define ALIGN_SIZE_UP(SIZE)  (SIZE % ALIGNMENT_SIZE == 0) ? (SIZE) : (SIZE + (ALIGNMENT_SIZE - SIZE % ALIGNMENT_SIZE))
#define ALIGN_ADR(ADR) (void*)((unsigned)(char*)ADR + ((unsigned)(char*)ADR % ALIGNMENT_SIZE))


#define META_SIZE(META) (META->m_size)
#define META_STATUS(META) (META->m_status)


#define NEXT_META(META) ( META + (META_SIZE(META) / ALIGNMENT_SIZE ) + 1 )    

#define IS_LAST_META(META) (META_SIZE(META) == INT_MAX)

/*--------------------------------------------------------------------*/

/*if next meta is free -> merge 2 blocks  
  return TRUE if merge was done	*/
static int TryMerge(MetaData* _meta);

/*return free block by selected algorithem - first fit
  gets parameter - first block meta data	*/ 
static MetaData* GetFreeBlock(MetaData* _meta, size_t _nBytes);

/*split - set next meta data*/ 
static void Split(MetaData* _meta, size_t _nBytes);

/*--------------------------------------------------------------------*/

void* MyMallocInit(void* _mem, size_t _nBytes)
{
	MetaData* firstMeta = NULL;
	MetaData* tailMeta = NULL;
	
	assert(_mem);
	assert(_nBytes >= MIN_INIT_SIZE);

	_mem = ALIGN_ADR(_mem);
	_nBytes = ALIGN_SIZE_DOWN(_nBytes);
	

	/*init memory*/
	memset(_mem , 0 , _nBytes );
	
	/*set head*/
	memcpy(_mem, &_nBytes, ALIGNMENT_SIZE);
	
	/*set first meta*/
	firstMeta = ((MetaData*)_mem + 1);
	META_SIZE(firstMeta) = _nBytes - ALIGNMENT_SIZE*2;

	/*set tail*/ 
	/*memset((char*)_mem + _nBytes , INT_MAX , ALIGNMENT_SIZE);*/
	tailMeta =  ((MetaData*)_mem + _nBytes / ALIGNMENT_SIZE);
	META_SIZE(tailMeta) = INT_MAX;
	META_STATUS(tailMeta) = ALLOCATED;
	
	return _mem;
}

/*--------------------------------------------------------------------*/

void* MyMalloc(void* _mem, size_t _nBytes)
{
	MetaData* meta = NULL;
	
	assert(_mem);
	
	if(_nBytes == 0){
		return NULL;
	}
	
	meta = (MetaData*)_mem;
	
	_nBytes = ALIGN_SIZE_UP(_nBytes);

	/*check total size of head*/
	if(_nBytes > META_SIZE(meta) ){
		return NULL;
	}
	
	/*advance to first meta*/
	meta = meta + 1;
	
	/*search for block to return */
	meta = GetFreeBlock(meta, _nBytes);
	if(meta == NULL){
		return NULL;
	}

	/*set next META - split*/
	Split(meta, _nBytes);

	/*set size and taken*/
	META_SIZE(meta) = _nBytes ;
	META_STATUS(meta) = ALLOCATED;	
	
	return (meta + 1);
}
/*--------------------------------------------------------------------*/

static void Split(MetaData* _meta, size_t _nBytes)
{
	MetaData* nextMeta = NULL;
	size_t splitPos;

	splitPos = (_nBytes / ALIGNMENT_SIZE);
	nextMeta = _meta + splitPos + 1;
	if(IS_LAST_META(nextMeta) || (META_STATUS(nextMeta) == ALLOCATED) ){
		return;
	}
	
	META_SIZE(nextMeta) = META_SIZE(_meta) - _nBytes - ALIGNMENT_SIZE ;
}

/*--------------------------------------------------------------------*/

static MetaData* GetFreeBlock(MetaData* _meta, size_t _nBytes)
{
	while(META_SIZE(_meta) < _nBytes || META_STATUS(_meta) == ALLOCATED){
		if(	IS_LAST_META(_meta)){
			return NULL;    /*block was not found*/
		}
		
		if(META_STATUS(_meta) == FREE){
			if(TryMerge(_meta)){
				continue;
			}
		}
		
		_meta = NEXT_META(_meta);
	}
	
	return _meta;
}

/*--------------------------------------------------------------------*/

void MyFree(void* _ptr)
{
	MetaData* meta = NULL;
	
	if(_ptr == NULL){
		return;
	}
	
	meta = (MetaData*)_ptr - 1;
	
	META_STATUS(meta) = FREE;
	
	TryMerge(meta);
}

/*--------------------------------------------------------------------*/

static int TryMerge(MetaData* _meta)
{
	MetaData* nextMeta = NEXT_META(_meta);
	
	if(IS_LAST_META(nextMeta)){
		return FALSE;
	} 
	
	if(META_STATUS(nextMeta) != FREE){
		return FALSE;
	}
	
	/*merge*/
	META_SIZE(_meta) += META_SIZE(nextMeta) + ALIGNMENT_SIZE;
	META_SIZE(nextMeta) = 0; /*not obligatory*/

	return TRUE;
}

/*--------------------------------------------------------------------*/

/*print all meta data*/
void PrintMem(void* _mem)
{
	/*header*/
	MetaData* meta = (MetaData*)_mem;
	
	/*advance to first meta*/
	meta = meta + 1;
	
	while( !IS_LAST_META(meta) ){
		printf("%d:%d\n", META_SIZE(meta) , META_STATUS(meta) );
		meta = NEXT_META(meta);
	}
	printf("/*------------------------------------------------*/\n");

}


/*print all meta data*/
void PrintMem2(void* _mem)
{
	size_t size;
	int i;

	/*header*/
	MetaData* meta = (MetaData*)_mem;
	size = META_SIZE(meta) / ALIGNMENT_SIZE;	

	printf("header : %d:%d\n", META_SIZE(meta) , META_STATUS(meta) );
	++meta;
	
	for(i=1; i<size; i++){
		printf("%d:%d ** ", META_SIZE(meta) , META_STATUS(meta) );
		++meta;
	}
	
	/*tail*/
	printf("\ntail : %d:%d\n", META_SIZE(meta) , META_STATUS(meta) );
	++meta;
	
	printf("/*------------------------------------------------*/\n");

}



/*--------------------------------------------------------------------*/



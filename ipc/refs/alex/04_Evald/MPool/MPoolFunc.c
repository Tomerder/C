/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-08    
    Last modified date:		2013-08-10
    Description: 	Memory management functions, in method of contiguous allocation.
    				Overhead:	metadata of each block - additional 4 bytes per block,
    							also general metadata in struct MPool.
    				"Weak" prevention of fragmentation: joining 2 adjacent free blocks into one.
    				Size of chunk must be at least sizeof(MPool) + 8
***************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "MPoolFunc.h"


/*######################################################################*/
/*				Definitions & static foreward declarations				*/
/*######################################################################*/

#define BOUND_SIZE		sizeof(void*)	/* size of boundary - 4 byte on 32-bit machines */

typedef unsigned int ui;

/* meta data for memory chunk */
struct MPool
{
	int		m_size;	/* size of memory chunck */
};

/* check if _mem is at a boundary, return ptr to next one */
static void* BoundaryCheck(void* _mem);

/* merge 2 adjoining memory chunks */
static void MergeMem(void* _mem1, void* _mem2);


/*######################################################################*/
/*								MPOOLINIT								*/
/*######################################################################*/

/*	Initialize memory chunck for use.
	Input:	_mem - handle to memory chunck
			_size - size in bytes
	Output: handle to MPool - beginning of memory chunk
*/
MPool* MPoolInit(void *_mem, int _size)
{
	MPool pool;
	void *newMem = NULL;
	
	/* debug error checks */
	assert(_mem);
	assert(_size >= sizeof(MPool) + 8);
	
	/* set all bytes to 0 */
	memset(_mem, 0, _size);
	
	/* get closest following boundary */
	newMem = BoundaryCheck(_mem);

	/* decrease size accordingly */
	if(newMem != _mem)
	{
		_size -= (ui)newMem-(ui)_mem;
	}

	/* write actual free size to metadata - subtract size of mPool struct and of one block-metadata */
	pool.m_size = _size - sizeof(MPool) - BOUND_SIZE;
	
	/* put mPool at beginning of memory chunk */
	*(MPool*)newMem = pool;
	/* put size at beginning of block */
	*(int*)((ui)newMem + sizeof(MPool)) = pool.m_size;
	
	/* return address of mPool */
	return newMem;
}
/*----------------------------------------------------------------------*/



/*######################################################################*/
/*								MPOOLALLOC								*/
/*######################################################################*/
/*	Assign a block of memory to user
	Input:	- MPool
			_size - # of bytes to allocate
	Output:	handle to assigned memory block
			if not enough memory - return NULL
			same if size=0
*/
void* 	MPoolAlloc	(MPool* _mPool, int _size)
{
	int mod = _size % BOUND_SIZE;
	int actual_size = _size;
	int tempSize;
	int blockSize;
	void *blockPtr = NULL, *nextPtr = NULL;
	
	assert(_mPool && _size >= BOUND_SIZE);
	
	/* increase size to closest boundary size */
	_size = ( !mod ) ? _size : _size + BOUND_SIZE - mod;
	/* including metadata */
	actual_size = _size + BOUND_SIZE;
	
	/* check that size is within possibility */
	if(actual_size >= _mPool->m_size)
	{
		return NULL;
	}
	
	
	/* start loop on all memory blocks */
	
	blockPtr = (void*)( (ui)_mPool + sizeof(MPool) );	/* metadata of 1st block */
	blockSize = *(int*)blockPtr;
	
	do {
		/* find next free block */
		while( blockSize < 0 )
		{
			blockPtr = (void*)( (ui)blockPtr + BOUND_SIZE - blockSize );
			blockSize = *(int*)blockPtr;
		} /* end while */
		
		/* check next block and if free - merge */
		nextPtr = (void*)( (ui)blockPtr + BOUND_SIZE + blockSize );
		if( *(int*)nextPtr > 0 && (ui)nextPtr - (ui)_mPool <= _mPool->m_size )
		{
			MergeMem(blockPtr, nextPtr);
			blockSize = *(int*)blockPtr;
		}
		
		/* if found a place - allocate */
		if( blockSize >= _size )
		{	
			/* if block & _size are the same, or difference < size of metadata - assign whole block */
			if( blockSize - _size <= BOUND_SIZE )
			{
				*(int*)blockPtr *= -1;
			}
			else /* split the block */
			{
				tempSize = blockSize - actual_size;
				*(int*)blockPtr = _size*(-1);
				nextPtr = (void*)( (ui)blockPtr + actual_size );
				*(int*)nextPtr = tempSize;
			}
			
			return (void*)( (ui)blockPtr + BOUND_SIZE );
		} /* end if - allocation */
		
	} while((ui)blockPtr < (ui)_mPool+_mPool->m_size);
	
	/* if reached end and didn't find place to allocate */
	return NULL;
}

/*----------------------------------------------------------------------*/



/*######################################################################*/
/*								MPOOLFREE								*/
/*######################################################################*/
/*	Free memory blocks
	Input:	- MPool
			- handle to block
	Output: none
	if block does not belong to MPool, nothing happens
*/
void	MPoolFree	(MPool* _mPool, void* _memFree)
{
	void* nextPtr = NULL;
	
	assert(_mPool && _memFree);
	assert( (ui)_memFree > (ui)_mPool && (ui)_memFree <= (ui)_mPool + _mPool->m_size + 2*BOUND_SIZE);
	
	/* go to block metadata */
	_memFree = (void*)( (ui)_memFree - BOUND_SIZE );
	
	/* if _memFree is not marked as start of a taken mempry block */
	if( *(int*)_memFree >= 0 )
	{
		return;
	}
	
	/* change size to positive to indicate free memory */
	*(int*)_memFree *= -1;
	
	/* check if next block is within range and also free and if so - merge */
	nextPtr = (void*)( (ui)_memFree + *(int*)_memFree + BOUND_SIZE );
	if( (ui)nextPtr < (ui)_mPool + _mPool->m_size + 2*BOUND_SIZE && *(int*)nextPtr > 0)
	{
		MergeMem(_memFree, nextPtr);
	}
}
/*----------------------------------------------------------------------*/



/*######################################################################*/
/*							Internal Functions							*/
/*######################################################################*/


/*----------------------------------------------------------------------*/
/* internal function: check if _mem is at a boundary */
/* return ptr to closest boundary - modified (or unmodified) input pointer */
void* BoundaryCheck(void* _mem)
{
	int mask = BOUND_SIZE - 1;	/* binary complement */
	int move; 
	
	assert(_mem);
	
	/* get distance to next boundary: _mem & max gives the complementary to the distance */
	move = (unsigned int)_mem & mask;
	/* modulo for case there is no need to move */
	move = ( BOUND_SIZE - move ) % BOUND_SIZE; 
	
	return (void*)( (ui)_mem + move );
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* internal function: merge two adjacent free memory blocks */
/* the pointers are to the metadata at the beginning of each block */
/* _mem2 should be after _mem1 */
void MergeMem(void* _mem1, void* _mem2)
{
	int newSize;
	
	assert(*(int*)_mem1 > 0 && *(int*)_mem2 > 0);
	
	/* now one metadata field is redundant, so adding it to new size */
	newSize = *(int*)_mem1 + *(int*)_mem2 + BOUND_SIZE;
	
	*(int*)_mem1 = newSize;
	*(int*)_mem2 = 0;
}
/*----------------------------------------------------------------------*/


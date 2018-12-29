/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-08    
    Last modified date:		2013-08-16
    Description: 	Memory management functions, in method of contiguous allocation.
    				Overhead:	metadata of each block, general metadata in struct MPool,
    					and an integer set to 0 at the end of the chunk (after last block).
    				
    				Requries externally assigned memory chunk, with size of at least sizeof(MPool) + 4*sizeof(block metadata);
    				
    				"Weak" defragmentation: when accessing a free block (in MPOOLALLOC or -FREE),
    				if following blocks are free - merging is performed.
    				
    				Data structures:
    					- MPool - metadata for whole chunk, containing size of chunk
    					  (exclusive of its own size)
    					- BlockData - metadata for each block, containing block size
    					  (including its own size). 
    					  Allocated blocks are marked by their size being a negative number,
    					  free blocks are marke by positive size.
    				
    				note: unless otherwise specified, references to blocks are to the location of
    				metadata, not to the start of contents.		
***************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "MPoolNew.h"


/*######################################################################*/
/*				Definitions & static foreward declarations				*/
/*######################################################################*/

#define MIN_SIZE			( meta_pool + (meta_block<<2) )		/* minimal size of pool */
#define MASK				(boundary - 1)						/* bit mask for alignment to boundaries */

#define FIRST_BLOCK(P)		( (void*)((unsigned int)(P) + meta_pool) )		/* go from pool metadata to 1st block */

/* go between block metadata and start of block contents */
#define DATA2BLOCK(X)		((void*)( (unsigned int)(X) + meta_block ))
#define BLOCK2DATA(X)		((void*)( (unsigned int)(X) - meta_block ))	
/* align addres to next or prev. boundary */
#define ALIGN_NEXT(X)		( ((unsigned int)(X) + MASK) & ~MASK )
#define ALIGN_PREV(X)		( (unsigned int)(X) 		 & ~MASK )

/*----------------------------------------------------------------------*/

/* meta data for memory chunk */
struct MPool
{
	int		m_size;		/* size of memory chunck, EXCLUSIVE of the metadata MPool struct */
};

/*----------------------------------------------------------------------*/

/* meta data for block */
typedef struct BlockData
{
	int		m_size;		/* size of memory block, INCLUSIVE of the metadata */
} BlockData;

/*----------------------------------------------------------------------*/

static const int meta_pool	= sizeof(MPool);		/* size of general metadata */
static const int meta_block	= sizeof(BlockData);	/* size of block metadata */
static const int boundary	= sizeof(int);			/* size of boundary */
static const int end_marker = sizeof(int);			/* size of end marker */

/*----------------------------------------------------------------------*/

/* get & set block size */
static int	GetBlockSize	(void* _block);
static void	SetBlockSize	(void* _block, int _size);

/* mark block as taken or free */
static void MarkBlockTaken	(void* _block);
static void MarkBlockFree	(void* _block);

/* return 1 for free block, 0 for not free */
static int 	IsBlockFree		(void* _block);
/* return address of (metadata of) next block */
static void* NextBlock		(void* _block);
/* return 1 if block is last, 0 if not */
static int 	IsLastBlock		(void* _block);

/* merge free memory block with next block if it's free too, return new size */
static int 	MergeNext		(void* _block);
/* split block to taken and free parts, merge free part with next if possible, return actual taken size */
static int	SplitBlock		(void* _block, int _sizeTaken);

/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								MPOOL INIT								*/
/*######################################################################*/

/*	Initialize memory chunck for use.
	Input:	_mem - handle to memory chunck
			_size - size in bytes
	Output: handle to MPool - beginning of aligned memory chunk
*/
MPool* MPoolInit(void *_mem, int _size)
{
	MPool pool;
	void *newMem = NULL;
	BlockData data;
	
	/* debug error checks */
	assert(_mem);
	assert(_size >= MIN_SIZE);
	
	/* set all bytes to 0 */
	memset(_mem, 0, _size);
	
	/* align to boundary */
	newMem = (void*)ALIGN_NEXT(_mem);

	/* decrease size accordingly */
	if(newMem != _mem)
	{
		_size -= ( (unsigned int)newMem-(unsigned int)_mem );
	}

	/* write actual free size to metadata - subtract size of mPool struct and end marker */
	pool.m_size = _size - meta_pool - end_marker;
	
	/* put mPool at beginning of memory chunk */
	*(MPool*)newMem = pool;
	
	/* put metadata at beginning of block & set size */
	*(BlockData*)FIRST_BLOCK(newMem) = data;
	SetBlockSize( FIRST_BLOCK(newMem), pool.m_size );
	
	/* return address of mPool */
	return newMem;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								MPOOL ALLOC								*/
/*######################################################################*/
/*	Allocate a block of memory to user
	Input:	- MPool
			_size - # of bytes to allocate
	Output:	handle to (start of contents of) allocated memory block
			if not enough memory - return NULL
			same if size=0
*/
void* 	MPoolAlloc	(MPool* _mPool, int _size)
{
	void *block = NULL;
	int blockSize;
	int notFirstRound = 0;		/* for preventing infinite loop */
	
	assert(_mPool && _size >= meta_block);
	
	/* increase size to include metadata, then to closest boundary size */
	_size += meta_block;
	_size = ALIGN_NEXT(_size);
	
	/* check that size is within possibility */
	if(_size >= _mPool->m_size - meta_block)
	{
		return NULL;
	}
	
	/* go to 1st block */
	block = FIRST_BLOCK( _mPool );
	
	/* start loop on all memory blocks */
	do {
		/* if already checked this block */
		if( notFirstRound )
			block = NextBlock( block );
		
		/* go to next free block */
		while( !IsBlockFree( block ) && !IsLastBlock(block) )
		{
			if( IsLastBlock(block) ) {
				return NULL;
			}
			
			block = NextBlock(block);
		}
		
		/* merge with following blocks if free */
		do {
			blockSize = GetBlockSize(block);	
		} while( blockSize < MergeNext(block) );
		
		/* if free block large enough: allocate */
		if( blockSize >= _size )
		{
			SplitBlock( block, _size );
			return DATA2BLOCK( block );
		}	
		/* else: */
		notFirstRound = 1;
		
	} while( !IsLastBlock(block) );
	
	/* if reached end and didn't find place to allocate */
	return NULL;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								MPOOL FREE								*/
/*######################################################################*/
/*	Free memory blocks
	Input:	- MPool
			- block to free
	Output: none
	if block does not belong to MPool: undefined behavior
	if block already free: may merge with following block(s)
*/
void	MPoolFree	(MPool* _mPool, void* _blockFree)
{
	int blockSize;
	
	assert( _mPool && _blockFree );
	
	assert( (unsigned int)_blockFree >= (unsigned int)(FIRST_BLOCK(_mPool)) &&
				(unsigned int)_blockFree <= ( (unsigned int)FIRST_BLOCK(_mPool) + ((MPool*)_mPool)->m_size ) );
	
	/* go to start of block metadata */
	_blockFree = BLOCK2DATA( _blockFree );
	
	/* mark block as free */
	MarkBlockFree( _blockFree );
	
	/* merge with following blocks if free */
	do {
		blockSize = GetBlockSize(_blockFree);	
	} while( blockSize < MergeNext(_blockFree) );
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							Internal Functions							*/
/*######################################################################*/


/*----------------------------------------------------------------------*/
/* get & set block size - as positive number */
/*----------------------------------------------------------------------*/
int GetBlockSize(void* _block)
{
	return (((BlockData*)_block)->m_size > 0) ? ((BlockData*)_block)->m_size :
			 (-1)*((BlockData*)_block)->m_size;
}
/*----------------------------------------------------------------------*/
void SetBlockSize(void* _block, int _size)
{
	((BlockData*)_block)->m_size = _size;
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* mark block as taken or free - by setting the size sign to negative or positive */
/*----------------------------------------------------------------------*/
void MarkBlockTaken(void* _block)
{
	SetBlockSize( _block, (-1)*GetBlockSize(_block));
}
/*----------------------------------------------------------------------*/
void MarkBlockFree(void* _block)
{
	SetBlockSize( _block, GetBlockSize(_block));
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* return 1 for free block, 0 for not free */
int IsBlockFree(void* _block)
{
	return ( ((BlockData*)_block)->m_size > 0 );
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* return address of (metadata of) next block */
void* NextBlock(void* _block)
{
	return (void*)( (unsigned int)_block + GetBlockSize(_block) );
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* return 1 if block is last, 0 if not */
/* using End Byte which is set to 0 */
int IsLastBlock(void* _block)
{
	return ( 0 == *(int*)(NextBlock(_block)) );
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* merge free memory block with next block if it's free too, return new size */
int MergeNext(void* _block)
{
	int newSize;
	void* nextBlock = NextBlock( _block );
	
	assert( IsBlockFree( _block ) );
	
	/* if next block not free return size of current block */
	if( IsLastBlock( _block) || !IsBlockFree( nextBlock ) )
		return GetBlockSize( _block );
		
	/* now one metadata field is redundant, so adding it to new size */
	newSize = GetBlockSize( _block ) + GetBlockSize( nextBlock );
	
	/* set new block size & mark as free */
	SetBlockSize( _block, newSize );
	MarkBlockFree( _block );
	
	/* delete next block metadata */
	memset( nextBlock, 0, meta_block );
	
	return GetBlockSize( _block );
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* split block to taken and free parts, merge free part with next if possible */
/* if not, and free part too small, don't split but set whole block as taken */
/* return size of taken block */
int SplitBlock(void* _block, int _sizeTaken)
{
	int sizeFree = GetBlockSize(_block) - _sizeTaken;	
	int isLast   = IsLastBlock( _block );
	void* freeBlock;
	BlockData data;
	
	assert( IsBlockFree(_block) && ( GetBlockSize(_block) >= _sizeTaken ) );
	
	/* if no free next block AND sizeFree too small - allocate whole block and return */
	if( ( isLast || !IsBlockFree( NextBlock( _block ) ) ) && (sizeFree < meta_block + boundary) )
	{
		MarkBlockTaken( _block );
		return GetBlockSize( _block );
	}
	
	/* else: */
	
	/* allocate required size & mark rest as free */
	SetBlockSize( _block, _sizeTaken );
	MarkBlockTaken( _block );
	
	freeBlock = NextBlock( _block );
	
	*(BlockData*)freeBlock = data;
	SetBlockSize( freeBlock, sizeFree );
	MarkBlockFree( freeBlock );
	
	/* if there is a next block - try to merge */
	if( !isLast )
	{
		MergeNext( freeBlock );
	}

	return GetBlockSize( _block );
}
/*----------------------------------------------------------------------*/



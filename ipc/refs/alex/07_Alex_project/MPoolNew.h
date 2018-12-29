/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-08    
    Last modified date:		2013-08-16
    Description: 	Memory management functions, in method of contiguous allocation.
     				Overhead:	metadata of each block, general metadata at start and 
     							end of memory chunk.
     				
     				Requries externally assigned memory chunk, with size of at least 
     				sizeof(MPool) + 4*block_metadata (usually 4 bytes)
     				
    				"Weak" defragmentation: when accessing a free block (in MPOOLALLOC or -FREE),
    				if following blocks are free - merging is performed.
    				
    				Note: MPool may not start at the exact beginning of the memory chunk;
***************************************************************************************/

#ifndef __MPOOL_H__
#define __MPOOL_H__

/* metadata for internal use, located at beginning of memory chunk */
typedef struct MPool MPool;


/*	Initialize memory chunck for use.
	Input:	_mem - handle to memory chunck
			_size - size in bytes
	Output: handle to MPool
*/
MPool* 	MPoolInit	(void *_mem, int _size);


/*	Allocate a block of memory to user
	Input:	_mPool
			_size - # of bytes to allocate
	Output:	handle to allocated memory block
			if not enough memory - return NULL
			same if size=0
*/
void* 	MPoolAlloc	(MPool* _mPool, int _size);


/*	Free memory blocks
	Input:	- _mPool
			- block to free
	Output: none
	if block does not belong to MPool: undefined behavior
	if block already free: may merge with following block(s)
*/
void	MPoolFree	(MPool* _mPool, void* _blockFree);

#endif /* __MPOOL_H__ */

/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-08    
    Last modified date:		2013-08-08
    Description: 	Memory management functions, in method of contiguous allocation.
    				Overhead:	- metadata of each block - additional 4 bytes per block,
    							- padding to work in 4-byte units
    							- general metadata in struct MPool.
    				"Weak" prevention of fragmentation: joining 2 adjacent free blocks into one.
    				Size of chunk must be at least sizeof(MPool) + 8
***************************************************************************************/

#ifndef __MPOOL_H__
#define __MPOOL_H__

/* metadata for internal use */
typedef struct MPool MPool;


/*	Initialize memory chunck for use.
	Input:	_mem - handle to memory chunck
			_size - size in bytes
	Output: handle to MPool - beginning of memory chunk
*/
MPool* 	MPoolInit	(void *_mem, int _size);


/*	Assign a block of memory to user
	Input:	_mPool
			_size - # of bytes to allocate
	Output:	handle to assigned memory block
			if not enough memory - return NULL
			same if size=0
*/
void* 	MPoolAlloc	(MPool* _mPool, int _size);


/*	Free memoty blocks
	Input:	_mPool
			_memFree - handle to block
	Output: none
	if block does not belong to MPool, nothing happens
*/
void	MPoolFree	(MPool* _mPool, void* _memFree);

#endif /* __MPOOL_H__ */

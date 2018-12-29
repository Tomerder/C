/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-08    
    Last modified date:		2013-08-08
    Description: 	Memory management functions, in method of contiguous allocation.
    				Overhead:	metadata of each block - additional 4 bytes per block,
    							also general metadata in struct MPool.
    				"Weak" prevention of fragmentation: joining 2 adjacent free blocks into one.
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
MPool* 	MPoolInit	(void *_mem, size_t _size);


/*	Assign a block of memory to user
	Input:	- MPool
			_size - # of bytes to allocate
	Output:	handle to assigned memory block
			if not enough memory - return NULL
*/
void* 	MPoolAlloc	(MPool* , int _size);


/*	Free memoty blocks
	Input:	- MPool
			- handle to block
	Output: none
*/
void	MPoolFree	(MPool* , void* );


#endif /* __MPOOL_H__ */

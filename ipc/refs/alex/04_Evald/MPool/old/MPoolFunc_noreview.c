/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-08    
    Last modified date:		2013-08-08
    Description: 	Memory management functions, in method of contiguous allocation.
    				Overhead:	metadata of each block - additional 4 bytes per block,
    							also general metadata in struct MPool.
    				"Weak" prevention of fragmentation: joining 2 adjacent free blocks into one.
***************************************************************************************/

#include <stdio.h>
#include <assert.h>

#include "MPoolFunc.h"


/*######################################################################*/
/*				Definitions & static foreward declarations				*/
/*######################################################################*/


/* meta data for memory chunk */
struct MPool
{
	/* size of memory chunck */
	/* ptr to 1st (metadata of) block */
	/* ptr to (metadata of) 1st free memory */
};

/* check if _mem is at a 4-boundary, return ptr to closest one */
void BoundaryCheck(void* _mem);
/* merge 2 adjoining memoty chunks */
void MergeMem(void* _mem1, void* _mem2);


/*######################################################################*/
/*								MPOOLINIT								*/
/*######################################################################*/

/*	Initialize memory chunck for use.
	Input:	_mem - handle to memory chunck
			_size - size in bytes
	Output: handle to MPool - beginning of memory chunk
*/
MPool* MPoolInit(void *_mem, size_t _size);

/* internal variables:	mPool
*/

/* assert on input */

/* check that memory starts at 4-boundary - using internal function */
/* move foreward if not */

/* assign fields of mPool */
/* put mPool at beginning of memory chunk ***HOW??*** */

/* return address of mPool */


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
void* 	MPoolAlloc	(MPool* _mPool, int _size);

/* internal variables:	i, actual_size
*/

/* assert on input */

/* if size not divisible by 4 - increase to match */
/* actual_size = size+4 to accomodate metadata */

/* check if actual_size is within possibility: smaller than pool size minus (ptr to 1st free - ptr to beginnig) */

/* loop: */
/* go to 1st free memory, check if next is also free, if so merge them (internal function) */
/* check if free size is enough, if yes break, if no continue loop */

/* if reached end and not found place - return null */

/* otherwise insert size*-1 at beginning of found block, return address of next byte */


/*----------------------------------------------------------------------*/



/*######################################################################*/
/*								MPOOLFREE								*/
/*######################################################################*/
/*	Free memoty blocks
	Input:	- MPool
			- handle to block
	Output: none
	if block does not belong to MPool, nothing happens
*/
void	MPoolFree	(MPool* _mPool, void* _memFree);

/* internal variables:	
*/

/* assert on input */

/* check that _memFree is within range, otherwise return */
/* check that _memFree contains a negative #, otherwise return */

/* change # to positive */
/* check if next chunch is also free, if so merge them (internal func) */


/*----------------------------------------------------------------------*/



/*######################################################################*/
/*							Internal Functions							*/
/*######################################################################*/




/* internal function: check if _mem is at a 4-boundary using bitwise functions */
/* return ptr to closest 4-boundary - modified or unmodified input pointer */
void BoundaryCheck(void* _mem); 

/* internal variables: 	int mask = 3 (=00000011)
						int move;
*/

/* take bitwise "and" of _mem address and mask */
/* move = (4 minus result) modulo 4 - if result is 1, need to move by 3 to reach a boundary etc., but if it's 0 no need to move */

/* return _mem + move */

/*----------------------------------------------------------------------*/



/* internal function: merge two adjacent free memory blocks */
/* _mem2 should be after _mem1 */
void MergeMem(void* _mem1, void* _mem2);

/* internal variables: int newsize
*/

/* newsize is sum of sizes + 4 */
/* remove size of _mem2 */
/* replace size of _mem1 with newsize */

/*----------------------------------------------------------------------*/


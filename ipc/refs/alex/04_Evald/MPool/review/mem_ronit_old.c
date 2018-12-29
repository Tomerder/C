/**************************************************************************************
    Author: Ronit Barzilay
    Creation date:  		8.8.13
    Last modified date:		8.8.13
    Description:  memory allocation func implementation
***************************************************************************************/

#include<stdio.h>

#include "mem.h"

/*keep mem- block memory + data:ptr to start,size,first free block*/
struct MPool
{
	void* m_block; /* memory block*/
	void* m_first; /* smallest free*/
	size_t _size; /* size of memory block (in boundery of 4)*/
};

/************************************************************/
/*------------------------DECLIRATION-----------------------*/
/************************************************************/

/* find the next number that devided by 4 */
int FindFour(int _num);

/* group empty adjacents blocks*/
void Defragmentation (MPool* _mpool);

/************************************************************/
/*--------------------------EXTERNAL------------------------*/
/************************************************************/


/*get block of memory and initialize it to Mpool */
/*work in contigues method*/
MPool* MPoolInit (void* _mem, size_t _size)
{
	/* 1. verify size and address */

	/* 2. allocate MPool and check allocation */

	/* 3. check if first address devided by 4*/
		/* 3.1 initialize m_first & m_block to address*/
		/*3.2 call FindFour that find first address that divided by 4 */

	/* 4. check if size devided by 4*/
		/* 4.1 initialize m_size*/
		/*4.2 call FindFour that find first size that divided by 4 */
}



/*allocate memory for size of _size, from MPool.*/
/* the real size is size + size of meta data, in boundery of 4 */
void* MPoolAlloc (MPool* _mpool ,int _size)
{
	/* 1. verify _mpool & _size */
	
	/*2. call Defragmentation to group empty adjacents blocks */
	
	/*4. check if size devided by 4*/
		/*4.1 call FindFour that find first size that divided by 4 */
	
	/*5. find free block that fits size */
		/* 5.1 if failed to find free space- return NULL*/
	
	/*6. allocate block */ 
	
	/*7. find smallest block, m_first points on it */	
	 
}


/*free memory */
void MPoolFree (MPool* _mpool, void* _freem)
{
		/* 1. verify _mpool & _freem */
		
		/* 2. free block */
		
		/* 3. call Defragmentation- group empty adjacents blocks*/	
}



/************************************************************/
/*--------------------------INTERNAL------------------------*/
/************************************************************/












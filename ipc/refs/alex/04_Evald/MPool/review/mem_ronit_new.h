/**************************************************************************************
    Author: Ronit Barzilay
    Creation date:  		8.8.13
    Last modified date:		8.8.13
    Description:header file for mem func
***************************************************************************************/

#ifndef __MEM_H__
#define __MEM_H__

#include<stdio.h> /*size_t*/

/*keep memory block+ metadata */
typedef struct MPool MPool;

/************************************************************/
/*------------------------DECLARATION-----------------------*/
/************************************************************/

/*work in contigues method*/
/*  */
/*get block of memory and initialize it to Mpool */
/*total memory is smaller then _size and depends on the number of alolocated block*/
MPool* MPoolInit(void* _mem, size_t _size);

/*allocate memory for size of _size, from MPool.*/
/* the real size is size + size of meta data, in boundery of 4 */
/*in case of fail- returns NULL*/
void* MPoolAlloc(MPool* _mpool ,int _size);

/*free memory */
/*the memory can be reuse after free*/
void MPoolFree(MPool* _mpool, void* _freem);


#endif /*#ifndef __MEM_H__*/



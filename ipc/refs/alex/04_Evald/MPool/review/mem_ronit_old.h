/**************************************************************************************
    Author: Ronit Barzilay
    Creation date:  		8.8.13
    Last modified date:		8.8.13
    Description:header file for mem func
***************************************************************************************/

#ifndef __MEM_H__
#define __MEM_H__

#include<stdio.h> /*size_t*/

/*keep mem- block memory + data:ptr to start,size,first free block*/
typedef struct MPool MPool;

/************************************************************/
/*------------------------DECLIRATION-----------------------*/
/************************************************************/

/*get block of memory and initialize it to Mpool */
/*work in contigues method*/
MPool* MPoolInit(void* _mem, size_t _size);

/*allocate memory for size of _size, from MPool.*/
/* the real size is size + size of meta data, in boundery of 4 */
void* MPoolAlloc(MPool* _mpool ,int _size);

/*free memory */
void MPoolFree(MPool* _mpool, void* _freem);


#endif /*#ifndef __MEM_H__*/




/*************************************************************/
/** 	Project:	My Malloc. Header file					**/
/** 	Author:		Alex Katz								**/
/** 	Created:	17/01/2012								**/
/** 	Updated:	17/01/2012								**/
/*************************************************************/
#ifndef __MYMALLOC_H_
#define __MYMALLOC_H_

/* _nBytes will be aligned to sizeof(int) - 
    allocated mem will be at least sizeof(int)*2 bigger then _nBytes 
    space that will be available for user usage is (_nBytes - sizeof(int)*2 )     */
void*		MyMallocInit(void* _mem, size_t _nBytes);

/* return NULL if not enough contiguous memory to allocate 
   _nBytes will be aligned to sizeof(int) */
void*		MyMalloc(void* _mem, size_t _nBytes);

/* free only allocated block */
void		MyFree(void* _ptr);

/* utility function for unitest. */
void		PrintMem(void* _mem);
void		PrintMem2(void* _mem);

#endif /* __MYMALLOC_H_ */

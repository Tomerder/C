/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 *
*    DATE: 28.01.14                                                                                              *
*    LAST MODIFIED: 28.01.14                                                                                  *
*    DESCRIPTION: header for a Free-List									 *
*****************************************************************************************************************/
#ifndef __FREE_LIST_H__
#define __FREE_LIST_H__

typedef struct FreeList FreeList;

/*-----------------------------------------------------------------------------*/
/* elemtent size must be at least 8 */
FreeList*	FreeListCreate( size_t _elementSize, size_t _nElements);

/*-----------------------------------------------------------------------------*/
void		FreeListDestroy( FreeList* _freeList);

/*-----------------------------------------------------------------------------*/
/* return NULL if no mem to allocate */
void* 		FreeListAlloc( FreeList* _freeList); /* O(1) */

/*-----------------------------------------------------------------------------*/
/* double free is forbidden */
void 		FreeListFree( FreeList* _freeList, void* _address); /* O(1) */

/*-----------------------------------------------------------------------------*/
/* count the number of elements left in free list */
size_t		FreeListCount( FreeList* _freeList); /* o(n) */

/*-----------------------------------------------------------------------------*/


#endif  /*__FREE_LIST_H__*/



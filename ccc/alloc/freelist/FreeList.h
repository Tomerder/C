/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 28.01.14                                                                                              
*    LAST MODIFIED: 28.01.14                                                                                  
*    DESCRIPTION: Free-List	
*                 implemantation of SELF DEPEND structure that contain fixed size memory blocks.
                  size of block and number of blocks will be set on creation.
                  the free list is a list of free blocks - except from allocated memory , a "pointer" to the last free block 
                  of memory will be saved.
                  next free block of every free block will be saved at the unused space of the block , 
                  so overhead of the structure is only the one word that points to last free block. 
                  malloc => will pop from free list , free => push to free list  
                  the all structure is POSITION INDEPENDENT- so all "pointers" will be saved as offset from the base of the structure
                  that enables the structure to be used from 2 different processes simultaneously    								 
*****************************************************************************************************************/
#ifndef __FREE_LIST_H__
#define __FREE_LIST_H__

typedef struct FreeList FreeList;

/*-----------------------------------------------------------------------------*/
/* elemtent size must be at least  2 * sizeof(pointer) => offset of next free block + block data      
 */
FreeList*	FreeListCreate( size_t _elementSize, size_t _nElements);

/*-----------------------------------------------------------------------------*/
void		FreeListDestroy( FreeList* _freeList);

/*-----------------------------------------------------------------------------*/
/* return NULL if no mem to allocate 
   O(1) */
void* 		FreeListAlloc( FreeList* _freeList); 

/*-----------------------------------------------------------------------------*/
/* double free is forbidden 
   O(1) */
void 		FreeListFree( FreeList* _freeList, void* _address);

/*-----------------------------------------------------------------------------*/
/* count the number of elements left in free list 
   O(n) */
size_t		FreeListCount( FreeList* _freeList); 

/*-----------------------------------------------------------------------------*/


#endif  /*__FREE_LIST_H__*/




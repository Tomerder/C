#ifndef __HEAP_H__
#define __HEAP_H__

#include "Adt.h"
#include "vec.h"

/*--------------------------------------------------------------------*/

/*returns 1 if item1 < item 2 , otherwise returns 0*/
typedef int (*WeakOrderFunc) (void*, void*);

/*--------------------------------------------------------------------*/

typedef struct Heap Heap;

/*--------------------------------------------------------------------*/

Heap*	HeapBuild	(Vector* _vec, WeakOrderFunc _WeakOrderFunc);
void	HeapDestroy	(Heap* _heap);
ADTERR	HeapInsert	(Heap* _heap, void* _data);
void*	HeapTop		(Heap* _heap);
void*	HeapExtract	(Heap* _heap);
int		HeapForEach	(Heap* _heap, DoFunc _DoFunc, void* params);
void	HeapSort	(Vector* _vec, WeakOrderFunc _WeakOrderFunc);

void HeapPrint (Heap* _heap , PrintFunc _PrintFunc);

int HeapIsEmpty (Heap* _heap);


int HeapTest (Heap* _heap);

/*--------------------------------------------------------------------*/

#endif /* #ifndef __HEAP_H__ */







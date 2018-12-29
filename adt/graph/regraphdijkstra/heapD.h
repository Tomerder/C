#ifndef __HEAP_H__
#define __HEAP_H__

#include "err.h"
#include "vec.h"



/*returns 1 if item1 < item 2 , otherwise returns 0*/
typedef int (*WeakOrderFunc) (void*, void*);

typedef int (*DoFunc) (void*, void*);


typedef struct Heap Heap;


Heap*	HeapBuild	(Vector* _vec, WeakOrderFunc _WeakOrderFunc);
void	HeapDestroy	(Heap* _heap);
ADTERR	HeapInsert	(Heap* _heap, void* _data);
void*	HeapTop		(Heap* _heap);
void*	HeapExtract	(Heap* _heap);
int		HeapForEach	(Heap* _heap, DoFunc _DoFunc, void* params);
void	HeapSort	(Vector* _vec, WeakOrderFunc _WeakOrderFunc);

void HeapPrint (Heap* _heap , PrintFunc _PrintFunc);

int HeapTest (Heap* _heap);

void HeapIncreaceKeyFix	(Heap* _heap , size_t _index);

int* HeapGetHushIndexes(Heap* _heap);

Vector* HeapGetVector(Heap* _heap);

#endif /* #ifndef __HEAP_H__ */







/**************************************************************************
    Author: Stav Ofer
    Creation date :  		2013-06-27    
    Last modified date:  	2013-08-01
    Description : heap handling functions (based on vector) - header file
    			  update 17 july: now conforming to generic vector
***************************************************************************/

#ifndef __HEAP_H__
#define __HEAP_H__

#include "ADTDefs.h"
#include "VectorGeneric.h"

typedef struct Heap_t 	Heap;
typedef void*			HeapData;

/* generic data comparison func */
typedef	int		(*CompFunc)	(HeapData _data1, HeapData _data2);
/* modify data */
typedef void	(*DoFunc)	(HeapData _data, void* _params);


/* get vector and make into heap */
Heap*	HeapBuild			(Vector* _vec, CompFunc _cmpFunc);
/* destroy only heap - not vector */
void	HeapDestroy			(Heap* _heap);

/* insert data & rearrange heap */
/* IMPORTANT: if not all vector items are part of the heap, they may be overwritten by this function */
ADTErr		HeapInsert		(Heap* _heap, HeapData _data);
/* get max value */
HeapData	HeapMax			(Heap* _heap);
/* extract max value & rearrange heap, returns NULL for empty heap */
HeapData	HeapExtractMax	(Heap* _heap);
/* apply _doFunc to all heap items */
void		HeapForEach		(Heap* _heap, DoFunc _doFunc, void *_params);
/* get heap size */
size_t		HeapNumItems	(Heap* _heap);

/* sort a vector using heap functions */
void 		HeapSort		(Vector* _vec, CompFunc _cmpFunc);

#endif /* __HEAP_H__ */

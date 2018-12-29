/**************************************************************************
    Author: Stav Ofer
    Creation date :  		2013-06-27    
    Last modified date:  	2013-08-01
    Description : heap handling functions (based on vector)
				  update 17 july: now conforming to generic vector
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ADTDefs.h"
#include "VectorGeneric.h"
#include "HeapGeneric.h"


/*----------------------------------------------------------*/
/*			definitions & foreward declaration				*/
/*----------------------------------------------------------*/

#define LEFT(I)			((I)+(I))
#define RIGHT(I)		((I)+(I)+1)
#define PARENT(I)		((I)/2)
#define COMPARE(X,Y,Z)	(X)->m_cmpFunc(Y,Z);
#define SIZE(X)			(X)->m_nItems

struct Heap_t {
	Vector*		m_vec;	/* in vector, nItems is heap length */
	size_t		m_nItems;
	CompFunc	m_cmpFunc;
};

/* rearrange items as heap from _index down in its sub-tree, which is already a heap excep _index */
static void			Heapify				(Heap* _heap, size_t _index);
/* rearrange a heap after inserting */
static void			HeapInsertSort		(Heap* _heap, int _index, void* _item);
/* dedicated extract for HeapSort */
static HeapData		HeapSortExtractMax	(Heap* _heap);
/*----------------------------------------------------------*/


/*----------------------------------------------------------*/
/*						Heap functions						*/
/*----------------------------------------------------------*/


/* get vector and make into heap */
/* calls internal functions: Heapify */
Heap*	HeapBuild(Vector* _vec, CompFunc _cmpFunc)
{
	Heap* heap;
	int i;

	assert(_vec);
	assert(_cmpFunc);

	/* allocate memory, link to vector, get size */
	heap = (Heap*)malloc(sizeof(Heap));	
	if( !heap ) {
		return NULL;
	}
	heap->m_vec = _vec;
	heap->m_cmpFunc = _cmpFunc;
	VectorNumItems(_vec, &heap->m_nItems);
	
	/* rearrange vector according to heap rules */
	for(i = PARENT(SIZE(heap)); i>0; --i) {	
		Heapify(heap, i);
	}

	return heap;
}
/*----------------------------------------------------------*/


/* destroy (only heap - not vector) */
void	HeapDestroy(Heap* _heap)
{
	if( _heap ) {
		free(_heap);
	}
}
/*----------------------------------------------------------*/


/* get max value */
HeapData	HeapMax(Heap* _heap)
{
	HeapData data;
	
	assert(_heap);
	if(SIZE(_heap) < 1) {
		return NULL;
	}

	/* actual action here - max item is always in 1st place */
	VectorGet(_heap->m_vec, 1, &data);	
	
	return data;
}
/*----------------------------------------------------------*/


/* extract max value, returns NULL for empty heap */
/* calls functions: HeapMax, Heapify */
HeapData	HeapExtractMax(Heap* _heap)
{
	HeapData data, dataTemp;

	assert(_heap);
	
	if( 0 == HeapNumItems(_heap) )
	{
		return NULL;
	}
	
	/* get max items */
	VectorGet(_heap->m_vec, 1, &data);
	/* move data of last item to 1st & decrement size */
	VectorDelete(_heap->m_vec, &dataTemp);
	VectorSet(_heap->m_vec, 1, dataTemp);
	--SIZE(_heap);
	
	Heapify(_heap, 1);	/* heapify */

	return data;
}
/*----------------------------------------------------------*/


/* insert data into an existing heap: insert at the end, then rearrange heap */
/* calls function: HeapInsertSort */
/* note: if not all vector items are part of the heap, they may be overwritten by this function */
ADTErr	HeapInsert(Heap* _heap, HeapData _data)
{
	size_t size;
	
	assert(_heap);
	VectorNumItems(_heap->m_vec, &size);
	
	/* add the data to the end. use Set or Add depending on the vector size. increment heap size */ 
	if( size > SIZE(_heap) ) {
		VectorSet(_heap->m_vec, SIZE(_heap)+1, _data);	
	} else {
		VectorAdd(_heap->m_vec, _data);
	}
	++SIZE(_heap);

	/* rearrange heap */
	HeapInsertSort(_heap, SIZE(_heap), _data);	

	return ERR_OK;
}
/*----------------------------------------------------------*/


/* apply _doFunc to all heap items */
/* note: does not handle cases where _doFunc causes duplicated data */
void HeapForEach(Heap* _heap, DoFunc _doFunc, void *_params)
{
	int i;
	size_t size = SIZE(_heap);
	HeapData data;
	
	assert(_heap);
	assert(_doFunc);

	/* apply _doFunc */
	for(i=1; i<=size; ++i)
	{
		VectorGet(_heap->m_vec, i, &data);
		_doFunc(data, _params);
	}
	
	/* rearrange heap */
	for(i = PARENT(SIZE(_heap)); i>0; --i) {	
		Heapify(_heap, i);
	}
}
/*----------------------------------------------------------*/


/* return heap size */
size_t	HeapNumItems(Heap* _heap)
{
	assert(_heap);
	return SIZE(_heap);
}
/*----------------------------------------------------------*/


/* sort a vector using heap functions */
void HeapSort(Vector* _vec, CompFunc _cmpFunc)
{
	Heap *heap = NULL;
	HeapData data;
	size_t size;
	int i;
	
	heap = HeapBuild(_vec, _cmpFunc);
	if(!heap) {
		return;
	}
	
	size = HeapNumItems(heap);
	for(i=size; i>0; --i)
	{
		data = HeapSortExtractMax(heap);
		VectorSet(_vec, i, data);
	}

	HeapDestroy(heap);
}
/*----------------------------------------------------------*/





/*----------------------------------------------------------*/
/*					internal functions						*/
/*----------------------------------------------------------*/


/* arrange items as heap from _index down in its sub-tree - recursive function */
/* assuming sub tree is already heap, except _index itself */
void	Heapify(Heap* _heap, size_t _index)
{
	HeapData childRight, childLeft, bigChild, parent;
	size_t iChild;
	int compare;
			
	/* stopping condition - index too high */
	if( _index > PARENT(SIZE(_heap)) ) {
		return;
	}
	
	/* get left child, assume by default that left child is larger */
	VectorGet(_heap->m_vec, LEFT(_index), &childLeft);	
	bigChild = childLeft;				
	iChild = LEFT(_index);
	
	/* if right child exists - get it & compare to left*/
	if( RIGHT(_index) <= SIZE(_heap) ) {					
		VectorGet(_heap->m_vec, RIGHT(_index), &childRight);

		compare = COMPARE(_heap, childRight, childLeft);
		if( compare > 0 ) {
			bigChild = childRight;
			iChild = RIGHT(_index);
		}
	}

	/* get parent value & compare to big child */
	VectorGet(_heap->m_vec, _index, &parent);			
	compare = COMPARE(_heap, bigChild, parent);
	/* if child > parent: swap by assigning each to the other's index, then recursion */
	if( compare > 0) {				
		VectorSet(_heap->m_vec, _index, bigChild);	
		VectorSet(_heap->m_vec, iChild, parent);
		Heapify(_heap, iChild);
	}
}
/*----------------------------------------------------------*/


/* rearrange heap after insert, going bottom-up - recursive */
/* arguments include both _index and _item to save callings to VectorGet */
void	HeapInsertSort(Heap* _heap, int _index, HeapData _item)
{
	HeapData parent;
	int compare;
	
	/* stopping condition 1 - reached root */
	if( PARENT(_index) < 1 ) {									
		return;
	}
	
	/* get parent value & compare to _item */
	VectorGet(_heap->m_vec, PARENT(_index), &parent);
	compare = COMPARE(_heap, _item, parent);
	
	/* stoping condition 2: if item <= parent (meaning heap is arranged) */
	if( compare <= 0 ) {						
		return;
	}

	/* else: swap values */
	VectorSet(_heap->m_vec, _index, parent);				
	VectorSet(_heap->m_vec, PARENT(_index), _item);
	/* recursion */
	HeapInsertSort(_heap, PARENT(_index), _item);				
}	
/*----------------------------------------------------------*/


/* extract max value for Heap Sort */
/* calls functions: HeapMax, Heapify */
/* note: does not decrease vector size, just rearranging */
HeapData	HeapSortExtractMax(Heap* _heap)
{
	HeapData data, dataTemp;

	assert(_heap);
	/* get max items */
	VectorGet(_heap->m_vec, 1, &data);
	/* move data of last item to 1st & decrement size */
	VectorGet(_heap->m_vec, SIZE(_heap), &dataTemp);
	VectorSet(_heap->m_vec, 1, dataTemp);
	--SIZE(_heap);
	
	/* put removed data in "removed" place */
	VectorSet(_heap->m_vec, SIZE(_heap)+1, data);
	
	Heapify(_heap, 1);	/* heapify */

	return data;
}
/*----------------------------------------------------------*/



#include <stdlib.h>
#include <assert.h>
#include "heap.h"


#define RETURN_ERR if(err!=0) return err
#define TOP_INDEX 0

/*--------------------------------------------------------------------------------*/

struct Heap{
	Vector*           m_vector;	
    WeakOrderFunc     m_WeakOrderFunc;
};



static void Heapify(Heap* _heap, int _index );
static void SiftUp(Heap* _heap, int _index);

static int Left(int _index);
static int Right(int _index);
static int Parent(int _index);

static ADTERR Swap(Vector* _vec,size_t _index1 , size_t _index2);


/*--------------------------------------------------------------------------------*/

Heap* HeapBuild (Vector* _vec, WeakOrderFunc _WeakOrderFunc){
	int nItems,i;
	Heap* heap;

	assert (_vec);
	assert (_WeakOrderFunc);

	heap = (Heap*)malloc(sizeof(Heap));
	if(heap == NULL){
		return NULL;
	}
	
	
	heap->m_WeakOrderFunc = _WeakOrderFunc;
	heap->m_vector = _vec;   

	nItems = VectorItemsNum(_vec);

    for (i = (nItems/2) -1 ; i >= TOP_INDEX ; i--) {
        Heapify(heap, i);
	}	
 
	

	return heap;

}

/*--------------------------------------------------------------------------------*/


static void Heapify(Heap* _heap, int _index){
	int nItems , indexLeft , indexRight , indexMax ; 
	void* itemLeft = NULL;
	void* itemRight = NULL;
	void* itemMax = NULL;	

	assert(_heap);

	nItems	= VectorItemsNum (_heap->m_vector);	
	indexLeft = Left (_index);
    indexRight = Right (_index);

	indexMax = _index;
	VectorGet (_heap->m_vector , indexMax , &itemMax); 
	
	if (indexLeft < nItems  ){
		VectorGet (_heap->m_vector , indexLeft , &itemLeft); 
		if( _heap->m_WeakOrderFunc (itemMax , itemLeft) ){ 
        	indexMax = indexLeft;
			VectorGet (_heap->m_vector , indexMax , &itemMax); 
   		}
	}

	if (indexRight < nItems){  
		VectorGet (_heap->m_vector, indexRight , &itemRight);    
		if ( _heap->m_WeakOrderFunc (itemMax , itemRight) ){    
        	indexMax = indexRight;
			VectorGet (_heap->m_vector , indexMax , &itemMax); 
		}
	}
	
	 /*swap + recursion*/
	if (indexMax != _index){  
     	Swap ( _heap->m_vector, _index , indexMax);
		Heapify (_heap, indexMax );
	} 
	
	
}


/*------------------------------------------------------------------------------------------*/

void HeapSort (Vector* _vec, WeakOrderFunc _WeakOrderFunc){
	int nItems,i ;
	Heap* heap;
	void** sortedItems;

	assert(_vec);
	assert(_WeakOrderFunc);

	nItems	= VectorItemsNum (_vec);	
	sortedItems = (void**)malloc (sizeof(void*) * nItems);
	if(sortedItems == NULL){
		return;
	}
	
	heap = HeapBuild (_vec, _WeakOrderFunc);
	if(heap == NULL){
		return;
	}

	i=0;
	while (i < nItems){
		sortedItems[i] = HeapExtract (heap);
		i++;
	}
	
	i=0;
	while (i < nItems){
		VectorInsert (_vec, sortedItems[i]);
		i++;
	}

	free(sortedItems);
	HeapDestroy(heap);
	

}
/*------------------------------------------------------------------------------------------*/

void* HeapTop (Heap* _heap){
	void* itemToRet;

	assert(_heap);
	
	VectorGet(_heap->m_vector, TOP_INDEX, &itemToRet);

	return itemToRet;
}

/*------------------------------------------------------------------------------------------*/

void* HeapExtract(Heap* _heap){
	int nItems;		
	void* itemToRet;

	assert(_heap);

	nItems	= VectorItemsNum (_heap->m_vector);	

	if (nItems < 1){
		return NULL;
	}
	
	Swap ( _heap->m_vector, TOP_INDEX, nItems - 1);  
	
	VectorRemove (_heap->m_vector, &itemToRet);
	
	/*fix heap*/
	Heapify(_heap ,0);
	

	return itemToRet;
}



/*------------------------------------------------------------------------------------------*/

static int Left(int _index){

	return ( (_index * 2) + 1 );
}

static int Right(int _index){
	return ( (_index * 2) + 2 );
}

static int Parent(int _index) {
	return ( (_index - 1)/2 )  ;
}
/*------------------------------------------------------------------------------------------*/


void HeapDestroy (Heap* _heap){
	free (_heap);
}


/*------------------------------------------------------------------------------------------*/

ADTERR HeapInsert (Heap* _heap, void* _data){
	int itemsNum ,err=0 ;

	itemsNum = VectorItemsNum(_heap->m_vector);
	
	err = VectorInsert (_heap->m_vector, _data);         /*insert to last*/  
	RETURN_ERR;

	SiftUp ( _heap, itemsNum );

	return OK;

}

/*----------------------------------------------------------------------------------------*/

static void SiftUp(Heap* _heap, int _index){
	int indexParent ; 
	void* itemParent = NULL;
	void* itemSon = NULL;	
	
	 assert (_heap);	

	if( _index == TOP_INDEX){
		return;
	}
		
	VectorGet (_heap->m_vector , _index , &itemSon); 
	
	indexParent = Parent(_index);

	if (indexParent != _index){
		VectorGet (_heap->m_vector , indexParent , &itemParent); 
		if( _heap->m_WeakOrderFunc (itemParent , itemSon) ){ 
			Swap ( _heap->m_vector, _index , indexParent);
			SiftUp ( _heap, indexParent);
		}		
	}
	

}

/*---------------------------------------------------------------------------------------*/

int	HeapForEach	(Heap* _heap, DoFunc _DoFunc, void* params){
	int itemsNum, i=0, err=0;
	void* item;

	itemsNum = VectorItemsNum(_heap->m_vector);

	while ( i < itemsNum){
		VectorGet ( _heap->m_vector, i, &item); 
		err = _DoFunc (item, params);
		RETURN_ERR;
		i++;
	}
	
	return OK;

}


/*------------------------------------------------------------------------------------*/


static ADTERR Swap(Vector* _vec,size_t _index1 , size_t _index2){
	void* item1;
	void* item2;
	int err=0;
	
	err = VectorGet(_vec,_index1,&item1); /*   &(void*) != void**   -> need to send the adress of what we wish to change => &(void*)  */
	RETURN_ERR;	

	err = VectorGet(_vec,_index2,&item2);
	RETURN_ERR;	
		
	err = VectorSet(_vec, _index2 , item1);
	RETURN_ERR;	

	err = VectorSet(_vec, _index1 , item2);
	RETURN_ERR;	

	return OK;
	
}

/*---------------------------------------------------------------------------------------*/

int HeapTest (Heap* _heap){	
	int itemsNum , i=0 , sonIndex;
	void* itemParent;
	void* itemSon;

	assert ( _heap);

	itemsNum = VectorItemsNum(_heap->m_vector);
	
	while (i <= itemsNum/2){
		VectorGet (_heap->m_vector, i, &itemParent  );
	
		sonIndex = Left(i) ; 
		if( sonIndex < itemsNum){
			VectorGet (_heap->m_vector, sonIndex, &itemSon  );
			if( _heap->m_WeakOrderFunc (itemParent , itemSon) ){ 
				return -1;
			}
		}

		sonIndex = Right(i) ; 
		if( sonIndex < itemsNum){
			VectorGet (_heap->m_vector, sonIndex, &itemSon  );
			if( _heap->m_WeakOrderFunc (itemParent , itemSon) ){ 
				return -1;
			}
		}
			
		i++;
	}

	return 0 ;

}
/*---------------------------------------------------------------------------------------*/

void HeapPrint (Heap* _heap , PrintFunc _PrintFunc){
	VectorPrint ( _heap->m_vector , _PrintFunc);

}

/*---------------------------------------------------------------------------------------*/



/*
static ADTERR compAndSwap(Vector* _vec,size_t _index1 , size_t _index2, CompFunc _compFunc,int* _isSwap){
	void* item1;
	void* item2;
	int err=0;
	
	*_isSwap = 0;
	
	err = VectorGet(_vec,_index1,&item1); 
	RETURN_ERR;	

	err = VectorGet(_vec,_index2,&item2);
	RETURN_ERR;	
	

	if(_compFunc(item1,item2)>0){
		*_isSwap = 1;
		
		err = VectorSet(_vec, _index2 , item1);
		RETURN_ERR;	

		err = VectorSet(_vec, _index1 , item2);
		RETURN_ERR;	
	}

	return OK;
	
}
*/

/*------------------------------------------------------------------------------------*/



















/**************************************************************************************
    Author: Stav Ofer
    Creation date :  		2013-06-12    
    Last modified date:  	2013-07-20
    Description : functions for creating and manipulating GENERIC vectors
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ADTDefs.h"
#include "VectorGeneric.h"


struct Vector {
	void**	m_items;	/*  array of items			*/
	int 	m_nItems;	/*  # of items in m_items	*/
	int		m_size;		/* the size of Items array	*/
	int		m_extend;	/*  by how much to extend	*/
};


/********* create & destroy *********/


/** create & initialize vector, allocate list of given initial size	**/
Vector *VectorCreate(int _size, int _extend)
{
	Vector *_vector;

	if( 0 > _size || ( 0 == _size && 0 >= _extend ) ) {
		return(NULL);
	}
	
	/* allocate memory for vector struct & items array */
	
	_vector=malloc(sizeof(struct Vector));					
	if( NULL==_vector ) {	
		return NULL;
	}

	if( 0 <= _size ) {
		_vector->m_items = (void**)malloc( sizeof(void*) * _size );

		if(NULL==_vector->m_items) {
			free(_vector);
			return NULL;
		}
	}
	
	/* initialize metadata */
	_vector->m_size 	= _size;
	_vector->m_nItems	= 0;
	_vector->m_extend	= _extend;

	return _vector;
}
/*----------------------------------------------------------*/


/**  delete vector, free everything	**/
void VectorDestroy(Vector *_vector)
{
	if( _vector ) {
		free(_vector->m_items);
		free(_vector);
		_vector = NULL;
	}
}
/*----------------------------------------------------------*/


/********* set & get *********/
/* apply only for existing indices - no adding */


/**  change value of existing item	**/
ADTErr VectorSet(Vector *_vector, int _index, void *_item)
{
	/* errors */
	assert(_vector);
	assert(_vector->m_items);
	if(_index > _vector->m_nItems) {
		return (ERR_OVERFLOW);
	} else if(_index < 1) {
		return (ERR_UNDERFLOW);
	}
	
	/* actual action */
	_vector->m_items[_index-1] = _item;					

	return ERR_OK;
}
/*----------------------------------------------------------*/


/**  get value of item	**/
ADTErr VectorGet(Vector *_vector, int _index, void **_item)
{
	/* errors */
	
	assert(_vector);
	assert(_item);	
	if(_index > _vector->m_nItems || _index < 1 ) {
		return(ERR_WRONG_INDEX);
	}
	
	/* actual action */
	*_item = _vector->m_items[_index-1];

	return ERR_OK;
}
/*----------------------------------------------------------*/


/********* add & delete *********/
/* chronologically only: last in first out */


/**  add item to end of list. if needed expand array by realloc	**/
ADTErr VectorAdd(Vector *_vector, void *_item)
{
	void **temp_ptr;
	size_t size, newSize;	

	/* errors */
	assert(_vector);							
	assert(_item);	
	
	VectorNumItems(_vector, &size);

	/* if need to extend */
	if( size == _vector->m_size ) {						
		
		if( 0 >= _vector->m_extend ) {	/* if extend <= 0 */
			return(ERR_OVERFLOW);
		}
		newSize = _vector->m_size + _vector->m_extend;
		
		/* if initial size == 0 : use malloc */
		if( 0 == _vector->m_size ) {						
			_vector->m_items = (void**)malloc(sizeof(void*) * newSize);
		}
		else	/* else: extend by realloc */
		{		
			temp_ptr = _vector->m_items;
			_vector->m_items = realloc(_vector->m_items, sizeof(void*) * newSize);
		}
		
		/* if realloc failed */
		if(NULL==_vector->m_items) {
			_vector->m_items = temp_ptr;
			return(ERR_REALLOCATION_FAILED);
		}
		else	/* else: update metadata */
		{
			_vector->m_size = newSize;					
		}
		
	}	/* end if for extension */

	/* actual action */
	++_vector->m_nItems;								
	_vector->m_items[_vector->m_nItems-1] = _item;

	return ERR_OK;
}
/*----------------------------------------------------------*/


/** delete last item. reallocate to a smaller size if appropriate	**/
ADTErr VectorDelete(Vector *_vector, void **_item)
{
	void **temp_ptr;	
	int size, newSize;

	/* errors */
	assert(_vector);							
	assert(_item);	

	VectorNumItems(_vector, (size_t*)&size);
	if (0==size) {
		return(ERR_GENERAL);
	}
	
	/* actual action */
	*_item = _vector->m_items[size-1];						
	
	newSize = _vector->m_size - 2*_vector->m_extend;		
	
	/* if need to downsize */
	
	if( (size <= newSize) && (_vector->m_extend > 0) ) {	

		temp_ptr = _vector->m_items;
		_vector->m_items = realloc(_vector->m_items, sizeof(void*) * newSize);
		
		/* if realloc failed */
		if(NULL==temp_ptr) {								
			_vector->m_items = temp_ptr;
			return(ERR_REALLOCATION_FAILED);
		}
		else	/* else: update metadata */
		{	
			_vector->m_size = newSize;
		}
	}

	/* update nItems */
	--_vector->m_nItems;								
	
	return (ERR_OK);
}
/*----------------------------------------------------------*/


/********* others *********/


/** get # of items **/
ADTErr VectorNumItems(Vector *_vector, size_t *_size)
{
	assert(_vector);
	*_size=_vector->m_nItems;

	return ERR_OK;
}
/*----------------------------------------------------------*/


/**  print all items	**/
void VectorPrint(Vector *_vector, PrintFunc _printFunc)
{
	int i;
	size_t N;

	assert(_vector);
	assert(_vector->m_items);
	
	VectorNumItems(_vector, &N);
	if ( 0 == N ) {	
		printf("Vector is empty\n");
		return;
	}
	
	/* use appropriate print function */
	for(i=1; i <= N; ++i) {			
		_printFunc(_vector->m_items[i-1], i);
	}
	printf("\n");
}
/*----------------------------------------------------------*/


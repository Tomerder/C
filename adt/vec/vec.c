#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "vec.h"



/*------------------------------------------------------------------------------------*/

Vector* VectorCreate(size_t _size,size_t _blockSize){
		
	if(_blockSize==0 && _size==0 ){
			return NULL;     //not legal size and block size - both 0
	}

	Vector* vector= (Vector*)malloc(sizeof(Vector));    //malloc : vector
	if (vector == NULL){
		return NULL;
	}	

	if(_size!=0){		
		vector->m_data = (void**)malloc (sizeof(void*) * _size);   //malloc : void**
		if (vector->m_data == NULL){
			free(vector);
			return NULL;
		}
	}

	vector->m_size = _size;	
	vector->m_size = _size;		
	vector->m_blockSize = _blockSize;
	vector->m_origSize=_size;
	vector->m_nItems = 0;

	return vector;
}

/*------------------------------------------------------------------------------------*/


void VectorDestroy(Vector* _vec){
	assert(_vec);
	free(_vec->m_data);
	free(_vec);
}

/*------------------------------------------------------------------------------------*/


ADTERR VectorInsert(Vector* _vec , void* _data){
	
	size_t newSize;	
	
	assert(_vec);
	assert(_data);
	
	if(_vec->m_nItems ==  _vec->m_size){   //vector is full
		if(_vec->m_blockSize == 0){
			return OVERFLOW;
		}
		//enlarge vector by block size
		newSize = _vec->m_size + _vec->m_blockSize;
		if(_vec->m_size == 0){      //check if m_size == 0   ->   m_data wasnt allocated
			_vec->m_data = (void**)malloc (sizeof(void*) * newSize);  //malloc							
		}else{
			_vec->m_data = (void**)realloc (_vec->m_data , sizeof(void*) * newSize);  //realloc			
		}
		if(_vec->m_data == NULL){
			return ALLOC_FAILED;
		}
		_vec->m_size = newSize;
	}
	

	_vec->m_data[_vec->m_nItems] = _data;   //insert
	(_vec->m_nItems)++;                     //items count ++ 


	return 0;

}


/*------------------------------------------------------------------------------------*/

/*send void** beacause we want the function to change the address =>  send void**=&(void*) : par => in func: *par= void*  
  set *par to point at the address of the item we removed from vector */
ADTERR VectorRemove(Vector* _vec , void** _data){	
	
	size_t newSize;	
	
	assert( _vec != NULL );
	assert(_vec->m_nItems != 0);
	

	(_vec->m_nItems)--;
	*_data = _vec -> m_data[_vec->m_nItems];   //return to user
	
	//decreasing algorithem
	if(	_vec-> m_nItems <= (_vec-> m_size / 2)){
		newSize = _vec-> m_nItems + _vec->m_blockSize;
		if(newSize > _vec->m_origSize){   //check orig size condition
			_vec->m_data = (void**)realloc (_vec->m_data , sizeof(void*) * newSize);
			if(_vec->m_data == NULL){
				return ALLOC_FAILED;
			}
			_vec->m_size = newSize;
		}
	}



	return 0;

}

/*------------------------------------------------------------------------------------*/

ADTERR VectorSet(Vector* _vec ,size_t _index , void* _data){
	
	assert(_vec);
	assert(_data);
	assert(_index < _vec->m_nItems);
	
	_vec->m_data[_index] = _data;

	return 0;

}


/*------------------------------------------------------------------------------------*/

/*send void** beacause we want the function to change the address =>  send void**=&(void*) : par => in func: *par= void*  
  set *par to point at the address of the item we removed from vector */
ADTERR VectorGet(Vector* _vec ,size_t _index , void** _data){
	
	assert(_vec);
	assert(_index < _vec->m_nItems);
	
	//_data = _vec->m_data[_index];  
	*_data = _vec -> m_data[_index];    //return to user

	return 0;

}


/*------------------------------------------------------------------------------------*/

size_t VectorItemsNum(Vector* _Vec){

	return _Vec->m_nItems;

}

/*------------------------------------------------------------------------------------*/

size_t VectorSize(Vector* _Vec){

	return _Vec->m_size;

}

/*------------------------------------------------------------------------------------*/

void VectorPrint(Vector* _vec , PrintFunc _printFunc){


	_printFunc(_vec);

}

/*------------------------------------------------------------------------------------*/


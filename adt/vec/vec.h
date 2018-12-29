#include<stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef __VEC_H__
#define __VEC_H__



typedef enum {SUCCESS,OVERFLOW,ALLOC_FAILED} ADTERR;

typedef struct Vector{
	void** m_data;
	size_t m_size;
	size_t m_blockSize;
	size_t m_origSize;
	size_t m_nItems;
}Vector;

typedef void(*PrintFunc)(Vector* _vec);


Vector* VectorCreate(size_t _size,size_t _BlockSize);          
void VectorDestroy(Vector* _vec);
ADTERR VectorInsert(Vector* _vec , void* _data);
ADTERR VectorRemove(Vector* _vec , void** _data);    
ADTERR VectorSet(Vector* _vec ,size_t _index , void* _data);
ADTERR VectorGet(Vector* _vec ,size_t _index , void** _data);
size_t VectorItemsNum(Vector* _Vec);
size_t VectorSize(Vector* _Vec);

void VectorPrint(Vector* _vec , PrintFunc _printFunc);



#endif /* #ifndef __VEC_H__ */

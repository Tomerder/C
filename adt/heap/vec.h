#ifndef __VEC_H__
#define __VEC_H__

#include "err.h"

/*  #define void* Item  */

typedef struct Vector Vector;
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

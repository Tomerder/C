/*-----------------------------------------Tomer Dery-----------------------------------------*/
#ifndef __Dog_LIST_H__
#define __Dog_LIST_H__

#include "list.h"
#include "Dog.h"

/*----------------------------------------------------------------------------------*/

typedef void* DogListItr;   
typedef struct DogList DogList;

DogList* DogListCreate();       
void DogListDestroy(DogList* _list);
ADTERR DogListPushHead(DogList* _list , Dog* _data);
ADTERR DogListPushTail(DogList* _list , Dog* _data);
ADTERR DogListPopHead(DogList* _list , Dog** _data);
ADTERR DogListPopTail(DogList* _list , Dog** _data);
size_t DogListCountItems(DogList* _list);
int DogListIsEmpty(DogList* _list);
void DogListPrint(DogList* _list , PrintFunc _printFunc);
DogListItr DogListBegin(DogList* _list);
DogListItr DogListEnd(DogList* _list);
DogListItr DogListNext(DogListItr _itr);
DogListItr DogListPrev(DogListItr _itr);
DogListItr DogListGetData(DogListItr _itr);
DogListItr DogListInsert(DogListItr _insertBefore , Dog* _data);
Dog* DogListRemove(DogListItr _toRemove);
DogListItr DogListFindFirst(DogListItr _from, DogListItr _to, MatchFunc _MatchFunc, Dog* _data);
DogListItr DogListForEach(DogListItr _from, DogListItr _to, DoFunc _DoFunc, Parm _parm);


/*----------------------------------------------------------------------------------*/

#endif /* #ifndef __Dog_LIST_H__ */

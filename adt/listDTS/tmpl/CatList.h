/*-----------------------------------------Tomer Dery-----------------------------------------*/
#ifndef __Cat_LIST_H__
#define __Cat_LIST_H__

#include "list.h"
#include "Cat.h"

/*----------------------------------------------------------------------------------*/

typedef void* CatListItr;   
typedef struct CatList CatList;

CatList* CatListCreate();       
void CatListDestroy(CatList* _list);
ADTERR CatListPushHead(CatList* _list , Cat* _data);
ADTERR CatListPushTail(CatList* _list , Cat* _data);
ADTERR CatListPopHead(CatList* _list , Cat** _data);
ADTERR CatListPopTail(CatList* _list , Cat** _data);
size_t CatListCountItems(CatList* _list);
int CatListIsEmpty(CatList* _list);
void CatListPrint(CatList* _list , PrintFunc _printFunc);
CatListItr CatListBegin(CatList* _list);
CatListItr CatListEnd(CatList* _list);
CatListItr CatListNext(CatListItr _itr);
CatListItr CatListPrev(CatListItr _itr);
CatListItr CatListGetData(CatListItr _itr);
CatListItr CatListInsert(CatListItr _insertBefore , Cat* _data);
Cat* CatListRemove(CatListItr _toRemove);
CatListItr CatListFindFirst(CatListItr _from, CatListItr _to, MatchFunc _MatchFunc, Cat* _data);
CatListItr CatListForEach(CatListItr _from, CatListItr _to, DoFunc _DoFunc, Parm _parm);


/*----------------------------------------------------------------------------------*/

#endif /* #ifndef __Cat_LIST_H__ */

/*-----------------------------------------Tomer Dery-----------------------------------------*/
#ifndef __XXX_LIST_H__
#define __XXX_LIST_H__

#include "list.h"
#include "XXX.h"

/*----------------------------------------------------------------------------------*/

typedef void* XXXListItr;   
typedef struct XXXList XXXList;

XXXList* XXXListCreate();       
void XXXListDestroy(XXXList* _list);
ADTERR XXXListPushHead(XXXList* _list , XXX* _data);
ADTERR XXXListPushTail(XXXList* _list , XXX* _data);
ADTERR XXXListPopHead(XXXList* _list , XXX** _data);
ADTERR XXXListPopTail(XXXList* _list , XXX** _data);
size_t XXXListCountItems(XXXList* _list);
int XXXListIsEmpty(XXXList* _list);
void XXXListPrint(XXXList* _list , PrintFunc _printFunc);
XXXListItr XXXListBegin(XXXList* _list);
XXXListItr XXXListEnd(XXXList* _list);
XXXListItr XXXListNext(XXXListItr _itr);
XXXListItr XXXListPrev(XXXListItr _itr);
XXXListItr XXXListGetData(XXXListItr _itr);
XXXListItr XXXListInsert(XXXListItr _insertBefore , XXX* _data);
XXX* XXXListRemove(XXXListItr _toRemove);
XXXListItr XXXListFindFirst(XXXListItr _from, XXXListItr _to, MatchFunc _MatchFunc, XXX* _data);
XXXListItr XXXListForEach(XXXListItr _from, XXXListItr _to, DoFunc _DoFunc, Parm _parm);


/*----------------------------------------------------------------------------------*/

#endif /* #ifndef __XXX_LIST_H__ */

/*-----------------------------------------Tomer Dery-----------------------------------------*/
#ifndef __LIST_H__
#define __LIST_H__

#include "err.h"

/*----------------------------------------------------------------------------------*/

typedef void* Data;
typedef void* Parm;
typedef void* ListItr;

typedef struct List List;

typedef void(*PrintFunc)(Data _data);

typedef int (*MatchFunc)(Data _data1, Data _data2);
typedef int (*DoFunc)(Data _data, Parm _parm);

/*----------------------------------------------------------------------------------*/

List* ListCreate();          
void ListDestroy(List* _list);

ADTERR ListPushHead(List* _list , Data _data);
ADTERR ListPushTail(List* _list , Data _data);

ADTERR ListPopHead(List* _list , Data* _data);
ADTERR ListPopTail(List* _list , Data* _data);

size_t ListCountItems(List* _list);

int ListIsEmpty(List* _list);

void ListPrint(List* _list , PrintFunc _printFunc);

/*----------------------------------------------------------------------------------*/

/*return first item after begin stub*/
ListItr ListBegin(List* _list);

/*return end stub*/
ListItr ListEnd(List* _list);

ListItr ListNext(ListItr _itr);
ListItr ListPrev(ListItr _itr);

ListItr ListGetData(ListItr _itr);

/*----------------------------------------------------------------------------------*/

/* will return _insertBefore in case of failure . otherwise  will return iterator of new node that was inserted */
ListItr ListInsert(ListItr _insertBefore , Data _data);

/*----------------------------------------------------------------------------------*/

/* will return null in case of failure . otherwise  will return Data that was removed */
Data ListRemove(ListItr _toRemove);

/*----------------------------------------------------------------------------------*/

/* check between _from _to  (not including _to) 
   return iterator of match node 
   in case of no match - will return _to or end stub                                   
 */
ListItr ListFindFirst(ListItr _from, ListItr _to, MatchFunc _MatchFunc, Data _data);

/*----------------------------------------------------------------------------------*/

/* Do DoFunc between _from _to  (not including _to) 
   return iterator of the first item that failed (in case of success will return end stub)     
*/
ListItr ListForEach(ListItr _from, ListItr _to, DoFunc _DoFunc, Parm _parm);

/*----------------------------------------------------------------------------------*/











#endif /* #ifndef __LIST_H__ */

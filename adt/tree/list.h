/*-----------------------------------------Tomer Dery-----------------------------------------*/
#ifndef __LIST_H__
#define __LIST_H__

#include "err.h"
#include "adt.h"

/*----------------------------------------------------------------------------------*/

typedef void* ListItr;

typedef struct List List;


/*----------------------------------------------------------------------------------*/

List* ListCreate();          
void ListDestroy(List* _list);

ADTERR ListPushHead(List* _list , Data _data);
ADTERR ListPushTail(List* _list , Data _data);

ADTERR ListPopHead(List* _list , Data* _data);
ADTERR ListPopTail(List* _list , Data* _data);

size_t ListCountItems(List* _list);

int ListIsEmpty(List* _list);


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














#endif /* #ifndef __LIST_H__ */

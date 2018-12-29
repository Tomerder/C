/**************************************************************************************
    Author: Hagai Moshe
    Creation date :      16.6.13
    Last modified date:  16.6.13
    Description : .
***************************************************************************************/

#ifndef __DLL_GEN_H__
#define __DLL_GEN_H__
#include "ADTErr.h"


typedef struct 	List List;
typedef void*	Data;
typedef struct 	Node Node;
typedef void* 	Itr;

typedef void 	(PrintFunc)(void* _params);
typedef int 	(*CmpFunc)( Data* _data1, Data* _data2);
typedef int 	(*DoFunc)( Data* _data, void* _params);



List*		ListCreate(void);
void		ListDestroy (List* _list);
ADTErr		ListPushHead(List* _list, Data _data);
ADTErr		ListPushTail(List* _list, Data _data);
Data		ListPopHead (List* _list);
Data		ListPopTail (List* _list);
size_t		ListCountItems(List* _list);
int			ListIsEmpty	(List* _list);
void		ListPrint(List* _list, PrintFunc _printFunc );

		/*return the first node of the list, on empty list return end stab*/
Itr		ListBegin(List* _list);
		/*return the end stab*/
Itr		ListEnd(List* _list);
 		/*increment, on end stub undifined*/
Itr		ListNext(Itr _itr);
		/*decrement, on first stub undefined*/
Itr		ListPrev(Itr _itr);
		/*return data, if on one of the stubs undifined*/
Data	ListGetData(Itr _itr);
		/*insert the data prev to the itr and return the new itr, if failed return original itr*/
Itr		ListInsertBefore(Itr _itr, Data _data);
		/*remove itr, if failed return NULL*/
Data	ListRemove(Itr _itr);
		/*find the first data according to CmpFunc and return _Itr, if failed to find return _to */
Itr		ListFindFirst(Itr _from, Itr _to, CmpFunc _cmpFunc, Data _data);
		/*apply DoFunc to each data and return _to, if failed return the node on which has it failed*/
Itr		ListForEach(Itr _from, Itr _to, DoFunc _DoFunc, void* _params);		


#endif /*end if DLL_GEN_H*/










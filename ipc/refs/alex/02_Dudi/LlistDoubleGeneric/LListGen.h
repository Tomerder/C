/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-19    
    Last modified date:		2013-07-24
    Description: header file for double linked list functions.
        		 update 21/7: now for generic data using typedef void ListData.
********************************************************************************/

#ifndef __DOUBLE_LIST_GEN_H__
#define __DOUBLE_LIST_GEN_H__

#include <stdlib.h>
#include <stdio.h>

#include "ADTDefs.h"


/* double-linked list */
typedef 	struct List_t 	List;
/* list iterator */
typedef		void* 			ListItr;
/* for generic data */
typedef 	void*	 		ListData;


/*** pointers to function ***/

/* to print generic data */
typedef 	void	(*PrintFunc)	(void*);

/* compare 2 list nodes, for search */
typedef 	int		(*CompFunc)		(ListData, ListData);

/* act on ListData, using void parameters if necessary */
typedef 	int		(*DoFunc)		(ListData, void*);


/* create list, return NULL for failure */
List* 		ListCreate		(void);
/* destroy list */
void 		ListDestroy		(List* _list);

/* insert at beginning */
ADTErr 		ListPushHead	(List *_list, ListData _data);	
/* insert at end */
ADTErr 		ListPushTail	(List *_list, ListData _data);	

/* remove from beginning */
ListData 	ListPopHead		(List *_list);	
/* remove from end */
ListData 	ListPopTail		(List *_list);	

/* # of items */
size_t 		ListCountItems	(List *_list);
/*return 1 if list is empty, 0 otherwise */
int 		ListIsEmpty		(List *_list);

/* print list */
void 		ListPrint		(List *_list, PrintFunc _printFunc);


/********* Iterators *********/


/* get iter to 1st node */
/* for empty list, returns ListEnd */
ListItr		ListBegin	(List *_list);

/* get iter to past-last (tail) node */
ListItr		ListEnd		(List *_list);

/* go to next or previous node */
/*** going beyond boundaries results in UNDEFINED BEHAVIOR ***/
ListItr		ListNext	(ListItr _itr);
ListItr		ListPrev	(ListItr _itr);

/* get data */
/* getting data from tail - UNDEFINED BEHAVIOR */
ListData	ListGetData	(ListItr _itr);

/* insert before _itr, returns itr to inserted data */
/* if failed, return original _itr */
ListItr		ListInsertBefore	(ListItr _itr, ListData _data);

/* remove node & return data */
/* if failed - UNDEFINED BEHAVIOR */
ListData	ListRemove	(ListItr _itr);

/* search from _from to _to, not including _to */
/* return iterator of required data if found, _to if not */
/* if data duplication, return 1st found */
ListItr	 ListFindFirst	(ListItr _from, ListItr _to, CompFunc _cmpfunc, ListData _data);

/* apply _dofunc to all nodes from _from to _to (not including _to) */
/* return _to for success, the Itr where failed if failed */
ListItr	 ListForEach	(ListItr _from, ListItr _to, DoFunc _dofunc, void* _params);



#endif /* __DOUBLE_LIST_GEN_H__ */

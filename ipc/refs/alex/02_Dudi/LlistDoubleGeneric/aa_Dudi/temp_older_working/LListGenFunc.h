/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-19    
    Last modified date:		2013-07-22
    Description: header file for double linked list functions.
        		 update 21/7: now for generic data using typedef void ListData.
********************************************************************************/

#ifndef __DOUBLE_LIST_GEN_H__
#define __DOUBLE_LIST_GEN_H__

typedef struct List_t List;
typedef struct Node_t Node;

/* for generic data */
typedef void ListData;

/* to print generic data */
typedef void(*PrintFunc)(void*);


/* create list */
List* 		ListCreate		(void);
/* destroy list */
void 		ListDestroy		(List* _list);

/* insert at beginning */
ADTErr 		ListPushHead	(List *_list, ListData* _data);	
/* insert at end */
ADTErr 		ListPushTail	(List *_list, ListData* _data);	

/* remove from beginning */
ListData* 	ListPopHead		(List *_list, ListData** _data);	
/* remove from end */
ListData* 	ListPopTail		(List *_list, ListData** _data);	

/* # of items */
size_t 		ListCountItems	(List *_list);
/* check if list is empty */
int 		ListIsEmpty		(List *_list);


/********* Iterators *********/

typedef Node* ListItr;

/* compare 2 list nodes, for search */
typedef int(*CompFunc)(ListData*, ListData*);

/* act on ListData, using void parameters if necessary */
typedef int(*DoFunc)(ListData*, void*);


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
ListData*	ListGetData	(ListItr _itr);

/* insert before _itr, returns itr to inserted data */
/* if failed, return original _itr */
ListItr		ListInsertBefore	(ListItr _itr, ListData* _data);

/* remove node & return data */
/* if failed - UNDEFINED BEHAVIOR */
ListData*	ListRemove	(ListItr _itr);

/* search from _from to _to, not including _to */
/* return iterator of required data if found, _to if not */
/* if data duplication, return 1st found */
ListItr	 ListFindFirst	(ListItr _from, ListItr _to, CompFunc _cmpfunc, ListData* _data);

/* apply _dofunc to all nodes from _from to _to (not including _to) */
/* return _to for success, the Itr where failed if failed */
ListItr	 ListForEach	(ListItr _from, ListItr _to, DoFunc _dofunc, void* _params);



#endif /* __DOUBLE_LIST_GEN_H__ */

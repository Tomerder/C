/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-19    
    Last modified date:		2013-07-23
    Description: header file for double linked list functions.
        		 update 21/7: now for generic data using typedef void* Data.
********************************************************************************/

#ifndef __DOUBLE_LIST_GEN_H__
#define __DOUBLE_LIST_GEN_H__

/* double-linked list */
typedef struct List_t	List;
/* Iterator */
typedef struct Itr_t* 	Itr;
/* generic data */
typedef void* 			Data;
/* generic function parameter */
typedef void* 			Param;



/* ptr to func: print generic data */
typedef void(*PrintFunc)(void*);
/* ptr to func: compare 2 list nodes, for search */
typedef int(*CompFunc)(Data, Data);
/* ptr to func: act on ListData, using void parameters if necessary */
typedef int(*DoFunc)(Data, void*);


/* create list */
List* 		ListCreate		(void);
/* destroy list */
void 		ListDestroy		(List* _list);

/* insert at beginning */
ADTErr 		ListPushHead	(List *_list, Data _data);	
/* insert at end */
ADTErr 		ListPushTail	(List *_list, Data _data);	

/* remove from beginning */
Data	 	ListPopHead		(List *_list);	
/* remove from end */
Data	 	ListPopTail		(List *_list);	

/* # of items */
size_t 		ListCountItems	(List *_list);
/* check if list is empty */
int 		ListIsEmpty		(List *_list);
/* print list data */
void 		ListPrint		(List *_list, PrintFunc _printFunc);


/********* Iterators *********/

/* free iterator *
void	ItrDestroy	(Itr _itr);
*/
/* get iter to 1st node */
/* for empty list, returns ListEnd */
Itr		ListBegin	(List *_list);

/* get iter to past-last (tail) node */
Itr		ListEnd		(List *_list);

/* go to next or previous node */
/*** going beyond boundaries results in UNDEFINED BEHAVIOR ***/
Itr		ListNext	(Itr _itr);
Itr		ListPrev	(Itr _itr);

/* get data */
/* getting data from tail - UNDEFINED BEHAVIOR */
Data	ListGetData	(Itr _itr);

/* insert before _itr, returns itr to inserted data */
/* if failed, return original _itr */
Itr		ListInsertBefore	(Itr _itr, Data _data);

/* remove node & return data */
/* if failed - UNDEFINED BEHAVIOR */
Data	ListRemove	(Itr _itr);

/* search from _from to _to, not including _to */
/* return iterator of required data if found, _to if not */
/* if data duplication, return 1st found */
Itr	 	ListFindFirst	(Itr _from, Itr _to, CompFunc _cmpfunc, Data _data);

/* apply _dofunc to all nodes from _from to _to (not including _to) */
/* return _to for success, the Itr where failed if failed */
Itr	 	ListForEach	(Itr _from, Itr _to, DoFunc _dofunc, Param _params);

/* free Itr */
void 	ItrDestroy	(Itr _itr);

#endif /* __DOUBLE_LIST_GEN_H__ */

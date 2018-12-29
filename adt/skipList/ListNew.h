/**
 *  Author:				Efrat Hassid
 *  Creation date:		22.3.12
 *  Date last modified:	29.4.12
 *	Description:	linked list Library - header file
 **/
#ifndef _LIST_H_
#define _LIST_H_

typedef struct List List;
typedef void*  ListItr;

#ifndef _DATA_PARAM_COMP_DO
#define _DATA_PARAM_COMP_DO
typedef const void*	Data;
typedef const void*	Param;
typedef int		(*DataComp) (Data, Data);  /* 0 if equal */
typedef int		(*DoFunc)   (Data, Param); /* 0=success */
typedef void	(*PrintFunc)(Data);
#endif /* _DATA_PARAM_COMP_DO */


/*____General List API____*/
List* 	ListCreate	();
void	ListDestroy	(List*);

ListItr	ListPushHead(List*, Data);
ListItr	ListPushTail(List*, Data);

Data	ListPopHead	(List*);
Data	ListPopTail	(List*);

size_t	ListCountItems(List*);
int 	ListIsEmpty	(List*);

/*____Iterator API________*/
ListItr ListBegin	(List*);
ListItr ListEnd		(List*);
ListItr ListNext	(ListItr);
ListItr ListPrev	(ListItr);
Data	ListGetData	(ListItr);
int 	ListIsSame  (ListItr, ListItr); /*compare iterators*/
	
/* ListInsert and retrn ListItr to new */
ListItr ListInsert	(ListItr, Data);
Data	ListRemove	(ListItr);

/* return ListItr to the first item in list
 * for which _cmp(list_data, argument_data) return 0
 */
ListItr ListFindFirst(ListItr _from, ListItr _to, DataComp _cmp, Data _data); 
/*
 * perform specified function _do(Data, Param) for each item in the
 * collection until error is returned from the _do function
 */
int		ListForEach	 (ListItr _from, ListItr _to, DoFunc _do, Param _param);

#endif /* #ifndef_LIST_H_ */

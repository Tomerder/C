/**************************************************************************************
    Author: Hagai Moshe
    Creation date :      16.6.13
    Last modified date:  16.6.13
    Description : .
***************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "DLLGen.h"




struct Node
{
	struct	Node* 	m_next;
	struct	Node* 	m_prev;
	Data			m_data;
};

struct List
{
	Node	m_first;
	Node	m_last;
};


static Node* 	BuildStruct( Data  _data);
static Node*	ListInsertBeforeImp(Node *_node, Data _data);
static Data		ListRemoveImp(Node *_node);




List*	ListCreate(void)
{
	List* list;

	list = (List*)malloc(sizeof(List));
	if( NULL ==  list ){
		return NULL;
	}/*if*/
	
	list->m_first.m_prev = NULL;
	list->m_first.m_next = &list->m_last;
	list->m_first.m_data = (Data*)0xbaaaaaaa;
		
	list->m_last.m_prev  = &list->m_first;
	list->m_last.m_next  = NULL;
	list->m_last.m_data  = (Data*)0xbaaaaaaa;

return list;
}/*ListCreate*/




void	ListDestroy(List* _list)
{
	Data ret;	

	assert(_list);

	ret = ListPopHead(_list);
	while( ret ){
		ret = ListPopHead(_list);
	}/*while*/

	free(_list);
	
}/*ListDestroy*/




ADTErr	ListPushHead(List* _list, Data _data)
{
	Node* new;
	assert(_list);

	new = ListInsertBeforeImp(_list->m_first.m_next, _data);
	if(!new){
		return ERR_ALLOCATION_FAILED;
	}

return ERR_OK;
}/*ListPushHead*/



ADTErr	ListPushTail(List* _list, Data _data)
{
	Node* new;
	assert(_list);

	new = ListInsertBeforeImp(&_list->m_last, _data);
	if(!new){
		return ERR_ALLOCATION_FAILED;
	}

return ERR_OK;
}/*ListPushTail*/




Data	ListPopHead(List* _list)
{
	assert(_list);
	
return  ListRemoveImp( _list->m_first.m_next);
}/*ListPophead*/



Data	ListPopTail(List* _list)
{
	assert(_list);
	
return ListRemoveImp( _list->m_last.m_prev);
}/*ListPopTail*/



size_t	ListCountItems(List* _list)
{
	Node *current;	
	int i;	

	assert(_list);

	current = _list->m_first.m_next;
	for( i=0; current != &_list->m_last; current = current->m_next, i++);
	
return i;
}/*ListCountItems*/



int	ListIsEmpty(List* _list)
{

	assert(_list);
	
return( _list->m_first.m_next == &_list->m_last );	
}/*IsListEmpty*/



void	ListPrint(List* _list, PrintFunc _printFunc)
{
	Node *current;	
	
	assert(_list);

	for(current = _list->m_first.m_next; current != &_list->m_last; current = current->m_next){
		_printFunc( current->m_data ) ;
	}/*for*/

	printf("\n");

}/*ListIsEmpty*/

/****************************** iter function *******************************/


Itr		ListBegin(List* _list)
{
	assert(_list);

return (Itr)_list->m_first.m_next;
}/*ListBegin*/


Itr		ListEnd(List* _list)
{
	assert(_list);

return (Itr)&_list->m_last;
}/*ListEnd*/


Itr		ListNext(Itr _itr)
{
	Node* tmpItr = (Node*)_itr;	

return (Itr)tmpItr->m_next;
}/*ListNext*/


Itr		ListPrev(Itr _itr)
{
	Node* tmpItr = (Node*)_itr;	

	if( !tmpItr->m_prev->m_prev ){
		return NULL;
	}

return (Itr)tmpItr->m_prev;
}/*ListPrev*/


Data	ListGetData(Itr _itr)
{
	Node* tmpItr = (Node*)_itr;	
	if( !tmpItr->m_next ){
		return NULL;
	}

return tmpItr->m_data;
}/*ListGetData*/



Itr		ListInsertBefore(Itr _itr, Data _data)
{
	Node* node; 

	node = ListInsertBeforeImp( (Node*)_itr, _data);
	if(!node){
		return _itr;
	}
	
return (Itr)node;
}/*ListInsertBefore*/



Data	ListRemove(Itr _itr)
{

return ListRemoveImp((Node*)_itr);
}/*ListRemove*/




Itr		ListFindFirst(Itr _from, Itr _to, CmpFunc _cmpFunc, Data _data)
{
	Node* current;
	int cmpFunkRet;

	assert(_cmpFunc);

	for(current=(Node*)_from; current != (Node*)_to; current= ListNext(current)){
		cmpFunkRet = _cmpFunc( current->m_data, _data);		
		if(!cmpFunkRet){
			break;
		}
	}/*for(i)*/

return (Itr)current;
}/*ListFindFirst*/



Itr		ListForEach(Itr _from, Itr _to, DoFunc _DoFunc, void* _params)
{
	Node* current;
	int DoFunckRet;

	assert(_DoFunc);

	for(current=(Node*)_from; current != (Node*)_to; current= ListNext(current)){
		DoFunckRet = _DoFunc( current->m_data, _params );		
		if(!DoFunckRet){
			break;
		}
	}/*for(i)*/

return (Itr)current;
}/*ListForEach*/





/****************** prototype ******************/


static Node* BuildStruct( Data _data){
	Node* new;
	
	new = (Node*)malloc(sizeof(Node));
	if(NULL == new){
		return NULL;
	}
	new->m_data = _data;

return new;
}/*BuildStruct*/



static Node*	ListInsertBeforeImp(Node *_node, Data _data)
{
	Node* new;

 	new = BuildStruct(_data);
	if( !new ){
		return NULL;
	}

	new->m_prev = _node->m_prev;
	new->m_next = _node;
	_node->m_prev->m_next = new;
	_node->m_prev = new;

return new;
}/*ListInsertBeforeImp*/



static Data		ListRemoveImp(Node *_node)
{
	Node* 	tmpPtr;
	Data 	tmpData;	

	if( 0xbaaaaaaa == (int)_node->m_data ){
		return NULL;
	}
	
	/* copy backups */
	tmpData = _node->m_data;
	tmpPtr  = _node;
	/* redirecting pointers */
	_node->m_next->m_prev = _node->m_prev;		
	_node->m_prev->m_next = _node->m_next;
	
	free(tmpPtr);
return tmpData;
}/*ListRemoveImp*/






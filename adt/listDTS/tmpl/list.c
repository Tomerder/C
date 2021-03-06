/*-----------------------------------------Tomer Dery-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"


#define LIST_BEGIN(list) (&(list->m_head))
#define LIST_END(list) (&(list->m_tail))
#define IS_EMPTY_LIST(list) (LIST_BEGIN(list)->m_next ==  LIST_END(list) )

#define NEXT_ITR(itr) (   (ListItr) (  ((Node*)itr)->m_next )  )
#define PREV_ITR(itr) (   (ListItr) (  ((Node*)itr)->m_prev )  )

#define IS_FIRST(itr) (   ((Node*)(itr))->m_prev->m_prev == NULL    ) 

#define SUCCESS 1
enum {FALSE, TRUE}; 


typedef struct Node Node;

/*------------------------------------------------------------------------------------*/

struct Node{
	void* m_data;
	Node* m_prev;
	Node* m_next;
};


struct List{
	Node m_head;
	Node m_tail;
};


static void InsertNode(Node* toInsert, Node* prev, Node* next); 


/*------------------------------------------------------------------------------------*/

List* ListCreate(){
       
	List* list;		

	list = (List*)malloc(sizeof(List));    /*malloc : vector*/
	if (list == NULL){
		return NULL;
	}	

	LIST_BEGIN(list)->m_next = LIST_END(list);
	LIST_END(list)->m_prev = LIST_BEGIN(list); 
	
	LIST_BEGIN(list)->m_prev = NULL;
	LIST_END(list)->m_next = NULL; 
	LIST_BEGIN(list)->m_data = NULL;
	LIST_END(list)->m_data = NULL;

	return list;
}

/*------------------------------------------------------------------------------------*/


void ListDestroy(List* _list){
	Node* curNode;
	
	assert(_list);
	
	curNode = LIST_BEGIN(_list)->m_next;	
	
	while(curNode != LIST_END(_list)  ){
		free(curNode);
		curNode = curNode->m_next;
	}

	free(_list);
}

/*------------------------------------------------------------------------------------*/


static void InsertNode(Node* _newNode, Node* _prev, Node* _next){ 
	
	_newNode->m_prev = _prev;
	_newNode->m_next = _next;

	_prev->m_next = _newNode;
	_next->m_prev = _newNode;

}


/*------------------------------------------------------------------------------------*/


static void RemoveNode(Node* _popNode, Node* _prev, Node* _next){ 
	
	/*fix list*/
	_prev->m_next = _next;
	_next->m_prev = _prev;
	
	/*clean*/
	free(_popNode);

}




/*------------------------------------------------------------------------------------*/

ADTERR ListPushHead(List* _list , Data _data){
	Node* newNode;
		
	assert(_list);
	assert(_data);
	
	newNode = (Node*) malloc (sizeof(Node));
	if ( newNode == NULL){
		return ALLOC_FAILED;
	}
	
	/*insert new*/
	newNode->m_data = _data; 
	
	InsertNode(newNode, LIST_BEGIN(_list) , LIST_BEGIN(_list)->m_next );
	

	return OK;

}


/*------------------------------------------------------------------------------------*/


ADTERR ListPushTail(List* _list , Data _data){
	Node* newNode;
		
	assert(_list);
	assert(_data);
	
	newNode = (Node*) malloc (sizeof(Node));
	if ( newNode == NULL){
		return ALLOC_FAILED;
	}
	
	/*insert new*/
	newNode->m_data = _data; 
	
	InsertNode(newNode, LIST_END(_list)->m_prev , LIST_END(_list) );
	

	return OK;

}


/*------------------------------------------------------------------------------------*/


ADTERR ListPopHead(List* _list , Data* _data){
	Node* popNode;
		
	assert(_list);
	assert(_data);
	
	if(IS_EMPTY_LIST(_list)){
		return UNDERFLOW;
	}
	
	/*pop node*/
	popNode = LIST_BEGIN(_list)->m_next;
	*_data = popNode->m_data;
	
	/*fix list*/
	RemoveNode(popNode , LIST_BEGIN(_list) , popNode->m_next  );
	
	return OK;

}


/*------------------------------------------------------------------------------------*/


ADTERR ListPopTail(List* _list , Data* _data){
	Node* popNode;
		
	assert(_list);
	assert(_data);
	
	if(IS_EMPTY_LIST(_list)){
		return UNDERFLOW;
	}
	
	/*pop*/
	popNode = LIST_END(_list)->m_prev;
	*_data = popNode->m_data;
	
	/*fix list*/
	RemoveNode(popNode , popNode->m_prev  , LIST_END(_list)   );
	
	return OK;

}


/*------------------------------------------------------------------------------------*/


size_t ListCountItems(List* _list){
	int count=0;
	Node* curNode;
		
	assert(_list);
	
	curNode = LIST_BEGIN(_list);
	while(curNode->m_next != LIST_END(_list) ){
		curNode = curNode->m_next;
		count++;
	}

	return count;

}

/*------------------------------------------------------------------------------------*/

int ListIsEmpty(List* _list){
	assert(_list);
	
	return IS_EMPTY_LIST(_list);

}

/*------------------------------------------------------------------------------------*/


void ListPrint(List* _list , PrintFunc _printFunc){
	Node* curNode;
	
	curNode = _list->m_head.m_next;
	while(curNode != LIST_END(_list) ){
		_printFunc(curNode->m_data);
		printf(" , ");
		curNode = curNode->m_next;
	}
	printf("\n");
}

/*------------------------------------------------------------------------------------*/


ListItr ListBegin(List* _list){
	return (ListItr)(LIST_BEGIN(_list)->m_next);
}


ListItr ListEnd(List* _list){
	return (ListItr)(LIST_END(_list));
}

/*------------------------------------------------------------------------------------*/

ListItr ListNext(ListItr _itr){
	
	assert (_itr); 
	
	return NEXT_ITR(_itr);
}


ListItr ListPrev(ListItr _itr){
	
	assert (_itr); 
	
	if( IS_FIRST(_itr) ){
		return NULL;
	}
	
	return PREV_ITR(_itr);
}

/*------------------------------------------------------------------------------------*/

Data ListGetData(ListItr _itr){
	Node* curNode;
	
	assert (_itr); 
	
	curNode = ((Node*)_itr);
	
	return (Data)(curNode->m_data);

}

/*------------------------------------------------------------------------------------*/


ListItr ListInsert(ListItr _insertBeforeItr , Data _data){
	Node* newNode;
	Node* insertBeforeNode;
	
	assert (_insertBeforeItr); 
	
	newNode = (Node*) malloc (sizeof(Node));
	if( newNode == NULL){
		return _insertBeforeItr;
	}
	
	insertBeforeNode = (Node*)_insertBeforeItr;
	
	if (insertBeforeNode->m_prev == NULL){   /*protection*/
		return NULL;
	} 
	
	newNode->m_data = _data;

	InsertNode(newNode, insertBeforeNode->m_prev , insertBeforeNode );

	return (ListItr)(newNode);

}


/*------------------------------------------------------------------------------------*/


Data ListRemove(ListItr _removeItr){
	Node* removeNode;
	Data toRet;
	
	assert (_removeItr); 
	
	removeNode = (Node*)(_removeItr);
	
	if (removeNode->m_prev == NULL || removeNode->m_next == NULL){   /*protection*/
		return NULL;
	} 
	
	toRet = removeNode->m_data; 
	
	RemoveNode(removeNode , removeNode->m_prev  , removeNode->m_next   );

	return toRet;

}


/*------------------------------------------------------------------------------------*/

ListItr ListFindFirst (ListItr _from, ListItr _to, MatchFunc _MatchFunc, Data _data){
	ListItr itr;
	Node* curNode;
	
	assert(_from);
	assert(_to);
	assert(_MatchFunc);
	
	/*compere while didnt reach   _to  or  end stub:(NEXT==NULL)*/
	itr = _from;
	while (itr != _to  &&  NEXT_ITR(itr) != NULL ) {
		curNode = (Node*)(itr);
		
		if ( _MatchFunc (curNode->m_data, _data) == TRUE ) {
			break;
		}	
		
		itr = NEXT_ITR(itr);	
	}
	

	return itr;

}

/*------------------------------------------------------------------------------------*/

ListItr ListForEach (ListItr _from, ListItr _to, DoFunc _DoFunc, Parm _parm){
	ListItr itr;
	Node* curNode;
	
	assert(_from);
	assert(_to);
	assert(_DoFunc);
	
	/*do func while didnt reach   _to  or  end stub:(NEXT==NULL)*/
	itr = _from;
	while (itr != _to  &&  NEXT_ITR(itr) != NULL ) {
		curNode = (Node*)(itr);
		
		 if( _DoFunc (curNode->m_data, _parm) != SUCCESS) {
		 	break;
		 }	
		
		itr = NEXT_ITR(itr);	
	}
	
	return itr;

}

/*------------------------------------------------------------------------------------*/



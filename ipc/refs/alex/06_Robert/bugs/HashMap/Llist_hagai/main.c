/**************************************************************************************
    Author: Hagai Moshe
    Creation date :      16.6.13
    Last modified date:  16.6.13
    Description : .
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DLLGen.h"
#include "GenericQueue.h"
#include "GenericStack.h"
#include "ADTErr.h"

#define MAXNUM 10


/*structs on main*/
struct Node{

	struct	Node* 	m_next;
	struct	Node* 	m_prev;
	int		m_data;
};


struct List{
	Node	m_first;
	Node	m_last;
};



int CmpFind( Data* _node1, Data* _node2)
{
	return *(int*)_node1 - *(int*)_node2;
}/*CmpFunc*/



int DoEach( Data* _node, void* _par)
{
	*(int*)_node = *(int*)_par + *(int*)_node;
	return 1;
}/*DoEach*/



void	Print(void* _p)
{
	
	printf("%d, ",*(int*)_p);

}/*print*/

/************************* unitest ***************************/


void	ListPushHeadTest()
{
	List* 	list=NULL;
	float 	check=0;
	int  	error ,i, numItem;
	int		arrA[MAXNUM], arrB[MAXNUM];

	/* ---- checks create list ----*/
	list = ListCreate();
	check = (list) ? check:1.1	;
	
	/* ---- checks normal operation ---- */

	for(i=0; i<MAXNUM; i++){
		arrA[i] = i;
		error = ListPushHead(list, (void*)&arrA[i]);	
		check = (error == ERR_OK) ? check:2.1	;
	}

	numItem = ListCountItems( list);
	check = ( MAXNUM == numItem ) ? check:2.2	;

	for(i=0; i<MAXNUM; i++){
		arrB[i] = *(int*)ListPopTail(list);
		check = ( arrB[i] == i ) ? check:2.3;
	}

	ListDestroy(list);
	if(check){
		printf("--- pushHead and create  	%2.2f ---\n",check);
	}
}/*ListPushHeadTest*/




void	ListPushTailTest()
{
	List* 	list=NULL;
	float 	check=0;
	int  	error ,i, numItem;
	int		arrA[MAXNUM];

	list = ListCreate();
	/* ---- checks normal operation ---- */

	for(i=0; i<MAXNUM; i++){
		arrA[i] = i;
		error = ListPushTail(list, (void*)&arrA[i]);	
		check = (error == ERR_OK) ? check:2.1	;
	}

	numItem = ListCountItems( list);
	check = ( MAXNUM == numItem ) ? check:2.2	;

	for(i=0; i<MAXNUM; i++){
		numItem = *(int*)ListPopHead(list);
		check = ( numItem == i ) ? check:2.3;
	}

	ListDestroy(list);
	if(check){
		printf("--- pushTail		  	%2.2f ---\n",check);
	}
}/*ListPushTailTest*/




void	ListPopHeadTest()
{

	List* 	list=NULL;
	float 	check=0;
	int		*dataPtr, data=1;

	/* ---- checks empty list ---- */			
	list = ListCreate();
	dataPtr = ListPopHead(list);
	check = (dataPtr == NULL) ? check:1.1	;

	/* ---- checks normal operation ---- */

	ListPushTail(list, (void*)&data);
	dataPtr = (int*) ListPopHead(list);
	check = (*dataPtr == 1) ? check:2.1	;

	ListDestroy(list);
	if(check){
		printf("--- ListPopHead 	  	%2.2f ---\n",check);
	}
}/*ListPopHeadTest*/




void	ListPopTailTest()
{

	List* 	list=NULL;
	float 	check=0;
	int		*dataPtr, data=1;
	
	list = ListCreate();
	/* ---- checks empty list ---- */	
	dataPtr = ListPopTail(list);
	check = (dataPtr == NULL) ? check:1.1	;

	/* ---- checks normal operation ---- */
	
	ListPushTail(list, (void*)&data);
	dataPtr = ListPopTail(list);
	check = (*dataPtr == 1) ? check:2.1	;

	ListDestroy(list);
	if(check){
		printf("--- ListPopTail 	  	%2.2f ---\n",check);
	}

}/*ListPopTailTest*/


void ListCountItemsTest()
{
	List* 	list=NULL;
	float 	check=0;
	int  	ret;
	int		data=1;

		/*empty list*/
	list = ListCreate();
	ret = ListCountItems(list);
	check = (ret == 0) ? check:1.1;
		/*normal run*/
	ListPushHead(list, (void*)&data);	
	ret = ListCountItems(list);
	check = (ret == 1) ? check:1.2;


	ListPopTail(list);
	ListDestroy(list);
	if(check){
		printf("--- ListCountItem   		%2.2f ---\n",check);
	}
}/*ListCountItemsTest*/




void ListIsEmptyTest()
{
	List* 	list=NULL;
	float 	check=0;
	int  	ret;
	int		data=1;
	
	list  = ListCreate();
	ret = ListIsEmpty(list);
	check =(ret == 1) ? check:1.1;
	ListPushHead(list, (void*)&data);	
	ret = ListIsEmpty(list);
	check = (ret == 0) ? check:1.2;

	ListPopTail(list);
	ListDestroy(list);
	if(check){
		printf("--- IsListEmpty   		%2.2f ---\n",check);
	}
}/*ListIsEmpty*/




void ListBeginTest()
{
	List* 	list=NULL;
	Itr		itr;
	float 	check=0;
	int		i, arr[MAXNUM];

	list  = ListCreate();
		/*empty list / last stab*/
	itr = ListBegin(list);
	check = ( 0xbaaaaaaa == (int)((Node*)itr)->m_data ) ? check:1.1;

	for(i=0; i<MAXNUM; i++){
		arr[i]=i+1;
		ListPushTail(list, (void*)&arr[i]);
	}
		/*normal run*/
	itr = ListBegin(list);
	check = ( 1 == *(int*)((Node*)itr)->m_data ) ? check:2.1;

	for(i=0; i<MAXNUM; i++){
		ListPopTail(list);
	}
	ListDestroy(list);
	if(check){	
		printf("--- ListBeginTest  		%2.2f ---\n",check);
	}
}/*ListBeginTest*/



void ListEndTest()
{
	List* 	list=NULL;
	Itr		itr;
	float 	check=0;
	int		i, arr[MAXNUM];

	list  = ListCreate();
		/*empty list / last stab*/
	itr = ListEnd(list);
	check = ( !(Node*)((Node*)itr)->m_next ) ? check:1.1;

	for(i=0; i<MAXNUM; i++){
		arr[i]=i+1;
		ListPushTail(list, (void*)&arr[i]);
	}
		/*normal run*/
	itr = ListEnd(list);
	check = ( !(Node*)((Node*)itr)->m_next ) ? check:2.1;

	for(i=0; i<MAXNUM; i++){
		ListPopTail(list);
	}
	ListDestroy(list);
	if(check){
		printf("--- ListEndTest  		%2.2f ---\n",check);
	}
}/*ListEndTest*/



void	ListNextTest()
{
	List* 	list=NULL;
	Itr		itr, itrPrev;
	float 	check=0;
	int		i, arr[MAXNUM];

	list  = ListCreate();
		/*empty list / last stab*/
	itr = ListBegin(list);
	itr = ListNext(itr);
	check = ( !(Node*)itr ) ? check:1.1;

	for(i=0; i<MAXNUM; i++){
		arr[i]=i+1;
		ListPushTail(list, (void*)&arr[i]);
	}
		/*normal run*/
	itr = ListBegin(list);
	itrPrev =itr;
	itr = ListNext(itr);
	check = ( (Node*)itr == (Node*)((Node*)itrPrev)->m_next ) ? check:2.1;
	itrPrev =itr;
	itr = ListNext(itr);
	check = ( (Node*)itr == (Node*)((Node*)itrPrev)->m_next ) ? check:2.2;

	for(i=0; i<MAXNUM; i++){
		ListPopTail(list);
	}
	ListDestroy(list);
	if(check){
		printf("--- ListEnd		  	%2.2f ---\n",check);
	}
}/*ListNextTest*/




void	ListPrevTest()
{
	List* 	list=NULL;
	Itr		itr, itrPrev;
	float 	check=0;
	int		i, arr[MAXNUM];

	list  = ListCreate();
		/*empty list / last stab*/
	itr = ListBegin(list);
	itrPrev = ListPrev(itr);
	check = ( !(Node*)itrPrev ) ? check:1.1;

	for(i=0; i<MAXNUM; i++){
		arr[i]=i+1;
		ListPushTail(list, (void*)&arr[i]);
	}
		/*normal run*/
	itr = ListEnd(list);
	itrPrev = itr;
	itrPrev = ListPrev(itr);
	check = ( (Node*)itr == (Node*)((Node*)itrPrev)->m_next ) ? check:2.1;
	itrPrev = itr;
	itrPrev = ListPrev(itr);
	check = ( (Node*)itr == (Node*)((Node*)itrPrev)->m_next ) ? check:2.2;

	for(i=0; i<MAXNUM; i++){
		ListPopTail(list);
	}
	ListDestroy(list);
	if(check){
		printf("--- ListPrev	  		%2.2f ---\n",check);
	}
}/*ListPrevTest*/





void	ListGetDataTest()
{
	List* 	list=NULL;
	Itr	itr;
	float 	check=0;
	int		i, *data, arr[MAXNUM];

	list  = ListCreate();
		/*empty list / last stab*/
	itr = ListBegin(list);
	data = ListGetData(itr);
	check = ( !data ) ? check:1.1;

	for(i=0; i<MAXNUM; i++){
		arr[i]=i+1;
		ListPushTail(list, (void*)&arr[i]);
	}

		/*normal run*/
	itr = ListBegin(list);
	data = ListGetData(itr);
	check = ( *(int*)((Node*)itr)->m_data == *(int*)data ) ? check:2.1;
	itr = ListNext(itr);
	data = ListGetData(itr);
	check = ( *(int*)((Node*)itr)->m_data == *(int*)data ) ? check:2.2;

	for(i=0; i<MAXNUM; i++){
		ListPopTail(list);
	}
	ListDestroy(list);
	if(check){
		printf("--- ListGetData	  		%2.2f ---\n",check);
	}
}/*ListGetDataTest*/



void	ListInsertTest()
{
	List* 	list=NULL;
	Itr		itr, itrPrev;
	float 	check=0;
	int		i, arr[MAXNUM];

	list  = ListCreate();

		/*normal run*/
	itr = ListBegin(list);
	for(i=0; i<MAXNUM; i++){
		arr[i]=i+1;
		itrPrev = ListInsertBefore(itr, (void*)&arr[i]);
		check = ( (Node*)itr == (Node*)((Node*)itrPrev)->m_next ) ? check:i;
		itr = itrPrev;
	}

	for(i=0; i<MAXNUM; i++){
		ListPopTail(list);
	}
	ListDestroy(list);
	if(check){
		printf("--- ListInsert	  		%2.2f ---\n",check);
	}
}/*ListInsertTest*/




void	ListRemoveTest()
{
	List* 	list=NULL;
	Itr		itr;
	float 	check=0;
	int		i, *data, arr[MAXNUM];

	list  = ListCreate();
		/*empty list / last stab*/
	itr = ListBegin(list);
	data = ListRemove(itr);
	check = ( !data ) ? check:1.1;	

		/*normal run*/
	for(i=0; i<MAXNUM; i++){
		arr[i]=i+1;
		ListPushTail(list, (void*)&arr[i]);
	}

		/*normal run*/
	for(i=0; i<MAXNUM; i++){
		itr = ListBegin(list);
		data = ListRemove(itr);
		check = ( *data == arr[i] ) ? check:i;
	}

	ListDestroy(list);
	if(check){
		printf("--- ListRemove	  		%2.2f ---\n",check);
	}
}/*ListRemoveTest*/




void ListFindFirstTest()
{
	List* 	list=NULL;
	Itr		itr, from, to;
	float 	check=0;
	int		i=0, data, arr[MAXNUM];

	list  = ListCreate();
		/*empty list / last stab*/
	from = ListBegin(list);
	itr = ListFindFirst( from, from, CmpFind, &data);
	check = ( !(Node*)((Node*)itr)->m_next ) ? check:1.1;	

		/*normal run*/
	for(i=0; i<MAXNUM; i++){
		arr[i]=i;
		ListPushTail(list, (void*)&arr[i]);
	}

		/*failing to find data*/
	data = MAXNUM+1;
	from = ListBegin(list);
	to  = ListEnd(list);
	itr = ListFindFirst( from, to, CmpFind, &data);
	check = ( (Node*)itr == (Node*)to ) ? check:2.1;	

		/*noraml run*/
	data = MAXNUM-1;
	from = ListBegin(list);
	to  = ListEnd(list);
	itr = ListFindFirst( from, to, CmpFind, &data);
	check = ( MAXNUM-1 == *(int*)((Node*)itr)->m_data  ) ? check:3.1;	


	for(i=0; i<MAXNUM; i++){
		ListPopTail(list);
	}
	ListDestroy(list);
	if(check){
		printf("--- ListFindFirst  		%2.2f ---\n",check);
	}
}/*ListFindFirstTest*/




void ListForEachTest()
{
	List* 	list=NULL;
	Itr		itr, from, to;
	float 	check=0;
	int		i=0, *data , change=3, arrA[MAXNUM], arrB[MAXNUM];

	list  = ListCreate();
		/*empty list / last stab*/
	from = ListBegin(list);
	itr = ListForEach( from, from, DoEach, &data);
	check = ( !(Node*)((Node*)itr)->m_next ) ? check:1.1;	

		/*normal run*/
	for(i=0; i<MAXNUM; i++){
		arrA[i]=i;
		ListPushTail(list, (void*)&arrA[i]);
	}

		/*noraml run*/
	from = ListBegin(list);
	to   = ListEnd(list);
	itr = ListForEach( from, to, DoEach, &change);

	check = ( (Node*)itr == (Node*)to ) ? check:3.1;
	for(i=0; i<MAXNUM; i++){
		arrB[i] = *(int*)ListPopHead(list);
		check = ( arrB[i] == i+3 ) ? check:3.2;
	}	

	ListDestroy(list);
	if(check){
		printf("--- ListForEach  		%2.2f ---\n",check);
	}
}/*ListForEachTest*/




void	QueueTest()
{
	Queue	*queue;
	float 	check=0;
	int  	*data, ret ,i;
	int		arrA[MAXNUM], arrB[MAXNUM];

	/* ---- checks create list ----*/
	queue = QueueCreate ();
	check = (queue) ? check:1.1	;

	/* ---- empty list ----*/
	data 	= QueueRemove ( queue );
	check 	= ( !data ) ? check:2.1;
	ret 	= QueueIsEmpty( queue );
	check 	= ( ret ) ? check:2.2;

	/* ---- checks normal operation ---- */

	for(i=0; i<MAXNUM; i++){
		arrA[i] = i;
		ret = QueueInsert ( queue, (void*)&arrA[i]);	
		check = (ret == ERR_OK) ? check:2.1	;
	}

	ret 	= QueueIsEmpty( queue );
	check 	= ( !ret ) ? check:2.2;

	for(i=0; i<MAXNUM; i++){
		arrB[i] = *(int*)QueueRemove ( queue);
		check = ( arrB[i] == i ) ? check:2.2;
	}
	/* ---- checks number of items ---- */
	data = QueueRemove ( queue);
	check = ( !data ) ? check:3.1;

	QueueDestroy(queue);
	if(check){
		printf("--- QueueTest 	 	%2.2f ---\n",check);
	}
}/*QueueTest*/




void	StackTest()
{
	Stack	*stack;
	float 	check=0;
	int  	*data, ret ,i;
	int		arrA[MAXNUM], arrB[MAXNUM];

	/* ---- checks create list ----*/
	stack = StackCreate ();
	check = (stack) ? check:1.1	;

	/* ---- empty list ----*/
	data 	= StackPop ( stack );
	check 	= ( !data ) ? check:2.1;
	ret 	= StackIsEmpty( stack );
	check 	= ( ret ) ? check:2.2;
	data 	= StackTop ( stack );
	check 	= ( !data ) ? check:2.3;

	/* ---- checks normal operation ---- */

	for(i=0; i<MAXNUM; i++){
		arrA[i] = i;
		ret = StackPush ( stack, (void*)&arrA[i]);	
		check = (ret == ERR_OK) ? check:3.1	;
	}

	ret 	= StackIsEmpty( stack );
	check 	= ( !ret ) ? check:3.2;

	for(i=MAXNUM-1; i>=0; i--){
		arrB[i] = *(int*)StackPop ( stack);
		check = ( arrB[i] == i ) ? check:3.3;

	}

	/* ---- checks number of items ---- */
	data = StackPop ( stack);
	check = ( !data ) ? check:4.1;

	StackDestroy(stack);
	if(check){
		printf("--- StackTest  		%2.2f ---\n",check);
	}
}/*StackTest*/







int main(){

	printf("\n	unitest\n");
	ListPushHeadTest();
	ListPushTailTest();
	ListPopHeadTest();
	ListPopTailTest();
	ListPushTailTest();
	ListCountItemsTest();
	ListIsEmptyTest();
	
	printf("\n");
	ListBeginTest();
	ListEndTest();
	ListNextTest();
	ListPrevTest();
	ListGetDataTest();
	ListInsertTest();
	ListRemoveTest();
	ListFindFirstTest();
	ListForEachTest();

	QueueTest();
	StackTest();
	

return 0;
}/*main	*/

/*
StackPrint  ( stack, Print);
QueuePrint  ( queue, Print);
ListPrint(list, Print);
*/



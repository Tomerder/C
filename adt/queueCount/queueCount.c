#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "err.h"


enum {FALSE, TRUE};

#define DEBUG FALSE

#define SIZE 10
#define PRINT_ERR if(err!=0){ printf("FAIL: error no. %d  , ",err); }else{printf("PASS ,");}

/*------------------------------------------------------------------------------------*/

void TestQueueCount(int* _arr);
Queue* InitQueue(int* _arr);
void QueueCount(Queue* _queueIn, Queue* _queueOut);
int IsEqItems(Item item , Item itemToCmp);
void CopyQueue(Queue* _to, Queue* _from, int* _countTotalItems);

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int arr[SIZE] = {22 , 35 , 7  , 5 , 2 , 5 , 77 , 32 , 5 , 22};
	
	TestQueueCount(arr);

    return 0; 	
}

/*------------------------------------------------------------------------------------*/

void PrintFuncInt(Item _item){
	printf("%d" , ((int)(_item))  );


}

/*------------------------------------------------------------------------------------*/

void TestQueueCount(int* _arr)
{
	/*init*/
	Queue* queueOut = QueueCreate(SIZE*2);
	Queue* queueIn = InitQueue(_arr);
	
	/*queue count*/
	QueueCount(queueIn, queueOut);	
	
	QueuePrint(queueIn , PrintFuncInt);
	QueuePrint(queueOut , PrintFuncInt);
	
	/*cleanup*/
	QueueDestroy(queueIn);
	QueueDestroy(queueOut);
}

/*------------------------------------------------------------------------------------*/

Queue* InitQueue(int* _arr)
{
	int i;
	
	Queue* queue = QueueCreate(SIZE);
	
	for(i = 0 ; i < SIZE; i++){
		QueueInsert(queue , (void*)_arr[i] );
	}

	return queue;
}

/*------------------------------------------------------------------------------------*/

void QueueCount(Queue* _queueIn, Queue* _queueOut)
{
	int i, countTotalItems = 0, countItem = 0;
	Item item, itemToCmp;
	Queue* tmpQ = QueueCreate(SIZE);

	/*copy to tmp and count items*/
	CopyQueue(tmpQ, _queueIn, &countTotalItems);
				
	/*build queueOut*/ 
	while ( ! IsQueueEmpty(tmpQ) ) {
		if(DEBUG){
			QueuePrint(tmpQ , PrintFuncInt);
			QueuePrint(_queueOut , PrintFuncInt);
			printf("countTotalItems : %d\n" , countTotalItems);
			printf("---------------------------------\n");
		}
	
		QueueRemove(tmpQ , &item);
		--countTotalItems;
		
		/*insert item*/
		QueueInsert(_queueOut , item);
		countItem = 1;
		
		if( IsQueueEmpty(tmpQ) ){
			QueueInsert(_queueOut , (void*)countItem);	
			break;
		}
		
		/*count and remove equal items*/
		for(i=0; i <= countTotalItems; i++){
			QueueRemove(tmpQ , &itemToCmp);
			
			if( IsEqItems(item , itemToCmp) ) {
				--countTotalItems;
				++countItem;
			}else{
				QueueInsert(tmpQ , itemToCmp);
			}
		}
		
		/*insert item count*/
		QueueInsert(_queueOut , (void*)countItem);	
	}

	QueueDestroy(tmpQ);
}

/*------------------------------------------------------------------------------------*/

void CopyQueue(Queue* _to, Queue* _from, int* _countTotalItems)
{
	Item item;
	int i;

	/*to tmp*/
	while(!IsQueueEmpty(_from) ){	
		QueueRemove(_from , &item);
		QueueInsert(_to , item);
		++(*_countTotalItems);
	}
	
	/*back to queueIn*/
	for(i=0; i < *_countTotalItems; i++){ 
		QueueRemove(_to , &item);
		QueueInsert(_from , item);
		QueueInsert(_to , item);
	}
}

/*------------------------------------------------------------------------------------*/

int IsEqItems(Item item , Item itemToCmp)
{
	return ((int)item == (int)itemToCmp);
}

/*------------------------------------------------------------------------------------*/








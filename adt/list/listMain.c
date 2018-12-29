/*-----------------------------------------Tomer Dery-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "list.h"


typedef enum {FALSE, TRUE} IsConected;
typedef enum {DIRECTED_BIT, WEIGHTED_BIT} Bitmap;

#define NUM_OF_ITEMS 8

#define RETURN_ERR if(err!=0) printf("FAIL:%d ",err)


typedef enum {FAIL,PASS} Result;
char* resultArr[2] = {"FAIL","PASS"};
#define SUCCESS 1

#define DEBUG 0

static List* Init (int* _srcArr);

static void PrintIntFunc (Data _data);
static List* CreateAndInsert (int* srcArr);
static void PopCheck (int* _srcArr);
static void IterCheck ( int* _srcArr);
static void InsertCheck ( int* _srcArr);
static void RemoveCheck( int* _srcArr);
static void FindFirstCheck( int* _srcArr);
static void ForEachCheck( int* _srcArr);

int IntMatchFunc(Data _data1, Data _data2);
int AddParmDoFunc(Data _data1, Parm _parm);
int SumDoFunc(Data _data1, Parm _parm);

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int srcArr[NUM_OF_ITEMS] = {0,1,2,3,4,5,6,7};
	PrintFunc printIntFunc = PrintIntFunc;
	
    CreateAndInsert(srcArr);
	/*ListPrint(list, printIntFunc);*/

	PopCheck(srcArr);	
	/*ListPrint(list, printIntFunc);*/
	
	IterCheck(srcArr);
	
	InsertCheck(srcArr);
	
	RemoveCheck(srcArr);
	
	FindFirstCheck(srcArr);
	
	ForEachCheck(srcArr);
	
	printf("/------------------------------------------------------/\n");
	
    return 0; 	

    
}

/*------------------------------------------------------------------------------------*/


static List* Init(int* _srcArr){
	List* list;
	int i , err;
	
	list = ListCreate();

	/*ListPushHead*/
	for(i=0; i<NUM_OF_ITEMS; i++){
		err = ListPushHead(list, &_srcArr[i]);
		RETURN_ERR;
	}
	
	return list;

}

/*------------------------------------------------------------------------------------*/

static List* CreateAndInsert(int* _srcArr){
	int i, err, resultIndex = PASS, count;
	List* list;
	
	printf("/------------------------------------------------------/\n");
	printf("check Create & push & countItems: ");
	
	list = ListCreate();
	
	if( !ListIsEmpty(list) ){
		resultIndex = FAIL;
	}
	
	count = ListCountItems(list);
	if( count != 0 ){
		resultIndex = FAIL;
	}
	
	/*ListPushHead*/
	for(i=0; i<NUM_OF_ITEMS; i++){
		err = ListPushHead(list, &_srcArr[i]);
		RETURN_ERR;
	}
	
	if( ListIsEmpty(list) ){
		resultIndex = FAIL;
	}
	
	count = ListCountItems(list);
	if( count != NUM_OF_ITEMS ){
		resultIndex = FAIL;
	}
	
	/*ListPushTail*/
	for(i=0; i<NUM_OF_ITEMS; i++){
		err = ListPushTail(list, &_srcArr[i]);
		RETURN_ERR;
	}
	
	count = ListCountItems(list);
	if( count != NUM_OF_ITEMS * 2 ){
		resultIndex = FAIL;
	}	
	
	printf("%s\n", resultArr[resultIndex]);
		
	ListDestroy(list);	
			
	return list;
}


/*------------------------------------------------------------------------------------*/


static void PopCheck (int* _srcArr){
	int* popTail;
	int* popHead;	
	int err, resultIndex = PASS;
	List* list;

	printf("/------------------------------------------------------/\n");
	printf("check Pop: ");
	
	
	list = Init(_srcArr);   /*init*/

	err = ListPopHead(list, (Data*)&popHead  )  ;
	RETURN_ERR;
	
	err = ListPopTail(list, (Data*)&popTail);
	RETURN_ERR;
	
	if(  (*popHead != _srcArr[NUM_OF_ITEMS-1]) || (*popTail != _srcArr[0] )  ){
		resultIndex = FAIL;
	}
	
	printf("%s\n", resultArr[resultIndex]);
	
	ListDestroy(list);	 /*cleanup*/
		
}

/*------------------------------------------------------------------------------------*/

static void IterCheck( int* _srcArr){
	List* list;
	ListItr itr; 
	int i, resultIndex = PASS , dataToGet;
	
	printf("/------------------------------------------------------/\n");
	printf("check Iter & getData: ");
	
	list = Init(_srcArr);       /*init*/

	/*iter next*/
	i=0;
	for(itr = ListBegin(list) ; itr != ListEnd(list) ; itr = ListNext(itr) , i++){		
		dataToGet = *(int*) (ListGetData(itr)) ;		
		
		if( dataToGet != _srcArr[NUM_OF_ITEMS  -1 - i] ){
			resultIndex = FAIL;
		}		
	}	
	
	/*iter prev positive check*/
	itr =  ListEnd(list);
	i=0;
	do{
		itr = ListPrev(itr);	
		
		dataToGet = *(int*) (ListGetData(itr)) ;
	
		if( dataToGet != _srcArr[i] ){
			resultIndex = FAIL;
		}	
		
		i++;
	}while( itr != ListBegin(list) );
	
	/*iter prev negetive check*/
	itr =  ListBegin(list);
	itr = ListPrev(itr);	
	if ( itr != NULL){
		resultIndex = FAIL;
	}
	
	

	
	printf("%s\n", resultArr[resultIndex]);
	
	ListDestroy(list);	    /*cleanup*/
	
}

/*------------------------------------------------------------------------------------*/

static void InsertCheck( int* _srcArr){
	List* list;
	ListItr itr; 
	ListItr itrRet;
	int resultIndex = PASS;
	int* dataToGet;
	int toInsert = 777;
	
	printf("/------------------------------------------------------/\n");
	printf("check Insert: ");
	
	list = Init(_srcArr);       /*init*/


	itr = ListBegin(list);     	
	itrRet = ListInsert(itr , (Data)&toInsert);
	dataToGet = (int*)ListGetData(itrRet);
	if( *dataToGet != toInsert || itr == itrRet ) {
		resultIndex = FAIL;
	}
	
	printf("%s\n", resultArr[resultIndex]);
	
	ListDestroy(list);	    /*cleanup*/
	
}

/*------------------------------------------------------------------------------------*/

static void RemoveCheck( int* _srcArr){
	List* list;
	ListItr itr; 
	int resultIndex = PASS;
	int* dataToGet;
	
	printf("/------------------------------------------------------/\n");
	printf("check Remove: ");
	
	list = Init(_srcArr);       /*init*/

	/*possitive check*/
	itr = ListBegin(list);     	
	dataToGet = ListRemove(itr);
	if( *dataToGet != _srcArr[NUM_OF_ITEMS -1]  ) {
		resultIndex = FAIL;
	}
	
	/*negetive check*/
	itr = ListEnd(list);     	
	dataToGet = ListRemove(itr);
	if( dataToGet != NULL ) {
		resultIndex = FAIL;
	}
	
	
	printf("%s\n", resultArr[resultIndex]);
	
	ListDestroy(list);	    /*cleanup*/
	
}


/*------------------------------------------------------------------------------------*/

static void FindFirstCheck( int* _srcArr){
	List* list;
	ListItr itrFrom; 
	ListItr itrTo;
	ListItr itrRet;
	MatchFunc intMatchFunc = IntMatchFunc ; 
	int valToComp = 5;
	int* dataToGet;	
	int resultIndex = PASS;

	
	printf("/------------------------------------------------------/\n");
	printf("check FindFirst: ");
	
	list = Init(_srcArr);       /*init*/


	itrFrom = ListBegin(list); 
	itrTo = ListEnd(list); 
	    
	/*possitive check*/
	itrRet = ListFindFirst (itrFrom, itrTo, intMatchFunc, (Data)&valToComp);  
	dataToGet = (int*)ListGetData(itrRet);
	if( *dataToGet != valToComp ) {
		resultIndex = FAIL;
	}
	    	
	/*negetive check*/
	valToComp = 777;
	itrRet = ListFindFirst (itrFrom, itrTo, intMatchFunc, (Data)&valToComp);  
	dataToGet = (int*)ListGetData(itrRet);
	if( itrRet != itrTo ) {
		resultIndex = FAIL;
	}
	
	
	
	printf("%s\n", resultArr[resultIndex]);
	
	ListDestroy(list);	    /*cleanup*/
	
}


/*------------------------------------------------------------------------------------*/

static void ForEachCheck( int* _srcArr){
	List* list;
	ListItr itrFrom; 
	ListItr itrTo;
	int saveSrcArr[NUM_OF_ITEMS];
	/*ListItr itrRet;*/
	int toAdd = 10 , sum = 0 , expectedSum = 0 , i ;
	int resultIndex = PASS;
	DoFunc addParmDoFunc = AddParmDoFunc;
	DoFunc sumDoFunc = SumDoFunc;
	
	printf("/------------------------------------------------------/\n");
	printf("check ForEach: ");
	
	
	for(i=0 ; i < NUM_OF_ITEMS ; i++){    /*save orig src arr*/
		saveSrcArr[i] = _srcArr[i];
	}
	
	list = Init(_srcArr);       /*init*/

	/*positive check*/	
	itrFrom = ListBegin(list); 
	itrTo = ListEnd(list); 

	itrFrom = ListNext(itrFrom);  
	itrTo = ListPrev(itrTo); 
	itrTo = ListPrev(itrTo); 

	ListForEach (itrFrom, itrTo, addParmDoFunc , &toAdd);
	ListForEach (itrFrom, itrTo, sumDoFunc , &sum);
	
	
	for(i = NUM_OF_ITEMS - 2 ; i > 1  ; i--){   /*calc expected sum*/
		expectedSum += saveSrcArr[i] + toAdd;
	}
	
	if (sum != expectedSum){
		resultIndex = FAIL;
	}  
	
	
	for(i=0 ; i < NUM_OF_ITEMS ; i++){    /*restore orig src arr*/
		_srcArr[i] = saveSrcArr[i];
	}
		
		
	/*negetive check*/	
		
		
		
		
	
	printf("%s\n", resultArr[resultIndex]);
	
	ListDestroy(list);	    /*cleanup*/
	

} 


/*------------------------------------------------------------------------------------*/

static void PrintIntFunc(Data _data){

	printf("%d", *(int*)_data);
}

/*------------------------------------------------------------------------------------*/


int IntMatchFunc(Data _data1, Data _data2){

	return  ( *(int*)(_data1)  == *(int*)(_data2) );   


}

/*------------------------------------------------------------------------------------*/


int AddParmDoFunc(Data _data1, Parm _parm){

	*(int*)_data1 += *(int*)_parm; 

	return SUCCESS;   


}

/*------------------------------------------------------------------------------------*/


int SumDoFunc(Data _data1, Parm _parm){

	*(int*)_parm +=  *(int*)_data1; 

	return SUCCESS;   


}

/*------------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------------*/




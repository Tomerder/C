#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "heap.h"


#define VEC_INIT_SIZE 100 
#define VEC_BLOCK_SIZE 100

#define NUM_OF_ITEMS 1000 
#define MAX_NUM_VALUE 1000

#define RETURN_ERR if(err!=0) printf("FAIL:%d\n",err); else printf("PASS\n")

#define DEBUG 0

typedef enum {FAIL,PASS} Result;


static int IntWeakOrderFunc(void* _item1,void* _item2);
static Vector* vectorInit(int* _arrOfInts);
static void printInt(Vector* _vec);
static int SumDoFunc (void* _item, void* _params);

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	/*char* resultArr[2]={"FAIL","PASS"};*/
	int err=0 , i=0;
	WeakOrderFunc weakOrderFunc = IntWeakOrderFunc ;	
	PrintFunc printIntsFunc = printInt;	
	DoFunc sumDoFunc = SumDoFunc;
	int arrOfInts[NUM_OF_ITEMS];   /*= {1,3,5,7,9,15,18} ;*/
	int numToInsert = 20;	
	int sum=0;
	void* itemTop;
	Vector* vec;
	Heap* heap;
	
	srand(time(NULL));
	while(i<NUM_OF_ITEMS){
		
		arrOfInts[i] = rand() % MAX_NUM_VALUE; 
		i++;
	}
	
	vec = vectorInit(arrOfInts);                       /*setup*/	
	
	if(DEBUG){		
		printf("vector before sort: ");
		VectorPrint(vec,printIntsFunc);
		printf("/*----------------------------------------------------*/\n");
	}
	
	
	heap = HeapBuild(vec , weakOrderFunc);          /*build*/
	
	err = HeapTest(heap);
	RETURN_ERR;

	if(DEBUG){		
		printf("heap after build:  ");
		HeapPrint (heap, printIntsFunc);
		printf("/*----------------------------------------------------*/\n");
	}


	err = HeapInsert(heap , &numToInsert);             /*insert*/
	RETURN_ERR;


	err = HeapTest(heap);
	RETURN_ERR;
	
	if(DEBUG){		
		printf("heap after insert:  ");
		HeapPrint (heap, printIntsFunc);
		printf("/*----------------------------------------------------*/\n");
	}

	
	itemTop = HeapTop (heap);                         /*item top*/
	
	if(DEBUG){	
		printf("Top = %d\n", *((int*)itemTop) );
		printf("/*----------------------------------------------------*/\n");
	}


	itemTop = HeapExtract (heap);                    /*extract*/
	
	err = HeapTest(heap);
	RETURN_ERR;

	if(DEBUG){	
		printf("heap after extract:  ");
		HeapPrint (heap, printIntsFunc);
		printf("extract = %d\n", *((int*)itemTop) );
		printf("/*----------------------------------------------------*/\n");
	}
							

	HeapForEach ( heap, sumDoFunc, (void*)(&sum) );  /*for each*/  
	
	err = HeapTest(heap);
	RETURN_ERR;

	if(DEBUG){	
		printf("sum = %d\n", sum );
		printf("/*----------------------------------------------------*/\n");
		
	}


	HeapSort (vec, weakOrderFunc);  				/*heap sort*/

	if(DEBUG){		
		printf("heap sort:  ");
		VectorPrint(vec,printIntsFunc);
		printf("/*----------------------------------------------------*/\n");
	}


	VectorDestroy(vec);                              /*cleanup*/
	HeapDestroy(heap); 

    return 0; 	
}


/*------------------------------------------------------------------------------------*/	


static Vector* vectorInit(int* _arrOfInts){
	int i=0,err=0;
	
	Vector* vec = VectorCreate(VEC_INIT_SIZE, VEC_BLOCK_SIZE);
	
	while(i<NUM_OF_ITEMS){
		if((err=VectorInsert(vec,(void*)(&_arrOfInts[i])))!=0){    /*insert*/
			printf("error no. %d\n",err);
		}
		i++;
	}
	return vec;
}


/*------------------------------------------------------------------------------------*/

static int IntWeakOrderFunc(void* _item1,void* _item2){
	int item1 = (*((int*)_item1)) ;
	int item2 = (*((int*)_item2)) ;
	return ( item1 < item2 );
}

/*------------------------------------------------------------------------------------*/

static int SumDoFunc (void* _item, void* _params){

	
	int itemVal = *(int*)_item;
	

	*(int*)_params += itemVal;
	
	return 0;
}

/*------------------------------------------------------------------------------------*/

static void printInt(Vector* _vec){
	int iter=0;
	int* toGet;	

	printf("\n"); 
	while(iter < VectorItemsNum(_vec) ){
		VectorGet (_vec,iter,(void**)&toGet) ;
		printf("%d , ", *toGet);		
		iter++;
	}
	printf("\n\n"); 
}


/*------------------------------------------------------------------------------------*/


	






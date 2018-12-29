#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "sorts.h"
#include "err.h"

#define NUM_OF_ITEMS 1000  /*8*/
#define MAX_NUM_VALUE 10000
#define VEC_INIT_SIZE 100
#define VEC_BLOCK_SIZE 100
#define NUM_OF_VALUES_PER_DIGIT 10 /*decimal - for radix*/

#define SORT_FUNCS_NO END-START

#define DEBUG 0

typedef enum {FAIL,PASS} Result;

int compInt(void* _item1,void* _item2);
void printInt(Vector* _vec);
Result isVecSorted(Vector* _vec);
Vector* vectorInit(int* _arrOfInts);

ADTERR callCountingSort (Vector* _vec, CompFunc _compFunc);
int getKey(void* _item);

ADTERR callRadixSort (Vector* _vec, CompFunc _compFunc);
int getKeyForRadix(void* _item , int _digitNum);
int getLengthForRadix(int max_num);

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int i=0;
	char* resultArr[2]={"FAIL","PASS"};
	int resultIndex=0,err=0;
	CompFunc compIntsFunc = compInt ;
	PrintFunc printIntsFunc = printInt;	
	SortFunc sortFuncArr[SORT_FUNCS_NO]= {BubbleSort , ShakeSort ,InsertionSort, QuickSort, MergeSort ,callCountingSort , callRadixSort};
	Vector* vec;
	int arrOfInts[NUM_OF_ITEMS]; /*= {4,2,8,0,4,1,2,7}; */

	srand(time(NULL));
	while(i<NUM_OF_ITEMS){
		
		arrOfInts[i] = rand() % MAX_NUM_VALUE; 
		i++;
	}

	i=0;	
	while(i<SORT_FUNCS_NO){
		vec = vectorInit(arrOfInts);      /*setup*/	
		if(DEBUG){		
			printf("vector before sort: ");
			VectorPrint(vec,printIntsFunc);
		}
		
		if((err=sortFuncArr[i](vec,compIntsFunc)) !=0){   /*sort function*/
			resultIndex = 0;
		}else{
			resultIndex = 1;
		}
	

		printf("check no. %d : %s(%d)",i+1,resultArr[resultIndex],err);   /* 1: input check -errors */
		
		resultIndex = isVecSorted(vec);
		printf(" , %s\n", resultArr[resultIndex]);               /* 2: functionality check */
		if(DEBUG){		
			printf("vector after sort:  ");
			VectorPrint(vec,printIntsFunc);
			printf("/*----------------------------------------------------*/\n");
		}
		i++;
		VectorDestroy(vec);       /*cleanup*/
	}	

    return 0; 	
}


/*------------------------------------------------------------------------------------*/	


Vector* vectorInit(int* _arrOfInts){
	int i=0,err=0;
	
	Vector* vec=VectorCreate(VEC_INIT_SIZE,VEC_BLOCK_SIZE);
	
	while(i<NUM_OF_ITEMS){
		if((err=VectorInsert(vec,(void*)(&_arrOfInts[i])))!=0){    /*insert*/
			printf("error no. %d\n",err);
		}
		i++;
	}
	return vec;
}


/*------------------------------------------------------------------------------------*/

int compInt(void* _item1,void* _item2){
	int item1 = (*((int*)_item1)) ;
	int item2 = (*((int*)_item2)) ;
	return ( item1 -item2 );
}

/*------------------------------------------------------------------------------------*/

void printInt(Vector* _vec){
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

Result isVecSorted(Vector* _vec){
	void* item1;
	void* item2;
	int i= 0;
	int vecItemsNum = VectorItemsNum(_vec);
	
	if(vecItemsNum<2){
		return FAIL;
	}

	while(i<vecItemsNum-1){
		VectorGet(_vec,i,&item1);
		VectorGet(_vec,i+1,&item2);
		if(compInt(item1,item2)>0){
			return FAIL;
		}
		i++;
	}
	
	return PASS;			

}

/*------------------------------------------------------------------------------------*/


int getKey(void* _item){
	return *((int*)_item);
}


/*------------------------------------------------------------------------------------*/	


ADTERR callCountingSort (Vector* _vec, CompFunc _compFunc){
	GetKey getKeyFunc = getKey;
	return CountingSort( _vec, getKeyFunc , MAX_NUM_VALUE+1);  
}


/*------------------------------------------------------------------------------------*/


ADTERR callRadixSort (Vector* _vec, CompFunc _compFunc){
	GetKeyForRadix getKeyForRadixFunc = getKeyForRadix;
	int length;
	length = getLengthForRadix(MAX_NUM_VALUE);
	return (RadixSort(_vec, getKeyForRadixFunc ,  length , NUM_OF_VALUES_PER_DIGIT)); 
}

/*------------------------------------------------------------------------------------*/

/*return the  _digitNum digit of the num */
int getKeyForRadix(void* _item , int _digitNum){
	int num = getKey(_item);
	int i=0;
	int key=0;
	
	while(i < _digitNum){
		key = num % 10;
		num = num / 10;
		i++;
	}

	return key;
}

/*------------------------------------------------------------------------------------*/

int getLengthForRadix(int _maxNumValue){
	int i=0;
	while(_maxNumValue>0){
		_maxNumValue = _maxNumValue / 10;
		i++;
	}
		
	return i;
}


/*------------------------------------------------------------------------------------*/





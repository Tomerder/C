#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "mem.h"

#define SIZE_OF_DINAMIC_ALOC1 100
#define SIZE_OF_DINAMIC_ALOC2 100000000 /*100M*/
#define SIZE_OF_STACK_ALOC 1000000      /*1M*/

#define PERCENT(ADR)           ( ( (unsigned int)ADR / (double)UINT_MAX ) * 100 )

#define PRINT_ADR_VAR(ADR)      printf("var - %s : %08x => %f \n" , #ADR  , (unsigned int)&ADR , PERCENT(&ADR) );  
#define PRINT_ADR_PTR(ADR)      printf("ptr - %s : %08x => %f \n" , #ADR  , (unsigned int)ADR , PERCENT(ADR) );  


#define PRINT_ADR_VAL(ADR)      printf("val - %s : %d\n" , #ADR  , (unsigned int) (*((unsigned char*)ADR))  );  


#define MALLOC_CHECK(PTR)       if(PTR == NULL)   {  printf("\n\n malloc failed!!!!!!!!!!!!!!!!!!!!\n\n");  }   

/*--------------------------------------------------------------------*/




int globIntNotInit;
int globIntInitB = 777; 
/*int globIntInit = 777;  */

/*static int globStatIntNotInit;*/
static int globStatIntNotInitB;
/*static int globStatIntInit = 888;*/
static int globStatIntInitB;

/*--------------------------------------------------------------------*/


void func1B(int _i){
	int intLocFunc1;
	
	printf("func1\n");	
	PRINT_ADR_VAR(_i);
	PRINT_ADR_VAR(intLocFunc1);	

	func2B(_i , &intLocFunc1 );
	
}

void func2B(int _i, int* _intPtr){
	int intLocFunc2;
	int* intPtrDynamic;

	printf("func2\n");	
	PRINT_ADR_VAR(_i);
	PRINT_ADR_PTR(_intPtr);
	PRINT_ADR_VAR(_intPtr);
	PRINT_ADR_VAR(intLocFunc2);	


	printf("---------------------------------------------------\n");	
	printf("dynamic allocated - HEAP:\n");
	printf("from func2 - before alocation and after\n");
	PRINT_ADR_PTR(intPtrDynamic);	
	
	intPtrDynamic = (int*) malloc (sizeof(int) * SIZE_OF_DINAMIC_ALOC2 );
	MALLOC_CHECK(intPtrDynamic);	

	PRINT_ADR_PTR(intPtrDynamic);
	
	free(intPtrDynamic);	
}

void func3B(int _i, int* _intPtr){
	
}
/*--------------------------------------------------------------------*/








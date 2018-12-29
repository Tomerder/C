#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "mem.h"

#define K 1000 
#define MEGA 1000000 

#define SIZE_OF_DINAMIC_ALOC1 100
#define SIZE_OF_DINAMIC_ALOC2 100 * MEGA /*100M*/
#define SIZE_OF_STACK_ALOC MEGA          /*1M*/

#define PERCENT(ADR)           ( ( (unsigned int)ADR / (double)UINT_MAX ) * 100 )

#define PRINT_ADR_VAR(ADR)      printf("var - %s : %08x => %f \n" , #ADR  , (unsigned int)&ADR , PERCENT(&ADR) );  
#define PRINT_ADR_PTR(ADR)      printf("ptr - %s : %08x => %f \n" , #ADR  , (unsigned int)ADR , PERCENT(ADR) );  


#define PRINT_ADR_VAL(ADR)      printf("val - %s : %d\n" , #ADR  , (unsigned int) (*((unsigned char*)ADR))  );  


#define MALLOC_CHECK(PTR)       if(PTR == NULL)   {  printf("\n\n malloc failed!!!!!!!!!!!!!!!!!!!!\n\n");  }   


void func1(int _i);
void func2(int _i, int* _intPtr);
void func3(int* _i);
static void staticFunc4();

int globIntNotInit;
int globIntInit = 777;

static int globStatIntNotInit;
static int globStatIntInit = 888;

const int globConstIntInit = 555;
const int globConstIntNotInit;

/*--------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	double bytesAlocated = 0;
	int i=0;

	
	char charInit = 'a';
	char charNotInit;	
	char* charPtrConst = "test charPtrConst";
	char* charPtrNotInit;
	char charArrInit[] = "test charPtrArr[]";
	char charArrSizeInit[SIZE_OF_STACK_ALOC] = "test charPtrArr[50]";
	/*static locals*/
	static char* locStatCharPtrConst = "test locStatCharPtrInit";
	static char locStatChar;
	static char locStatCharInit = 't';
	/*dynamic alocated*/
	char* charPtrDynamicBig;
	char* charPtrDynamic1;

	
	int intLocNotInit; 
	int intLocInit = 5; 
	int* intPtrNotInit;
	int* intPtrInitLoc = &intLocInit;
	int intPtrArrInit[] = {5,10} ;
	int intPtrArrSizeInit[SIZE_OF_STACK_ALOC] = {7} ;
	/*consts*/
	const int locConstIntInit = 555;
	const int locConstIntInit2 = 111;
	const int locConstIntNotInit;	
	/*static locals*/
	static int* locStatIntPtr;
	static int locStatInt;
	static int locStatIntInit = 10;
	/*dynamic alocated*/
	int* intPtrDynamicBig;
	int* intPtrDynamic1;
	int* intPtrDynamic2;	
	int* intPtrDynamicSmall1;
	int* intPtrDynamicSmall2;
	
	
	intPtrDynamic1 = (int*) malloc (sizeof(int));
	MALLOC_CHECK(intPtrDynamic1);	
	*intPtrDynamic1 = 1;
	bytesAlocated += sizeof(int);
	
	intPtrDynamic2 = (int*) malloc (sizeof(int));
	MALLOC_CHECK(intPtrDynamic1);   
    *intPtrDynamic2 = 3;
    bytesAlocated += sizeof(int);

    charPtrDynamic1 = (char*) malloc (sizeof(char));
	MALLOC_CHECK(charPtrDynamic1);
	*charPtrDynamic1 = 1;
	bytesAlocated += sizeof(char);
    
  	intPtrDynamicSmall1 = (int*) malloc (sizeof(int) * SIZE_OF_DINAMIC_ALOC1 );
	MALLOC_CHECK(intPtrDynamicSmall1);
  	bytesAlocated += sizeof(int) * SIZE_OF_DINAMIC_ALOC1;

	intPtrDynamicSmall2 = (int*) malloc (sizeof(int) * SIZE_OF_DINAMIC_ALOC1 );
	MALLOC_CHECK(intPtrDynamicSmall2);
	bytesAlocated += sizeof(int) * SIZE_OF_DINAMIC_ALOC1;
	
	charPtrDynamicBig = (char*) malloc (sizeof(char) * SIZE_OF_DINAMIC_ALOC2 );
	MALLOC_CHECK(charPtrDynamicBig);
	bytesAlocated += sizeof(char) * SIZE_OF_DINAMIC_ALOC2;
  
	intPtrDynamicBig = (int*) malloc (sizeof(int) * SIZE_OF_DINAMIC_ALOC2 );
	MALLOC_CHECK(charPtrDynamicBig);
	bytesAlocated += sizeof(int) * SIZE_OF_DINAMIC_ALOC2;


	printf("---------------------------------------------------\n");	
	printf("txt segment - TXT:\n");
	printf("other objects funcs -> main -> funcs -> consts\n");
	printf("allocated at LINK time!!!!!!!!!!!!!! \n");
	
	PRINT_ADR_PTR(func1B);
	PRINT_ADR_PTR(func2B);
	PRINT_ADR_PTR(func3B);
	
	PRINT_ADR_PTR(main);
	PRINT_ADR_PTR(func1);
	PRINT_ADR_PTR(func2);
	PRINT_ADR_PTR(func3);
	PRINT_ADR_PTR(staticFunc4);
	PRINT_ADR_PTR(charPtrConst);
	PRINT_ADR_PTR(locStatCharPtrConst);
	PRINT_ADR_PTR(globConstIntInit);
	PRINT_ADR_PTR(globConstIntNotInit);
	PRINT_ADR_PTR(locConstIntInit);
	PRINT_ADR_PTR(locConstIntNotInit);
	PRINT_ADR_PTR(locConstIntInit2);
	
	printf("---------------------------------------------------\n");		
	printf("globals and statics - DS:\n");
	printf("initialized vars -> uninitialized vars (bss - auto initialized to 0)\n");
	printf("allocated at LINK time!!!!!!!!!!!!!! \n");
		
	PRINT_ADR_VAR(globIntNotInit);
	PRINT_ADR_VAR(globIntInit);
	PRINT_ADR_VAR(globStatIntNotInit);
	PRINT_ADR_VAR(globStatIntInit);	
	
	/*PRINT_ADR_PTR(locStatCharPtrConst);*/
	PRINT_ADR_VAR(locStatCharPtrConst);	
	PRINT_ADR_VAR(locStatChar);		
	PRINT_ADR_VAR(locStatCharInit);

	PRINT_ADR_PTR(locStatIntPtr);
	PRINT_ADR_VAR(locStatIntPtr);
	PRINT_ADR_VAR(locStatInt);
    PRINT_ADR_VAR(locStatIntInit);
		

	printf("---------------------------------------------------\n");	
	printf("dynamic allocated - HEAP:\n");
	printf("addresses goes upwards - or if no space we will get space where ever there are empty chunks of memory available\n");
	printf("allocated at RUN time!!!!!!!!!!!!!! (by calling malloc at standard library)\n");
	
	
	/*PRINT_ADR_VAL(intPtrDynamic1 - sizeof(int));*/
	
	PRINT_ADR_PTR(intPtrDynamic1);
	
	/*for(i=1; i < 17; i++){
		PRINT_ADR_VAL(intPtrDynamic1 + sizeof(char) * i);
	}*/

	PRINT_ADR_PTR(intPtrDynamic2);
	
	/*for(i=1; i < 17; i++){
		PRINT_ADR_VAL(intPtrDynamic2 + sizeof(char) * i);
	}*/
	PRINT_ADR_PTR(charPtrDynamic1);
	
	PRINT_ADR_PTR(intPtrDynamicSmall1);
	PRINT_ADR_PTR(intPtrDynamicSmall2);
	
	PRINT_ADR_PTR(charPtrDynamicBig);
	PRINT_ADR_PTR(intPtrDynamicBig);
  
	/*aloc until malloc fails*/
	/*while(intPtrDynamicSmall1){
		intPtrDynamicSmall1 = (int*) malloc (sizeof(int) * K );
		PRINT_ADR_PTR(intPtrDynamicSmall1);
		bytesAlocated += sizeof(int) * K;
	}
	printf("Total bytes allocated in heap = %f M\n" , bytesAlocated / MEGA  );	

	while(intPtrDynamicSmall1){
		intPtrDynamicSmall1 = (int*) malloc (sizeof(int));
		PRINT_ADR_PTR(intPtrDynamicSmall1);
		bytesAlocated += sizeof(int);
	}
	printf("Total bytes allocated in heap = %f M\n" , bytesAlocated / MEGA  );	
	*/

	printf("---------------------------------------------------\n");	
	printf("automatic vars (locals) - STACK:\n");
	printf("addresses goes downwards\n");
	printf("allocated at LOAD time!!!!!!!!!!!!!! \n");

	PRINT_ADR_VAR(charInit);
	PRINT_ADR_VAR(charNotInit);
	
	PRINT_ADR_PTR(charPtrNotInit);
	PRINT_ADR_PTR(charArrInit);
	PRINT_ADR_PTR(charArrSizeInit);	
		
	PRINT_ADR_VAR(intLocNotInit);
	PRINT_ADR_VAR(intLocInit);
	
	PRINT_ADR_PTR(intPtrNotInit);
	PRINT_ADR_PTR(intPtrInitLoc);	
	PRINT_ADR_VAR(intPtrInitLoc);	
	PRINT_ADR_PTR(intPtrArrInit);
	PRINT_ADR_PTR(intPtrArrSizeInit);		

	func1(intLocInit);

	free(intPtrDynamic1);
	free(intPtrDynamic2);
	free(intPtrDynamicSmall1);
	free(intPtrDynamicSmall2);
	free(charPtrDynamicBig);
	free(intPtrDynamicBig);

	printf("\n");	
		
	return 0;
}

/*--------------------------------------------------------------------*/


void func1(int _i){
	int intLocFunc1;
	static int intStatLocInFunc1;
	const int locConstIntAtFunc1;
	const static int locConstStatIntAtFunc1;

	printf("func1\n");	
	PRINT_ADR_VAR(_i);
	PRINT_ADR_VAR(intLocFunc1);	
	PRINT_ADR_VAR(locConstIntAtFunc1);

	printf("AT DATA SEGMENT **** : ");
	PRINT_ADR_VAR(intStatLocInFunc1);
	printf("AT DATA SEGMENT **** : ");
	PRINT_ADR_VAR(locConstStatIntAtFunc1);

	func2(_i , &intLocFunc1 );
	
}

void func2(int _i, int* _intPtr){
	int intLocFunc2;

	printf("func2\n");	
	PRINT_ADR_VAR(_i);
	PRINT_ADR_PTR(_intPtr);
	PRINT_ADR_VAR(_intPtr);
	PRINT_ADR_VAR(intLocFunc2);	

	func3(&intLocFunc2);
	
}

void func3(int* _adrLastFuncLocal){
	int i;
	printf("func3\n");	
	
	printf("adrress of last local from func2:\n");	
	PRINT_ADR_PTR(_adrLastFuncLocal);
	printf("adrress of local parameter of func3:\n");	
	PRINT_ADR_VAR(_adrLastFuncLocal);
	printf("number of bytes between them : %d\n" , (unsigned int)_adrLastFuncLocal - (unsigned int)&_adrLastFuncLocal );	

	PRINT_ADR_VAR(i);


	
	staticFunc4();
}


static void staticFunc4(){
	int* intPtrDynamic;

	printf("---------------------------------------------------\n");	
	printf("dynamic allocated - HEAP:\n");
	printf("from func2 - before alocation and after\n");
	PRINT_ADR_PTR(intPtrDynamic);	
	
	intPtrDynamic = (int*) malloc (sizeof(int) * SIZE_OF_DINAMIC_ALOC2 );
	MALLOC_CHECK(intPtrDynamic);	

	PRINT_ADR_PTR(intPtrDynamic);
	
	free(intPtrDynamic);
}
/*--------------------------------------------------------------------*/








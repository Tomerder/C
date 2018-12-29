/*************************************************************/
/** 	Project:	My Malloc.              				**/
/** 	Author:		Tomer Dery								**/
/** 	Created:	27/1/2112								**/
/** 	Updated:	27/1/2112								**/
/*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

/*------------------------------------------------------------------------------------*/

#include "MyMalloc.h"

/*------------------------------------------------------------------------------------*/

enum {FALSE, TRUE};
enum {FAIL, PASS};

char* resultArr[2] = {"FAIL","PASS"};

#define SUCCESS 1

#define DEBUG 0

/*------------------------------------------------------------------------------------*/

void Test();

void InteractiveTest();

/*------------------------------------------------------------------------------------*/

void* InitMem(size_t _memSize);

void UnitTest1(); /*check NULL returned when ask for bigger chunk then available*/ 

void UnitTest2(); /*check allocation successed until it should fail*/ 

void UnitTest3(); /*check free makes space for allocation*/ 

void UnitTest3B(); /*check free makes space for allocation between 2 taken blocks*/ 

void UnitTest4(); /*check double free*/

void UnitTest5(); /*check join on free*/ 

void UnitTest6(); /*check join on malloc pass*/ 

void UnitTest7(); /*check alignment*/ 

void UnitTest8(); /*check - compere addresses*/

void UnitTest9(); /*check - hole on first block*/

void UnitTest20(); /*check combination of malloc & free*/ 

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	printf("/*------------------------------------------------*/\n");
	/*Test();*/
	/*InteractiveTest();*/
	
	
	UnitTest1();
	UnitTest2();
	UnitTest3();
	UnitTest3B();
	UnitTest4();
	UnitTest5();
	UnitTest6();
	UnitTest7();
	UnitTest8();
	UnitTest9();
	
	
	UnitTest20();
	
		
    return 0; 	
}

/*------------------------------------------------------------------------------------*/

void InteractiveTest()
{
	void* mem = NULL;
	void* block1 = NULL;
	void* block2 = NULL;
	void* block3 = NULL;
	int memSize;
	int blockToFree;
	int cont = 1;
	int ch;
	
	printf("enter size of memory to init: ");
	scanf("%d", &memSize);
	
	mem = InitMem(memSize);

	PrintMem2(mem);
	
	while(cont) {
		printf("1 - malloc\n");
		printf("2 - free\n");
		printf("Please, choose option: ");
	
		scanf("%d",&ch);
		
		switch(ch) {
			case 1 : 	
						printf("enter size to allocate: ");
						scanf("%d", &memSize);
						block3 = block2;
						block2 = block1;
						block1 = MyMalloc(mem, memSize);
						PrintMem2(mem);
						break;		
			case 2 :  
						printf("enter block to free 1=last 2=before 3= before before: ");
						scanf("%d", &blockToFree);
						switch(blockToFree) {
							case 1 : MyFree(block1); block1=block2; block2=block3; PrintMem2(mem);   break;	
							case 2 : MyFree(block2); block2=block3;  PrintMem2(mem);  break;	
							case 3 : MyFree(block3); PrintMem2(mem);    break;	
							default  : break;
						}
						break;	
			
			default: cont = 0;
		}
	}

}

/*------------------------------------------------------------------------------------*/
void* InitMem(size_t _memSize)
{
	void* mem = malloc(_memSize + sizeof(int)*2 );
	void* myMem =  MyMallocInit(mem , _memSize);

	return myMem;
}

/*------------------------------------------------------------------------------------*/

void UnitTest1()
{
	size_t memSize = 50;
	void* block = NULL;
	int resultIndex = PASS;
	
	/*set up*/
	void* mem = InitMem(memSize);

	block = MyMalloc(mem, memSize);
	
	if(block != NULL){
		resultIndex = FAIL;
	}

	printf("/*check NULL returned when ask for bigger chunk then available*/\n");
	printf("UnitTest1: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/
	free(mem);
}

/*------------------------------------------------------------------------------------*/

void UnitTest2()
{
	size_t memSize = 28;
	size_t chunkSize = 4;
	void* block = NULL;
	int resultIndex = PASS;
	int i;
	
	/*set up*/
	void* mem = InitMem(memSize);

	/*positive test*/
	for(i=0; memSize >= (chunkSize)*i + sizeof(int)*(i+3); i++){
		block = MyMalloc(mem, chunkSize);
		/*PrintMem2(mem);*/
		if(block == NULL){
			resultIndex = FAIL;
			break;
		}
	}

	/*negetive test*/
	block = MyMalloc(mem, chunkSize);	
	if(block != NULL){
		resultIndex = FAIL;
	}

	printf("/*check allocation successed until it should fail*/\n"); 
	printf("UnitTest2: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/
	free(mem);
}

/*------------------------------------------------------------------------------------*/

void UnitTest3()
{
	size_t memSize = 28;
	size_t chunkSize = 4;
	void* block = NULL;
	int resultIndex = PASS;
	int i;
	
	/*set up*/
	void* mem = InitMem(memSize);

	/*positive test*/
	for(i=0; memSize >= (chunkSize)*i + sizeof(int)*(i+3); i++){
		block = MyMalloc(mem, chunkSize);
		/*PrintMem2(mem);*/
		if(block == NULL){
			resultIndex = FAIL;
			break;
		}
	}

	MyFree(block);
	
	block = MyMalloc(mem, chunkSize);
	if(block == NULL){
		resultIndex = FAIL;
	}
	
	/*negetive test*/
	block = MyMalloc(mem, chunkSize);	
	if(block != NULL){
		resultIndex = FAIL;
	}

	printf("/*check free makes space for allocation*/ \n"); 
	printf("UnitTest3: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/
	free(mem);
}

/*------------------------------------------------------------------------------------*/

void UnitTest3B()
{
	size_t memSize = 28;
	size_t chunkSize = 4;
	void* block = NULL;
	void* block2 = NULL;
	int resultIndex = PASS;
	
	/*set up*/
	void* mem = InitMem(memSize);

	block = MyMalloc(mem, chunkSize);	
	block2 = MyMalloc(mem, chunkSize);	
	block = MyMalloc(mem, chunkSize);	
	
	MyFree(block2);
	
	block = MyMalloc(mem, chunkSize);
	if(block == NULL || block != block2){
		resultIndex = FAIL;
	}


	printf("/*check free makes space for allocation between 2 taken blocks*/ \n"); 
	printf("UnitTest3B: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/
	free(mem);
}


/*------------------------------------------------------------------------------------*/

void UnitTest4()
{
	size_t memSize = 28;
	size_t chunkSize = 4;
	void* block = NULL;
	int resultIndex = PASS;
	
	/*set up*/
	void* mem = InitMem(memSize);

	
	block = MyMalloc(mem, chunkSize);	

	MyFree(block);
	MyFree(block);

	printf("/*check double free*/\n"); 
	printf("UnitTest4: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/
	free(mem);
}

/*------------------------------------------------------------------------------------*/

void UnitTest5()
{
	size_t memSize = 28;
	size_t chunkSize = 4;
	void* block = NULL;
	void* block2 = NULL;
	int resultIndex = PASS;
	int i;
	
	/*set up*/
	void* mem = InitMem(memSize);

	/*positive test*/
	for(i=0; memSize > (chunkSize)*i + sizeof(int)*(i+3); i++){
		block = MyMalloc(mem, chunkSize);
	}

	block2 = MyMalloc(mem, chunkSize);;

	/*join on second free*/
	MyFree(block2);
	MyFree(block);
	
	/*PrintMem2(mem);*/
	
	block = MyMalloc(mem, chunkSize*3);
	
	if(block == NULL){
		resultIndex = FAIL;
	}
	
	/*PrintMem2(mem);*/

	printf("/*check join on free*/\n"); 
	printf("UnitTest5: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/
	free(mem);
}


/*------------------------------------------------------------------------------------*/

void UnitTest6()
{
	size_t memSize = 28;
	size_t chunkSize = 4;
	void* block = NULL;
	void* block2 = NULL;
	int resultIndex = PASS;
	int i;
	
	/*set up*/
	void* mem = InitMem(memSize);

	/*positive test*/
	for(i=0; memSize > (chunkSize)*i + sizeof(int)*(i+3); i++){
		block = MyMalloc(mem, chunkSize);
	}

	block2 = MyMalloc(mem, chunkSize);;

	MyFree(block);
	MyFree(block2);
	
	/*PrintMem2(mem);*/
	
	/*join free sapce on malloc pass*/
	block = MyMalloc(mem, chunkSize*3);
	
	if(block == NULL){
		resultIndex = FAIL;
	}
	
	/*PrintMem2(mem);*/

	printf("/*check join on malloc pass*/\n"); 
	printf("UnitTest6: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/
	free(mem);
}


/*------------------------------------------------------------------------------------*/

void UnitTest7()
{
	size_t memSize = 36;
	size_t chunkSize = 3;
	void* block = NULL;
	int resultIndex = PASS;
	int i;
	
	/*set up*/
	void* mem = InitMem(memSize);

	/*positive test*/
	for(i=0; memSize >= (chunkSize + 1)*i + sizeof(int)*(i+3); i++){
		block = MyMalloc(mem, chunkSize);
		/*PrintMem2(mem);*/
		if(block == NULL){
			resultIndex = FAIL;
			break;
		}
	}

	/*PrintMem2(mem);*/

	/*negetive test*/
	block = MyMalloc(mem, chunkSize);	
	if(block != NULL){
		resultIndex = FAIL;
	}

	printf("/*check alignment*/\n"); 
	printf("UnitTest7: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/
	free(mem);
}

/*------------------------------------------------------------------------------------*/

void UnitTest8()
{
	size_t memSize = 36;
	size_t chunkSize = 4;
	void* block1 = NULL;
	void* block2 = NULL;
	int resultIndex = PASS;
	
	/*set up*/
	void* mem = InitMem(memSize);
	
	block1 = MyMalloc(mem, chunkSize);
	
	block2 = MyMalloc(mem, chunkSize*2);	
	
	block1 = MyMalloc(mem, chunkSize);
	
	MyFree(block2);
	
	block1 = MyMalloc(mem, chunkSize*2);
	
	if(block1 != block2){
		resultIndex = FAIL;
	}

	printf("/*check - compere addresses*/\n"); 
	printf("UnitTest8: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/
	free(mem);
}

/*------------------------------------------------------------------------------------*/

void UnitTest9()
{
	size_t memSize = 52;
	size_t chunkSize = 4;
	void* block1 = NULL;
	void* block2 = NULL;
	int resultIndex = PASS;
	
	/*set up*/
	void* mem = InitMem(memSize);
	
	block1 = MyMalloc(mem, chunkSize*2);
	block2 = MyMalloc(mem, chunkSize);	
	
	/*PrintMem2(mem);*/
	
	/*make hole*/
	MyFree(block1);
	
	/*PrintMem2(mem);*/
	
	/*skip hole*/
	block1 = MyMalloc(mem, chunkSize*3);
	if(block1 == NULL){
		resultIndex = FAIL;
	}
	
	/*PrintMem2(mem);*/
	
	/*fill hole with smaller block*/
	block1 = MyMalloc(mem, chunkSize);
	
	/*PrintMem2(mem);*/
	
	/*skip hole after first block*/
	block2 = MyMalloc(mem, chunkSize*2);
	if(block2 == NULL){
		resultIndex = FAIL;
	}
	/*PrintMem2(mem);*/
	
	

	printf("/*check - skip holes*/\n"); 
	printf("UnitTest9: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/
	free(mem);
}

/*------------------------------------------------------------------------------------*/

void UnitTest20()
{
	int resultIndex = PASS;
	void* block1 = NULL;
	void* block2 = NULL;
	void* block3 = NULL;
	size_t sizeToAlloc = 50;

	void* myMem = InitMem(sizeToAlloc);
	
	/*---------------------------------------------*/
		
	block1 = MyMalloc(myMem, 5);	
	
	if(block1 == NULL){
		resultIndex = FAIL;
	}
	
	/*---------------------------------------------*/
	
	block2 = MyMalloc(myMem, 2);
		
	if(block2 == NULL){
		resultIndex = FAIL;
	}
	
	/*---------------------------------------------*/
	
	block3 = MyMalloc(myMem, 4);
	
	if(block3 == NULL){
		resultIndex = FAIL;
	}
	
	/*---------------------------------------------*/
	
	MyFree(block2);
	
	/*---------------------------------------------*/
	
	MyFree(block3);
	
	/*---------------------------------------------*/
	
	block2 = MyMalloc(myMem, 10);
	
	if(block2 == NULL){
		resultIndex = FAIL;
	}
	
	/*---------------------------------------------*/
	
	block3 = MyMalloc(myMem, 20);
	
	if(block3 != NULL){
		resultIndex = FAIL;
	}

	/*---------------------------------------------*/
		
	MyFree(block1);

	/*---------------------------------------------*/
	
	block1 = MyMalloc(myMem, 1);
	if(block1 == NULL){
		resultIndex = FAIL;
	}

	/*---------------------------------------------*/
		
	MyFree(block2);

	/*---------------------------------------------*/
	
	block2 = MyMalloc(myMem, 19);
	if(block2 == NULL){
		resultIndex = FAIL;
	}
	
	/*---------------------------------------------*/
		
	MyFree(block1);
	
	/*---------------------------------------------*/
	
	block3 = MyMalloc(myMem, 7);
	if(block3 == NULL){
		resultIndex = FAIL;
	}

	/*---------------------------------------------*/
	
	MyFree(block2);
	
	/*---------------------------------------------*/
	
	block1 = MyMalloc(myMem, 8);
	if(block1 == NULL){
		resultIndex = FAIL;
	}
	
	
	/*---------------------------------------------*/

	/*check combination of malloc & free*/ 
	printf("/*check combination of malloc & free*/ \n"); 
	printf("UnitTest20: %s\n", resultArr[resultIndex]);
	printf("/*------------------------------------------------*/\n");	

	/*clean up*/	
	free(myMem);

}


/*------------------------------------------------------------------------------------*/
void Test()
{
	void* block1 = NULL;
	void* block2 = NULL;
	void* block3 = NULL;
	size_t sizeToAlloc = 50;

	void* mem = malloc(sizeToAlloc + sizeof(int)*2 );
	void* myMem =  MyMallocInit(mem , sizeToAlloc);
	
	printf("MyMallocInit(mem , %d);\n" ,sizeToAlloc );
	PrintMem2(myMem);
	PrintMem(myMem);
	
	/*---------------------------------------------*/
	
	block1 = MyMalloc(myMem, 5);	
	
	printf("block1 = MyMalloc(myMem, 5);\n");
	PrintMem2(myMem);
	PrintMem(myMem);
	
	/*---------------------------------------------*/
	
	block2 = MyMalloc(myMem, 2);
	
	printf("block2 = MyMalloc(myMem, 2);\n");	
	PrintMem2(myMem);
	PrintMem(myMem);
	
	/*---------------------------------------------*/
	
	block3 = MyMalloc(myMem, 4);
	
	printf("block3 = MyMalloc(myMem, 4);\n");	
	PrintMem2(myMem);
	PrintMem(myMem);
	
	/*---------------------------------------------*/
	
	printf("MyFree(block2);\n");	
	MyFree(block2);
	PrintMem2(myMem);
	
	/*---------------------------------------------*/
	
	printf("MyFree(block3);\n");	
	MyFree(block3);
	PrintMem2(myMem);
	PrintMem(myMem);
	
	/*---------------------------------------------*/
	
	block2 = MyMalloc(myMem, 10);
	
	printf("block2 = MyMalloc(myMem, 10);\n");	
	PrintMem2(myMem);
	PrintMem(myMem);
	
	/*---------------------------------------------*/
	
	block3 = MyMalloc(myMem, 20);
	
	printf("block3 = MyMalloc(myMem, 20);\n");	
	PrintMem2(myMem);

	/*---------------------------------------------*/
	
	printf("MyFree(block1);\n");	
	MyFree(block1);
	PrintMem2(myMem);
	PrintMem(myMem);

	/*---------------------------------------------*/
	
	block1 = MyMalloc(myMem, 1);
	
	printf("block1 = MyMalloc(myMem, 1);\n");	
	PrintMem2(myMem);
	PrintMem(myMem);

	/*---------------------------------------------*/
	
	printf("MyFree(block2);\n");	
	MyFree(block2);
	PrintMem2(myMem);
	PrintMem(myMem);

	/*---------------------------------------------*/
	
	block2 = MyMalloc(myMem, 19);
	
	printf("block2 = MyMalloc(myMem, 19);\n");	
	PrintMem2(myMem);
	PrintMem(myMem);
	
	/*---------------------------------------------*/
	
	printf("MyFree(block1);\n");	
	MyFree(block1);
	PrintMem2(myMem);
	PrintMem(myMem);
	
	/*---------------------------------------------*/
	
	block3 = MyMalloc(myMem, 7);
	
	printf("block3 = MyMalloc(myMem, 7);\n");	
	PrintMem2(myMem);
	PrintMem(myMem);

	/*---------------------------------------------*/
	
	printf("MyFree(block2);\n");	
	MyFree(block2);
	PrintMem2(myMem);
	PrintMem(myMem);
	
	/*---------------------------------------------*/
	
	block1 = MyMalloc(myMem, 8);
	
	printf("block1 = MyMalloc(myMem, 8);\n");	
	PrintMem2(myMem);
	PrintMem(myMem);
	
	/*---------------------------------------------*/

	free(myMem);

}


/*------------------------------------------------------------------------------------*/




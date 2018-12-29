#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------------------*/

void func1(int n);
void func2();

/*--------------------------------------------------------------*/

int stopFlag = 0;

/*--------------------------------------------------------------*/

int main()
{
	printf("/*----------------------------------------------------*/\n");
	printf("main - begin\n");
	
	unsigned i=5;
	unsigned j=7;
	

	printf("&i : %x\n" , (unsigned)&i );
	printf("&j : %x\n" , (unsigned)&j );
	
	if(!stopFlag){
		stopFlag = 1;		
		func1(9);
	}
	
	printf("i = %d\n", i);
	printf("j = %d\n", j);
	
	printf("main - end\n");
	printf("/*----------------------------------------------------*/\n");
	
	return 0;
}

/*--------------------------------------------------------------*/

void func1(int n)
{
	printf("/*----------------------------------------------------*/\n");
	printf("func1 - begin\n");

	unsigned func2Adr = (unsigned)func2; 
	void* iAdr = &func2Adr + 15;    /*i*/  
	void* returnAdr = &func2Adr + 8;  
	
	printf("mainAdr: %x\n" , (unsigned)main );
	printf("func2Adr: %x\n" , func2Adr );
	printf("iAdr: %x\n" , (unsigned)iAdr );
	printf("returnAdr : %x\n" , (unsigned)returnAdr );
	printf("returnAdr value => main line after call: %x\n" , *(unsigned*)returnAdr );
	printf("/*----------------------------*/\n");

	unsigned replaceI = 8;

	int i;
	for(i=16; i>= 1; i--){
		printf("func2Adr + %d: %x : %x\n",i , (unsigned)(&func2Adr + i) , *(unsigned*)(&func2Adr + i) );
	}
	printf("/*----------------------------*/\n");

	memcpy(iAdr , &replaceI , 4);
	
	memcpy(returnAdr , &func2Adr , 4);
	
	
	for(i=16; i>= 1; i--){
		printf("func2Adr + %d: %x : %x\n",i , (unsigned)(&func2Adr + i) , *(unsigned*)(&func2Adr + i) );
	}

	
	printf("func1 - end\n");
}

/*--------------------------------------------------------------*/

void func2()
{
	printf("/*----------------------------------------------------*/\n");
	printf("func2 - begin\n");

	unsigned mainAdr = (unsigned)main; 
	void* returnAdr = &mainAdr + 7;  

	void* iAdr = &mainAdr + 5;   

	printf("iAdr: %x\n" , (unsigned)iAdr );

	unsigned replaceI = 777777;

	memcpy(iAdr , &replaceI , 4);

	memcpy(returnAdr , &mainAdr , 4);

	printf("func2 - end\n");
}

/*--------------------------------------------------------------*/

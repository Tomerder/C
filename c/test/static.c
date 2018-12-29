#include<stdio.h>
#include <math.h>


int globalInt;
static int globalStaticInt;

int main(int argc,char** argv)
{	
	static int parStaticInt;
	/*int localInt = 0;*/

	printf("g:%d  gs:%d sl:%d \n" , globalInt , globalStaticInt , parStaticInt); 

	incFunc(parStaticInt);

	printf("g:%d  gs:%d sl:%d \n" , globalInt , globalStaticInt , parStaticInt); 
	
	incFunc(++parStaticInt);

	incFunc(parStaticInt++);

	return 0;

}



void incFunc(int parStaticInt){
	static int localStaticInt;

	globalInt++;
	globalStaticInt++;
	localStaticInt++;

	printf("in func => g:%d  gs:%d ls:%d parStaticInt:%d\n" , globalInt , globalStaticInt , localStaticInt , parStaticInt); 


}

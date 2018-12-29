/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-00-00    
    Last modified date:		2013-00-00
    Description: 
***************************************************************************************/

#include <stdio.h>

#define LIMIT 5	/* limit=5 -> seg. fault; limit=4 -> seg fault *after* return to main*/

void foo();
void bar();

int main()
{
	int a=42;
	
	printf("\nstart\n");
	
	printf("a=%d\n", a);
	
	foo();

	printf("a=%d\n", a);
	
	printf("end\n\n");
	return 0;
}



void foo()
{
	int i=0, *ptr;
	
	ptr=&i;
	
	printf("foo\n");
	printf("*ptr=%d, ptr=%u\n", *ptr, (unsigned int)ptr);
	
	ptr += LIMIT;
		
/*	printf("*ptr=%d, ptr=%u\n", *ptr, (unsigned int)ptr);*/
	
	*ptr += 212;
}


void bar()
{
	int i=5, *ptr;
	
	ptr=&i;
	
	printf("bar\n");
	printf("*ptr=%d, ptr=%u\n", *ptr, (unsigned int)ptr);
	
}




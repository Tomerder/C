/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-28    
    Last modified date:		2013-07-28
    Description: pointers
***************************************************************************************/
#include <stdio.h>


#define SWAP(X,Y,TYPE)		{void *temp = (void*)(*(X));\
							*(X) = *(Y);\
							*(Y) = (TYPE*)temp; }




void subtract(int *_a, int *_b)
{
	*_a = *_a - *_b;
}


int main()
{
	int x = 5, y = 3;
	int *ptr1, *ptr2;
	
	ptr1=&x;
	ptr2=&y;
	
	printf("x = %d, y = %d\n", *ptr1, *ptr2);
	
	SWAP(&ptr1, &ptr2, int);

	printf("x = %d, y = %d\n", *ptr1, *ptr2);
	
	return 0;
}







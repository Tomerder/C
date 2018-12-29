/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-21    
    Last modified date:		2013-07-21
    Description: swapping generic pointers
***************************************************************************************/

#include <stdio.h>

/* swap any 2 pointers of the same type - any type */
void GenericSwapPtrs(void **ptr1, void **ptr2)
{
	void *temp;
	
	temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
}



int main()
{
	int one = 1, two = 2;
	int *ptr1, *ptr2;
	char a='A', b='B';
	char *ptrA, *ptrB;
	
	ptr1 = &one;
	ptr2 = &two;
	ptrA = &a;
	ptrB = &b;
	
	printf("\nbefore:\tone: %d, two: %d, A: %c, B: %c\n", *ptr1, *ptr2, *ptrA, *ptrB);
	
	GenericSwapPtrs((void**)ptr1, (void**)ptr2);
	GenericSwapPtrs((void**)ptrA, (void**)ptrB);

	printf("after:\tone: %d, two: %d, A: %c, B: %c\n\n", *ptr1, *ptr2, *ptrA, *ptrB);

	return 0;
}

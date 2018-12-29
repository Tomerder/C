#include <stdlib.h>

int* maxFunc()
{
	int* ptr = (int*)malloc(sizeof(int)*10);
	int* ptr2 = (int*)malloc(sizeof(int)*10);
	
	free(ptr);
	*ptr2 = 0;
	
	return ptr;
}




int main()
{
	int *a;
	
	a = maxFunc();
	
	a[9] = 5;
	
	return 0;
}


#include <stdlib.h>


void func(char* ptr)
{
	static n = 7;	
	ptr[1] = n;
}


int main()
{
	char* ptr;
	int i;
	
	ptr = malloc(sizeof(char*));
	
	for(i=0; i<10; ++i)
	{
		++ptr;
		func(ptr);
	}
	
	free(ptr);
}




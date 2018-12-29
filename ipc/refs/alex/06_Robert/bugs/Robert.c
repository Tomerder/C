#include <stdio.h>
#include <string.h>

void fRobert(int* ptr)
{
	memset(ptr, -1, 21);
}



int main()
{
	int rval;
	int a;
	int b;

	a=1;
	b=2;
	
	fRobert(&rval);
	
	return 0;
}






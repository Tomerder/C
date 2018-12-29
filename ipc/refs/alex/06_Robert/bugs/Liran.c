#include <stdio.h>
#include <stdlib.h>


void bad_func()
{
	static int x=1;
	
	if( (x--) == 0 )
	{
		return;
	}
	
	bad_func();	
}

int main()
{
	bad_func();
	bad_func();
	
	return 0;
}

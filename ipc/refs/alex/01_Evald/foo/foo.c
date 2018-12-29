#include "main.h"

static int g_j;

void foo()
{
	int i;
	extern int g_i;
	static int j;
	
	printf("foo:\n");
	
	printf("&i=%x\n", (unsigned int)&i);
	printf("&g_i=%x\n", (unsigned int)&g_i);
	
	printf("&j=%x\n", (unsigned int)&j);
	printf("&g_j=%x\n", (unsigned int)&g_j);
}




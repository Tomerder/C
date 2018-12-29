#include <stdio.h>
#include "Count.h"

int g_index = 0;


int count()
{
	static int g_index = 0;
	++g_index;
	printf("index %d\n", g_index);
	return g_index;
}

int add(int a, int b)
{
	return a+b;
}




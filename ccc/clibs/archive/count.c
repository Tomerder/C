#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "count.h"

/*------------------------------------------------------------------------------------*/

enum{FALSE , TRUE};

int global;

/*------------------------------------------------------------------------------------*/

int Count()
{
	return  global++;
}

/*------------------------------------------------------------------------------------*/

int Mul(int x, int y)
{
	return x*y;
}

/*------------------------------------------------------------------------------------*/


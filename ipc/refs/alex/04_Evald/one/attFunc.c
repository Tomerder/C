/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-07    
    Last modified date:		2013-08-07
    Description: 
***************************************************************************************/

#include <stdio.h>

#include "attFunc.h"


void __attribute__ ((constructor)) LoadFunc(void)
{
	printf("--loading--\n");
}


void __attribute__ ((destructor)) UnloadFunc(void)
{
	printf("--unloading--\n");
}


void foo(int i)
{
	printf("foo %d\n", i);
}


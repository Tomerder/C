/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-00-00    
    Last modified date:		2013-00-00
    Description: 
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>




void foo(int i)
{
	if(i==LIMIT)
	{
		return;
	}
	
	printf("foo: i=%d, &i=%u\n", i, (unsigned int)&i);
	foo(++i);
}



void bar(int j)
{
	if(j==LIMIT)
	{
		return;
	}
	
	printf("bar: j=%d, &j=%u\n", j, (unsigned int)&j);
	bar(++j);
}


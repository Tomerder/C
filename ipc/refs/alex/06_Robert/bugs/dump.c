/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-15    
    Last modified date:		2013-08-15
    Description: causing core dumps
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>

int bad_func(int* _stam)
{
	char* ptr = calloc(1, sizeof(char));
	
	*ptr = 'a';
	*(_stam++) = 10;
	
	return *_stam+*ptr;
}


int main()
{
	int b = 5;
	
	int rval = bad_func(&b);
	
	return rval;
}



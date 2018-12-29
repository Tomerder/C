/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-00-00    
    Last modified date:		2013-00-00
    Description: 
***************************************************************************************/

#include <stdio.h>


int main()
{
	long l=1;
	char *cp=(char*)&l;

	printf("%d\n", *cp);

	return 0;
}

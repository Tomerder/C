/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-00-00    
    Last modified date:		2013-00-00
    Description: 
***************************************************************************************/

#include <stdio.h>

#pragma pack(push, 2)
#pragma pack(push, 1)

typedef struct X
{
	char a;
	long l;
	char b;
	int i;
} X;

#pragma pack(pop)

typedef struct Y
{
	char a;
	long l;
	char b;
	int i;
} Y;

#pragma pack(pop)

typedef struct Z
{
	char a;
	long l;
	char b;
	int i;
} Z;



int main()
{
	printf("X: %d\n", sizeof(X));
	printf("Y: %d\n", sizeof(Y));
	printf("Z: %d\n", sizeof(Z));
	return 0;
}

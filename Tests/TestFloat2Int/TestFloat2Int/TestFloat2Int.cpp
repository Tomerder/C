// TestFloat2Int.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <conio.h>
#include "stdafx.h"


#define BITS_IN_BYTE 32

void Print4BytesBinaric(void* _address);

int _tmain(int argc, _TCHAR* argv[])
{
	float f; //1.7f;
	
	while(true)
	{
		printf("enter float : ");
		scanf("%f", &f);

		//---------------
		//int allBitsOn = -1;
		//f = *(float*)&allBitsOn;
		//---------------

		int i1 = (int)f;

		int i2 = *(int*)(&f);

		printf("float f = %f           : ", f);
		Print4BytesBinaric(&f);

		printf("int i1 = (int)f  = %d  : ", i1);
		Print4BytesBinaric(&i1);

		printf("int i2 = *(int*)(&f) = %d  : ", i2 );
		Print4BytesBinaric(&i2);

		printf("\n");
	}

	getchar();

	return 0;
}


void Print4BytesBinaric(void* _address)
{
	int num = *(int*)_address;

	char binaricRep[BITS_IN_BYTE+1]; 

	for(int i=0; i < BITS_IN_BYTE; i++)
	{
		binaricRep[BITS_IN_BYTE-1-i] = (num & 1) + '0';
		num = num >> 1;
	}

	binaricRep[BITS_IN_BYTE] = '\0';

	printf("%s\n", binaricRep);
}
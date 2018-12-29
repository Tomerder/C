// testC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>

/*-------------------------------------------------------------------------*/

#define BITS_IN_CHAR (8)

#define LUT_CHAR_SIZE (256U)
#define LUT_INT_SIZE (LUT_CHAR_SIZE * LUT_CHAR_SIZE * LUT_CHAR_SIZE * 256U -1)

#define KILO (1024U)
#define MEGA (KILO * KILO)
#define GIGA (MEGA * KILO)

/*-------------------------------------------------------------------------*/

void testMemStructToArr();

void SwitchBitsMain();

void TestMemoryTypesMaxAlocSize();

void casting();

void testNotDeclareH();

void TestEnum();

/*-------------------------------------------------------------------------*/

int _tmain(int argc, _TCHAR* argv[])
{
	//testMemStructToArr();

	//SwitchBitsMain();

	//TestMemoryTypesMaxAlocSize();

	//casting();

	//testNotDeclareH();

	TestEnum();
}

/*------------------------------TestEnum--------------------------------------------------*/

typedef enum testEnum{A, B, C} testEnum;

void TestEnum()
{
	testEnum enumVar1 = (testEnum)8;
	testEnum enumVar2 = (testEnum)-1;
}

/*------------------------------testNotDeclareH-------------------------------------------*/

#include "testNotDeclaredFile.h"
extern int testNotDeclaredFunc(int a, int b, int c);
//extern int globalInH;
extern int globalInC;

void testNotDeclareH()
{
	int result = testNotDeclaredFunc(1, 2, 3);

	globalInC = 5;
	globalInH = 7;

	/*
	int a=7;
	int b;
	
	memcpy(&b, &a, sizeof(int));
	*/
}

/*------------------------------TestMemoryTypesMaxAlocSize-------------------------------------------*/

class GigaBlock
{
	char m_giga[GIGA];
};

class MegaBlock
{
	char m_mega[MEGA];
};

class KiloBlock
{
	char m_kilo[KILO];
};

//-------------------DATA-----------------------------
//char globalDataSection[GIGA] = {1}; // LINK ERROR : error LNK1102: out of memory	
//char globalDataSection[GIGA/2] = {1}; //OK

void TestMemoryTypesMaxAlocSize()
{
	//-------------------STACK-----------------------------
	//char arrLutChar[LUT_CHAR_SIZE]; //OK
	//char arrLutInt[LUT_INT_SIZE];   //COMPILE ERROR : total size of array must not exceed 0x7fffffff bytes
	//char arrLutInt[0x7fffffff]; //COMPILE ERROR : automatic allocation exceeds 2G
	//char arrLutInt[GIGA * 2 - 1]; //COMPILE ERROR : automatic allocation exceeds 2G
	//char arrLutInt[GIGA * 2 - 500]; //RUN TIME ERROR - Stack overflow
	//char arrLutInt[MEGA]; //RUN TIME ERROR - Stack overflow
	//  char arrLutInt[MEGA - 20  * KILO]; //***************OK
	//GigaBlock giga; //RUN TIME ERROR - Stack overflow
	//MegaBlock mega; //RUN TIME ERROR - Stack overflow
	//KiloBlock kilo[1024]; //RUN TIME ERROR - Stack overflow
	//KiloBlock kilo[1000]; //***************OK

	//-------------------HEAP-----------------------------
	//char* pointer = new char[2 * GIGA - 1];  //RUN TIME ERROR - exception: std::bad_alloc at memory location 	
	//char* pointer = new char[1.57 * GIGA];  // ***************OK

	int counter = 0;	
	while(true)
	{
		counter++;		
		try
		{
			//char* pointer = new char;  //33,029,418 : RUN TIME ERROR - exeption because virtual table is overfloaded after ~ 33M allocations
			//int* pointer = new int;    //33,031,286 : RUN TIME ERROR - exeption because virtual table is overfloaded after ~ 33M allocations
			//KiloBlock* kilo = new KiloBlock;  //1,942,955 : RUN TIME ERROR - exeption because out of memory for dynamic allocation ~ 2G
			MegaBlock* mega = new MegaBlock;    //1894 : RUN TIME ERROR - exeption because out of memory for dynamic allocation ~ 2G
		}
		catch(...)
		{
			printf("exeption alloc : %d\n", counter);				
			break;
		}
	}	
	//----------------------------------------------------
	

	getch();
}

/*-------------------------------------------------------------------------*/

typedef struct test
{
	int a1;
	int a2;
	int a3;
}StructTest;

void testMemStructToArr()
{
	StructTest structTest;
	structTest.a1 = 1;
	structTest.a2 = 2;
	structTest.a3 = 3;

	int** arr;

	//int arr[3];

	arr = (int**)&structTest;

	for(int i=0; i < 3; i++)
	{
		printf("%d\n", arr[i]);
	}
}

/*-------------------------------------------------------------------------*/

char SwitchBits(char _toSwitch, int _bitNum1, int _bitNum2);
void PrepereSwitchHushMap(char* _hashmap, int _bitNum1, int _bitNum2);

void SwitchBitsMain()
{
	char hashMap[256];

	PrepereSwitchHushMap(hashMap, 7, 0);

	while(1)
	{
		int i;
		printf("\nEnter Char to Fix : ");
		scanf("%d", &i);

		//fix char
		char fixedChar = hashMap[i];

		printf("Fixed char : %i", fixedChar);
	}
}

void PrepereSwitchHushMap(char* _hashmap, int _bitNum1, int _bitNum2)
{
	for(int i=0; i<256; i++)
	{
		_hashmap[i] = SwitchBits( (char)i, _bitNum1, _bitNum2);
	}
}

char SwitchBits(char _toSwitch, int _bitNum1, int _bitNum2)
{
	char mask1 = 1;
	char mask2 = 1;
	char maskSwitch = 0;

	//input check
	if (_bitNum1 == _bitNum2 || _bitNum1 >= BITS_IN_CHAR || _bitNum2 >= BITS_IN_CHAR)
	{
		return _toSwitch;
	}

	//prepere masks
	mask1 = mask1 << _bitNum1;
	mask2 = mask2 << _bitNum2;
	maskSwitch = maskSwitch | mask1 | mask2 ;

	//check if bits are equal -> if so, no change should be done
	char result1 = _toSwitch & mask1;
	char result2 = _toSwitch & mask2;

	if ( (result1 == 0 && result2 == 0) || (result1 != 0 && result2 != 0) )
	{
		return _toSwitch;
	}

	//if bits are not equal -> swich both bits
	char switchedToRet =  _toSwitch ^ maskSwitch;

	return switchedToRet;
}

/*-------------------------------------------------------------------------*/

void casting()
{
	int iNeg = -2;
	int iPosFE = 0xfe;
	int iPos = 2;
	
	char cNeg = (char)iNeg;
	char cPosFE = (char)iPosFE;
	char cPos = (char)iPos;
	
	int iBackNeg = (int)cNeg; 
	int iBackPosFE = (int)cPosFE; //cast from char to int => the msb bit of lsb Byte will be spreded to the msb : 0xf7 => 0xfffffff7
	int iBackPos = (int)cPos; 

	//--------------------
	//will cut 3 MSB Bytes
	int iOver = 256;
	char cOver = (char)iOver;

	int iOver2 = 257;
	char cOver2 = (char)iOver2;
}

/*-------------------------------------------------------------------------*/
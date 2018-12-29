#include <stdio.h>
#include <stdlib.h>

#include "bitwise.h"

/*------------------------------------------------------------------------------------*/

enum{FALSE , TRUE};

#define BITS_OF_INT sizeof(int) * 8 

#define HEXA_DIGITS_IN_INT 8
#define BITS_1111 15 
#define BITS_OF_HEXA 4 
/*------------------------------------------------------------------------------------*/

/*building lookUpTable of powers of 2*/
static void  buildLUTPowOf2(int* _lookUpTable);

/*------------------------------------------------------------------------------------*/

int AddBitWise(int _num1 , int _num2)
{
	int carry=0 , carryShift=0 ,sum=0;;
	
	carry = _num1 & _num2;  /*where 1+1*/
	sum = _num1 ^ _num2;    /*where 1+0*/

	while(carry != 0){
		carryShift = carry << 1;
		carry = sum & carryShift;
		sum = sum ^ carryShift;
	}

	return sum;
}

int AddBitWise2(int _num1 , int _num2)
{
	int carry=0, sum=0, i=0, digit1=0, digit2=0, sumDigit=0, mask=0, one=1;
   
	for(i=0; i<BITS_OF_INT; i++)
    {
		mask = one << i ;
        digit1 = (_num1 & mask)>>i; 
        digit2 = (_num2 & mask)>>i; 

        sumDigit = digit1 ^ digit2 ^ carry; 
        carry = (carry==0) ? (digit1 & digit2): (digit1 | digit2);
        sum = sum | (sumDigit << i);
    }

    return sum;
}
/*------------------------------------------------------------------------------------*/

int shiftLCirc(int _num, int _shiftNum)
{
	int leftPart = (_num >> (BITS_OF_INT - _shiftNum));
	 
	_num <<= _shiftNum;
	
	return (_num | leftPart); 
}

int shiftRCirc(int _num, int _shiftNum)
{
	int rightPart = (_num << (BITS_OF_INT - _shiftNum));
	 
	_num >>= _shiftNum;
	
	return (rightPart | _num); 
}

/*------------------------------------------------------------------------------------*/

int flip2Bits(int _num, int _bitNum1, int _bitNum2)
{
	int mask = 1;
	mask <<= _bitNum1;
	
	_num = _num ^ mask;
	
	mask = 1;
	mask <<= _bitNum2;
	
	return _num ^ mask;
}

int flip2BitsLUT(int _num, int _bitNum1, int _bitNum2)
{
	static int lookUpTable[BITS_OF_INT];
	static int isLUTinitialized = FALSE;
	
	if(!isLUTinitialized){
		buildLUTPowOf2(lookUpTable);
		isLUTinitialized = TRUE;
	}
	
	return (_num ^ lookUpTable[_bitNum1]) ^ lookUpTable[_bitNum2]  ;
}

/*------------------------------------------------------------------------------------*/

int flipBit(int _num, int _bitNum)
{
	int mask = 1;
	mask = mask << _bitNum;
	
	return _num ^ mask;
}

int flipBitLUT(int _num, int _bitNum)
{
	static int lookUpTable[BITS_OF_INT];
	static int isLUTinitialized = FALSE;
	
	if(!isLUTinitialized){
		buildLUTPowOf2(lookUpTable);
		isLUTinitialized = TRUE;
	}
	
	return (_num ^ lookUpTable[_bitNum]);
}

/*------------------------------------------------------------------------------------*/

int flipNumFromTo(int _num, int _from , int _to)
{
	int i, mask = 0;
	
	for(i=_from; i <= _to; i++){
		mask <<= 1; 
		mask = mask | 1;
	} 
	
	mask <<= _from;

	return (_num ^ mask);

}

/*------------------------------------------------------------------------------------*/
int BitsOnCount(int _num)
{
	int i=0 , sumOfBits=0;
		
	for(i=0; i < BITS_OF_INT; i++){
		sumOfBits += _num & 1;
		_num = _num >> 1; 
	} 
		
	return sumOfBits;
}

int BitsOnCountLUT(int _num)
{
	static int lookUpTable[BITS_OF_INT];
	static int isLUTinitialized = FALSE;
	int i=0, count=0;
	
	if(!isLUTinitialized){
		buildLUTPowOf2(lookUpTable);
		isLUTinitialized = TRUE;
	}
	
	for(i=0; i < BITS_OF_INT; i++){
		count += ((_num & lookUpTable[i]) != 0);
	}
		
	return count;
}

/*------------------------------------------------------------------------------------*/

void PrintBinar(int _num)
{
	int i;
	char binar[32]={'0'};
	
	for(i=0; i < BITS_OF_INT; i++){
		binar[32-i-1] = (_num & 1) + '0';
		_num = _num >> 1; 
	} 
	printf("%s\n", binar);
}


/*------------------------------------------------------------------------------------*/

int IsPowerOf2(int _num)
{
	int i=0 , sumOfBits=0;
		
	for(i=0; i < BITS_OF_INT; i++){
		sumOfBits += _num & 1;
		_num = _num >> 1; 
	} 
		
	return (sumOfBits==1);
}


static void buildLUTPowOf2(int* _lookUpTable)
{
	int i=0;
	int powNum = 1;
	
	for(i=0; i < BITS_OF_INT; i++){
		_lookUpTable[i] = powNum;
		powNum <<= 1;
	}
}


int IsPowerOf2LUT(int _num)
{
	static int lookUpTable[BITS_OF_INT];
	static int isLUTinitialized = FALSE;
	int i;
	
	if(!isLUTinitialized){
		buildLUTPowOf2(lookUpTable);
		isLUTinitialized = TRUE;
	}
	
	for(i=0; i < BITS_OF_INT; i++){
		if(_num == lookUpTable[i]){
			return TRUE;
		} 
	}

	return FALSE;
}

/*------------------------------------------------------------------------------------*/

int Abs1(int _num)
{
	int sign = (_num >> (BITS_OF_INT - 1) );
	return (_num + sign) ^ sign;
}

int Abs2(int _num)
{
	int sign = (_num >> (BITS_OF_INT - 1) );
	return  (sign ^ _num) - sign;
}

int Abs3(int _num)
{
	return  _num * ((_num>0) - (_num<0));
}

/*------------------------------------------------------------------------------------*/

int IsNumPos(int _num)
{
	int sign = _num >> (BITS_OF_INT - 1) ;
	return !(sign);
}

/*------------------------------------------------------------------------------------*/

void Swap1(int* num1, int* num2)
{
	*num1 = *num1 + *num2;
	*num2 = *num1 - *num2;
	*num1 = *num1 - *num2;
}

void Swap2(int* num1, int* num2)
{
	*num1 = *num1 * *num2;
	*num2 = *num1 / *num2;
	*num1 = *num1 / *num2; 
}

void Swap3(int* num1, int* num2)
{
	*num1 = *num1 ^ *num2;
	*num2 = *num1 ^ *num2;
	*num1 = *num1 ^ *num2; 
}

/*------------------------------------------------------------------------------------*/

void num2Hex(int num, char* hexaNum)
{
	/*will be build only on first function call*/
	static char lookUpHexa[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; 
	int hexaCurdigit=0 , i=0;

	while(num != 0   &&   i<HEXA_DIGITS_IN_INT){
		hexaCurdigit = num & BITS_1111;  /*get current hexa digit*/
		
		/*hexaNum[i] = lookUpHexa[hexaCurPart];  =>   if we want this function to work for types other then int*/  
		hexaNum[HEXA_DIGITS_IN_INT - i - 1] = lookUpHexa[hexaCurdigit];
		
		num = num >> BITS_OF_HEXA;	  /* >>4 bits => to next hexa digit*/	
		++i;
	}

	/*reverseStr(hexaNum);   =>   if we want this function to work for types other then int*/
}

/*------------------------------------------------------------------------------------*/


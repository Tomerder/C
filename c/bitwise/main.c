#include <stdio.h>
#include <stdlib.h>

#include "bitwise.h"
/*------------------------------------------------------------------------------------*/

enum{FALSE , TRUE};

enum{N0,N1,N2,N3,N4,N5,N6,N7,N8,N9,N10,N11,N12,N13,N14,N15,N16};
#define N64 64
#define SHIFT_R_CIRC_7 -2147483645
/*------------------------------------------------------------------------------------*/

void SwapChk();
void IsNumPosChk();
void AbsChk();
void IsPowerOf2Chk();
void AddChk();
void PrintBinarChk();
void BitsCountChk();
void FlipBitFrmToChk();
void FlipBitChk();
void Flip2BitsChk();
void ShiftCircChk();

/*------------------------------------------------------------------------------------*/

int main()
{
	
	SwapChk();
	
	IsNumPosChk();
	
	AbsChk();
	
	IsPowerOf2Chk();
	
	AddChk();
	
	PrintBinarChk();
	
	BitsCountChk();
	
	FlipBitFrmToChk();
	
	FlipBitChk();
		
	Flip2BitsChk();

	ShiftCircChk();	
	
	
	return 0;

}

/*------------------------------------------------------------------------------------*/

void SwapChk()
{
	int num1=7 , num2=3;

	printf("/*----------------------------------------------------*/\n");
	printf("num1=%d , num2=%d\n", num1 , num2);
	Swap1(&num1, &num2);
	printf("num1=%d , num2=%d\n", num1 , num2);
	Swap2(&num1, &num2);
	printf("num1=%d , num2=%d\n", num1 , num2);
	Swap3(&num1, &num2);
	printf("num1=%d , num2=%d\n", num1 , num2);
	printf("/*----------------------------------------------------*/\n");
}



void IsNumPosChk()
{
	int num1=3 , numNeg = -7;  

	printf("num1=%d , IsNumPos=%d\n", num1 ,  IsNumPos(num1));
	printf("numNeg=%d , IsNumPos=%d\n", numNeg ,  IsNumPos(numNeg));
	printf("/*----------------------------------------------------*/\n");
}

void AbsChk()
{
	int num1=3 , numNeg = -7;  

	printf("numNeg=%d , numAbs=%d\n", numNeg ,  Abs1(numNeg));
	printf("num1=%d , numAbs=%d\n", num1 ,  Abs1(num1));

	printf("numNeg=%d , numAbs=%d\n", numNeg ,  Abs2(numNeg));
	printf("num1=%d , numAbs=%d\n", num1 ,  Abs2(num1));

	printf("numNeg=%d , numAbs=%d\n", numNeg ,  Abs3(numNeg));
	printf("num1=%d , numAbs=%d\n", num1 ,  Abs3(num1));

	printf("/*----------------------------------------------------*/\n");
}

void IsPowerOf2Chk()
{
	printf("is %d power of 2 => %d\n", N16 ,  IsPowerOf2(N16));
	printf("is %d power of 2 => %d\n", N64 ,  IsPowerOf2(N64));
	printf("is %d power of 2 => %d\n", N7 ,  IsPowerOf2(N7));
	
	printf("LUT - is %d power of 2 => %d\n", N16 ,  IsPowerOf2LUT(N16));
	printf("LUT - is %d power of 2 => %d\n", N64 ,  IsPowerOf2LUT(N64));
	printf("LUT - is %d power of 2 => %d\n", N7 ,  IsPowerOf2LUT(N7));
	printf("/*----------------------------------------------------*/\n");
}

void PrintBinarChk()
{	
	printf("binar %d => ", N7);
	PrintBinar(N7);

	printf("binar %d => ", SHIFT_R_CIRC_7 );
	PrintBinar(SHIFT_R_CIRC_7);
	printf("/*----------------------------------------------------*/\n");
}

void BitsCountChk()
{
	printf("bits on count of number %d => %d\n", N15 ,  BitsOnCount(N15));
	printf("bits on count of number %d => %d\n", N16 ,  BitsOnCount(N16));
	
	printf("LUT - bits on count of number %d => %d\n", N15 ,  BitsOnCountLUT(N15));
	printf("LUT - bits on count of number %d => %d\n", N16 ,  BitsOnCountLUT(N16));
	printf("/*----------------------------------------------------*/\n");
}

void FlipBitFrmToChk()
{
	printf("flip from bit %d to %d of num %d => %d\n", N1, N2, N15 ,  flipNumFromTo(N15,N1,N2));
	printf("/*----------------------------------------------------*/\n");
}

void FlipBitChk()
{
	printf("flip bit %d of num %d => %d\n", N2, N15 ,  flipBit(N15,N2));
	
	printf("LUT - flip bit %d of num %d => %d\n", N2, N15 ,  flipBitLUT(N15,N2));
	printf("/*----------------------------------------------------*/\n");
}

void Flip2BitsChk()
{
	printf("flip bits %d&%d of num %d => %d\n", N1 , N2 ,N15 ,  flip2Bits(N15,N1,N2));
	
	printf("LUT - flip bits %d&%d of num %d => %d\n", N1 , N2 , N15 ,  flip2BitsLUT(N15,N1,N2));
	printf("/*----------------------------------------------------*/\n");
}

void ShiftCircChk()
{
	printf("shiftL circular %d << %d  =  %d\n", N7 , N1 , shiftLCirc(N7,N1) );
	
	printf("shiftR circular %d >> %d  =  %d\n", N10 , N1 , shiftRCirc(N10,N1) );	
	printf("shiftR circular %d >> %d  =  %d\n", N7 , N1 , shiftRCirc(N7,N1) );
	printf("/*----------------------------------------------------*/\n");
}

void AddChk()
{
	printf("%d + %d  =  %d\n", N7 , N15 , AddBitWise(N7,N15) );
	
	printf("%d + %d  =  %d\n", N7 , N15 , AddBitWise2(N7,N15) );
	printf("/*----------------------------------------------------*/\n");
}
/*------------------------------------------------------------------------------------*/


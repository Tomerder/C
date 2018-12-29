#include<stdio.h>
#include <math.h>

#ifndef STRINGS2_H_
#define STRINGS2_H_

void enterBetweenWord(char* sentence);

int binarToDec(char* binarNum);
char* decToBin(int num,char* resultStr);  

char*  bigInts(char* num1,char* num2, char* result);  

char* squeeze(char* s1,char* s2);  

int any(char* str1,char* str2);


static char* reverseStr(char* str);    
static void leftShift(char* str , int i);

#endif

/*------------------------------------------------------------------------------------------------------------------*/


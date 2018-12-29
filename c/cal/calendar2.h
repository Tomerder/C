#include<stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef STRINGS2_H_
#define STRINGS2_H_


#define EXPEND_VALUE 4
#define N 4



typedef struct meeting{
	int start;
	int end;
	char subject[50];
}meeting;


typedef struct calendar{
	meeting** adr;
	int curIndex;
	int size;	
}calendar;




int Insert(calendar* cal , meeting* meet);
int Find(calendar cal , meeting* meet);
int Remove(calendar* cal , int startHour);

meeting** create(int n);          
void printArr(calendar cal); 
void freeAll(calendar* cal);   

static int findIndexToInsert(calendar* cal , int start , int end);
static void shiftR(calendar* cal , int indexToInsert);



#endif

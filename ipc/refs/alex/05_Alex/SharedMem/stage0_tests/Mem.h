/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-29    
    Last modified date:		2013-08-29
    Description: 	memory structure & shared memory objects
****************************************************************************/

#ifndef __MEM_H__
#define __MEM_H__

typedef void (*funcInt)(int*);


void HeapInt(int* _int);
void HeapChar(char* _c);
void StackInt();
void StackRecursion(int i);

void dummy1(int* _a);
void dummy2(int* _a);
void dummy3(int* _a);
void dummy4(int* _a);
static void dummy5(int* _a);
static void dummy6(int* _a);

void dummy11(int* _a);
void dummy12(int* _a);
void dummy13(int* _a);
void dummy14(int* _a);

void libFunc1(int* _a);
void libFunc2(int* _a);
void libFunc3(int* _a);
void libFunc4(int* _a);


#endif

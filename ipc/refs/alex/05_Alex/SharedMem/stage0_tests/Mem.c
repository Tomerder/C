/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-29    
    Last modified date:		2013-08-29
    Description: 	memory structure & shared memory objects
****************************************************************************/


#include <stdlib.h>
#include <stdio.h>

#include "Mem.h"

/*######################################################################*/
/*				definitions & foreward declarations						*/
/*######################################################################*/


static void dummy5(int* _a);
static void dummy6(int* _a);
static void dummy15(int* _a);
static void dummy16(int* _a);


/* globals */

funcInt g_func1 = dummy1, g_func2 = dummy2, g_func3 = dummy3, g_func5 = dummy5;
funcInt	lib_func1 = libFunc1;

int 	g_int1, g_int2, g_int3, g_int4, g_int5;
char	g_char1, g_char2, g_char3, g_char4, g_char5;

const int	g_c_int1 = 1, g_c_int2 = 2, g_c_int3 = 3, g_c_int4 = 4, g_c_int5 = 5;
const char	g_c_char1 = 'a', g_c_char2 = 'b', g_c_char3 = 'c', g_c_char4 = 'd', g_c_char5 = 'e';

/* static globals */
static int 	gs_int1, gs_int2, gs_int3, gs_int4, gs_int5;
static char	gs_char1, gs_char2, gs_char3, gs_char4, gs_char5;

static const int	gs_c_int1 = 1, gs_c_int2 = 2, gs_c_int3 = 3, gs_c_int4 = 4, gs_c_int5 = 5;
static const char	gs_c_char1 = 'a', gs_c_char2 = 'b', gs_c_char3 = 'c', gs_c_char4 = 'd', gs_c_char5 = 'e';




/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main()
{
	/* locals */
	int		l_int1, l_int2, l_int3, l_int4, l_int5;
	char	l_char1, l_char2, l_char3, l_char4, l_char5;
	
	const int	l_c_int1 = 1, l_c_int2 = 2, l_c_int3 = 3, l_c_int4 = 4, l_c_int5 = 5;
	const char	l_c_char1 = 'a', l_c_char2 = 'b', l_c_char3 = 'c', l_c_char4 = 'd', l_c_char5 = 'e';
	
	int		*h_int1, *h_int2, *h_int3;
	char	*h_char1, *h_char2, *h_char3;
	
	funcInt l_func1 = dummy11, l_func2 = dummy12, l_func3 = dummy13, l_func5 = dummy15;
	funcInt	lib_func3 = libFunc3;
	
	g_int1 = 1; g_int2 = 2; g_int3 = 3; g_int4 = 4; g_int5 = 5; 
	g_char1 = 'a'; g_char2 = 'b'; g_char3 = 'c'; g_char4 = 'd'; g_char5 = 'e'; 
	
	l_int1 = 1; l_int2 = 2; l_int3 = 3; l_int4 = 4; l_int5 = 5; 
	l_char1 = 'a'; l_char2 = 'b'; l_char3 = 'c'; l_char4 = 'd'; l_char5 = 'e'; 
	
	HeapInt(h_int1);
	HeapInt(h_int2);
	HeapInt(h_int3);
	
	HeapChar(h_char1);
	HeapChar(h_char2);
	HeapChar(h_char3);
	
	
	
	printf("\n--Locals - main--\n");
	
	printf("\n-int\n");
	printf("%p\n", &l_int1);
	printf("%p\n", &l_int2);
	printf("%p\n", &l_int3);
	printf("%p\n", &l_int4);
	printf("%p\n", &l_int5);
	printf("\n-char\n");
	printf("%p\n", &l_char1);
	printf("%p\n", &l_char2);
	printf("%p\n", &l_char3);
	printf("%p\n", &l_char4);
	printf("%p\n", &l_char5);
	
	printf("\n-const int\n");
	printf("%p\n", &l_c_int1);
	printf("%p\n", &l_c_int2);
	printf("%p\n", &l_c_int3);
	printf("%p\n", &l_c_int4);
	printf("%p\n", &l_c_int5);
	printf("\n-const char\n");
	printf("%p\n", &l_c_char1);
	printf("%p\n", &l_c_char2);
	printf("%p\n", &l_c_char3);
	printf("%p\n", &l_c_char4);
	printf("%p\n", &l_c_char5);
	
	printf("\n--globals--\n");
	
	printf("\n-int\n");
	printf("%p\n", &g_int1);
	printf("%p\n", &g_int2);
	printf("%p\n", &g_int3);
	printf("%p\n", &g_int4);
	printf("%p\n", &g_int5);
	printf("\n-char\n");
	printf("%p\n", &g_char1);
	printf("%p\n", &g_char2);
	printf("%p\n", &g_char3);
	printf("%p\n", &g_char4);
	printf("%p\n", &g_char5);
	
	printf("\n-const int\n");
	printf("%p\n", &g_c_int1);
	printf("%p\n", &g_c_int2);
	printf("%p\n", &g_c_int3);
	printf("%p\n", &g_c_int4);
	printf("%p\n", &g_c_int5);
	printf("\n-const char\n");
	printf("%p\n", &g_c_char1);
	printf("%p\n", &g_c_char2);
	printf("%p\n", &g_c_char3);
	printf("%p\n", &g_c_char4);
	printf("%p\n", &g_c_char5);
	
	printf("\n-static int\n");
	printf("%p\n", &gs_int1);
	printf("%p\n", &gs_int2);
	printf("%p\n", &gs_int3);
	printf("%p\n", &gs_int4);
	printf("%p\n", &gs_int5);
	printf("\n-static char\n");
	printf("%p\n", &gs_char1);
	printf("%p\n", &gs_char2);
	printf("%p\n", &gs_char3);
	printf("%p\n", &gs_char4);
	printf("%p\n", &gs_char5);
	
	printf("\n-static const int\n");
	printf("%p\n", &gs_c_int1);
	printf("%p\n", &gs_c_int2);
	printf("%p\n", &gs_c_int3);
	printf("%p\n", &gs_c_int4);
	printf("%p\n", &gs_c_int5);
	printf("\n-static const char\n");
	printf("%p\n", &gs_c_char1);
	printf("%p\n", &gs_c_char2);
	printf("%p\n", &gs_c_char3);
	printf("%p\n", &gs_c_char4);
	printf("%p\n", &gs_c_char5);
	
	printf("\n--Heap--\n");
	
	printf("\n-int\n");
	printf("%p\n", &h_int1);
	printf("%p\n", &h_int2);
	printf("%p\n", &h_int3);
	printf("\n-char\n");
	printf("%p\n", &h_char1);
	printf("%p\n", &h_char2);
	printf("%p\n", &h_char3);
	
	
	printf("\n--Local - function--\n");
	StackInt();
	
	printf("\n--Local - recursive fucntion--\n");
	printf("\nin:\n");
	StackRecursion(0);
	
	
	printf("\n--Functions\n");
	

	printf("%p\n", dummy1);
	printf("%p\n", dummy2);
	printf("%p\n", dummy3);
	printf("%p\n", dummy4);
	printf("%p\n", dummy5);
	printf("%p\n", dummy6);

	printf("%p\n", &dummy11);
	printf("%p\n", &dummy12);
	printf("%p\n", &dummy13);
	printf("%p\n", &dummy14);
	printf("%p\n", &dummy15);
	printf("%p\n", &dummy16);
	
	printf("\n-shared\n");
	printf("%p\n", libFunc1);
	printf("%p\n", libFunc2);
	printf("%p\n", libFunc3);
	printf("%p\n", libFunc4);

	
	printf("\n");
	return 0;
}



/*######################################################################*/
/*								Functions								*/
/*######################################################################*/


void HeapInt(int* _int)
{
	_int = (int*)malloc(sizeof(int));
}
/*######################################################################*/

void HeapChar(char* _c)
{
	_c = (char*)malloc(sizeof(char));
}
/*######################################################################*/

void StackInt()
{
	int		l_int1, l_int2, l_int3;
	char	l_char1, l_char2, l_char3;
	
	static int	ls_int1, ls_int2, ls_int3;
	static char	ls_char1, ls_char2, ls_char3;
	
	l_int1 = 1; l_int2 = 2; l_int3 = 3;
	l_char1 = 'a'; l_char2 = 'b'; l_char3 = 'c';
	ls_int1 = 1; ls_int2 = 2; ls_int3 = 3;
	ls_char1 = 'a'; ls_char2 = 'b'; ls_char3 = 'c';
	
	
	printf("\n-int\n");
	printf("%p\n", &l_int1);
	printf("%p\n", &l_int2);
	printf("%p\n", &l_int3);
	printf("\n-char\n");
	printf("%p\n", &l_char1);
	printf("%p\n", &l_char2);
	printf("%p\n", &l_char3);
	
	printf("\n-static int\n");
	printf("%p\n", &ls_int1);
	printf("%p\n", &ls_int2);
	printf("%p\n", &ls_int3);
	printf("\n-static char\n");
	printf("%p\n", &ls_char1);
	printf("%p\n", &ls_char2);
	printf("%p\n", &ls_char3);
}
/*######################################################################*/

void StackRecursion(int i)
{
	int a, b;
	
	if(i==5)
	{
		printf("\nout:\n");
		return;
	}
	
	a=i;
	printf("%p\n", &a);
	
	StackRecursion(++i);
	
	b=i;
	printf("%p\n", &b);
}
/*######################################################################*/



void dummy1(int* _a)
{
	++*_a;
}

void dummy2(int* _a)
{
	++*_a;
}

void dummy3(int* _a)
{
	int b = 5;
	*_a += b;
}

void dummy4(int* _a)
{
	int b = 5;
	*_a += b;
}

static void dummy5(int* _a)
{
	++*_a;
}

static void dummy6(int* _a)
{
	int b = 5;
	*_a += b;
}





void dummy11(int* _a)
{
	++*_a;
}

void dummy12(int* _a)
{
	++*_a;
}

void dummy13(int* _a)
{
	int b = 5;
	*_a += b;
}

void dummy14(int* _a)
{
	int b = 5;
	*_a += b;
}

static void dummy15(int* _a)
{
	++*_a;
}

static void dummy16(int* _a)
{
	int b = 5;
	*_a += b;
}

/*######################################################################*/




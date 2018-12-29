#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define PRT_S(STRUCT)  printf("size of %s : %d\n" , #STRUCT , sizeof(STRUCT))


/* #pragma pack(push, 4)  */ 

#pragma pack(push, 1)
typedef struct A{ 
	int i;
	char c;
}A;

typedef struct A2{
	int i;
	char c1;
	char c2;
	char c3;
}A2;

typedef struct A3{
	int i;
	char c1;
	char c2;
	char c3;
	char c4;
}A3;

typedef struct A4{
	int i;
	char c1;
	char c2;
	char c3;
	char c4;
	char c5;
}A4;

typedef struct B{
	char c;
	int i;	
}B;

typedef struct B2{
	char c1;
	char c2;
	int i;	
}B2;

typedef struct C{
	int i;
	double d;
	char c;
	char c2;
	int i2;
	char c3;
	float f;	
}C;

typedef struct D{
	char c;
	char c2;
	int i;
	char c3;	
	char c4;	
}D;
#pragma pack(pop)


typedef struct AnotAligned{ 
	int i;
	char c;
}AnotAligned;


/*--------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	A a1,a2;
	AnotAligned an1,an2;
	
	a1.i = 5; a2.i = 5;
	a1.c = 65; a2.c = 65;
	
	an1.i = 5; an2.i = 5;
	an1.c = 65; an2.c = 65;
	
	printf("\n");
	printf("A aligned : ");
	if(  ! memcmp(&a1, &a2 , sizeof(A) )    ){
		printf("equal\n");
	}else{
		printf("not equal\n");
	}

	printf("A not aligned : ");
	if(  ! memcmp(&an1, &an2 , sizeof(AnotAligned) )    ){
		printf("equal\n");
	}else{
		printf("not equal\n");
	}

	printf("\n");

	PRT_S(A);
	PRT_S(AnotAligned);
	PRT_S(A2);
	PRT_S(A3);
	PRT_S(A4);
	PRT_S(B);
	PRT_S(B2);
	PRT_S(C);
	PRT_S(D);
	
	return 0;
}

/*--------------------------------------------------------------------*/










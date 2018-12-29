#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define SWAP(X,Y,TYPE)   {TYPE tmp = X;   X = Y;    Y = tmp;} 

#define ASSERT(X)    {     if(!X)    {   ;abort();   }            }     

/*  &int* => will swap ptrs
	int* \ &int => will swap values (size of void*)
	double* \ & double => will swap only msb 4 bytes of the double  (size of void*)   */   
void SwapPtr(void** _x, void** _y);


void SwapMem(void* _x, void* _y, int _size);

/*--------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int i1=5 , i2=7 ; 
	double d1=5000000005.0 , d2=700000007.0 ; 
	int *ip1, *ip2;
	double *dp1, *dp2;
	
	ip1 = &i1;
	ip2 = &i2;
	
	dp1 = &d1;
	dp2 = &d2;
	
	/*int swap*/
	printf("\nstart type int\n\n");

	printf("i1: %d , i2: %d\n",i1,i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);		
	printf("SwapPtr((void**)&ip1, (void**)&ip2)\n"); 
	SwapPtr((void**)&ip1, (void**)&ip2); 	
	printf("i1: %d , i2: %d\n", i1, i2);	
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);	
	
	printf("\n");
	
	printf("i1: %d , i2: %d\n",i1,i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);		
	printf("SwapPtr((void**)ip1, (void**)ip2)\n"); 
	SwapPtr((void**)ip1, (void**)ip2); 	
	printf("i1: %d , i2: %d\n", i1, i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);	
	
	printf("\n");
	
	
	/*double swap*/
	printf("\nstart type double\n\n");
	
	printf("d1: %f , d2: %f\n", d1, d2);	
	printf("*dp1: %f , *dp2: %f\n", *dp1, *dp2);	
	printf("SwapPtr((void**)&dp1, (void**)&dp2)\n"); 
	SwapPtr((void**)&dp1, (void**)&dp2); 	
	printf("d1: %f , d2: %f\n", d1, d2);	
	printf("*dp1: %f , *dp2: %f\n", *dp1, *dp2);	
	
	printf("\n");
	
	printf("d1: %f , d2: %f\n", d1, d2);
	printf("*dp1: %f , *dp2: %f\n", *dp1, *dp2);	
	printf("SwapPtr((void**)dp1, (void**)dp2)\n"); 
	SwapPtr((void**)dp1, (void**)dp2); 	
	printf("d1: %f , d2: %f\n", d1, d2);
	printf("*dp1: %f , *dp2: %f\n", *dp1, *dp2);	
	
	printf("\n");
	
	
	/*double swap MACRO*/
	printf("\nstart type double MACRO\n\n");
	
	d1=5000000005.0 ;
	d2=700000007.0;
	
	printf("d1: %f , d2: %f\n",d1,d2);
	printf("*dp1: %f , *dp2: %f\n", *dp1, *dp2);	
	printf("SWAP(dp1, dp2, double*)\n"); 
	SWAP(dp1, dp2, double*); 	
	printf("d1: %f , d2: %f\n", d1, d2);	
	printf("*dp1: %f , *dp2: %f\n", *dp1, *dp2);	
	
	printf("\n");
	
	printf("d1: %f , d2: %f\n", d1, d2);
	printf("*dp1: %f , *dp2: %f\n", *dp1, *dp2);	
	printf("SWAP(d1, d2, double)\n"); 
	SWAP(d1, d2, double); 
	printf("d1: %f , d2: %f\n", d1, d2);
	printf("*dp1: %f , *dp2: %f\n", *dp1, *dp2);	
	
	printf("\n");
	
	
	/*int swap MACRO*/
	printf("\nstart type int MACRO\n\n");
	
	printf("i1: %d , i2: %d\n", i1, i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);		
	printf("SWAP(ip1, ip2, int*)\n"); 
	SWAP(ip1, ip2, int*); 
	printf("i1: %d , i2: %d\n", i1, i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);	
	
	printf("\n");
	
	printf("i1: %d , i2: %d\n", i1, i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);	
	printf("SWAP(ip1, ip2, int)\n");  
	SWAP(ip1, ip2, int); 
	printf("i1: %d , i2: %d\n", i1, i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);	
	
	printf("\n");
	
	printf("i1: %d , i2: %d\n", i1, i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);	
	printf("SWAP(i1, i2, int)\n");  
	SWAP(i1, i2, int); 
	printf("i1: %d , i2: %d\n", i1, i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);	
	
	printf("\n");
	
	
	
	/*int swap mem*/
	printf("\nstart type int SwapMem\n\n");
	printf("i1: %d , i2: %d\n", i1, i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);	
	printf("SwapMem(ip1, ip2, sizeof(int) )\n"); 
	SwapMem(ip1, ip2, sizeof(int) );    /*   <=> SwapMem(&i1, &i2, sizeof(int) ); */	
	printf("i1: %d , i2: %d\n", i1, i2);
	printf("*ip1: %d , *ip2: %d\n", *ip1, *ip2);	
	
	printf("\n");
	
	/*double swap mem*/
	printf("\nstart type double SwapMem\n\n");
	printf("d1: %f , d2: %f\n", d1, d2);
	printf("*dp1: %f , *dp2: %f\n", *dp1, *dp2);	
	printf("SwapMem(dp1, dp2, sizeof(double) )\n"); 
	SwapMem(dp1, dp2, sizeof(double) );    /*   <=> SwapMem(&d1, &d2, sizeof(int) ); */	
	printf("d1: %f , d2: %f\n", d1, d2);
	printf("*dp1: %f , *dp2: %f\n", *dp1, *dp2);	
	
	printf("\n");
	
	
	printf("\n");
	
	return 0;
}

/*--------------------------------------------------------------------*/


void SwapPtr(void** _x, void** _y){
	void* tmp; 
	
	assert(_x);
	assert(_y);
	
	tmp = *_x;
	*_x = *_y;
	*_y = tmp;
}

/*--------------------------------------------------------------------*/

void SwapMem(void* _x, void* _y, int _size){
	void* tmp = malloc(_size);
	
	memcpy(tmp , _x , sizeof(_size)  );
	memcpy(_x , _y  , sizeof(_size)  );
	memcpy(_y , tmp  , sizeof(_size)  );
	
}


/*--------------------------------------------------------------------*/








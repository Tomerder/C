/*-----------------------------------------Tomer Dery-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define SIZE 10


/*------------------------------------------------------------------------------------*/

int fnull();
int faccess();

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{  
    
    int rval = 0;
    rval = fnull();
    rval = faccess();
    
    
    
    printf("main end\n");
    
    
    return rval;
    
    
  
}

/*------------------------------------------------------------------------------------*/

int fnull(){
	fprintf(stderr, "fnull - begin\n");
	//int* stam = NULL;
	int* stam = calloc (1, sizeof(int));
	*stam = 5;
	fprintf(stderr, "fnull - end\n");
	
	return *stam;
}

int faccess(){
	fprintf(stderr, "faccess - begin\n");
	
	int adr = 777;
	int* ptr = &adr + 100 * sizeof(int);   /*writing to unalocated address*/
	*ptr = 7;	
	
	fprintf(stderr, "faccess - end\n");
	
	return 0;
}




/*------------------------------------------------------------------------------------*/


	

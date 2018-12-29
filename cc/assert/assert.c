#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define MAX(X,Y)  ( ((X) > (Y)) ? (X) : (Y) ) 

#ifdef _DEBUG
	/*#define ASSERT(X)       extern const char* __progname; \
							if(!X){ \
							  		fprintf(stderr, \
										"%s: %s:%d: " \
										"%s: Assertion \'%s\' failed.\n", \
										__progname, \
										__FILE__, \
										__LINE__, \
										__func__, \
										#X      ); \
									abort(); \
						     } 
	*/
	#define ASSERT(X)   extern const char* __progname; \
						(X) ? (0) : \
						  		(fprintf(stderr, \
										"%s: %s:%d: " \
										"%s: Assertion \'%s\' failed.\n", \
										__progname, \
										__FILE__, \
										__LINE__, \
										__func__, \
										#X      ); \
								 abort();    ) 										
#else  
	#define ASSERT(X)
#endif

void func(int* intPtr);


/*--------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int* intPtr = NULL;	
	func(intPtr);
	
	return 0;
}

/*--------------------------------------------------------------------*/


void func(int* _intPtr){
	/*assert(_intPtr);*/
	ASSERT(_intPtr);
	
	printf("xxx\n");
}


/*--------------------------------------------------------------------*/








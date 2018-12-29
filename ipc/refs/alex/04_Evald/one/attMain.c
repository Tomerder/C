/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-07    
    Last modified date:		2013-08-07
    Description: 
***************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

#include "attFunc.h"

int main(int argc, char **argv)
{
	const char *libname = argv[1];
	const char *symbol = argv[2];
	
	void (*Func)(int);
	char *error;
	void *handle;
	
	printf("one\n");
	
	handle = dlopen(libname, RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}
	
	printf("two\n");
	
	dlerror();	/* clear existing errors */

	*(void**) (&Func) =  dlsym(handle, symbol);
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}
	
	printf("three\n");

	Func(100);
	
	dlclose(handle);
	
	printf("four\n");
	
	return 0;
}

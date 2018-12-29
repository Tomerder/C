#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

#include "Count.h"

int main(int argc, char **argv)
{
	int a=2, b=3;
	
	const char *libname = (argc > 2) ? argv[1] : 0;
	const char *symbol = (argc > 2) ? argv[2] : 0;
	char *error;
	
	void* handle;
	int (*Add)(int, int);	/* ptr to func */
	
	printf("%s  %s\n", libname, symbol);
	
	handle = dlopen(libname, RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	dlerror();	/* clear existing errors */
	
	*(void**) (&Add) =  dlsym(handle, symbol);
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}
	
	printf("%d + %d = %d\n", a, b, Add(a,b) );

	return 0;
}

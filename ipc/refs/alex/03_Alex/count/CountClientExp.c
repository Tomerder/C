#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

#include "Count.h"

int main(int argc, char **argv)
{
	int a = (argc == 5) ? atoi(argv[3]) : 2;
	int b = (argc == 5) ? atoi(argv[4]) : 3;

	const char *libname = argv[1];
	const char *symbol = argv[2];
	
	char *error;
	void *handle;			/* function handle */
	int (*Add)(int, int);	/* ptr to func */
	
	if(argc != 3 && argc != 5)
	{
		printf("number of arguments must be 2 or 4\n");
		return 1;
	}
	
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


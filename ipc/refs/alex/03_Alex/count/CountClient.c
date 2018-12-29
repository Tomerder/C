#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

#include "Count.h"

int main(int argc, char **argv)
{
	switch(argc) {
	
		case 1:
		case 2:
		{
			int i;
			int n = (argc == 2) ? atoi(argv[1]) : 5;
			
			for(i=0; i<n; ++i)
			{
				count();
				printf("count out %d\n", g_index);
				sleep(1);
			}
			
			break;
		}	
		
		case 3:
		case 5:
		{
			int a = (argc == 4) ? atoi(argv[3]) : 2;
			int b = (argc == 4) ? atoi(argv[4]) : 3;
	
			const char *libname = argv[1];
			const char *symbol = argv[2];
			
			char *error;
			void *handle;			/* function handle */
			int (*Add)(int, int);	/* ptr to func */
	
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
			
			break;
		}
		
		default:
			printf("invalide # of arguments\n");
			break;
		
	} /* end switch */
	
	return 0;
}


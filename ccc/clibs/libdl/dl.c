#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

/*------------------------------------------------------------------------------------*/

enum{FALSE , TRUE};

/*------------------------------------------------------------------------------------*/

void DoFunc(char* _libName, char* _symbolName , int _isFunc);

/*------------------------------------------------------------------------------------*/

int main(int argc , char** argv)
{
	int isFunc = TRUE; 

	if(argc < 3){
		DoFunc("./count.so", "Mul" , isFunc);
		return 0;
	}	

	if(argc > 3 && argv[3] != "0" ){
		isFunc = FALSE;
	}	

	DoFunc(argv[1], argv[2] , isFunc);
	
	return 0;
}

/*------------------------------------------------------------------------------------*/

void DoFunc(char* _libName, char* _symbolName , int _isFunc)
{

	void *handle;
	int (*Func)(int,int);
	char *error;
	int* globToGet=0;

	/*handle = dlopen ("./count.so", RTLD_LAZY);*/
	handle = dlopen (_libName, RTLD_LAZY);
	if (!handle) {
		fputs (dlerror(), stderr);
		exit(1);
	}

	/*Func = dlsym(handle, "Mul");*/
	if(_isFunc){
		Func = dlsym(handle, _symbolName);
		printf ("%d\n", (*Func)(7,5));
	}else{
		globToGet = dlsym(handle, _symbolName);
		printf ("%d\n", *globToGet);
	}

	if ((error = dlerror()) != NULL)  {
		fputs(error, stderr);
		exit(1);
	}

	
	dlclose(handle);

}

/*------------------------------------------------------------------------------------*/












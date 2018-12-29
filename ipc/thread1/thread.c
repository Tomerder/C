/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 06.02.14                                                                                              
*    LAST MODIFIED: 06.02.14                                                                                  
*    DESCRIPTION: Thread
*****************************************************************************************************************/
#define _BSD_SOURCE 
#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE 500
/*-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <pthread.h>

/*-----------------------------------------------------------------*/

enum{FALSE, TRUE};

#define NUM_OF_THREADS 100

int glob;

void* func(void* _num);

/*typedef (void*)(*Func)(void*);*/

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	/*Func threadFunc = Func;*/
  	int i, num=0 ,ret;
  	pthread_t thread;
  	
  	for(i=0; i<NUM_OF_THREADS; i++)
  	{
		if(pthread_create(&thread, NULL, func, &num)) {
			fprintf(stderr, "Error creating thread\n");
			return 1;
		}  		
		
		printf("glob: %d\n", glob);
  	}
	
	/*
	for(i=0; i<NUM_OF_THREADS; i++)
  	{
		if(pthread_join(thread, NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 2;
		}	
	}
	*/
	
	
	return 0;
}


/*-----------------------------------------------------------------*/

void* func(void* _num)
{

	return (void*)(glob++);
}

/*-----------------------------------------------------------------*/








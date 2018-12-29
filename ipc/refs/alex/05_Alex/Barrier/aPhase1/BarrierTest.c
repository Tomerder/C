/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-09    
    Last modified date:		2013-09-09
    Description: 	conditional variables
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#include "Barrier.h"

/*######################################################################*/

#define DEF_THRESH	5
#define N_ITER		3

void* FuncThread(void* _barrier);
void* LastThread(void* _barrier);

int g_sum;


/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char* argv[])
{
	Barrier* myBar;
	pthread_t lastFunc, *funcs;
	int N = (argc > 1) ? atoi(argv[1]) : DEF_THRESH;
	int i;
	
	/*** initialize & allocate memory ***/
	g_sum = 0;
	
	myBar = BarrierInit(N);
	if(!myBar)
	{
		perror("BarrierInit");
		return -1;
	}
	
	funcs = (pthread_t*)malloc( (N-1) * sizeof(pthread_t) );
	if(!funcs)
	{
		perror("malloc");
		return -1;
	}
	
	
	/* create N-1 regular threads */
	for(i=0; i<N-1; ++i)
	{
		pthread_create(&funcs[i], 0, FuncThread, (void*)myBar);
	}
	sleep(1);
	
	printf("main before: sum is %d\n", g_sum);
	
	/* iterate */
	for(i=0; i<N_ITER; ++i)
	{
		/* create last thread */
		pthread_create(&lastFunc, 0, LastThread, (void*)myBar);
		sleep(1);
		/* join last thread */
		pthread_join(lastFunc, 0);
		
		printf("main i=%d: sum is %d\n", i, g_sum);
	}
	
	/* join regular threads */
	for(i=0; i<N-1; ++i)
	{
		pthread_join(funcs[i], 0);
	}
	
	printf("main after: sum is %d\n", g_sum);
	
	free(funcs);
	return 0;
}
/*######################################################################*/



/*######################################################################*/
void* FuncThread(void* _barrier)
{
	Barrier* bar = (Barrier*)_barrier;
	int i;
	
	printf("FuncThread here\n");
	
	for(i=0; i<N_ITER; ++i)
	{
		BarrierWait(bar);
		printf("FuncThread: done waiting %d\n", i);
	
		++g_sum;
	}
	
	pthread_exit(0);	/*** temp ***/
}
/*######################################################################*/


/*######################################################################*/
void* LastThread(void* _barrier)
{
	Barrier* bar = (Barrier*)_barrier;
	
/*	printf("sum: %d\n", g_sum);	*/
	BarrierWait(bar);
	++g_sum;
/*	printf(" new sum: %d\n", g_sum);*/
	
	pthread_exit(0);
}
/*######################################################################*/



/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-13    
    Last modified date:		2013-09-13
    Description: 	conditional variables
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "BarrierPh2.h"


#define DEF_THRESH	5
#define N_ITER		2

void* FuncThread	(void* _params);
void WhileWaiting	(void* _funcParams);

int g_sum;
int g_arr[DEF_THRESH];


typedef struct Params_t
{
	Barrier *m_barEnter;
	Barrier *m_barLeave;
	Func	m_func;
	void*	m_funcParams;
} Params;


/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char* argv[])
{
	Barrier *barEnterWaitRoom, *barLeaveWaitRoom;
	pthread_t *funcs;
	Params *params;
	
	int N = (argc > 1) ? atoi(argv[1]) : DEF_THRESH;
	int i;
	int success = 1;
	
	/*** initialize & allocate memory ***/
	g_sum = 0;
	memset(g_arr, 0, DEF_THRESH*sizeof(int));
	
	
	barEnterWaitRoom = BarrierInit(N);
	barLeaveWaitRoom = BarrierInit(N);
	if( !barEnterWaitRoom || !barLeaveWaitRoom )
	{
		perror("BarrierInit");
		return -1;
	}
	
	params = (Params*)malloc(N*sizeof(Params));
	if(!params)
	{
		perror("malloc");
		return -1;
	}
	for(i=0; i<N; ++i)
	{
		params[i].m_barEnter	= barEnterWaitRoom;
		params[i].m_barLeave	= barLeaveWaitRoom;
		params[i].m_func		= WhileWaiting;
		params[i].m_funcParams	= (void*)(i+1);
	}
	
	funcs = (pthread_t*)malloc(N*sizeof(pthread_t));
	if(!funcs)
	{
		perror("malloc");
		return -1;
	}
	
	/* create N-1 threads */
	for(i=0; i<N-1; ++i)
	{
		pthread_create(funcs+i, 0, FuncThread, params+i);
	}
	sleep(1);
	
	/* check success */
	printf("g_arr: ");
	for(i=0; i<N-1; ++i)
	{
		printf("%d ", g_arr[i]);
		success *= ( g_arr[i] == 1 );
	}
	printf("\nstage 1: %s, g_sum = %d\n", (success)?"success":"failure", g_sum);
	
	/* create Nth thread */
	pthread_create(funcs+N-1, 0, FuncThread, params+N-1);
	sleep(1);
		
	
	/* iterate */
/*	for(i=1; i<N_ITER; ++i)*/
/*	{*/
		/* check success */
/*		printf("g_arr: ");*/
/*		for(i=0; i<N-1; ++i)*/
/*		{*/
/*			printf("%d ", g_arr[i]);*/
/*			success *= ( g_arr[i] == i*2 + 1 );*/
/*		}*/
/*		printf("\nstage 1: %s, g_sum = %d\n", (success)?"success":"failure", g_sum);*/
/*	}*/
	
	/* join threads */
	for(i=N-1; i>=0; --i)
	{
		pthread_join(funcs[i], 0);
	}
	sleep(1);
	
	/* check success */
	printf("g_arr: ");
	for(i=0; i<N; ++i)
	{
		printf("%d ", g_arr[i]);
		success *= (g_arr[i]==N_ITER*2);
	}
	printf("\nstage 2: %s, g_sum = %d\n", (success)?"success":"failure", g_sum);
	
	
	BarrierDestroy(barEnterWaitRoom);
	BarrierDestroy(barLeaveWaitRoom);
	free(funcs);
	free(params);
	return 0;
}
/*######################################################################*/




void* FuncThread(void* _params)
{
	Barrier* barEnter	= ((Params*)_params)->m_barEnter;
	Barrier* barLeave	= ((Params*)_params)->m_barLeave;
	Func	 func 		= ((Params*)_params)->m_func;
	void*	 funcParams = ((Params*)_params)->m_funcParams;
	int i;
	
	for(i=0; i<N_ITER; ++i)
	{
		BarrierWait(barEnter, func, funcParams);
		++g_sum;
		printf("functhread\n");
		sleep(1);
	
		BarrierWait(barLeave, func, funcParams);
		++g_sum;
		printf("functhread2\n");
		sleep(1);
	}
	
	pthread_exit(0);
	return NULL;
}
/*######################################################################*/


void WhileWaiting(void* _funcParams)
{
	++g_arr[ (int)_funcParams - 1 ];
	printf("WhileWaiting, g_sum = %d\n", g_sum);
}
/*######################################################################*/




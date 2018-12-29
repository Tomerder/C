/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-12
    Last modified date:		2013-08-12
    Description: single threaded scheduler - testing
********************************************************************************/

#include <stdio.h>

#include "Sched.h"


static Status	CountOnes(void* _params);
static Status	CountTens(void* _params);
static Status	CountHundreds(void* _params);
static Status	Pause(void* _params);

int g_ones;
int g_tens;
int g_hundreds;


int main()
{
	Sched *scheduler = NULL;
	int one = 1;
	int ten = 10;
	int hundred = 100;
	
	g_ones = g_tens = g_hundreds = 0;
	
	scheduler = SchedCreate();
	if(!scheduler)
	{
		printf("create failed\n");
		return 1;
	}
	
	printf("created\n");
	
	SchedAdd(scheduler, CountOnes, (void*)&one, 4.5);
	SchedAdd(scheduler, CountTens, (void*)&ten, 1.0);
	SchedAdd(scheduler, CountHundreds, (void*)&hundred, 2.3);
	SchedAdd(scheduler, Pause, (void*)scheduler, 0.5);
	printf("added\n");
	
	SchedRun(scheduler);
	
	printf("paused\n");
	SchedRun(scheduler);

	printf("done\n");
	
	SchedDestroy(scheduler);
	printf("destroyed\n");
	
	return 0;
}

static Status	CountOnes(void* _params);


/*######################################################################*/
/*							functions for tasks							*/
/*######################################################################*/




Status	CountOnes(void* _params)
{
	const int stop = 3*(*(int*)_params);
	
	g_ones += (*(int*)_params);
	printf("%d\n", g_ones);
	
	return (g_ones>=stop) ? TASK_DONE : TASK_CONTINUE;
}


Status	CountTens(void* _params)
{
	const int stop = 5*(*(int*)_params);
	
	g_tens += (*(int*)_params);
	printf("%d\n", g_tens);
	
	return (g_tens>=stop) ? TASK_DONE : TASK_CONTINUE;
}



Status	CountHundreds(void* _params)
{
	const int stop = 5*(*(int*)_params);
	
	g_hundreds += (*(int*)_params);
	printf("%d\n", g_hundreds);
	
	return (g_hundreds>=stop) ? TASK_DONE : TASK_CONTINUE;
}




Status	Pause(void* _params)
{
	if(g_hundreds >= 300)
	{
		SchedPause( (Sched*)_params );
		return TASK_DONE;
	}
	else
	{
		return TASK_CONTINUE; 
	}
}





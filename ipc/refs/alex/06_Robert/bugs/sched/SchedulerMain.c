/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-12
    Last modified date:		2013-08-12
    Description: single threaded scheduler - testing
********************************************************************************/

#include <stdio.h>
#include <string.h>

#include "Sched.h"


#define SIZE1	15


/*######################################################################*/
/*							Test Functions								*/
/*######################################################################*/
static int		TestSched();





/*######################################################################*/
/*							functions for tasks							*/
/*######################################################################*/
static Status	CountOnes(void* _params);
static Status	CountTwos(void* _params);
static Status	CountThrees(void* _params);
/*static Status	Pause(void* _params);*/




int g_num1;
int g_test1[SIZE1];
const int g_test1Compare[SIZE1] = { 1, 2, 3, 1, 2, 1, 2, 1, 3, 1, 2, 2, 3, 3, 3};

int g_ones, g_twos, g_threes;



/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main()
{
	int error;
	char *msgS="--Success--", *msgF="--Failure--";

	printf("\n\t****** Testing Started ******\n\n");

	error = TestSched();
	printf("Test1:\t\t%s\n", (!error)?msgS:msgF);
	
	
	
	
	
	printf("\n\t****** Testing finished ******\n\n");
	return 0;
}
/*######################################################################*/


int TestSched()
{
	Sched *scheduler = NULL;
	int one = 1, two = 2, three = 3;
	int error = 0;
	int i;
	
	g_num1 = 0;
	memset(&g_test1, 0, sizeof(int)*SIZE1);
	
	g_ones = g_twos = g_threes = 0;
	
	scheduler = SchedCreate();
	if(!scheduler)
	{
		return 1;
	}
	
	SchedAdd(scheduler, CountOnes, (void*)&one, 1.5);
	SchedAdd(scheduler, CountTwos, (void*)&two, 2.1);
	SchedAdd(scheduler, CountThrees, (void*)&three, 5.15);
	
	SchedRun(scheduler);
	
	for(i=0; i<SIZE1; ++i)
	{
		error += (g_test1[i] != g_test1Compare[i]);
	}
	
/*	SchedAdd(scheduler, Pause, (void*)scheduler, 0.5);*/
	
	
	
	
	SchedDestroy(scheduler);
	return error;
}













/*######################################################################*/
/*							functions for tasks							*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
Status	CountOnes(void* _params)
{
	const int stop = 5;
	
	g_test1[g_num1] = *(int*)_params;
	++g_num1;
	++g_ones;
	
	return (g_ones>=stop) ? TASK_DONE : TASK_CONTINUE;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
Status	CountTwos(void* _params)
{
	const int stop = 5;
	
	g_test1[g_num1] = *(int*)_params;
	++g_num1;
	++g_twos;
	
	return (g_twos>=stop) ? TASK_DONE : TASK_CONTINUE;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
Status	CountThrees(void* _params)
{
	const int stop = 5;
	
	g_test1[g_num1] = *(int*)_params;
	++g_num1;
	++g_threes;
	
	return (g_threes>=stop) ? TASK_DONE : TASK_CONTINUE;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
Status	Pause(void* _params)
{
	if(g_threes >= 3)
	{
		SchedPause( (Sched*)_params );
		return TASK_DONE;
	}
	else
	{
		return TASK_CONTINUE; 
	}
}
/*----------------------------------------------------------------------*/




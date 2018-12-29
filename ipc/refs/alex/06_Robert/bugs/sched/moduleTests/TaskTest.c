/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-12
    Last modified date:		2013-08-12
    Description: single threaded scheduler - task module testing
********************************************************************************/

#include <stdio.h>

#include "../TimeSched.h"
#include "../Task.h"


static Status	CountOnes(void* _params);

int g_ones;


/********************************************************************************/
int main()
{
	Task *task = NULL;

	task = TaskCreate(CountOnes, NULL, 1.5);
	if(!task)
	{
		printf("create failed\n");
		return 1;
	}
	printf("created\n");
	
	
	TaskDestroy(task);
	printf("destroyed\n");


	return 0;
}
/********************************************************************************/


Status	CountOnes(void* _params)
{
	const int stop = 3*(*(int*)_params);
	
	g_ones += (*(int*)_params);
	printf("%d\n", g_ones);
	
	return (g_ones>=stop) ? TASK_DONE : TASK_CONTINUE;
}



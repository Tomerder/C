/*************************************************************
 	Project:	Task               			
 	Author:		Tomer Dery							
 	Created:	27/1/2112								
 	Updated:	27/1/2112							
*************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "Task.h"
#include "Time.h"
#include "log.h"

/*--------------------------------------------------------------------*/

enum {FALSE, TRUE};

#define WRT_LOG(MODE, STR) ToLog(MODE, STR, __FILE__, __func__ );

/*--------------------------------------------------------------------*/

struct Task{
	Func m_func;
	Params m_params;
	TimeSpec* m_interval;
	TimeSpec* m_nextTimeToRun;
}; 

/*--------------------------------------------------------------------*/


Task* TaskCreate(Func _func, Params _params, float _interval )
{
	Task* task = NULL;


	task = malloc(sizeof(Task));
	if(task == NULL){
		return NULL;
	}
	
	task->m_func = _func;
	task->m_params = _params;
	
	task->m_interval = malloc(sizeof(TimeSpec));
	if(task->m_interval == NULL){
		free(task);
		return NULL;
	}	

	task->m_nextTimeToRun = malloc(sizeof(TimeSpec));
	if(task->m_nextTimeToRun == NULL){
		free(task->m_nextTimeToRun);
		free(task);
		return NULL;
	}	

	/*alloc task->m_nextTimeToRun & m_interval*/	
	GetTimeFromFloat(_interval, task->m_interval);
	
	TaskSetNextTimeToRun(task);	


	return task;
}

/*--------------------------------------------------------------------*/

void TaskDestroy(Task* _task)
{
	free(_task->m_interval);
	free(_task->m_nextTimeToRun);
	free(_task);
}

/*--------------------------------------------------------------------*/

int CompTasks(void* _task1, void* _task2 )
{
	/*send tasks m_nextTimeToRun to Time.CompTimes() */
	return CompTimes( ((Task*)_task1)->m_nextTimeToRun, ((Task*)_task2)->m_nextTimeToRun  );	
}


/*--------------------------------------------------------------------*/

FuncStatus TaskRun(Task* _task)
{
	TimeSpec curTime; 
	float sleepTime=0; 

	while(TRUE){
		GetCurTime(&curTime);
		
		/*check if it is time to run -> else sleep */
		if(  CompTimes( &curTime, _task->m_nextTimeToRun  )      ){			
			return ( _task->m_func(_task->m_params) ); 
		}else{
			sleepTime = GetFloatFromTimeSub(_task->m_nextTimeToRun , &curTime);

			sleep(sleepTime);
		}
	
	}
	
	return 0;
}

/*--------------------------------------------------------------------*/

void TaskSetNextTimeToRun(Task* _task)
{
	GetTimeAddToCur(_task->m_interval, _task->m_nextTimeToRun);
}

/*--------------------------------------------------------------------*/




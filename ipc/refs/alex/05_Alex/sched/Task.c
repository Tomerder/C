/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-12
    Last modified date:		2013-08-12
    Description: single threaded scheduler - Task module implementation
********************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "Task.h"

/*	Task: contains function Func with parameters m_params. if the function
	stops beroe it's done, it resumes working after a time of at least m_interval.
	m_wakeUp is the next time to start working
*/
struct Task
{
	Func	m_do;
	void*	m_params;
	Time*	m_interval;
	Time*	m_wakeUp;
};



/*######################################################################*/
/*								TASK CREATE								*/
/* 						create Task struct from input					*/
/*######################################################################*/
Task* TaskCreate(Func _do, void *_params, float _interval)
{
	Task *newTask = NULL;
	
	assert( _do && _interval >= 0);
	
	newTask = (Task*)malloc(sizeof(Task));
	if( !newTask )
		return NULL;
	
	newTask->m_do = _do;
	newTask->m_params = _params;
	newTask->m_interval = Float2Time(_interval);
	
	newTask->m_wakeUp = TimeGetCurrent();
	
	return newTask;
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/*								TASK DESTROY							*/
/* 							free Task struct							*/
/*######################################################################*/
void TaskDestroy(Task *_task)
{
	assert(_task);
	
	TimeSpecDestroy(_task->m_interval);
	TimeSpecDestroy(_task->m_wakeUp);
	free(_task);
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/*								TASK RUN								*/
/* 		run task. if current time is earlier than task's wakeup time,	*/
/*		sleep until then.												*/
/*######################################################################*/
Status TaskRun(Task *_task)
{
	TimeSleep( _task->m_wakeUp );
	return _task->m_do(_task->m_params);
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/*							TASK COMPARE								*/
/* 	compare 2 tasks, return positive # if task1 > task2, 0 if equal,	*/
/*	negative if task1 < task2											*/
/*######################################################################*/
int TaskCompare(Task *_task1, Task *_task2)
{
	return TimeCompare(_task1->m_wakeUp, _task2->m_wakeUp);
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							TASK SET WAKE UP							*/
/* 	update wake up time of task: _mode can be CURRENT - current time,	*/
/*	or INTERVAL	- current time + task interval							*/
/*######################################################################*/
void TaskSetWakeUp(Task *_task, TaskMode _mode)
{
	TimeSpecDestroy(_task->m_wakeUp);
	_task->m_wakeUp = (_mode==CURRENT) ? TimeGetCurrent() : TimeAdd(_task->m_interval);
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/*								TASK PRINT								*/
/* 		print data of a single task	- conforming to doFunc format		*/
/*######################################################################*/
void TaskPrint(void* _task, void* _params)
{
	Task* task = (Task*)_task;
	
	printf("Task:\n");
	printf("\tinterval: %f\n", Time2Float(task->m_interval) );
	printf("\twake-up time: %f\n", Time2Float(task->m_wakeUp) );
}
/*----------------------------------------------------------------------*/



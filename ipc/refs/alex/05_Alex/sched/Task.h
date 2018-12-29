/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-12    
    Last modified date:		2013-08-12
    Description: header file for single threaded scheduler - task module
********************************************************************************/

#ifndef __SCHED_TASK_H__
#define __SCHED_TASK_H__

#include "TimeSched.h"

typedef struct Task Task;

typedef enum Status
{
	TASK_ERROR,
	TASK_DONE,
	TASK_CONTINUE
} Status;

typedef Status(*Func)(void* _param);

/* for setting task wake up time as either: */
typedef enum TaskMode
{
	CURRENT,	/* current time */
	INTERVAL	/* current time + task interval */
} TaskMode;

/* create Task struct from input */
Task* 	TaskCreate		(Func _do, void *_params, float _interval);
/* free Task struct */
void 	TaskDestroy		(Task *_task);
/* run task */
Status 	TaskRun			(Task *_task);
/* compare 2 tasks, return positive # if task1 > task2, 0 if equal, negative if task1 < task2 */
int 	TaskCompare		(Task *_task1, Task *_task2);

/* update wakeup time */
void 	TaskSetWakeUp	(Task *_task, TaskMode _mode);
/* print a single task */
void 	TaskPrint		(void* _task, void* _params);


#endif /*__SCHED_TASK_H__ */

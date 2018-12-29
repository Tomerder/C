#ifndef __TASK_H__
#define __TASK_H__

/*---------------------------------------------------------------------------*/

#include "Adt.h"

/*---------------------------------------------------------------------------*/

typedef enum 
{
	FUNC_ERROR,	/* Error occurs during running 	*/
	FUNC_DONE,	/* Function finish running	*/
	FUNC_CONTINUE   /* Function should be re-scheualed in at least interval seconds */
}FuncStatus; 

typedef FuncStatus(*Func)(Params _params); 

/*---------------------------------------------------------------------------*/

typedef struct Task Task;

/*---------------------------------------------------------------------------*/

Task* TaskCreate(Func _func, Params _param, float _interval );

void TaskDestroy(Task* _task);

/*run func with params - returns what is returned from func */
FuncStatus TaskRun(Task* _task);

/*calculate nextTimeToRun as current Time + task inteval */
void TaskSetNextTimeToRun(Task* _task);

/* returns 1 if _task1 is "bigger" then task2 - 0 otherwise 
   calls comp times with parameters nextTimeToRun of the tasks => will return 1 if task2 should run before task1*/
int CompTasks(void* _task1, void* _task2 );

/*---------------------------------------------------------------------------*/

#endif /* __TASK_H__ */

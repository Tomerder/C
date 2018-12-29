/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-11
    Last modified date:		2013-08-12
    Description: single threaded scheduler - implementation
********************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "VectorGeneric.h"
#include "HeapGeneric.h"
#include "Sched.h"


/*######################################################################*/
/*				Definitions & static foreward declarations				*/
/*######################################################################*/

#define VECTOR_INIT_SIZE	4
#define VECTOR_EXTEND		4


/*	scheduler: contains Vector of tasks (m_tasks), Priority Queue (Heap m_pQ)
	according to wake-up times, current task (m_curTask), and a flag m_paused:
	usually 0, 1 if paused
*/
struct Sched
{
	Heap	*m_pQ;		/* priority queue */
	Vector	*m_tasks;	/* list of tasks */
	Task	*m_curTask;	/* current task */
	int		 m_paused;	/* flag for pausing */
};


/* compFunc for heap */
static int		SchedCompFunc	(HeapData _time1, HeapData _time2);
/* insert task to scheduler queue, setting its time according to _mode */
static ADTErr	InsertTask		(Sched *_sched, Task *_task, TaskMode _mode);

/* compare to tasks for scheduling */
/*static int		SchedCompare	(Task *_task1, Task *_task2);*/

/*----------------------------------------------------------------------*/



/*######################################################################*/
/*								SCHED CREATE							*/
/*				Create a scheduler with a priority queue				*/
/*######################################################################*/
Sched	*SchedCreate	(void)
{
	Sched *newSched = NULL;
	
	/* create new scheduler */
	newSched = (Sched*)malloc(sizeof(Sched));
	if( !newSched )
		goto SCHED_FAILED;
	
	/* empty vector for tasks */
	newSched->m_tasks = VectorCreate(VECTOR_INIT_SIZE, VECTOR_EXTEND);
	if( !newSched->m_tasks )
		goto VECTOR_FAILED;
	
	/* heap from vector, for priority queue, with compFunc */
	newSched->m_pQ = HeapBuild(newSched->m_tasks, SchedCompFunc);
	if( !newSched->m_pQ )
		goto HEAP_FAILED;
	
	newSched->m_curTask = NULL;
	
	/* starting as not paused */
	newSched->m_paused = 0;
	
	return newSched;
	
	
HEAP_FAILED:
	VectorDestroy(newSched->m_tasks);

VECTOR_FAILED:
	free(newSched);

SCHED_FAILED:
	return NULL;
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/*								SCHED DESTROY							*/
/*					Free everything from the scheduler					*/
/*######################################################################*/
void 	SchedDestroy	(Sched *_sched)
{
	Task* task = NULL;
	size_t vectorSize;
	assert(_sched);
	
	/* free heap */
	HeapDestroy( _sched->m_pQ );
	
	/* free all tasks */
	VectorNumItems( _sched->m_tasks, &vectorSize);
	while( vectorSize )
	{
		VectorDelete( _sched->m_tasks, (void**)&task );
		TaskDestroy(task);
		VectorNumItems( _sched->m_tasks, &vectorSize);
	}
	
	if(_sched->m_curTask)
		TaskDestroy( _sched->m_curTask );
		
	/* free vector & scheduler */
	VectorDestroy( _sched->m_tasks );
	free(_sched);
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/*								SCHED ADD								*/
/*		Add a function with parameters as a task to the scheduler		*/
/*######################################################################*/
ADTErr	SchedAdd		(Sched *_sched, Func _do, void *_params, float _interval)
{
	Task *newTask;
	int error;
	
	assert(_sched && _do && _interval>=0);

	/* create new task */
	newTask = TaskCreate(_do, _params, _interval);
	if( !newTask )
		return ERR_ALLOCATION_FAILED;
	
	/* insert into priority queue */
	error = InsertTask(_sched, newTask, CURRENT);

	return error;
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/*								SCHED RUN								*/
/*		Run the scheduler until told to pause or having nothing to do	*/
/*######################################################################*/
void	SchedRun		(Sched *_sched)
{
	int status;
	
	/* set paused flag to 0 */
	_sched->m_paused = 0;
	
	/* start running */
	while( !_sched->m_paused )
	{		
		/* get task */
		_sched->m_curTask = (Task*)HeapExtractMax(_sched->m_pQ);
		/* if no tasks in queue */
		if(!_sched->m_curTask)
		{
			return;
		}
		
		status = TaskRun(_sched->m_curTask);
		
		/* check return status of task */
		if(status == TASK_CONTINUE)
		{
			InsertTask(_sched, _sched->m_curTask, INTERVAL);
		}
		else
		{
			TaskDestroy(_sched->m_curTask);
		}
		
	} /* end while */
	
	/* if paused */
	return;
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/*								SCHED PAUSE								*/
/*							Pause the scheduler							*/
/*######################################################################*/
void	SchedPause		(Sched *_sched)
{
	_sched->m_paused = 1;
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/*								SCHED PRINT								*/
/*						print contents of scheduler						*/
/*######################################################################*/
void 	SchedPrint		(Sched *_sched)
{
	printf("Current time: %f\n", Time2Float( TimeGetCurrent() ) );
	HeapForEach(_sched->m_pQ, TaskPrint, NULL);
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							Internal Functions							*/
/*######################################################################*/


/* compFunc for heap */
int		SchedCompFunc	(HeapData _task1, HeapData _task2)
{
	return TaskCompare( (Task*)_task2, (Task*)_task1 );
}
/*----------------------------------------------------------------------*/

/* insert task to scheduler queue, setting its time according to _mode */
ADTErr InsertTask(Sched *_sched, Task *_task, TaskMode _mode)
{
	int error;
	
	TaskSetWakeUp(_task, _mode);
	error = HeapInsert( _sched->m_pQ, (HeapData)_task );
	
	return error;
}
/*----------------------------------------------------------------------*/

/* compare to tasks for scheduling */
int		SchedCompare	(Task *_task1, Task *_task2)
{
	return TaskCompare(_task1, _task2);
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/



/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-11    
    Last modified date:		2013-08-11
    Description: header file for single threaded scheduler
********************************************************************************/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "ADTDefs.h"
#include "TimeSched.h"
#include "Task.h"

typedef struct Sched Sched;


/*	Create a scheduler with a priority queue	*/
Sched	*SchedCreate	(void);
/*	Free everything from the scheduler	*/
void 	SchedDestroy	(Sched *_sched);

/*	Add a function with parameters as a task to the scheduler	*/
ADTErr	SchedAdd		(Sched *_sched, Func _do, void *_params, float _interval);

/* Run the scheduler until told to pause or having nothing to do */
void	SchedRun		(Sched *_sched);
/*	Pause the scheduler		*/
void	SchedPause		(Sched *_sched);

/* print contents of scheduler */
void 	SchedPrint		(Sched *_sched);

#endif /* __SCHED_H__ */

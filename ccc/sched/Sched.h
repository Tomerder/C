#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__ 

#include "Adt.h"
#include "Task.h"

/*---------------------------------------------------------------------------*/

typedef struct Sched Sched;

/*---------------------------------------------------------------------------*/

/* Initialize Sched */
Sched*	SchedCreate();

/* Free Sched allocated memory */
void	SchedDestroy(Sched* _sched);

/* Add function to sched queue, schedule it to current time */
/* Note: interaval is non-negative number in seconds unit, error will be returned for negative interval */  
ADTERR	SchedAdd(Sched* _sched, Func _function, Params _param, float _interval); 

/* Run functions in Sched queue until it empty or Sched is paused */
void	SchedRun(Sched* _sched);

/*---------------------------------------------------------------------------*/

#endif /* __SCHEDULER_H__ */

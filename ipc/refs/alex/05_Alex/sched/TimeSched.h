/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-12    
    Last modified date:		2013-08-12
    Description: header file for single threaded scheduler - time module
********************************************************************************/

#ifndef __SCHED_TIME_H__
#define __SCHED_TIME_H__

#include <time.h>


typedef struct timespec		Time;


/* allocate memory for a variable of type Time */
Time* 	TimeSpecCreate();
/* free memory of a variable of type Time */
void 	TimeSpecDestroy	(Time *_time);

/* convert floating point # of seconds to time format */
Time* 	Float2Time		(float _seconds);
/* convert time format to floating point # of seconds */
float 	Time2Float		(Time *_time);

/* add _time to current time */
Time* 	TimeAdd			(Time *_time);
/* subtract 2 times */
Time* 	TimeSubtract	(Time *_time1, Time *_time2);

/* compare 2 times, return  positive # if time1 > time2, 0 if equal, negative if time1 < time2 */
int 	TimeCompare		(Time *_time1, Time* _time2);
/* get current time from system clock */
Time* 	TimeGetCurrent();
/* sleep until _wakeUp time arrives */
void 	TimeSleep		(Time *_wakeUp);

/* print time from Time format */
void 	TimePrint		(Time *_time);


#endif /*__SCHED_TIME_H__ */


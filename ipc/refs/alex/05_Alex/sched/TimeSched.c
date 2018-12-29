/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-12
    Last modified date:		2013-08-12
    Description: single threaded scheduler - Time module implementation
********************************************************************************/

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE	199309L
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

#include "TimeSched.h"

#define BILLION		1000000000L
#define NANO		0.000000001D



/*######################################################################*/
/*							TIME SPEC CREATE							*/
/* 				allocate memory for a variable of type Time				*/
/*######################################################################*/
Time* TimeSpecCreate()
{
	Time* time;
	
	time = (Time*)malloc(sizeof(Time));
	return (time) ? time : NULL;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							TIME SPEC DESTROY							*/
/* 				free memory of a variable of type Time					*/
/*######################################################################*/
void TimeSpecDestroy(Time *_time)
{
	assert(_time);
	free(_time);
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								FLOAT TO TIME							*/
/* 			convert floating point # of seconds to Time format			*/
/*######################################################################*/
Time *Float2Time(float _seconds)
{
	Time *specTime;
	
	specTime = TimeSpecCreate();
	if(!specTime)
		return NULL;
	
	specTime->tv_sec = (int)_seconds;
	specTime->tv_nsec = (_seconds - (int)_seconds)*BILLION;
	
	return specTime;
}
/*----------------------------------------------------------------------*/

/*######################################################################*/
/*								TIME TO FLOAT							*/
/* 			convert time format to floating point # of seconds			*/
/*######################################################################*/
float Time2Float(Time *_time)
{
	float time;
	
	time = _time->tv_sec;
	time += (double)_time->tv_nsec/BILLION;
	
	return time;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*									TIME ADD							*/
/* 							add _time to current time					*/
/*######################################################################*/
Time* TimeAdd(Time *_time)
{
	Time *curTime;
	Time *newTime;
	
	newTime = TimeSpecCreate();
	if(!newTime)
		return NULL;
	
	curTime = TimeGetCurrent();
	newTime->tv_sec = _time->tv_sec + curTime->tv_sec;
	
	newTime->tv_nsec =_time->tv_nsec + curTime->tv_nsec;
	if( newTime->tv_nsec >= BILLION )
	{
		++newTime->tv_sec;
		newTime->tv_nsec -= BILLION;
	}
	
	TimeSpecDestroy(curTime);
	return newTime;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								TIME SUBTRACT							*/
/* 					get difference between 2 time values				*/
/*######################################################################*/
Time* TimeSubtract(Time *_time1, Time *_time2)
{
	Time *result;
	long nanoResult;
	
	result = TimeSpecCreate();
	if(!result)
		return NULL;
	
	result->tv_sec = _time1->tv_sec - _time2->tv_sec;
	nanoResult = _time1->tv_nsec - _time2->tv_nsec;
	
	if( nanoResult < 0 )
	{
		--result->tv_sec;
		result->tv_nsec = nanoResult*(-1);
	}
	else
	{
		result->tv_nsec = nanoResult;
	}
	
	return result;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								TIME COMPARE							*/
/* 	compare 2 times, return  positive # if time1 > time2, 0 if equal,	*/
/*	negative if time1 < time2											*/
/*######################################################################*/
int TimeCompare(Time *_time1, Time *_time2)
{
	long diff;
	
	if( _time1->tv_sec != _time2->tv_sec )
	{
		return _time1->tv_sec - _time2->tv_sec;
	}
	
	diff = _time1->tv_nsec - _time2->tv_nsec;	
	if( 0 == diff )
	{
		return 0;
	}
	
	return (diff > 0) ? 1 : -1;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							TIME GET CURRENT							*/
/* 		get current time from system clock, in time format				*/
/*######################################################################*/
Time* TimeGetCurrent()
{
	Time *time;
	
	time = TimeSpecCreate();
	if(!time)
		return NULL;
	
	clock_gettime(CLOCK_REALTIME, time);
	return time;
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								TIME SLEEP								*/
/* 		sleep for the remaining time between current time and 			*/
/*		wakeup time of calling task, if time remaining is > 0			*/
/*######################################################################*/
void TimeSleep(Time *_wakeUp)
{
	Time *diff, zero;
	
	diff = TimeSubtract(_wakeUp, TimeGetCurrent());
	zero.tv_sec = 0;
	zero.tv_nsec = 0;
	nanosleep( ( diff->tv_sec > 0 || (diff->tv_sec==0 && diff->tv_nsec > 0 ) ) ? diff : &zero, NULL );
	TimeSpecDestroy(diff);
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								TIME PRINT								*/
/* 						print time from Time format		 				*/
/*######################################################################*/
void TimePrint(Time *_time)
{
	printf("%d s and %ld ns\n", (int)_time->tv_sec, _time->tv_nsec);
}
/*----------------------------------------------------------------------*/





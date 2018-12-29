#ifndef __TIME_H__
#define __TIME_H__

/*---------------------------------------------------------------------------*/

/*#include <linux/time.h>*/

#include <time.h>

/*---------------------------------------------------------------------------*/

typedef struct timespec TimeSpec;

/*---------------------------------------------------------------------------*/

/* will return 1 if task2 should run before task1*/
int CompTimes(TimeSpec* _time1, TimeSpec* _time2 );

/*---------------------------------------------------------------------------*/

void GetCurTime(TimeSpec* _curTime);

/*sets _timespec from float _interval parameter : float->timespec */
void GetTimeFromFloat(float _interval, TimeSpec* _timespec);  

/*sets nextTimeToRun = current Time + task inteval */
void GetTimeAddToCur(TimeSpec* _interval, TimeSpec* _nextTimeToRun); 

/* return float value of _nextTimeToRun - _curTime => time to wait until the task should run */
float GetFloatFromTimeSub(TimeSpec* _nextTimeToRun , TimeSpec* _curTime); 

/*---------------------------------------------------------------------------*/

void TimePrint(TimeSpec* _time);

/*---------------------------------------------------------------------------*/

#endif /* __TIME_H__ */

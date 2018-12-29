/*************************************************************
 	Project:	Time               			
 	Author:		Tomer Dery							
 	Created:	27/1/2112								
 	Updated:	27/1/2112							
*************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

#include "Time.h"

/* gc -D_POSIX_C_SOURCE=199309L *.c -lrt -O2 -o sched*/

/*--------------------------------------------------------------------*/

#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000L

#define SEC_RES GIGA
#define SEC_RES_F 1000000000.0
#define MAX_NANO 999999999L

enum {FALSE, TRUE};

/*--------------------------------------------------------------------*/

void GetTimeFromFloat(float _interval, TimeSpec* _timeSpec)
{	
	
	_timeSpec->tv_sec = _interval / SEC_RES;

   _timeSpec->tv_nsec =  _interval - _timeSpec->tv_sec * SEC_RES ;	
}

/*--------------------------------------------------------------------*/

void GetTimeAddToCur(TimeSpec* _interval, TimeSpec* _nextTimeToRun)
{
	TimeSpec curTime; 
	unsigned long nanoAdd = 0;
	unsigned long nanoAddCarry = 0;
	
	GetCurTime(&curTime);

	nanoAdd = _interval->tv_nsec + curTime.tv_nsec;
	
	nanoAddCarry = (nanoAdd >=  SEC_RES) ? (1) : (0);

	_nextTimeToRun->tv_sec = _interval->tv_sec + curTime.tv_sec + nanoAddCarry;

	_nextTimeToRun->tv_nsec = nanoAdd - nanoAddCarry * SEC_RES; 

}

/*--------------------------------------------------------------------*/

void GetCurTime(TimeSpec* _curTime)
{
	clock_gettime(CLOCK_REALTIME, _curTime);
}

/*--------------------------------------------------------------------*/

float GetFloatFromTimeSub(TimeSpec* _nextTimeToRun , TimeSpec* _curTime)
{
	float sleepTime;
	
	sleepTime = (_nextTimeToRun->tv_nsec - _curTime->tv_nsec) / SEC_RES_F + (_nextTimeToRun->tv_sec - _curTime->tv_sec) ;

	if(sleepTime < 0 ){
		sleepTime = 0;
	}

	return sleepTime;
}

/*--------------------------------------------------------------------*/

int CompTimes(TimeSpec* _time1, TimeSpec* _time2 )
{
    float secResult = _time1->tv_sec - _time2->tv_sec;

	if( secResult !=0 ){
		return ( secResult > 0 );
	}else{
		return (_time1->tv_nsec - _time2->tv_nsec) > 0;
	}
}

/*--------------------------------------------------------------------*/

void TimePrint(TimeSpec* _time)
{
	printf("%ld . %ld\n", _time->tv_sec, _time->tv_nsec );
}

/*--------------------------------------------------------------------*/












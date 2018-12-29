/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-12
    Last modified date:		2013-08-12
    Description: single threaded scheduler - time module testing
********************************************************************************/

#include <stdio.h>

#include "../TimeSched.h"


#define NANO	10e-9


int main()
{
	Time *one, *two, *cur;
	Time *add, *subt;
	float three, four;
	
	
	three = 26.9038;
	four = 12.0502;
	
	one = Float2Time(three);
	two = Float2Time(four);
	
	subt = TimeSubtract(one, two);
	
	
	printf("\n%f = %d sec + %ld nsec\n", three, (int)one->tv_sec, one->tv_nsec);
	printf("%f = %d sec + %ld nsec\n", four, (int)two->tv_sec, two->tv_nsec);
	printf("subtracting:\t%d sec + %ld nsec\n", (int)subt->tv_sec, subt->tv_nsec);
	printf("%f\n\n", Time2Float(subt) );
	
	cur = TimeGetCurrent();
	add = TimeAdd(one);
	
	printf("current time:\t%d sec + %ld nsec\n", (int)cur->tv_sec, cur->tv_nsec);
	printf("adding:\t\t%d sec + %ld nsec\n", (int)add->tv_sec, add->tv_nsec);
	
	TimePrint(cur);
	
	printf("%f\n\n", Time2Float(add) );
	
	TimeSpecDestroy(one);
	TimeSpecDestroy(two);
	TimeSpecDestroy(cur);
	TimeSpecDestroy(add);
	TimeSpecDestroy(subt);
	
	return 0;
}




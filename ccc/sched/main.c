/*************************************************************/
/** 	Project:	Scheduler              				    **/
/** 	Author:		Tomer Dery								**/
/** 	Created:	28/1/2112								**/
/** 	Updated:	28/1/2112								**/
/*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*------------------------------------------------------------------------------------*/

#include "Sched.h"

/*------------------------------------------------------------------------------------*/

enum {FALSE, TRUE};
enum {FAIL, PASS};

char* resultArr[2] = {"FAIL","PASS"};

#define SUCCESS 1

#define DEBUG 0

#define SEC 1000000000.0

#define MAX_RUNS 128

#define FUNC1_INTRVAL (SEC*2)
#define FUNC2_INTRVAL (SEC*10)
#define FUNC3_INTRVAL (SEC*15)
#define FUNC4_INTRVAL (SEC*25)

#define NUM_OF_RUNS_1 13
#define NUM_OF_RUNS_2 3

/*------------------------------------------------------------------------------------*/

time_t runStartTimeGlob; 
int testResultIndex = PASS;

/*------------------------------------------------------------------------------------*/
void TestOrder();
void TestTimes();

FuncStatus Func1t(Params _data);
FuncStatus Func2t(Params _data);
FuncStatus Func3t(Params _data);
FuncStatus Func4t(Params _data);

FuncStatus Func1(Params _data);
FuncStatus Func2(Params _data);
FuncStatus Func3(Params _data);
FuncStatus Func4(Params _data);

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	printf("/*-----------------------------------------------------------*/\n");	
	
	TestOrder();

	TestTimes();
		
	printf("/*-----------------------------------------------------------*/\n");	
    
    return 0; 	
}

/*------------------------------------------------------------------------------------*/

void TestOrder()
{
	int resultInd = PASS;
	char funcsOrder[MAX_RUNS] = "";
	Sched* sched = SchedCreate();

	printf("CHECK ORDER:\n");
	printf("/*-----------------------------------------------------------*/\n");		

	SchedAdd(sched, Func1, funcsOrder , FUNC1_INTRVAL );

	SchedAdd(sched, Func2, funcsOrder , FUNC2_INTRVAL );

	SchedAdd(sched, Func3, funcsOrder , FUNC3_INTRVAL );

	SchedAdd(sched, Func4, funcsOrder , FUNC4_INTRVAL );

	SchedRun(sched);

							
	if( strcmp(funcsOrder , "111112111311121412") != 0) {
		resultInd = FAIL; 
	}

	printf("TEST : %s\n", resultArr[resultInd]);
	printf("/*-----------------------------------------------------------*/\n");	

	/*cleen up*/
	SchedDestroy(sched);
}

/*------------------------------------------------------------------------------------*/

void TestTimes()
{
	Sched* sched = SchedCreate();

	printf("CHECK TIMES:\n");
	printf("/*-----------------------------------------------------------*/\n");		

	/*set test start time*/
	runStartTimeGlob = time(NULL);
	
	SchedAdd(sched, Func1t, NULL , FUNC1_INTRVAL );

	SchedAdd(sched, Func2t, NULL , FUNC2_INTRVAL );

	SchedAdd(sched, Func3t, NULL , FUNC3_INTRVAL );

	SchedAdd(sched, Func4t, NULL , FUNC4_INTRVAL );

	SchedRun(sched);

	printf("TEST : %s\n", resultArr[testResultIndex]);
	printf("/*-----------------------------------------------------------*/\n");		

	/*cleen up*/
	SchedDestroy(sched);
}

/*------------------------------------------------------------------------------------*/

FuncStatus Func1(Params _data)
{	
	FuncStatus statusToRet = FUNC_CONTINUE;
	static int count = 0;
	
	if(++count == NUM_OF_RUNS_1){
		statusToRet = FUNC_DONE;
		printf("func1 Done\n");
	}else{
		printf("func1\n");
	}
	
	strcat( (char*)(_data) , "1");
	
	printf("/*-----------------------------------------------------------*/\n");	

	return statusToRet;
}

FuncStatus Func2(Params _data)
{
	FuncStatus statusToRet = FUNC_CONTINUE;
	static int count = 0;
	
	if(++count == NUM_OF_RUNS_2){
		statusToRet = FUNC_DONE;
		printf("func2 Done\n");
	}else{
		printf("func2\n");
	}
	
	strcat( (char*)(_data) , "2");
	
	printf("/*-----------------------------------------------------------*/\n");	

	return statusToRet;
}

FuncStatus Func3(Params _data)
{
	printf("func3\n");
	printf("/*-----------------------------------------------------------*/\n");	

	strcat( (char*)(_data) , "3");

	return FUNC_DONE;
}

FuncStatus Func4(Params _data)
{
	printf("func4\n");
	printf("/*-----------------------------------------------------------*/\n");	

	strcat( (char*)(_data) , "4");

	return FUNC_ERROR;
}

/*------------------------------------------------------------------------------------*/

FuncStatus Func1t(Params _data)
{	
	FuncStatus statusToRet = FUNC_CONTINUE;
	static int count = 0;
	static time_t lastTime = 0;
	time_t curTime;

	printf("func1\n");
	
	/*first intialization of lastTime*/ 
	if(lastTime == 0){
		lastTime = runStartTimeGlob;
	}

	curTime = time(NULL); /*clock_gettime(NULL);*/
	
	if( difftime(curTime ,lastTime) < FUNC1_INTRVAL / SEC  ){
		testResultIndex = FAIL;
		printf("func1 Error\n");
	}
	
	printf("difftime(lastTime , curTime) = %f\n",difftime(curTime ,lastTime));
	printf("/*-----------------------------------------------------------*/\n");		

	/*printf("%s\n",ctime(&curTime));*/
	
  	lastTime = curTime;
	
	/*stop condition*/
	if(++count == NUM_OF_RUNS_1){
		statusToRet = FUNC_DONE;		
	}	

	return statusToRet;
}

FuncStatus Func2t(Params _data)
{
	FuncStatus statusToRet = FUNC_CONTINUE;
	static int count = 0;
	static time_t lastTime = 0;
	time_t curTime;

	printf("func2\n");
	
	/*first intialization of lastTime*/ 
	if(lastTime == 0){
		lastTime = runStartTimeGlob;
	}

	curTime = time(NULL); /*clock_gettime(NULL);*/
	
	if( difftime(curTime ,lastTime) < FUNC2_INTRVAL / SEC  ){
		testResultIndex = FAIL;
		printf("func2 Error\n");
	}
	
	printf("difftime(lastTime , curTime) = %f\n",difftime(curTime ,lastTime));
	printf("/*-----------------------------------------------------------*/\n");	

  	lastTime = curTime;
	
	/*stop condition*/
	if(++count == NUM_OF_RUNS_2){
		statusToRet = FUNC_DONE;
	}

	return statusToRet;
}

FuncStatus Func3t(Params _data)
{
	static time_t lastTime = 0;
	time_t curTime;

	printf("func3\n");
	
	/*first intialization of lastTime*/ 
	if(lastTime == 0){
		lastTime = runStartTimeGlob;
	}
		
	curTime = time(NULL); /*clock_gettime(NULL);*/
	
	if( difftime(curTime ,lastTime) < FUNC3_INTRVAL / SEC  ){
		testResultIndex = FAIL;
	}
	
	printf("difftime(lastTime , curTime) = %f\n",difftime(curTime ,lastTime));
	printf("/*-----------------------------------------------------------*/\n");	

  	lastTime = curTime;

	return FUNC_DONE;
}

FuncStatus Func4t(Params _data)
{
	static time_t lastTime = 0;
	time_t curTime;

	printf("func4\n");

	/*first intialization of lastTime*/ 
	if(lastTime == 0){
		lastTime = runStartTimeGlob;
	}

	curTime = time(NULL); /*clock_gettime(NULL);*/
	
	if( difftime(curTime ,lastTime) < FUNC4_INTRVAL / SEC  ){
		testResultIndex = FAIL;
		printf("func4 Error\n");
	}
	
	printf("difftime(lastTime , curTime) = %f\n",difftime(curTime ,lastTime));
	printf("/*-----------------------------------------------------------*/\n");	

  	lastTime = curTime;

	return FUNC_ERROR;
}


/*------------------------------------------------------------------------------------*/







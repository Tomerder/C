/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-12
    Last modified date:		2013-08-16
    Description: single threaded scheduler - testing
********************************************************************************/

#include <stdio.h>
#include <string.h>

#include "Sched.h"


/*######################################################################*/
/*					Definitions & global variables						*/
/*######################################################################*/

/* for passing one parameter and one target to task, both integers */
typedef struct ParamsInt
{
	int param;
	int target;
} ParamsInt;



/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							Test Functions								*/
/*######################################################################*/

static int 	SchedTestCreateAddDestroy();
static int 	SchedTestSingle();
static int 	SchedTestMulti();





/*######################################################################*/
/*				Error handling & debug mode functions					*/
/*######################################################################*/

/* error handling in main */
static void 	ErrorPrint	(int _error);
/* error handling in test functions */
static void 	ErrorCheckPrint	(int *_errTemp, int *_errTot, char *_text, int _number);
/* print in debug mode only */
static void 	PrintDebug		(char *_text, int _number);

/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							functions for tasks							*/
/*######################################################################*/

/****** single execution functions ******/
static Status 	VarSet		(void *_params);
static Status 	VarPlusPlus	(void *_target);
static Status 	VarMultiply	(void *_params);


/*----------------------------------------------------------------------*/


/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main()
{
	int error;

	printf("\n\t****** Testing Started ******\n\n");

	error = SchedTestCreateAddDestroy();
	printf("Create, Add & Destroy:\t\t");
	ErrorPrint(error);
	
	error = SchedTestSingle();
	printf("Run with single task:\t\t");
	ErrorPrint(error);
	
	error = SchedTestMulti();
	printf("Run with 3 tasks:\t\t");
	ErrorPrint(error);
	
	
	
	printf("\n\t****** Testing finished ******\n\n");
	
	return 0;
}
/*----------------------------------------------------------------------*/



/*######################################################################*/
/*							TEST FUNCTIONS								*/
/* 			return 0 for success, positive # for failure,				*/
/* 			-1 for error unrelated to tested function(s)				*/
/*######################################################################*/


/*----------------------------------------------------------------------*/
/* create a scheduler, insert 2 tasks & destroy */
/*----------------------------------------------------------------------*/
int SchedTestCreateAddDestroy()
{
	Sched *scheduler = NULL;
	int errorTemp = 0, errorTot = 0;
	ParamsInt params = {0};
	
	/* create */
	scheduler = SchedCreate();
	if(!scheduler)
	{
		PrintDebug("Error: SchedCreate, line ", __LINE__-3);
		return 1;
	}
	
	/* add task 1 */
	errorTemp += SchedAdd(scheduler, VarSet, &params, 1);
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: SchedAdd, 1st task, line ", __LINE__-1);
	/* add task 2 */
	errorTemp += SchedAdd(scheduler, VarPlusPlus, &params, 2);
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: SchedAdd, 2nd task, line ", __LINE__-1);
	
	/* destroy */
	SchedDestroy(scheduler);
	
	return errorTot;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* insert single task and run scheduler */
/*----------------------------------------------------------------------*/
int SchedTestSingle()
{
	Sched *scheduler = NULL;
	int errorTemp = 0, errorTot = 0;
	ParamsInt paramsSet = { 1, 0};	/* param for VarSet */
	
	/* create scheduler & add task */
	
	scheduler = SchedCreate();
	if(!scheduler)
	{
		PrintDebug("Error: SchedCreate failed, line ", __LINE__-3);
		return -1;
	}
	
	errorTemp += SchedAdd(scheduler, VarSet, &paramsSet, 1);
	if( errorTemp )
	{
		PrintDebug("Error: SchedAdd failed, line ", __LINE__-3);
		return -1;
	}
	
	/* run */
	SchedRun( scheduler );
	
	errorTemp += ( paramsSet.target != paramsSet.param );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: SchedRun with one task, line ", __LINE__-3);
	
	return errorTot;
}



/*----------------------------------------------------------------------*/
/* insert three tasks and run scheduler - done several times in different orders */
/*----------------------------------------------------------------------*/
int SchedTestMulti()
{
	Sched *scheduler = NULL;
	
	Func functions[] = { VarSet, VarPlusPlus, VarMultiply };	/* functions for tasks */
	float times[] = {0.05, 0.12, 0.3};		/* times for ordering the tasks */
	ParamsInt params;	/* parameters for tasks */
	
	int errorTemp = 0, errorTot = 0;
	
	/* create scheduler & add tasks */
	
	scheduler = SchedCreate();
	if(!scheduler)
	{
		PrintDebug("Error: SchedCreate failed, line ", __LINE__-3);
		return -1;
	}
	
	params.param  = 2;
	params.target = 0;
	
	errorTemp += SchedAdd(scheduler, functions[0], &params, times[0]);
	errorTemp += SchedAdd(scheduler, functions[1], &params.target, times[1]);
	errorTemp += SchedAdd(scheduler, functions[2], &params, times[2]);
	
	if( errorTemp )
	{
		PrintDebug("Error: SchedAdd failed, line ", __LINE__-3);
		return -1;
	}
	
	/* run */
	SchedRun( scheduler );
	
	errorTemp += ( params.target != (params.param+1)*params.param );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: SchedRun with 3 tasks, line ", __LINE__-3);
	
	
	
	/* add in different orders */
	
	
	
	
	
	
	return errorTot;
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/





/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/





/*----------------------------------------------------------------------*/



/*######################################################################*/
/*				Error handling & debug mode functions					*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* error handling for main */
/*----------------------------------------------------------------------*/
void ErrorPrint(int _error)
{
	if( !_error )
		printf("--Success--\n");
	else if( -1 == _error )
		printf("--Error in test--\n");
	else
		printf("--Failure--\n");
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* error handling for test functions */
/*----------------------------------------------------------------------*/
static void ErrorCheckPrint(int *_errTemp, int *_errTot, char *_text, int _number)
{
	if(*_errTemp)
	{
		PrintDebug(_text, _number);
		*_errTot += *_errTemp;
		*_errTemp = 0;
	}
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* printing in debug mode only */
/*----------------------------------------------------------------------*/
static void PrintDebug(char *_text, int _number)
{
#ifdef DEBUG
	printf("%s", _text);
	if( _number != -1 ) {
		printf("%d", _number);
	}
	printf("\n");
#endif
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							functions for tasks							*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/*					single execution functions							*/
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* input: ParamsInt struct. action: set target to value of param */
Status VarSet(void *_params)
{
	((ParamsInt*)_params)->target = ((ParamsInt*)_params)->param;
	return TASK_DONE;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* input: integer target. action: add 1 to target */
Status VarPlusPlus(void *_target)
{
	++( *(int*)_target );
	return TASK_DONE;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* input: ParamsInt struct. action: multiply target by param */
Status VarMultiply(void *_params)
{
	((ParamsInt*)_params)->target *= ((ParamsInt*)_params)->param;
	return TASK_DONE;
}
/*----------------------------------------------------------------------*/



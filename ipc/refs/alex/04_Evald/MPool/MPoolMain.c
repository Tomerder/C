/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-09    
    Last modified date:		2013-08-09
    Description: 	unit test for MPool functions - memory management functions
***************************************************************************************/

#include <stdio.h>

#include "MPoolFunc.h"


/*######################################################################*/
/*							TEST FUNCTIONS								*/
/*######################################################################*/

extern int MPoolTestInit();
extern int MPoolTestAlloc();
extern int MPoolTestFreeDefrag();


/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/

int main()
{
	int error;
	char *msgS="--Success--", *msgF="--Failure--";

	printf("\n\t****** Testing Started ******\n\n");

	error = MPoolTestInit();
	printf("MPoolInit:\t\t\t\t%s\n", (!error)?msgS:msgF);
	
	error = MPoolTestAlloc();
	printf("MPoolAlloc:\t\t\t\t%s\n", (!error)?msgS:msgF);
	
	error = MPoolTestFreeDefrag();
	printf("MPoolFree, MPoolAlloc with defrag:\t%s\n", (!error)?msgS:msgF);
	
	printf("\n\t****** Testing finished ******\n\n");
	return 0;
}





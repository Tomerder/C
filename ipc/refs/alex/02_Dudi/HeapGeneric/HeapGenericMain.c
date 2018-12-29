/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-29    
    Last modified date:		2013-07-29
    Description: generic heap - unit test
***************************************************************************/

#include <stdio.h>

#include "HeapGeneric.h"
#include "HeapGenericTest.h"


int main()
{
	int error;
	char *msgS="--Success--", *msgF="--Failure--";

	printf("\n\t****** Testing Started ******\n\n");

	error = HeapTestBuildDestroy();
	printf("Build & Destroy:\t\t%s\n", (!error)?msgS:msgF);
	
	error = HeapTestMaxSort();
	printf("ExtractMax, HeapSort:\t\t%s\n", (!error)?msgS:msgF);
	
	error = HeapTestInsertExtractNum();
	printf("Insert, ExtractMax, NumItems:\t%s\n", (!error)?msgS:msgF);
	
	error = HeapTestForEach();
	printf("HeapForEach:\t\t\t%s\n", (!error)?msgS:msgF);
	
	printf("\n\t****** Testing finished ******\n\n");
	return 0;
}


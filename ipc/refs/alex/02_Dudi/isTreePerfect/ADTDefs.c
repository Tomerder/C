/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-16    
    Last modified date:		2013-06-21
    Description: fuctions for handling errors
***************************************************************************************/

#include <stdio.h>
#include "ADTDefs.h"

/**  list of error messeges	**/
void HandleErr(ADTErr errNum, char *msg)
{
	if(ERR_GENERAL==errNum) {
		sprintf(msg, "Error: general error");

	} else if(ERR_NOT_INITIALIZED==errNum) {
		sprintf(msg, "Error: not initialized");

	} else if(ERR_ALLOCATION_FAILED==errNum) {
		sprintf(msg, "Error: allocation failed");

	} else if(ERR_REALLOCATION_FAILED==errNum) {
		sprintf(msg, "Error: reallocation failed");

	} else if(ERR_UNDERFLOW==errNum) {
		sprintf(msg, "Error: attempted underflow");

	} else if(ERR_OVERFLOW==errNum) {
		sprintf(msg, "Error: attempted overflow");

	} else if(ERR_WRONG_INDEX==errNum) {
		sprintf(msg, "Error: index does not exist");
	}
}

/**  print error messeges with file & line info		**/
void ErrorPrint(ADTErr _errNum, char* _thisFile, int _lineNum)
{
	char errMsg[40];
	
	HandleErr(_errNum, errMsg);
	printf("\n%s\tin file %s, line %d\n", errMsg, _thisFile, _lineNum);
}

/**  print "Failure" if received wrong error, "Succes" for right error **/
void ErrorCheck(ADTErr _actual, ADTErr _expected, char* _thisFile, int _lineNum)
{
	if( _actual == _expected ) {
		printf("Success\n");
	} else {
		if( ERR_OK == _expected ) {
			fprintf(stdout, "Failure: error received in file %s, line %d\n", _thisFile, _lineNum);
		} else {
			fprintf(stdout, "Failure: no error, in file %s, line %d\n", _thisFile, _lineNum);
		}
	}
}





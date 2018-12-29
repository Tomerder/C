/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-24
    Date last modified :	2013-07-24
    Description : Hash set - for integers only - unit test
***************************************************************************************/

#include <stdio.h>
#include <time.h>

#include "ADTDefs.h"
#include "HashSet.h"
#include "HashSetTest.h"




int main()
{
	int error;
	char *msgS="Success", *msgF="Failure";

	printf("\n\t****** Testing Started ******\n\n\n");
	
	/****** Create & Destroy ******/
	
	error = HashTestCreateDestroy();
	printf("Create & Destroy:\t%s\n", (!error)?msgS:msgF);
	
	/****** insert, find, remove ******/
	
	error = HashTestInsert();
	printf("Insert:\t\t\t%s\n", (!error)?msgS:msgF);
	
	error = HashTestIsFound();
	printf("IsFound:\t\t%s\n", (!error)?msgS:msgF);
	
	error = HashTestRemove();
	printf("Remove:\t\t\t%s\n", (!error)?msgS:msgF);
	

	/****** is effective, numItems ******/
	error = HashTestNumEffect();
	printf("NumItems, IsEffective:\t%s\n", (!error)?msgS:msgF);
	
	
	printf("\n\t****** Testing finished ******\n\n");
	return 0;
}

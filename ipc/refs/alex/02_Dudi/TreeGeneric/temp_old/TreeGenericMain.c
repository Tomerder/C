/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-25    
    Last modified date:		2013-07-27
    Description: generic binary tree.
 
***************************************************************************/

#include <stdio.h>

#include "TreeGeneric.h"
#include "TreeGenericTest.h"



int main()
{
	int error;
	char *msgS="--Success--", *msgF="--Failure--";

	printf("\n\t****** Testing Started ******\n\n");

	/****** Create & Destroy ******/
	
	error = TreeTestCreateDestroy();
	printf("Create & Destroy:\t\t\t\t%s\n", (!error)?msgS:msgF);
	

	/****** Insert, GetData, Iterators ******/
	
	error = TreeTestInsertGetDestroy();
	printf("Insert, GetData, Destroy non-empty tree:\t%s\n", (!error)?msgS:msgF);
	
	error = TreeTestIterators();
	printf("TreeBegin, -End, -Prev, -Next:\t\t\t%s\n", (!error)?msgS:msgF);
	
	
	/****** Find, Count, Remove, ForEach ******/
	
	error = TreeTestFindCountRemove();
	printf("Find, Count, Remove:\t\t\t\t%s\n", (!error)?msgS:msgF);
	
	error = TreeTestForEach();
	printf("TreeForEach:\t\t\t\t\t%s\n", (!error)?msgS:msgF);
	
	
	
	printf("\n\t****** Testing finished ******\n\n");
	return 0;
}



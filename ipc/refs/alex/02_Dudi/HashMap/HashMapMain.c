/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-31
    Date last modified :	2013-07-31
    Description : Hash Map - unit test
***************************************************************************************/

#include <stdio.h>

#include "HashMap.h"


/*	test functions	*/

extern int HashTestCreateDestroy();
extern int HashTestInsertRemoveFind1();
extern int HashTestInsertRemoveFind2();
extern int HashTestCountForEach();


/*	 main	*/

int main()
{
	int error;
	char *msgS="--Success--", *msgF="--Failure--";

	printf("\n\t****** Testing Started ******\n\n");

	error = HashTestCreateDestroy();
	printf("Create & Destroy:\t\t\t%s\n", (!error)?msgS:msgF);

	error = HashTestInsertRemoveFind1();
	printf("insert, remove, find - no collisions:\t%s\n", (!error)?msgS:msgF);

	error = HashTestInsertRemoveFind2();
	printf("insert, remove, find - with collisions:\t%s\n", (!error)?msgS:msgF);

	error = HashTestCountForEach();
	printf("CountItems, ForEach:\t\t\t%s\n", (!error)?msgS:msgF);

	printf("\n\t****** Testing finished ******\n\n");
	return 0;
}



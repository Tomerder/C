/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-21    
    Last modified date:		2013-07-22
    Description: generic (double) linked list - unit test
********************************************************************************/

#include <stdio.h>

#include "ADTDefs.h"
#include "LListGenFunc.h"
#include "LListGenTest.h"



int main()
{
	int error;
	size_t listSize = LIST_SIZE;
	char *msgS="Success", *msgF="Failure";

	printf("\n\t****** Testing Started ******\n\n\n");

	/****** NULL pointers - optional ******/

#ifdef CHECK_NULL
	VectorTestNull();
#endif

	/****** Create & Destroy ******/
	
	error = LlistTestCreateDestroy();
	printf("Create & Destroy:\t%s\n", (!error)?msgS:msgF);
	
	/****** Push Head & Tail ******/
	
	error = LlistTestPushHead(listSize);
	printf("Push Head:\t\t%s\n", (!error)?msgS:msgF);
	error = LlistTestPushTail(listSize);
	printf("Push Tail:\t\t%s\n", (!error)?msgS:msgF);
	
	/****** Pop Head & Tail ******/
	
	error = LlistTestPopHead(listSize);
	printf("Pop Head:\t\t%s\n", (!error)?msgS:msgF);	
	error = LlistTestPopTail(listSize);
	printf("Pop Tail:\t\t%s\n", (!error)?msgS:msgF);	

#ifdef PRINTALL
	printf("\n\n");
#endif
	/****** countItems & isEmpty ******/
	
	error = ListTestCountItems(listSize);
	printf("Count Items:\t\t%s\n", (!error)?msgS:msgF);	
	error = ListTestIsEmpty(listSize);
	printf("Is Empty:\t\t%s\n\n", (!error)?msgS:msgF);	
		
	
	/****** iterators ******/
	
	error = ListItrTestBeginEnd(listSize);
	printf("ListBegin, ListEnd:\t%s\n", (!error)?msgS:msgF);	
	error = ListItrTestGet(listSize);
	printf("ListGetData:\t\t%s\n", (!error)?msgS:msgF);	
	
	error = ListItrTestNext(listSize);
	printf("ListNext:\t\t%s\n", (!error)?msgS:msgF);	
	error = ListItrTestPrev(listSize);
	printf("ListPrev:\t\t%s\n", (!error)?msgS:msgF);	
	
	error = ListItrTestInsert(listSize);
	printf("ListInsert:\t\t%s\n", (!error)?msgS:msgF);	
	error = ListItrTestRemove(listSize);
	printf("ListRemove:\t\t%s\n", (!error)?msgS:msgF);	
	
	error = ListTestFindFirst(listSize);
	printf("ListFindFirst:\t\t%s\n", (!error)?msgS:msgF);	
	error = ListTestForEach(listSize);
	printf("ListForEach:\t\t%s\n", (!error)?msgS:msgF);	
	
	
	
	printf("\n\t****** Testing finished ******\n\n");
	return 0;
}




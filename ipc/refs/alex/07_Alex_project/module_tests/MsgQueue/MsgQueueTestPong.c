/*******************************************************************************
	Author:				Stav Ofer
	Creation date:		2013-09-19
	Date last modified:	2013-09-19
	Description:		some tests for MsgQueue module - pong
	
						note: compile: gc -I ../../ -pthread MsgQueueTestPong.c ../../*.c -o pong
********************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "SharedMem.h"
#include "MsgQueue.h"


#define HANDLE_ERR(X,R)				fprintf(stderr, "%s:%d Error: %s\n", __FILE__, __LINE__, X); return(R)

#define PAGESIZE	getpagesize()
#define MAXSIZE		32
#define QSIZE		3

#define NMSG		6
#define SLEEP		1


static char* filename = "MsgQueueTestPing.c";

/*######################################################################*/

int main()
{
	MQ_t* myMQ = NULL;
	char inMsg[MAXSIZE];
	int length;
	int i;
	
	myMQ = MsgQueueConnect( filename );
	if(!myMQ) {
		HANDLE_ERR("MsgQueueConnect", -1);
	}
	printf("Pong: connected to MQ\n");
	
	for( i=0; i<NMSG; ++i)
	{
		length = MsgQueueRecv(myMQ, inMsg, MAXSIZE);
		inMsg[length] = '\0';
		printf("Pong: received %s\n", inMsg);
		sleep(SLEEP);
	}
	
	MsgQueueDestroy(myMQ);
	printf("Pong: MQ destroyed\n");
	
	
	return 0;
}

/*######################################################################*/







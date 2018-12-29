/*******************************************************************************
	Author:				Stav Ofer
	Creation date:		2013-09-19
	Date last modified:	2013-09-19
	Description:		some tests for MsgQueue module - ping
						
						note: compile: gc -I ../../ -pthread MsgQueueTestPing.c ../../*.c -o ping
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
	char outMsg[MAXSIZE];
	int i;
	
	myMQ = MsgQueueCreate( filename, QSIZE, PAGESIZE );
	if(!myMQ) {
		HANDLE_ERR("MsgQueueCreate", -1);
	}
	printf("Ping: MQ created\n");
	sleep(SLEEP);
	
	
	for( i=0; i<NMSG; ++i)
	{
		sprintf(outMsg, "Message #%d", i+1);
		MsgQueueSend(myMQ, outMsg, strlen(outMsg));
		printf("Ping: sent: %s\n", outMsg);
/*		sleep(SLEEP);*/
	}
	
	
	MsgQueueDestroy(myMQ);
	printf("Ping: MQ destroyed\n");
	
	
	return 0;
}

/*######################################################################*/







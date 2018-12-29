/*******************************************************************************
    Author:					Stav Ofer
    Creation date:  		2013-09-20    
    Last modified date:		2013-10-01
    Description: 	Test for Message Queue over Shared Memory - pong.
    				Connect to a MsgQueue for regular messages and another one for
    				handshake messages, send handshake, start reading msgs until
    				received byebye msg, then resend it back & disconnect.
********************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>

#include "MsgQueue.h"
#include "PingPong.h"


/*######################################################################*/
/*				definitions & foreward declarations						*/
/*######################################################################*/

#define HANDLE_ERR(X)		fprintf(stderr, "%s:%d Error: %s\n", __FILE__, __LINE__, X); return -1

#define VERBOSE_INT(X,Y,Z)	if(options.m_verbose) { printf("%s %d %s", X,Y,Z); }

/* handshake & byebye messages */
static char* handshake = HANDSHAKE_MSG;
static char* byeMsg = BYE_MSG;


/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char *argv[])
{
	Options_t options;				/* runtime options		*/
	MQ_t* myMQ = NULL;				/* message queue		*/
	MQ_t* handshakeMQ = NULL;		/* handshake msgQ		*/
	char inMsg[MAX_MSG_SIZE+1];		/* reading buffer		*/
	int nReceived = 0;				/* # of msgs received	*/
	int length;						/* length of message	*/
	int pid = getpid();				/* process id			*/
	
	/* initialize & handle runtime options */
	OptHandler(argc, argv, &options);
	
	/* connect to handshake queue */
	handshakeMQ = MsgQueueConnect(options.m_fileHandshake);
	if( !handshakeMQ ) {
		HANDLE_ERR("MsgQueueConnect");
	}
	VERBOSE_INT("Pong", pid, ": connected to handshake queue\n");
	
	/* send handshake */
	length = strlen(handshake);
	length = MsgQueueSend( handshakeMQ, handshake, length );
	if( 0 == length ) {
		HANDLE_ERR("MsgQueueSend");
	}
	printf("Pong %d: sent handshake\n", pid);
	
	sleep(2);
	
	/* connect to message queue */
	myMQ = MsgQueueConnect(options.m_filename);
	if( !myMQ ) {
		HANDLE_ERR("MsgQueueConnect");
	}
	VERBOSE_INT("Pong", pid, ": connected to message queue\n");
	
	/****** READ MESSAGES ******/
	do {
		length = MsgQueueRecv( myMQ, inMsg, MAX_MSG_SIZE);
		if( 0 == length ) {
			HANDLE_ERR("MsgQueueRecv");
		}
		++nReceived;
		
		inMsg[length] = '\0';
		printf("Pong %d: received msg #%d: %s\n", pid, nReceived, inMsg);
		
		sleep(options.m_sleep);
		
	} while ( 0 != strcmp(inMsg, byeMsg) );
	
	printf("Pong %d: received byebye, exiting\n", pid);
	
	/* resend bye msg */
	length = strlen(byeMsg);
	length = MsgQueueSend( myMQ, byeMsg, length );
	if( 0 == length ) {
		HANDLE_ERR("MsgQueueSend");
	}
	VERBOSE_INT("Pong", pid, ": re-sent byebye msg\n");
	
	/* also to handshake queue */
	length = strlen(byeMsg);
	length = MsgQueueSend( handshakeMQ, byeMsg, length );
	if( 0 == length ) {
		HANDLE_ERR("MsgQueueSend");
	}
	VERBOSE_INT("Pong", pid, ": sent byebye msg to Handshake queue\n");
	
	/* disconnect from msg queues */
	MsgQueueDestroy(myMQ);
	MsgQueueDestroy(handshakeMQ);
	VERBOSE_INT("Pong", pid, ": disconnected from queues\n");
	
	return 0;
}
/*######################################################################*/



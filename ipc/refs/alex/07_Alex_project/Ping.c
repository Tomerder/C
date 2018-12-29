/*******************************************************************************
    Author:					Stav Ofer
    Creation date:  		2013-09-20    
    Last modified date:		2013-10-01
    Description: 	Test for Message Queue over Shared Memory - ping.
    				Create MsgQueue for regular messages and another one for
    				handshake messages. Start sending messages when 1st pong sends
    				handshake, send a set # of msgs increasing with each pong.
    				when finished, send byebye msg & destroy queues.
********************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "SharedMem.h"
#include "Sync.h"
#include "MsgQueue.h"
#include "PingPong.h"


/*######################################################################*/
/*				definitions & foreward declarations						*/
/*######################################################################*/


#define HANDLE_ERR(X)		fprintf(stderr, "%s:%d Error: %s\n", __FILE__, __LINE__, X); return -1

#define VERBOSE(X)			if(options.m_verbose) { printf("%s", X); }
#define VERBOSE_INT(X,Y,Z)	if(options.m_verbose) { printf("%s %d %s", X,Y,Z); }

#define PAGESIZE			getpagesize()

/* shared data between ping processes: handshake-reader and message-sender */
typedef struct PingSharedData
{
	int		m_nPongs;	/* # of pongs		*/
	int		m_nToSend;	/* total # of msgs to send (updated when more pongs enter) */
	int		m_nSent;	/* # of msgs sent	*/
	
} PingSharedData_t;


/* ping shared memory */
static char* fileShmem = "SharedMem.c";

/* message for test */
static char* defMsg = "This message is 32 characters AB";
/* byebye message */
static char* byeMsg = BYE_MSG;


/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char *argv[])
{
	Options_t options;				/* runtime options	*/
	MQ_t* myMQ = NULL;				/* message queue	*/
	MQ_t* handshakeMQ = NULL;		/* handshake msgQ	*/
	char outMsg[MAX_MSG_SIZE+1];	/* outgoing buffer 	*/
	int pid;						/* process id (of handshake-reader) */
	int length;						/* sent msg length */
	
	Shmem_t* pingShmem = NULL;			/* shared memory for ping parent & child */
	void* pingShmAddr = NULL;			/* address of shared mem */
	PingSharedData_t* pingData = NULL;	/* data in shared mem */
	Sync_t*	pingSync = NULL;			/* synchronize handshake-reading process & msg sending process */
	SyncTool_t* pingMutex = NULL;		/* " */
	int mutexInitVal = 0;				/* mutex starts as locked */
	
	
	/* initialize & handle runtime options */
	OptHandler(argc, argv, &options);

	/* get shared mem for ping processes */
	pingShmem = ShmemGet(fileShmem, PAGESIZE, 1);
	pingShmAddr = ShmemConnect(pingShmem);
	
	/* initialize */
	pingData = (PingSharedData_t*)pingShmAddr;
	pingData->m_nPongs = 0;
	pingData->m_nToSend = 0;
	pingData->m_nSent = 0;
	
	/* create ping mutex, initialized to 0 */
	pingSync = SyncCreate( (char*)pingShmAddr + sizeof(PingSharedData_t) , 1);
	if( !pingSync ) {
		HANDLE_ERR("SyncCreate");
	}
	if( -1 == SyncToolInit( pingSync, 0, MUTEX, mutexInitVal) )
	{
		HANDLE_ERR("SyncToolInit");
	}
	
	pingMutex = SyncAttach(pingSync, 0);
	VERBOSE("Ping: initialized & attached mutex\n");
	
	
/* FORK */
	pid = fork();
	
	if(pid < 0)	{	/* fork failed */
		HANDLE_ERR("fork");
	}
	
/*** CHILD - handshakes ***/
	else if( 0 == pid )
	{	
		char inHandShake[MAX_MSG_SIZE];
		
		/* create handshake queue */
		handshakeMQ = MsgQueueCreate( options.m_fileHandshake, options.m_queueSize, PAGESIZE);
		if( !handshakeMQ ) {
			HANDLE_ERR("MsgQueueCreate");
		}
		
		VERBOSE("Ping child: created handshake queue\n");
		sleep(1);
		
		/* read 1st handshake */
		length = MsgQueueRecv( handshakeMQ, inHandShake, MAX_MSG_SIZE);
		if( 0 == length ) {
			HANDLE_ERR("MsgQueueRecv (handshake)");
		}
		
		pingData->m_nToSend += options.m_nMessages;
		++pingData->m_nPongs;

		/* unlock mutex */
		if( -1 == SyncMutexUnlock(pingMutex) ) {
			HANDLE_ERR("SyncMutexUnlock");
		}
		VERBOSE("Ping child: read 1st handshake & unlocked mutex\n");
		
		/* continue while parent still active */
		while( pingData->m_nSent < pingData->m_nToSend )
		{	
			length = MsgQueueRecv( handshakeMQ, inHandShake, MAX_MSG_SIZE);
			if( 0 == length ) {
				HANDLE_ERR("MsgQueueRecv (handshake)");
			}
		
			if( strncmp(inHandShake, byeMsg, length) )
			{
				pingData->m_nToSend += options.m_nMessages;
				++pingData->m_nPongs;
				
				VERBOSE_INT("Ping child: read handshake #", pingData->m_nPongs, "\n");
				continue;
			}
			/* else */
			break;
			
		} /* end while */
		
		MsgQueueDestroy( handshakeMQ );
		VERBOSE("Ping child: destroyed handshake queue\n");
		
		return 0;
	}	/*** END CHILD ***/
	
/*** PARENT - actual messages ***/
	else /*if(pid > 0)*/
	{
		int sent;
		
		/* create message queue */
		myMQ = MsgQueueCreate( options.m_filename, options.m_queueSize, PAGESIZE);
		if( !myMQ ) {
			HANDLE_ERR("MsgQueueCreate");
		}
		VERBOSE("Ping parent: created message queue\n");
		
		/* wait for 1st pong to arrive */
		VERBOSE("Ping parent: waiting for 1st pong\n");
		if( -1 == SyncMutexLock(pingMutex) ) {
			HANDLE_ERR("SyncMutexLock");
		}		
		VERBOSE("Ping parent: notified of 1st pong\n");
		
		srand(time(0));
		
		/****** SEND MESSAGES ******/
		
		while( pingData->m_nSent < pingData->m_nToSend )
		{
			length = rand()%MAX_MSG_SIZE + 1;	/* random length */
			memcpy( outMsg, defMsg, length );
			
			sent = MsgQueueSend( myMQ, outMsg, length );
			if( 0 == sent ) {
				HANDLE_ERR("MsgQueueSend");
			}
			++pingData->m_nSent;
			
			outMsg[sent] = '\0';
			printf("Ping: sent msg # %d of %d: %s\n", pingData->m_nSent, pingData->m_nToSend, outMsg);
			
			sleep(options.m_sleep);
		}	/* end while */
		
		/* send bye-bye message */
		length = strlen(byeMsg);
		sent = MsgQueueSend( myMQ, byeMsg, length );
		if( 0 == sent ) {
			HANDLE_ERR("MsgQueueSend");
		}
		
		++pingData->m_nSent;
		printf("Ping: sent byebye msg: %s\n", byeMsg);
		
		/* destroy mutex & shared mem */
		SyncDestroy(pingSync);
		ShmemDestroy(pingShmem);
		VERBOSE("Ping parent: destroyed mutex & shared mem\n");
		
		/* destroy myMQ */
		MsgQueueDestroy( myMQ );
		VERBOSE("Ping parent: destroyed message queue\n");
		
	}	/*** END PARENT ***/
	
	/* wait for all processes to finish */
	wait(NULL);
	
	return 0;
}
/*######################################################################*/


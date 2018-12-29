/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-22    
    Last modified date:		2013-08-22
    Description: 	interprocess communication excercise - messege queue
    				multiple processes from 2 files - ping (sending) and pong (receiving)
    				
    				Each ping enters an initial message into a dedicated channel ("registrar")
    				and deletes a message there on exit.
    				When a pong checks messages, if the messege channel is empty it checks
    				the registrar channel. If not empty, sends back in the same message.
    				If empty, exits.
    				(Note: possibility of race conditions).
    				
    				runtime options:
    					-s: sleep in msec
    					-f: filename
    					-n: # of messeges to send
    					-c: create MessegeQueue
    					-d: destroy MessegeQueue
    					-v:	verbose mode

****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "PingPong.h"


/****************************************************************************/
/*				definitions & foreward declarations							*/
/****************************************************************************/

#define MSG_FLAGS_OUT	0
#define MSG_FLAGS_IN	0


/****************************************************************************/
/*									MAIN									*/
/****************************************************************************/
int main(int argc, char *argv[])
{
	Options options;		/* runtime options 		*/
	Messege messegeOut;		/* outgoing messege 	*/
	Messege messegeIn;		/* incoming messege 	*/
	SendRecvArg sendArgs; 	/* arguments for send	*/
	SendRecvArg recvArgs;	/* arguments for receive */
	SendRecvArg regArgs;	/* arguments for registering */
	int qid;				/* MsgQue id 			*/
	int pid = getpid();		/* process id			*/

	/* initialize & handle runtime options */
	OptHandler(argc, argv, &options);

	/* create/open messege queue */
	if( -1 == (qid = MsgQueCreate(&options)) )
	{
		perror("Ping: MsgQueCreate");
		return errno;
	}
	
	/* initialize msg send, receive, eof agruments */
	
	sendArgs.m_qid 		= qid;
	sendArgs.m_flags 	= MSG_FLAGS_OUT;
	sendArgs.m_counter 	= 0;
	sendArgs.m_from 	= CH_PING;
	sendArgs.m_pid		= pid;
	
	recvArgs.m_qid 		= qid;
	recvArgs.m_length 	= MAX_MSG_LEN;
	recvArgs.m_flags 	= MSG_FLAGS_IN;
	recvArgs.m_counter 	= 0;
	recvArgs.m_from 	= CH_PONG;
	recvArgs.m_pid		= pid;
	
	regArgs.m_qid 		= qid;
	regArgs.m_flags 	= MSG_FLAGS_OUT;
	regArgs.m_from 		= CH_CHCK_IN;
	regArgs.m_pid		= pid;
	
	
	/* check in at registrar channel */
	printf("Ping %d: ", pid);
	if( -1 == MsgSend(&regArgs, &messegeOut, TRUE) )
	{
		fprintf(stderr, "error: MsgSnd %d\n", pid);
		return 1;
	}
	PrintV(&options, "Ping: checking in ", pid);
	
	
	/* send m_nMesseges # of messeges, after each messege wait for reply */
	while ( sendArgs.m_counter < options.m_nMesseges )
	{
		/* create & send messege */
		printf("Ping %d: ", pid);
		
		if( -1 == MsgSend(&sendArgs, &messegeOut, FALSE) )
		{
			fprintf(stderr, "error: MsgSnd %d\n", pid);
			return 1;
		}
		
		/* get response */
		printf("Ping %d: ", pid);
		if( -1 == MsgRecv(&recvArgs, &messegeIn) )
		{
			fprintf(stderr, "error: MsgRecv %d\n", pid);
			return 1;
		}
		
		/* sleep */	
		if(-1 == usleep(options.m_usleep) )
		{
			perror("usleep");
			return errno;
		}
	} /* end while */
	
	/* check out */
	recvArgs.m_from = CH_CHCK_IN;
	printf("Ping %d: ", pid);
	if( -1 == MsgRecv(&recvArgs, &messegeIn) )
	{
		fprintf(stderr, "error: MsgRecv %d\n", pid);
		return 1;
	}
	
	PrintV(&options, "Ping: checking out. # of messeges sent: ", sendArgs.m_counter);
	
	/* destroy messege queue */
	if(options.m_destroy)
	{
		if( -1 == msgctl(qid, IPC_RMID, NULL) )
		{
			perror("Ping: msgctl");
			return errno;
		}
		PrintV(&options, "Ping: Queue Destroyed", -1);
	}
	
	return errno;
}
/****************************************************************************/




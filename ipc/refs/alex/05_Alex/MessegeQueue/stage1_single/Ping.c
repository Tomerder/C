/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-21    
    Last modified date:		2013-08-21
    Description: 	interprocess communication excercise - messege queue
    				2 files - ping (sending) and pong (receiving)
    				
    				runtime options:
    					-s: sleep in msec
    					-f: filename
    					-n: # of messeges to send
    					-c: create MessegeQueue
    					-d: destroy MessegeQueue
    					-v:	verbose mode
    					-e: EOF sent as messege type (channel)

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
	SendRecvArg eofArgs;	/* arguments for eof	*/
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
	
	recvArgs.m_qid 		= qid;
	recvArgs.m_length 	= MAX_MSG_LEN;
	recvArgs.m_flags 	= MSG_FLAGS_IN;
	recvArgs.m_counter 	= 0;
	recvArgs.m_from 	= CH_PONG;
	
	eofArgs.m_qid 		= qid;
	eofArgs.m_flags 	= MSG_FLAGS_OUT;
	eofArgs.m_counter 	= 0;
	eofArgs.m_from		= (options.m_typeEOF) ? CH_EOF : CH_PING;
	
	/* send m_nMesseges # of messeges, after each messege wait for reply */
	while ( sendArgs.m_counter < options.m_nMesseges )
	{
		/* create & send messege */
		printf("Ping %d: ", pid);
		
		if( -1 == MsgSend(&sendArgs, &messegeOut, FALSE) )
		{
			fprintf(stderr, "error: MsgSnd\n");
			return 1;
		}
		
		/* get response */
		printf("Ping %d: ", pid);
		if( -1 == MsgRecv(&recvArgs, &messegeIn) )
		{
			fprintf(stderr, "error: MsgRecv\n");
			return 1;
		}
		
		/* sleep */	
		if(-1 == usleep(options.m_usleep) )
		{
			perror("usleep");
			return errno;
		}
	} /* end while */
	
	/* create & send EOF messege */
	printf("Ping %d: EOF messege: ", pid);
	if( -1 == MsgSend(&eofArgs, &messegeOut, TRUE) )
	{
		fprintf(stderr, "error: MsgSnd\n");
		return 1;
	}
	
	/* get response */
	printf("Ping %d: ", pid);
	if( -1 == MsgRecv(&recvArgs, &messegeIn) )
	{
		fprintf(stderr, "error: MsgRecv\n");
		return 1;
	}
	
	PrintV(&options, "Ping: # of messeges sent: ", sendArgs.m_counter + eofArgs.m_counter);
	
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




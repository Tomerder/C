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
#define MSG_FLAGS_IN	IPC_NOWAIT


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
	
	/* open messege queue */
	if( -1 == (qid = MsgQueCreate(&options)) )
	{
		perror("Pong: MsgQueCreate");
		return errno;
	}
	
	
	/* initialize msg send, receive agruments */
	
	sendArgs.m_qid 		= qid;
	sendArgs.m_flags 	= MSG_FLAGS_OUT;
	sendArgs.m_counter 	= 0;
	sendArgs.m_from 	= CH_PONG;
	sendArgs.m_pid		= pid;
	
	recvArgs.m_qid 		= qid;
	recvArgs.m_length 	= MAX_MSG_LEN;
	recvArgs.m_flags 	= MSG_FLAGS_IN;
	recvArgs.m_counter 	= 0;
	recvArgs.m_from 	= (-1)*CH_CHCK_IN;
	recvArgs.m_pid		= pid;
	
	regArgs.m_qid 		= qid;
	regArgs.m_flags 	= MSG_FLAGS_OUT;
	regArgs.m_from 		= CH_CHCK_IN;
	regArgs.m_pid		= 0;
	
	/* receive messeges and send replies */
	printf("Pong %d: ", pid);
	
	while( -1 != MsgRecv(&recvArgs, &messegeIn) )
	{
		/* if received messege from registrar (meaning no regular messeges but pings still exist) */
		if( CheckPings(&messegeIn) )
		{
			PrintV(&options, "Pong: received registrar messege, sending back in", -1);
			--recvArgs.m_counter;
			
			
			/* send messege back in */
			if( -1 == MsgSend(&regArgs, &messegeOut, TRUE) )
			{
				fprintf(stderr, "error: MsgSnd %d\n", pid);
				return 1;
			}
			
			/* sleep */
			if(-1 == usleep(options.m_usleep) )
			{
				perror("usleep");
				return errno;
			}

		}
		else
		{
			/* sleep */
			if(-1 == usleep(options.m_usleep) )
			{
				perror("usleep");
				return errno;
			}

			/* create & send reply */
			printf("Pong %d: ", pid);
			if( -1 == MsgSend(&sendArgs, &messegeOut, FALSE) )
			{
				fprintf(stderr, "error: MsgSnd %d\n", pid);
				return 1;
			}
		}
		printf("Pong %d: ", pid);
	} /* end while */
	
	/* if received -1 b/c no messeges in either channel */
	if( errno == ENOMSG )
	{
		PrintV(&options, "Pong: # of messeges received: ", recvArgs.m_counter);
		printf("exit %d\n", pid);
		return 0;
	}
	
	/* else: error */
	perror("Pong: msgrcv");
	return errno;
}
/****************************************************************************/




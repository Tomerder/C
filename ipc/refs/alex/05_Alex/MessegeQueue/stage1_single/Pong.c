/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-21    
    Last modified date:		2013-08-21
    Description: 	interprocess communication excercise - messege queue
    				2 files - ping (sending) and pong (receiving)
    				
    				runtime options:
    					-s: sleep in msec
    					-f: filename
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
	
	recvArgs.m_qid 		= qid;
	recvArgs.m_length 	= MAX_MSG_LEN;
	recvArgs.m_flags 	= MSG_FLAGS_IN;
	recvArgs.m_counter 	= 0;
	recvArgs.m_from 	= (options.m_typeEOF) ? (-1)*CH_EOF : CH_PING;
	
	/* receive messeges and send replies */
	printf("Pong %d: ", pid);
	
	while( -1 != MsgRecv(&recvArgs, &messegeIn) )
	{
		/* if received EOF */
		if( CheckEOF(&options, &messegeIn) )
		{
			PrintV(&options, "Pong: received EOF", -1);
			
			/* create & send reply */
			printf("Pong: ");
			if( -1 == MsgSend(&sendArgs, &messegeOut, TRUE) )
			{
				fprintf(stderr, "error: MsgSnd\n");
				return 1;
			}
		
			printf("Pong: sent final messege: \"%s\"\n", messegeOut.m_msg);
			PrintV(&options, "Pong: # of messeges received: ", recvArgs.m_counter);
			
			return errno;
		}
		
		/* else */
		
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
			fprintf(stderr, "error: MsgSnd\n");
			return 1;
		}
		
		printf("Pong %d: ", pid);
	} /* end while */
	
	/* can reach this point only if error */
	perror("Pong: msgrcv");
	return errno;
}
/****************************************************************************/




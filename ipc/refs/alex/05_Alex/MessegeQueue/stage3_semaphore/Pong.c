/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-25    
    Last modified date:		2013-08-28
    Description: 	interprocess communication excercise - message queue - functions
    				multiple processes from 2 files - ping (sending) and pong (receiving)
    				controlled by semaphores.
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include "PingPong.h"



/*######################################################################*/
/*				definitions & foreward declarations						*/
/*######################################################################*/

#define NSEMS			2
#define SEMFLAGS		0
#define MSG_FLAGS_OUT	0
#define MSG_FLAGS_IN	IPC_NOWAIT

/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char *argv[])
{
	Options options;		/* runtime options	*/
	
	Messege messegeOut;		/* outgoing message 	*/
	Messege messegeIn;		/* incoming message 	*/
	SendRecvArg sendArgs; 	/* arguments for send	*/
	SendRecvArg recvArgs;	/* arguments for receive */
	struct sembuf sopsPing[2], sopsPong;
	
	int semid, mqid;		/* semaphore & msgQue id	*/
	int pid = getpid();		/* process id			*/
	int semCheck;

	/* initialize & handle runtime options */
	OptHandler(argc, argv, &options);

	/* create/open messege queue */
	if( -1 == (mqid = MsgQueCreate(&options)) )
	{
		fprintf(stderr, "%s:%d Error: MsgQueCreate\n", __FILE__, __LINE__-2);
		return errno;
	}
	
	/* create/connect to semaphore */
	if( -1 == (semid = SemConnect(NSEMS, SEMFLAGS)) )
	{
		fprintf(stderr, "%s:%d Error: SemConnect\n", __FILE__, __LINE__-2);
		return errno;
	}
	
	/* initialize semaphore operation structures */
	sopsPing[0].sem_num = PING;
	sopsPing[0].sem_op  = -1;
	sopsPing[0].sem_flg = 0;
	sopsPing[1].sem_num = PING;
	sopsPing[1].sem_op  = 1;
	sopsPing[1].sem_flg = 0;
	
	sopsPong.sem_num = PONG;
	sopsPong.sem_op  = 0;
	sopsPong.sem_flg = 0;
	
	/* initialize msg send, receive agruments */
	sendArgs.m_qid 		= mqid;
	sendArgs.m_flags 	= MSG_FLAGS_OUT;
	sendArgs.m_counter 	= 0;
	sendArgs.m_from 	= CH_PONG;
	sendArgs.m_pid		= pid;
	
	recvArgs.m_qid 		= mqid;
	recvArgs.m_length 	= MAX_MSG_LEN;
	recvArgs.m_flags 	= MSG_FLAGS_IN;
	recvArgs.m_counter 	= 0;
	recvArgs.m_from 	= CH_PING;
	recvArgs.m_pid		= pid;
	
	
	/***** semaphore initial actions ****/
	
	/* check in at semaphore */
	
	/* semaphore - up */
	sopsPong.sem_op = 1;
	if( -1 == semop(semid, &sopsPong, 1) )
	{
		perror("semop");
		return errno;
	}
	SemValuePrint(semid, &options);
	
	/* check Ping semaphore: down and immediately up */
	if( -1 == semop(semid, sopsPing, 2) )
	{
		perror("semop");
		return errno;
	}
	
	printf("Pong %d: ", pid);
	
	/***** messages receive & reply loop *****/
	
	while( 1 )
	{
		if(-1 == MsgRecv(&recvArgs, &messegeIn) )
		{
			if(errno != ENOMSG)
			{
				fprintf(stderr, "%s:%d Error: MsgRecv\n", __FILE__, __LINE__-4);
				return errno;
			}
			
			/* check Ping semaphore */
			PrintV(&options, "checking Semaphore", -1);
			SemValuePrint(semid, &options);
			
			semCheck = semctl(semid, PING, GETVAL); 
			
			if( -1 == semCheck )
			{
				perror("semctl");
				return errno;
			}
			
			if(0 == semCheck)
			{
				break;
			}
			
			/* sleep */
			if(-1 == usleep(options.m_usleep) )
			{
				perror("usleep");
				return errno;
			}
			
			continue;
		}
		
		/* sleep */
		if(-1 == usleep(options.m_usleep) )
		{
			perror("usleep");
			return errno;
		}

		/* create & send reply */
		printf("Pong %d: ", pid);
		if( -1 == MsgSend(&sendArgs, &messegeOut) )
		{
			fprintf(stderr, "%s:%d Error: MsgSend\n", __FILE__, __LINE__-2);
			return errno;
		}
		
		printf("Pong %d: ", pid);
	} /* end while */
	
	/* check out at semaphore */
	printf("Checking out\n");
	
	sopsPong.sem_op = -1;
	if( -1 == semop(semid, &sopsPong, 1) )
	{
		perror("semop");
		return errno;
	}
	SemValuePrint(semid, &options);
	
	PrintV(&options, "# of messeges received:", recvArgs.m_counter);
	PrintV(&options, "# of messeges sent:", sendArgs.m_counter);
	
	return errno;
}

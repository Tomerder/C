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
#define SEMFLAGS		IPC_CREAT
#define MSG_FLAGS_OUT	0
#define MSG_FLAGS_IN	0

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

	semun semUn;			/* semaphores */
	struct sembuf sopsPing, sopsPong;
	unsigned short semval[2] = {0};
	
	int semid, mqid;		/* semaphore & msgQue id	*/
	int pid = getpid();		/* process id			*/
	

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
	
	/* initialize semaphores (only with -c) */
	if(options.m_create)
	{
		semUn.array = semval;
		if( -1 == semctl(semid, 0, SETALL, semUn) )
		{
			perror("semctl");
			return errno;
		}
	}
	
	/* initialize semaphore operation structures */
	sopsPing.sem_num = PING;
	sopsPing.sem_op  = 0;
	sopsPing.sem_flg = 0;
	
	sopsPong.sem_num = PONG;
	sopsPong.sem_op  = 0;
	sopsPong.sem_flg = 0;
	
	/* initialize msg send, receive arguments */
	sendArgs.m_qid 		= mqid;
	sendArgs.m_flags 	= MSG_FLAGS_OUT;
	sendArgs.m_counter 	= 0;
	sendArgs.m_from 	= CH_PING;
	sendArgs.m_pid		= pid;
	
	recvArgs.m_qid 		= mqid;
	recvArgs.m_length 	= MAX_MSG_LEN;
	recvArgs.m_flags 	= MSG_FLAGS_IN;
	recvArgs.m_counter 	= 0;
	recvArgs.m_from 	= CH_PONG;
	recvArgs.m_pid		= pid;
	
	
	/* check in at semaphore */
	
	/* semaphore - up */
	sopsPing.sem_op = 1;
	if( -1 == semop(semid, &sopsPing, 1) )
	{
		perror("semop");
		return errno;
	}
	SemValuePrint(semid, &options);
	printf("Ping %d: ", pid);
	
	/***** messages send & receive loop *****/
	
	while ( sendArgs.m_counter < options.m_nMesseges )
	{
		/* create & send messege */
		if( -1 == MsgSend(&sendArgs, &messegeOut) )
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
		
		/* get response */
		printf("Ping %d: ", pid);
		if( -1 == MsgRecv(&recvArgs, &messegeIn) )
		{
			fprintf(stderr, "error: MsgRecv %d\n", pid);
			return 1;
		}
		
		printf("Ping %d: ", pid);
	} /* end while */
	
	
	/***** closing *****/
	
	/* check out at semaphore */
	printf("Checking out\n");
	
	sopsPing.sem_op = -1;
	if( -1 == semop(semid, &sopsPing, 1) )
	{
		perror("semop");
		return errno;
	}
	SemValuePrint(semid, &options);
	
	PrintV(&options, "# of messeges sent:", sendArgs.m_counter);
	PrintV(&options, "# of messeges received:", recvArgs.m_counter);
	
	/*** if -d: wait for last pong, then delete ***/
	if(options.m_destroy)
	{
		/* wait until pong semaphore is 0 */
		PrintV(&options, "Ping: waiting for pongs to finish", -1);
		
		sopsPong.sem_op = 0;
		if( -1 == semop(semid, &sopsPong, 1) )
		{
			perror("semop");
			return errno;
		}
		
		/* delete semaphore & message queue */
		SemValuePrint(semid, &options);
		PrintV(&options, "Ping: deleteing semaphore & message queue", -1);
		
		if(-1 == semctl(semid, 0, IPC_RMID) )
		{
			perror("semctl");
		}
		if( -1 == msgctl(mqid, IPC_RMID, NULL) )
		{
			perror("Ping: msgctl");
		}
	}
	
	return errno;
}
/*######################################################################*/




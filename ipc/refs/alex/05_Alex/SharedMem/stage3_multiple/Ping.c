/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-31    
    Last modified date:		2013-09-05
    Description: 	ping pong with shared memory - passing messages one at a time.
    				stage 1 - single ping & single pong, single message
    				controlled by semaphore: each player locks itself & frees
    				the other.
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "PingPong.h"


/*######################################################################*/
/*				definitions & foreward declarations						*/
/*######################################################################*/

#define NSEMS				3
#define SIZE				getpagesize()
#define SEMFLAGS			IPC_CREAT
#define SHMFLAGS			IPC_CREAT
#define SEMOP_PING_FLG		0
#define SEMOP_PONG_FLG		0
#define SEMOP_PINGLOCK_FLG	0

/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char *argv[])
{
	Options options;				/* runtime options	*/
	SendRecvArg sendArgs; 			/* arguments for send	*/
	SendRecvArg recvArgs;			/* arguments for receive */
	char inboxBuffer[MAX_MSG_LEN];	/* buffer for incoming message */
	
	semun semUn;			/* semaphores */
	struct sembuf sopsPing, sopsPong, sopsPingLock;
	unsigned short semval[NSEMS];
	
	char* myShm;			/* ptr to start of shared memory */
	Message* message;
	
	int semid, shmid;		/* semaphore & sharedMem id	*/
	int pid = getpid();		/* process id */
	int pongSemValue;
	
	/* initialize & handle runtime options */
	OptHandler(argc, argv, &options);

	/* create/connect to semaphore */
	if( -1 == (semid = SemConnect(NSEMS, SEMFLAGS)) )
	{
		fprintf(stderr, "%s:%d Error: SemConnect\n", __FILE__, __LINE__-2);
		return errno;
	}
	
	/* create/connect to shared memory */
	if( -1 == (shmid = ShmConnect(SIZE, SEMFLAGS)) )
	{
		fprintf(stderr, "%s:%d Error: ShmConnect\n", __FILE__, __LINE__-2);
		return errno;
	}
	
	/* attach */
	if( (char *)(-1) == (myShm = shmat(shmid, (void *)0, 0)) )
	{
		perror("shmat in Ping");
		return errno;
	}
	
	message = (Message*)myShm;
	
	/* initialize semaphores, counter (only with -c) */
	if(options.m_create)
	{
		semval[PING] = 0;
		semval[PONG] = 0;
		semval[PING_LOCK] = 1;
		semUn.array = semval;
		
		if( -1 == semctl(semid, 0, SETALL, semUn) )
		{
			perror("semctl in Ping - create");
			return errno;
		}
		message->m_counter = 0;
	}
	
	message->m_counter += options.m_nMesseges;
	
	/* initialize semaphore operation structures: down for ping and up for pong */
	sopsPing.sem_num = PING;
	sopsPing.sem_op  = DOWN;
	sopsPing.sem_flg = SEMOP_PING_FLG;
	
	sopsPong.sem_num = PONG;
	sopsPong.sem_op  = UP;
	sopsPong.sem_flg = SEMOP_PONG_FLG;
	
	sopsPingLock.sem_num = PING_LOCK;
	sopsPingLock.sem_op  = DOWN;
	sopsPingLock.sem_flg = SEMOP_PINGLOCK_FLG;
	
	/* initialize msg send, receive arguments */
	sendArgs.m_counter 	= 0;
	sendArgs.m_from 	= PING;
	sendArgs.m_pid		= pid;
	sendArgs.m_msg 		= message;
	
	recvArgs.m_counter 	= 0;
	recvArgs.m_from 	= PONG;
	recvArgs.m_pid		= pid;
	recvArgs.m_msg 		= message;
	
		
	/*** message send & receive loop ***/
	
	while( sendArgs.m_counter < options.m_nMesseges )
	{
		PrintV(&options, "Counter:", message->m_counter);
		
		/*** SEND ***/
		
		/* pinglock semaphore lock */
		if( -1 == semop(semid, &sopsPingLock, 1) )
		{
			perror("semop in Ping - lock");
			return errno;
		}
		
		/* write message to shared memory */
		printf("Ping %d: ", pid);
		MsgSend(&sendArgs);

		/* check if pong locked & unlock it */
		pongSemValue = semctl(semid, PONG, GETVAL);
		if( -1 == pongSemValue)
		{
			perror("semctl in Ping");
			return errno;
		}
		
		if(0 == pongSemValue)
		{
			if( -1 == semop(semid, &sopsPong, 1) )
			{
				perror("semop 1 in Ping");
				return errno;
			}
			SemValuePrint(semid, &options);
		}
		
		/*** RECEIVE ***/
		
		/* ping semaphore down - waiting for pong to unlock it */
		if( -1 == semop(semid, &sopsPing, 1) )
		{
			perror("semop 2 in Ping");
			return errno;
		}
		SemValuePrint(semid, &options);
		
		/* read message */
		printf("Ping %d: ", pid);
		MsgRecv(&recvArgs, inboxBuffer);
		
		/* pinglock semaphore unlock */
		sopsPingLock.sem_op = UP;
		if( -1 == semop(semid, &sopsPingLock, 1) )
		{
			perror("semop in Ping - lock");
			return errno;
		}
		
		/* sleep */
		usleep(options.m_usleep);
	}
	/*** end send-receive loop ***/
	
	PrintV(&options, "Ping: ", pid);
	PrintV(&options, "# of messages sent:", sendArgs.m_counter);
	PrintV(&options, "# of messages received:", recvArgs.m_counter);
	
	/* detach */
	if( -1 == shmdt(myShm) )
	{
		perror("shmdt in Ping");
		return -1;
	}
	
	/* if -d, delete semaphore & close shared memory */
	if(options.m_destroy)
	{	
		if(-1 == semctl(semid, 0, IPC_RMID) )
		{
			perror("semctl in Ping - destroy");
		}
		if( -1 == (shmctl(shmid, IPC_RMID, NULL)) )
		{
			perror("shmctl in Ping - destroy");
			return -1;
		}
	}
	
	return errno;
}
/*######################################################################*/



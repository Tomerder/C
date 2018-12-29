/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-29    
    Last modified date:		2013-08-31
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

#define NSEMS			2
#define SIZE			getpagesize()
#define SEMFLAGS		IPC_CREAT
#define SHMFLAGS		IPC_CREAT
#define SEMOP_PING_FLG	0
#define SEMOP_PONG_FLG	0


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
	struct sembuf sopsPing, sopsPong;
	unsigned short semval[2];
	
	char* myShm;			/* ptr to start of shared memory */
	
	int semid, shmid;		/* semaphore & sharedMem id	*/
	int pid = getpid();		/* process id */

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
	
	/* attach & set counter */
	if( (char *)(-1) == (myShm = shmat(shmid, (void *)0, 0)) )
	{
		perror("shmat in Ping");
		return errno;
	}
	memcpy( &(COUNTER(myShm)), &options.m_nMesseges, sizeof(int));
	
	/* initialize semaphores (only with -c) */
	if(options.m_create)
	{
		semval[PING] = 0;
		semval[PONG] = 0;
		semUn.array = semval;
		
		if( -1 == semctl(semid, 0, SETALL, semUn) )
		{
			perror("semctl in Ping - create");
			return errno;
		}
	}
	
	/* initialize semaphore operation structures: down for ping and up for pong */
	sopsPing.sem_num = PING;
	sopsPing.sem_op  = DOWN;
	sopsPing.sem_flg = SEMOP_PING_FLG;
	
	sopsPong.sem_num = PONG;
	sopsPong.sem_op  = UP;
	sopsPong.sem_flg = SEMOP_PONG_FLG;
	
	/* initialize msg send, receive arguments */
	sendArgs.m_counter 	= 0;
	sendArgs.m_from 	= PING;
	sendArgs.m_pid		= pid;
	sendArgs.m_shm 		= myShm;
	
	recvArgs.m_counter 	= 0;
	recvArgs.m_from 	= PONG;
	recvArgs.m_pid		= pid;
	recvArgs.m_shm 		= myShm;
	
	
	/*** message send & receive loop ***/
	
	while( COUNTER(myShm) > 0 )
	{
		PrintV(&options, "Counter:", COUNTER(myShm));
		
		/*** SEND ***/
		
		/* write message to shared memory */
		printf("Ping %d: ", pid);
		MsgSend(&sendArgs);
		
		/* unlock pong */
		if( -1 == semop(semid, &sopsPong, 1) )
		{
			perror("semop 1 in Ping");
			return errno;
		}
		SemValuePrint(semid, &options);
		
		/* sleep */
		usleep(options.m_usleep);
		
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
	}
	/* end send-receive loop */
	
	PrintV(&options, "Ping: ", pid);
	PrintV(&options, "# of messeges sent:", sendArgs.m_counter);
	PrintV(&options, "# of messeges received:", recvArgs.m_counter);
	
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



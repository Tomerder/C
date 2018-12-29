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
#define _XOPEN_SOURCE 600
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <pthread.h>

#include "PingPong.h"


/*######################################################################*/
/*				definitions & foreward declarations						*/
/*######################################################################*/

#define SIZE			getpagesize()
#define SEMFLAGS		0
#define SHMFLAGS		0


/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char *argv[])
{
	Options options;				/* runtime options	*/
	SendRecvArg sendArgs; 			/* arguments for send	*/
	SendRecvArg recvArgs;			/* arguments for receive */
	
	char* myShm;			/* ptr to start of shared memory */
	Message* message;
	
	int shmid;				/* sharedMem id	*/
	int pid = getpid();		/* process id */
	int pingSemValue;
	
	/* initialize & handle runtime options */
	OptHandler(argc, argv, &options);
	
	/* create/connect to shared memory */
	if( -1 == (shmid = ShmConnect(SIZE, SEMFLAGS)) )
	{
		fprintf(stderr, "%s:%d Error: ShmConnect\n", __FILE__, __LINE__-2);
		return errno;
	}
	
	/* attach */
	if( (char *)(-1) == (myShm = shmat(shmid, (void *)0, 0)) )
	{
		perror("shmat in Pong");
		return errno;
	}
	
	message = (Message*)myShm;
	
	/* initialize msg send, receive arguments */
	sendArgs.m_counter 	= 0;
	sendArgs.m_from 	= PONG;
	sendArgs.m_pid		= pid;
	sendArgs.m_msg 		= message;
	
	recvArgs.m_counter 	= 0;
	recvArgs.m_from 	= PING;
	recvArgs.m_pid		= pid;
	recvArgs.m_msg 		= message;
	
	
	/*** message receive & reply loop ***/
	
	while( message->m_counter > 0 )
	{
		PrintV(&options, "Counter:", message->m_counter);
		
/* pong semaphore down - wait for ping to unlock it */
		if( -1 == semop(semid, &sopsPong, 1) )
		{
			perror("semop 1 in Pong");
			return errno;
		}
		SemValuePrint(semid, &options);		
		
		/* read message */
		printf("Pong %d: ", pid);
		MsgRecv(&recvArgs, inboxBuffer);
		
		/* sleep */
		usleep(options.m_usleep);
		
		/* send reply */
		printf("Pong %d: ", pid);
		MsgSend(&sendArgs);
		
		
/* check if ping locked & unlock it */
		pingSemValue = semctl(semid, PING, GETVAL);
		if( -1 == pingSemValue)
		{
			perror("semctl in Pong");
			return errno;
		}
		
		if(0 == pingSemValue)
		{
			if( -1 == semop(semid, &sopsPing, 1) )
			{
				perror("semop 2 in Pong");
				return errno;
			}
			SemValuePrint(semid, &options);
		}
	}
	/*** end receive-reply loop ***/
	
	PrintV(&options, "Pong: ", pid);
	PrintV(&options, "# of messages received:", recvArgs.m_counter);
	PrintV(&options, "# of messages sent:", sendArgs.m_counter);
	
	/* detach */
	if( -1 == shmdt(myShm) )
	{
		perror("shmdt in Pong");
		return -1;
	}
	
	return errno;	
}
/*######################################################################*/



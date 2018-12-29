/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-31    
    Last modified date:		2013-09-05
    Description: 	ping pong with shared memory
    				stage 1 - single ping & single pong
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


#define NMSG_DEFAULT		7
#define SLEEP_DEFAULT		100000
#define PERMISSIONS			0666
#define KEYGEN				301

#define THOUSAND			1000

#define NSEMS				3

static const char *fileDefault 	= "PingPong.c";
static const char *msgs[] 		= {"Message", "Response"};


/*######################################################################*/
/* 							Options Handler								*/
/*######################################################################*/
void OptHandler(int _argc, char *_argv[], Options *_options)
{
	int opt;
	
	/* initialize all options to default */
	_options->m_verbose = FALSE;
	_options->m_create 	= FALSE;
	_options->m_destroy = FALSE;
	_options->m_usleep 	= SLEEP_DEFAULT;
	_options->m_nMesseges = NMSG_DEFAULT;
	
	memset(_options->m_filename, 0, sizeof(char) * FILE_NAME_LEN);
	strcpy(_options->m_filename, fileDefault);
	
	/* read flags */
	while ((opt = getopt(_argc, _argv, "s:f:n:vcd")) != -1)
	{
		switch(opt)
		{
		case 's':
			_options->m_usleep = atoi(optarg)*THOUSAND;
			break;
	
		case 'f':
			memset(_options->m_filename, 0, sizeof(char) * FILE_NAME_LEN);
			strcpy(_options->m_filename, optarg);
			break;
		
		case 'n':
			_options->m_nMesseges = atoi(optarg);
			break;
		
		case 'v':
			_options->m_verbose = TRUE;
			break;
		
		case 'c':
			_options->m_create = TRUE;
			break;
			
		case 'd':
			_options->m_destroy = TRUE;
			break;
		
		default:
			printf("Usage: -s [msec] -n [int] -f [filename] -v -c -d\n");
			exit(1);
		}
	}
}
/*######################################################################*/




/*######################################################################*/
/* 								SEMAPHORES						 		*/
/*######################################################################*/


/*######################################################################*/
/* 		create/connect to semaphore set, return id, -1 for errors 		*/
int SemConnect(int _num, int _flags)
{
	key_t key;
	int sid;
	
	/* generate key */
	if( -1 == (key = ftok(fileDefault, KEYGEN)) )
	{
		perror("ftok in SemConnect");
		return -1;
	}
	
	/* create/connect to semaphore set */
	if( -1 == (sid = semget(key, _num, PERMISSIONS | _flags)) )
	{
		printf("key = %d,  nSems = %d\n", key, _num);
		
		perror("semget in SemConnect");
		return -1;
	}
	
	return sid;
}
/*######################################################################*/


/*######################################################################*/
/* 						print semaphore values							*/
void SemValuePrint(int _semid, Options *_options)
{
	semun semUn;			/* semaphores */
	unsigned short semval[NSEMS] = {0};
	int i;
	
	semUn.array = semval;
	if( -1 == semctl(_semid, NSEMS, GETALL, semUn) )
	{
		perror("semctl in SemValuePrint");
		return;
	}
	
	PrintV(_options, "Semaphores:", -1);
	for(i=0; i<NSEMS; ++i)
	{
		PrintV(_options, "", semval[i]);
	}
}
/*######################################################################*/




/*######################################################################*/
/* 								SHARED MEMORY					 		*/
/*######################################################################*/


/*######################################################################*/
/* 		create/connect to shared memory, return id, -1 for errors 		*/
int ShmConnect(int _size, int _flags)
{
	key_t key;
	int shmid;
	
	/* generate key */
	if( -1 == (key = ftok(fileDefault, KEYGEN)) )
	{
		perror("ftok in ShmConnect");
		return -1;
	}
	/* get mem id */
	if( -1 == (shmid = shmget(key, _size, PERMISSIONS | _flags)) )
	{
		perror("shmget in ShmConnect");
		return -1;
	}
	
	return shmid;
}
/*######################################################################*/


/*######################################################################*/
/* 		write a messege to shared memory, return message length			*/
int MsgSend(SendRecvArg *_args)
{
	int msgLen;
	char buffer[MAX_MSG_LEN];
		
	msgLen = sprintf(buffer, "%s from %d: #%d", msgs[ _args->m_from], _args->m_pid, _args->m_counter + 1);
	
	/* send message */
	_args->m_msg->m_length = msgLen;
	memcpy( _args->m_msg->m_msg, buffer, msgLen);
	
	if(PING == _args->m_from) {
		-- _args->m_msg->m_counter;
	}
	
	++(_args->m_counter);
	printf("sent:\t\"%s\"\n", buffer);
	return msgLen;
}
/*######################################################################*/

/*######################################################################*/
/*		read message from shared memory, return message length			*/
int MsgRecv(SendRecvArg *_args, char* _buffer)
{
	int msgLen;
	
	/* get message length */
	msgLen = _args->m_msg->m_length;
	
	/* get message */
	memcpy(_buffer, _args->m_msg->m_msg, msgLen);
	_buffer[msgLen] = '\0';
	
	++(_args->m_counter);
	printf("received:\t\"%s\"\n", _buffer);
	return msgLen;
}
/*######################################################################*/



/*######################################################################*/
/* 				print in verbose mode, _num can't be -1					*/
/*######################################################################*/
void PrintV(Options *_options, char* _text, int _num)
{
	if(_options->m_verbose)
	{
		printf("%s", _text);
		if(_num!=-1)
		{
			printf(" %d", _num);
		}
		printf("\n");
	}
}
/*######################################################################*/




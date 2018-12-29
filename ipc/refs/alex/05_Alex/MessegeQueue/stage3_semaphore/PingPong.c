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
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include "PingPong.h"



/*######################################################################*/
/*				definitions & foreward declarations						*/
/*######################################################################*/


#define NMSG_DEFAULT		7
#define SLEEP_DEFAULT		10000
#define PERMISSIONS			0666
#define KEYGEN				301

#define THOUSAND			1000

#define NSEMS			2

static const char *fileDefault = "PingPong.c";

static const char *msgs[] = {"Messege", "Response"};



/*######################################################################*/
/* 							Options Handler								 */
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
/* 		create/connect to semaphore set, return id, -1 for errors 		*/
/*######################################################################*/
int SemConnect(int _num, int _flags)
{
	key_t key;
	int sid;
	
	/* generate key */
	if( -1 == (key = ftok(fileDefault, KEYGEN)) )
	{
		perror("ftok");
		return -1;
	}
	
	/* create/connect to semaphore set */
	if( -1 == (sid = semget(key, _num, PERMISSIONS | _flags)) )
	{
		perror("semget");
		return -1;
	}
	
	return sid;
}
/*######################################################################*/


/*######################################################################*/
/* 			create/open MsgQue, returns identifier, -1 for error		*/
/*######################################################################*/
int 	MsgQueCreate		(Options *_options)
{
	key_t key;
	int msgQid;
	int flags = (_options->m_create) ? PERMISSIONS | IPC_CREAT : PERMISSIONS;
	
	/* generate key */
	if( -1 == (key = ftok(fileDefault, KEYGEN)) )
	{
		perror("ftok");
		return -1;
	}
	
	/* create/open messege queue */
	if( -1 == (msgQid = msgget(key, flags)) )
	{
		perror("msgget");
		return -1;
	}
	
	return msgQid;
}
/*######################################################################*/


/*######################################################################*/
/* 			create and send a messege, return messege length			*/
/*######################################################################*/
int MsgSend(SendRecvArg *_args, Messege *_messege)
{
	int msgLen;
	
	msgLen = sprintf(_messege->m_msg, "%s from %d: #%d", msgs[ _args->m_from - CH_PING], _args->m_pid, _args->m_counter + 1);
	_messege->m_type = _args->m_from;
	
	/* send messege */
	if( -1 == msgsnd(_args->m_qid, (void *)_messege, msgLen+1, _args->m_flags) )
	{
		perror("msgsnd");
		return -1;
	}
	
	++(_args->m_counter);
	printf("sent \"%s\"\n", _messege->m_msg);
	
	return msgLen;
}
/*######################################################################*/


/*######################################################################*/
/* 			fetch a messege from queue, return messege length			*/
/*######################################################################*/
int MsgRecv(SendRecvArg *_args, Messege *_messege)
{	
	int msgLen;
	
	if( -1 == (msgLen = msgrcv(_args->m_qid, (void *)_messege, _args->m_length, _args->m_from, _args->m_flags)) )
	{
		if(errno != ENOMSG) {
			perror("msgrcv");
		}
		return -1;
	}
	printf("received \"%s\"\n", _messege->m_msg);
	++(_args->m_counter);
	
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




/*######################################################################*/
/* 						print semaphore values							*/
/*######################################################################*/
void SemValuePrint(int _semid, Options *_options)
{
	semun semUn;			/* semaphores */
	unsigned short semval[NSEMS] = {0};
	int i;
	
	semUn.array = semval;
	if( -1 == semctl(_semid, NSEMS, GETALL, semUn) )
	{
		perror("semctl");
		return;
	}
	
	PrintV(_options, "Semaphores:", -1);
	for(i=0; i<NSEMS; ++i)
	{
		PrintV(_options, "", semval[i]);
	}
}
/*######################################################################*/






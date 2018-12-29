/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-22    
    Last modified date:		2013-08-22
    Description: 	interprocess communication excercise - message queue - functions
    				multiple processes from 2 files - ping (sending) and pong (receiving)
    				
    				Each ping enters an initial message into a dedicated channel ("registrar")
    				and deletes a message there on exit.
    				When a pong checks messages, if the messege channel is empty it checks
    				the registrar channel. If not empty, sends back in the same message.
    				If empty, exits.
    				(Note: possibility of race conditions).
    					
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

#include "PingPong.h"



/****************************************************************************/
/*				definitions & foreward declarations							*/
/****************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "PingPong.h"


#define NMSG_DEFAULT		7
#define MODE_DEF			0666
#define KEYGEN				301

#define THOUSAND			1000


enum MsgTypes {PING, CHCK_IN, PONG};

static const char *fileDefult = "PingPong.c";

static const char *msgs[] = {"Messege", "Chceking in", "Response"};

/****************************************************************************/
/* options handler */
void OptHandler(int _argc, char *_argv[], Options *_options)
{
	int opt;
	
	/* initialize all options to default */
	_options->m_verbose = FALSE;
	_options->m_create 	= FALSE;
	_options->m_destroy = FALSE;
	_options->m_typeEOF = FALSE;
	_options->m_usleep 	= 0;
	_options->m_nMesseges = NMSG_DEFAULT;
	
	memset(_options->m_filename, 0, sizeof(char) * FILE_NAME_LEN);
	strcpy(_options->m_filename, fileDefult);
	
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
			fprintf(stderr, "Error reading options\n");
			exit(1);
		}
	}
}
/****************************************************************************/



/****************************************************************************/
/* create/open MsgQue, returns identifier, -1 for error */
int 	MsgQueCreate		(Options *_options)
{
	key_t key;
	int msgQid;
	int flags = (_options->m_create) ? MODE_DEF |  IPC_CREAT : MODE_DEF;
	
	/* generate key */
	if( -1 == (key = ftok(fileDefult, KEYGEN)) )
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
/****************************************************************************/


/****************************************************************************/
/* create and send a messege, return messege length */
int MsgSend(SendRecvArg *_args, Messege *_messege, int _isCheckIn)
{
	int msgLen;
	
	if(_isCheckIn)
	{
		msgLen = sprintf(_messege->m_msg, "%d %s", _args->m_pid, msgs[CHCK_IN]);
	}
	else
	{
		msgLen = sprintf(_messege->m_msg, "%s from %d: #%d", msgs[ _args->m_from - CH_PING], _args->m_pid, _args->m_counter + 1);
	}
	
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
/****************************************************************************/


/****************************************************************************/
/* fetch a messege from queue, return messege length */
int MsgRecv(SendRecvArg *_args, Messege *_messege)
{	
	int msgLen;
	
	if( -1 == (msgLen = msgrcv(_args->m_qid, (void *)_messege, _args->m_length, _args->m_from, _args->m_flags)) )
	{
		perror("msgrcv");
		return -1;
	}
	printf("received \"%s\"\n", _messege->m_msg);
	++(_args->m_counter);
	
	return msgLen;
}
/****************************************************************************/


/****************************************************************************/
/* check if messege is from registrar channel */
int CheckPings(Messege *_messege)
{
	return ( CH_CHCK_IN == _messege->m_type );
}
/****************************************************************************/






/****************************************************************************/
/* print in verbose mode, _num can't be -1 */
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
/****************************************************************************/







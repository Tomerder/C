/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-21    
    Last modified date:		2013-08-21
    Description: 	interprocess communication excercise - messege queue - functions
    				2 files - ping (sending) and pong (receiving)
    				
    				pong active until receives EOF messege from ping.
    				default: empty messege (just type) on regular channel
    					if -e flag used: messege sent on EOF channel.
    					
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


enum MsgTypes {PING, EOF_MSG, PONG};

static const char *fileDefult = "PingPong.c";

static const char *msgs[] = {"Messege", "Goodbye", "Response"};

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
	while ((opt = getopt(_argc, _argv, "s:f:n:evcd")) != -1)
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
		
		case 'e':
			_options->m_typeEOF = TRUE;
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
/* destroy MsgQue, returns errno  */
/*
int 	MsgQueDestroy		(Options *_options, int _qid)
{
	int cmd = (_options->m_destroy) ? IPC_RMID : 
	
	msgctl(_qid, cmd, 
	
}
*/
/****************************************************************************/


/****************************************************************************/
/* create and send a messege, return messege length */
int MsgSend(SendRecvArg *_args, Messege *_messege, int _isEOF)
{
	int msgLen;
	
	if(_isEOF)
	{
		msgLen = sprintf(_messege->m_msg, "%s", msgs[EOF_MSG]);
	}
	else
	{
		msgLen = sprintf(_messege->m_msg, "%s %d", msgs[ _args->m_from - CH_PING], _args->m_counter + 1);
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
/* check if a messege signals EOF */
int CheckEOF(Options *_options, Messege *_msg)
{
	if(_options->m_typeEOF)
	{
		return ( CH_EOF == _msg->m_type );
	}
	
	return ( !strcmp(_msg->m_msg, msgs[1]) );
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







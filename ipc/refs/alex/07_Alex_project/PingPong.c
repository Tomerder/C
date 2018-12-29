/*******************************************************************************
    Author:					Stav Ofer
    Creation date:  		2013-09-20    
    Last modified date:		2013-09-28
    Description: 	Test for Message Queue over Shared Memory - functions
********************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "MsgQueue.h"
#include "PingPong.h"


/* filenames for queues */
static char *fileDefMain = FILE_MAINQUEUE;
static char *fileHandShake = FILE_HANDSHAKEQ;


/*######################################################################*/
/* 							Options Handler								*/
/*######################################################################*/
void OptHandler(int _argc, char *_argv[], Options_t *_options)
{
	int opt;
	
	/* initialize all options to default */
	_options->m_verbose 	= FALSE;
	_options->m_sleep 		= SLEEP_DEFAULT;
	_options->m_nMessages 	= NMSG_DEFAULT;
	_options->m_queueSize 	= QSIZE_DEFAULT;
	
	memset(_options->m_filename, 0, sizeof(char) * FILE_NAME_LEN);
	strcpy(_options->m_filename, fileDefMain);
	
	memset(_options->m_fileHandshake, 0, sizeof(char) * FILE_NAME_LEN);
	strcpy(_options->m_fileHandshake, fileHandShake);
	
	/* read flags */
	while ((opt = getopt(_argc, _argv, "s:f:h:m:q:v")) != -1)
	{
		switch(opt)
		{
		case 's':
			_options->m_sleep = atoi(optarg);
			break;
		
		case 'f':
			memset(_options->m_filename, 0, sizeof(char) * FILE_NAME_LEN);
			strcpy(_options->m_filename, optarg);
			break;
		
		case 'h':
			memset(_options->m_fileHandshake, 0, sizeof(char) * FILE_NAME_LEN);
			strcpy(_options->m_fileHandshake, optarg);
			break;
		
		case 'm':
			_options->m_nMessages = atoi(optarg);
			break;
		
		case 'q':
			_options->m_queueSize = atoi(optarg);
			break;
		
		case 'v':
			_options->m_verbose = TRUE;
			break;
		
		default:
			printf("Usage: -s [sec] -f [filename] -h [filename] -m [int] -q [int] -v\n");
			exit(1);
		}
	}
}
/*######################################################################*/



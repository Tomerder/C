/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-21    
    Last modified date:		2013-08-21
    Description: 	interprocess communication excercise - named pipe - functions
    				2 files - ping (sending) and pong (receiving)
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "PingPongFunc.h"



#define MIN_READ_BUFFER		8
#define MAX_READ_BUFFER		128
#define DEF_READ_BUFFER		64

#define NMSG_DEFAULT		7
#define MODE_FULL			0777
#define THOUSAND			1000


typedef enum {FALSE, TRUE} Bool;

static const char *fifofile = "pingpong";



/****************************************************************************/
/* options handler */
void OptHandler(int _argc, char *_argv[], Options *_options)
{
	int opt;
	
	/* initialize all options to default */
	_options->m_usleep = 0;
	_options->m_create = _options->m_destroy = _options->m_verbose = FALSE;
	_options->m_nMesseges = NMSG_DEFAULT;
	_options->m_bufferLen = DEF_READ_BUFFER;
	strcpy(_options->m_filename, fifofile);
	
	/* read flags */
	while ((opt = getopt(_argc, _argv, "s:f:n:r:vcd")) != -1)
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
		
		case 'r':
			_options->m_bufferLen = atoi(optarg);
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
/* create FIFO file, returns errno */
int FifoCreate(Options *_options)
{
	if( -1 == mkfifo(_options->m_filename, MODE_FULL) )
		{
			fprintf(stderr, "Ping: Error: mkfifo\n");
			return errno;
		}
	PrintV(_options, "Ping: Created FIFO file\n", -1);
	
	return errno;
}
/****************************************************************************/

/****************************************************************************/
/* destroy (by unlinking) FIFO file, returns errno */
int FifoDestroy(Options *_options)
{
	if( -1 == unlink(_options->m_filename) )
	{
		printf("Error: unlink\n");
		return errno;
	}
	PrintV(_options, "destroyed fifo file", -1);
	
	return errno;
}
/****************************************************************************/



/****************************************************************************/
/* open pipe in required mode (read/write), return descriptor or -1 for error */
int PipeOpen(Options *_options, int _mode)
{
	int fifo;	/* file descriptor */
	
	if( -1 == ( fifo = open(_options->m_filename, _mode) ) )
	{
		fprintf(stderr, "Error: open\n");
		return -1;
	}
	PrintV(_options, "opened pipe\n", -1);
	
	return fifo;
}
/****************************************************************************/

/****************************************************************************/
/* close pipe, input is file descriptor, returns errno */
int PipeClose(Options *_options, int _desc)
{
	if( -1 == close(_desc) )
	{
		printf("Error: close\n");
		return errno;
	}
	PrintV(_options, "closed pipe", -1);
	
	return errno;
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




/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-14    
    Last modified date:		2013-08-14
    Description: 	interprocess communication excercise - named pipe
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



/****************************************************************************/
/*				definitions & foreward declarations							*/
/****************************************************************************/

#define FILE_NAME_LEN		16
#define MAX_MSG_LEN			20
#define NMSG_DEFAULT		7
#define MODE_FULL			0777
#define THOUSAND			1000


const char *fifofile 	= "pingpong";
const char *msg			= "Messege ";


typedef struct Options
{
	int		m_sleep;
	char 	m_filename[FILE_NAME_LEN];
	int		m_nMesseges;
	int 	m_create;
	int 	m_destroy;
} Options;


/* verbose flag */
static int g_verbose;


/* options handler */
void 	OptHandler			(int _argc, char *_argv[], Options *_options);
/* print in verbose mode */
void 	PrintV				(char* _text, int _num);


/****************************************************************************/


/****************************************************************************/
/*									MAIN																	*/
/*																											*/
/*	options: 	-s [num] 	  -f [str]  	-n [num]		-v				-c				-d				*/
/*		 	sleep time (msec), filename, # of messeges, verbose mode, mkfifo at start, destroy fifo at end	*/
/****************************************************************************/
int main(int argc, char *argv[])
{
	Options options;				/* runtime options */
	int fifo;						/* file descriptor */
	char messege[MAX_MSG_LEN];		/* messege to send */
	int msgLen = strlen(msg);		/* messege length */
	
	int nWrite = 0;							/* # of writes performed */
	int nCharSent = 0, nCharSentTotal = 0;	/* # of characrters sent */
	
	
	/* initialize & handle runtime options */
	options.m_sleep = options.m_create = options.m_destroy = 0;
	memset(options.m_filename, 0, sizeof(char) * FILE_NAME_LEN);
	options.m_nMesseges = NMSG_DEFAULT;
	
	OptHandler(argc, argv, &options);
	
	if(!options.m_filename)
	{
		strcpy(options.m_filename, fifofile);
	}
	
	/* create fifo file if got -c flag */
	if(options.m_create)
	{	
		if( -1 == mkfifo(options.m_filename, MODE_FULL) )
		{
			fprintf(stderr, "Ping: Error: mkfifo\n");
			return errno;
		}
		PrintV("Ping: Created FIFO file\n", -1);
	}
	
	/* open pipe for writing */
	if( -1 == ( fifo = open(options.m_filename, O_WRONLY) ) )
	{
		fprintf(stderr, "Ping: Error: open\n");
		return errno;
	}
	PrintV("Ping: opened pipe\n", -1);
	
	
	strcpy(messege, msg);
	
	while ( nWrite < options.m_nMesseges )
	{
		++nWrite;
		messege[ msgLen ] = '0'+nWrite;
		
		/* send messege */
		if( -1 == ( nCharSent = write(fifo, &messege, msgLen+1) ) )
		{
			fprintf(stderr, "Ping: Error: write");
			return errno;
		}
		nCharSentTotal += nCharSent;
		
		messege[ msgLen+1 ] = '\0';
		printf("Ping: sent \"%s\"\n", messege);
		
		/* sleep */
		usleep(options.m_sleep);
	}
	
	PrintV("Ping: # of messeges sent:", nWrite);
	PrintV("Ping: # of characters sent:", nCharSentTotal);
	
	
	/* close file */

	if( -1 == close(fifo) )
	{
		printf("Ping: Error: close\n");
		return errno;
	}
	PrintV("Ping: closed pipe", -1);
	
	/* destroy file by unlink if got flag -d */
	if(options.m_destroy)
	{
		if( -1 == unlink(options.m_filename) )
		{
			printf("Ping: Error: unlink\n");
			return errno;
		}
		PrintV("Ping: destroyed fifo file", -1);
	}
	
	return errno;
}

/****************************************************************************/


/* options handler */
void OptHandler(int _argc, char *_argv[], Options *_options)
{
	int opt;
	
	while ((opt = getopt(_argc, _argv, "s:f:n:vcd")) != -1)
	{
		switch(opt)
		{
		case 's':
			_options->m_sleep = atoi(optarg)*THOUSAND;
			break;
	
		case 'f':
			strcpy(_options->m_filename, optarg);
			break;
		
		case 'n':
			_options->m_nMesseges = atoi(optarg);
			break;
		
		case 'v':
			g_verbose = 1;
			break;
		
		case 'c':
			_options->m_create = 1;
			break;
			
		case 'd':
			_options->m_destroy = 1;
			break;
		
		default:
			fprintf(stderr, "Error reading options\n");
			exit(1);
		}
	}
}
/****************************************************************************/


/* print in verbose mode, _num can't be -1 */
void PrintV(char* _text, int _num)
{
	if(g_verbose)
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



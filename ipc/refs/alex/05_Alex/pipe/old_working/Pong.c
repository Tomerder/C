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
#define MODE_FULL			0777
#define THOUSAND			1000


const char *fifofile = "pingpong";


typedef struct Options
{
	int		m_sleep;
	char 	m_filename[FILE_NAME_LEN];
} Options;


/* verbose flag */
static int g_verbose;


/* options handler */
void 	OptHandler			(int _argc, char *_argv[], Options *_options);
/* print in verbose mode */
void 	PrintV				(char* _text, int _num);


/****************************************************************************/


/****************************************************************************/
/*									MAIN									*/
/*																			*/
/*	options: 	-s [num] 	  -f [str]  	-v								*/
/*		 	sleep time (msec), filename, verbose mode						*/
/****************************************************************************/
int main(int argc, char *argv[])
{
	Options options;				/* runtime options */
	int fifo;						/* file descriptor */
	char readBuffer[MAX_MSG_LEN];	/* buffer to read into */
	
	int nRead = 0;							/* # of reads performed */
	int nCharRead = 0, nCharReadTotal = 0;	/* # of characrters read */
	
	
	/* initialize & handle runtime options */
	options.m_sleep = 0;
	memset(options.m_filename, 0, sizeof(char) * FILE_NAME_LEN);
	
	OptHandler(argc, argv, &options);
	
	if(!options.m_filename)
	{
		strcpy(options.m_filename, fifofile);
	}
	
	memset(readBuffer, 0, MAX_MSG_LEN*sizeof(char));
	
	/* open pipe for reading */
	if( -1 == ( fifo = open(options.m_filename, O_RDONLY) ) )
	{
		fprintf(stderr, "Pong: Error: open\n");
		return errno;
	}
	PrintV("Pong: opened pipe\n", -1);
	
	
	while( (nCharRead = read(fifo, &readBuffer, MAX_MSG_LEN-1) ) > 0 )
	{
		++nRead;
		nCharReadTotal += nCharRead;
		
		readBuffer[nCharRead] = '\0';
		printf("Pong: received \"%s\"\n", readBuffer);
		
		/* sleep */
		usleep(options.m_sleep);
	}
	
	PrintV("Pong: # of messeges read:", nRead);
	PrintV("Pong: # of characters read:", nCharReadTotal);
	
	/* close file */
	if( -1 == close(fifo) )
	{
		printf("Pong: Error: close\n");
		return errno;
	}
	PrintV("Pong: closed pipe", -1);
	
	
	return errno;
}

/****************************************************************************/



/* options handler */
void OptHandler(int _argc, char *_argv[], Options *_options)
{
	int opt;
	
	while ((opt = getopt(_argc, _argv, "s:f:v")) != -1)
	{
		switch(opt)
		{
		case 's':
			_options->m_sleep = atoi(optarg)*THOUSAND;
			break;
	
		case 'f':
			strcpy(_options->m_filename, optarg);
			break;
	
		case 'v':
			g_verbose = 1;
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



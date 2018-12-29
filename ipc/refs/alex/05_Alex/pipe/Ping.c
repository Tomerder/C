/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-14    
    Last modified date:		2013-08-21
    Description: 	interprocess communication excercise - named pipe
    				2 files - ping (sending) and pong (receiving)
    				
    				runtime options:
    					-s: sleep in msec
    					-f: filename
    					-n: # of messeges to send
    					-c: create fifo file
    					-d: destroy fifo file
    					-v:	verbose mode

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


/****************************************************************************/
/*				definitions & foreward declarations							*/
/****************************************************************************/

#define MAX_MSG_LEN			20

const char *msg			= "Messege";	/* default messege */


/****************************************************************************/


/****************************************************************************/
/*									MAIN									*/
/****************************************************************************/
int main(int argc, char *argv[])
{
	Options options;			/* runtime options */
	int fifo;					/* file descriptor */
	char messege[MAX_MSG_LEN];	/* messege to send */
	int msgLen;					/* messege length */
	
	int nWrite = 0;							/* # of writes performed */
	int nCharSent = 0, nCharSentTotal = 0;	/* # of characrters sent */
	
	
	/* initialize & handle runtime options */
	OptHandler(argc, argv, &options);
	
	/* create fifo file if got -c flag */
	if(options.m_create)
	{	
		FifoCreate(&options);
	}
	
	/* open pipe for writing */
	PrintV(&options, "Ping: ", -1);
	fifo = PipeOpen(&options, O_WRONLY);
	
	/* send required # of messeges */
	while ( nWrite < options.m_nMesseges )
	{
		++nWrite;
		msgLen = sprintf(messege, "%s %d", msg, nWrite);
		
		/* send messege */
		if( -1 == ( nCharSent = write(fifo, &messege, msgLen) ) )
		{
			fprintf(stderr, "Ping: Error: write");
			return errno;
		}
		nCharSentTotal += nCharSent;
		
		printf("Ping: sent \"%s\"\n", messege);
		
		/* sleep */
		usleep(options.m_usleep);
	}
	
	PrintV(&options, "Ping: # of messeges sent:", nWrite);
	PrintV(&options, "Ping: # of characters sent:", nCharSentTotal);
	
	/* close pipe */
	PrintV(&options, "Ping: ", -1);
	PipeClose(&options, fifo);

	/* destroy file by unlink if got flag -d */
	if(options.m_destroy)
	{
		PrintV(&options, "Ping: ", -1);
		FifoDestroy(&options);
	}
	
	return errno;
}

/****************************************************************************/



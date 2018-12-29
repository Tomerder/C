/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-14    
    Last modified date:		2013-08-21
    Description: 	interprocess communication excercise - named pipe
    				2 files - ping (sending) and pong (receiving)
    				
    				runtime options:
    					-s: sleep in msec
    					-f: filename
    					-v:	verbose mode
    					-r:  read buffer length 
    					
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

#define MAX_READ_BUFFER		128

/****************************************************************************/


/****************************************************************************/
/*									MAIN									*/
/****************************************************************************/
int main(int argc, char *argv[])
{
	Options options;					/* runtime options */
	int fifo;							/* file descriptor */
	char readBuffer[MAX_READ_BUFFER];	/* buffer to read into */
	
	int nRead = 0;							/* # of reads performed */
	int nCharRead = 0, nCharReadTotal = 0;	/* # of characrters read */
	
	
	/* initialize & handle runtime options */
	OptHandler(argc, argv, &options);
	
	/* initialize read buffer */
	memset(readBuffer, 0, MAX_READ_BUFFER * sizeof(char));
	
	/* open pipe for reading */
	PrintV(&options, "Pong: ", -1);
	fifo = PipeOpen(&options, O_RDONLY);
	

	/* read while pipe is open on the other end (writing( */
	while( (nCharRead = read(fifo, &readBuffer, options.m_bufferLen - 1) ) > 0 )
	{
		++nRead;
		nCharReadTotal += nCharRead;
		
		readBuffer[nCharRead] = '\0';
		printf("Pong: received \"%s\"\n", readBuffer);
		
		/* sleep */
		usleep(options.m_usleep);
	}
	
	PrintV(&options, "Pong: # of messeges read:", nRead);
	PrintV(&options, "Pong: # of characters read:", nCharReadTotal);
	
	/* close pipe */
	PrintV(&options, "Pong: ", -1);
	PipeClose(&options, fifo);
	
	return errno;
}
/****************************************************************************/



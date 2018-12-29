/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-14    
    Last modified date:		2013-08-14
    Description: 	interprocess communication excercise - unnamed pipe
    				create a pipe, then fork, parent is writing to pipe
    				and child is reading.
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>



/****************************************************************************/
/*				definitions & foreward declarations							*/
/****************************************************************************/

#define MAX_MSG_LEN			20

typedef struct Options
{
	int		m_childSleep;
	int 	m_parentSleep;
	int		m_nMessege;
} Options;


typedef enum
{
	READ,
	WRITE
} ReadWrite;


/* verbose flag */
static int g_verbose;


/* options handler */
void 	OptHandler			(int _opt, Options *_options);
/* print in verbose mode */
void 	PrintV				(char* _text, int _num);


/****************************************************************************/


/****************************************************************************/
/*									MAIN									*/
/*																			*/
/*	options: 	-p [num] 			-c [num]   -n [num] 	-v				*/
/*		 parent's sleep time (usec), child's, # of messeges, verbose mode	*/
/****************************************************************************/
int main(int argc, char *argv[])
{
	Options options;	/* runtime options */
	int opt;			/* for getting runtime options */
	int pidChild;		/* child process id for fork */
	int pipefd[2];		/* file descriptors for pipe */
	
	int nRead = 0, nWrite = 0;				/* # of read & write performed */
	int nCharRead = 0, nCharReadTotal = 0;	/* # of characrters read */
	int nCharSent = 0, nCharSentTotal = 0;	/* # of characrters sent */
	
	char *msg1 = "messege ";	/* for composing messeges */
	char msg2;					/*		"				*/
	char messege[MAX_MSG_LEN];	/*		"				*/
	char buffer[MAX_MSG_LEN];	/* buffer for reading */
	
	/* handle runtime options */
	while ((opt = getopt(argc, argv, "c:p:n:v")) != -1)
	{
		OptHandler(opt, &options);
	}
	
	/* create pipe*/
	PrintV("Creating pipe", -1);
	if (pipe(pipefd) == -1)
	{
		fprintf(stderr, "Error: pipe creation\n");
		return errno;
	}
	
	
	/*** fork ***/
	PrintV("Calling fork() to create child process", -1);
	pidChild = fork();
	
	/* fork failed */
	if(pidChild < 0)
	{	
		printf("Error: fork failed\n");
		return errno;
	}
	
	/********* Parent - writing to pipe *********/
	else if(pidChild > 0)
	{		
		/* close reading end */
		PrintV("Parent: closing reading end", -1);
		close(pipefd[READ]);
		
		while( nWrite < options.m_nMessege )
		{
			/* create messege to send */
			++nWrite;
			strcpy(messege, msg1);
			msg2='0'+nWrite;
			strcat(messege, &msg2);
			
			/* send messege */
			if( -1 == (nCharSent = write( pipefd[WRITE], messege, strlen(messege)) ) )
			{
				fprintf(stderr, "Error: write");
				return errno;
			}
			
			nCharSentTotal += nCharSent;
			PrintV("Parent: sent messege #", nWrite);
			
			/* sleep */
			PrintV("Parent: going to sleep", -1);
			usleep(options.m_parentSleep);
			PrintV("Parent: wake up", -1);
		}
		
		/* closing pipe and waiting for child */
		PrintV("Parent: closing pipe and waiting for child", -1);
		close(pipefd[WRITE]);
		wait(NULL);
		
		PrintV("Parent: # of messeges sent:", nWrite);
		PrintV("Parent: # of characters sent:", nCharSentTotal);
		return errno;
	}
	
	/********* child - reading from pipe *********/
	else
	{
		/* close writing end */
		PrintV("Child: closing writing end", -1);
		close(pipefd[WRITE]);
		
		/* sleep */
		PrintV("Child: going to sleep", -1);
		usleep(options.m_childSleep);
		PrintV("Child: wake up", -1);
		
		/* initialize buffer */
		memset(buffer, 0, MAX_MSG_LEN*sizeof(char));
		
		while ( (nCharRead = read(pipefd[READ], &buffer, MAX_MSG_LEN-1)) > 0 )
		{
			/* insert end-of-string for printing */
			buffer[nCharRead] = '\0';
			
			++nRead;
			nCharReadTotal += nCharRead;
			PrintV("Child: received messege #", nRead);
			
			printf("%s\n", buffer);
			
			/* sleep */
			PrintV("Child: going to sleep", -1);
			usleep(options.m_childSleep);
			PrintV("Child: wake up", -1);
		}
		
		PrintV("Child: # of reads:", nRead);
		PrintV("Child: # of characters read:", nCharReadTotal);
		return errno;
	}
}


/****************************************************************************/



/* options handler */
void OptHandler(int _opt, Options *_options)
{
	switch(_opt)
	{
	case 'c':
		_options->m_childSleep = atoi(optarg);
		break;
	
	case 'p':
		_options->m_parentSleep = atoi(optarg);
		break;
	
	case 'n':
		_options->m_nMessege = atoi(optarg);
		break;
		
	case 'v':
		g_verbose = 1;
		break;
	
	default:
		fprintf(stderr, "Error reading options\n");
		exit(1);
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






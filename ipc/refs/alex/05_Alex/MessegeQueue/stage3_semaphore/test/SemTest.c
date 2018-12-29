/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-25    
    Last modified date:		2013-08-25
    Description: 	interprocess communication excercise - semaphore
****************************************************************************/


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "SemTest.h"


#define NLINE_DEFAULT		10
#define SLEEP_DEFAULT		100
#define THOUSAND			1000
#define MODE_DEF			0666
#define KEYGEN				302


static const char *fileDefult = "SemTest.c";




/*######################################################################*/
/* 								Options Handler							*/
/*######################################################################*/
void OptHandler(int _argc, char *_argv[], Options *_options)
{
	int opt;
	
	/* initialize all options to default */
	_options->m_pause 	= FALSE;
	_options->m_resume 	= FALSE;
	_options->m_destroy	= FALSE;
	_options->m_usleep 	= SLEEP_DEFAULT;
	_options->m_nLines 	= NLINE_DEFAULT;
	
	/* read flags */
	while ((opt = getopt(_argc, _argv, "n:s:prd")) != -1)
	{
		switch(opt)
		{
		case 's':
			_options->m_usleep = atoi(optarg)*THOUSAND;
			break;
	
		case 'n':
			_options->m_nLines = atoi(optarg);
			break;
		
		case 'p':
			_options->m_pause = TRUE;
			break;
		
		case 'r':
			_options->m_resume = TRUE;
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
/*######################################################################*/


/*######################################################################*/
/* 		create/connect to semaphore set, return id, -1 for errors 		*/
/*######################################################################*/
int SemConnect(int _num, int _flags)
{
	key_t key;
	int sid;
	
	/* generate key */
	if( -1 == (key = ftok(fileDefult, KEYGEN)) )
	{
		perror("ftok");
		return -1;
	}
	
	/* create/connect to semaphore set */
	if( -1 == (sid = semget(key, _num, MODE_DEF | _flags)) )
	{
		perror("semget");
		return -1;
	}int 	SemConnect	(int _num, int _flags);
	
	return sid;
}
/*######################################################################*/






/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-13    
    Last modified date:		2013-08-13
    Description: interprocess communication excercise
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

/****************************************************************************/
/*				definitions & foreward declarations							*/
/****************************************************************************/


static int g_verbose;



/* signal handlers for parent and child */
void 	SigHandlerParent	(int sig);
void 	SigHandlerChild		(int sig);
/* options handler */
void 	OptHandler			(int _opt, int _options[4]);
/* print in verbose mode */
void 	PrintV				(char* _text, int _num);




/****************************************************************************/



/****************************************************************************/
/*									MAIN									*/
/****************************************************************************/
int main(int argc, char *argv[])
{
	int opt;
	int options[4] = {0};	/* values for options: c, p, s, v */
	int sigChild, sigParent, pause;	/* values for options from above */
	
	int forkFlag;
	int pidParent, pidChild;
	
	struct sigaction saParent;
	struct sigaction saChild;
	
	/* handle runtime options */	
	
	while ((opt = getopt(argc, argv, "c:p:s:v")) != -1)
	{
		OptHandler(opt, options);
	}
	
	sigChild	= options[0];
	sigParent	= options[1];
	pause		= options[2];
	g_verbose	= options[3];
	
	
	/* establish signal handler for parent */
	
	PrintV("Establishing parent signal handler", -1);
	
	saParent.sa_handler = SigHandlerParent;
	saParent.sa_flags = 0;
	sigemptyset(&saParent.sa_mask);
	
	if (sigaction(sigParent, &saParent, NULL) == -1) {
		fprintf(stderr, "error: parent sigaction\n");
		return 1;
    }
	
	/*** fork ***/
	PrintV("Calling fork() to create child process", -1);
	forkFlag = fork();
	
	/* fork failed */
	if(forkFlag < 0)
	{	
		printf("Error: fork failed\n");
		return 1;
	}
	
	/********* Parent *********/
	else if(forkFlag > 0)
	{
		/* get child process id */
		pidChild = forkFlag;
		PrintV("Parent: child process id is", pidChild);
		
		/* sending signal to child */
		PrintV("Parent: sending signal to child, then waiting for return signal", -1);
		kill(pidChild, sigChild);
		
		/* wait for signal from child */
/*		system("pause");*/
		
		sleep(pause);
/*		_exit(0);		*/
	}
	
	/********* child *********/
	else
	{
		/* establish signal handler for child */
		
		PrintV("Child: establishing child signal handler", -1);
		
		saChild.sa_handler = SigHandlerChild;
		saChild.sa_flags = 0;
		sigemptyset(&saChild.sa_mask);
		
		if (sigaction(sigChild, &saChild, NULL) == -1)
		{
			fprintf(stderr, "error: child sigaction\n");
			return 1;
		}
		
		/* get parent process id */
		pidParent = getppid();
		PrintV("Child: parent process id is", pidParent);
		
		/* wait for signal from parent */
		PrintV("Child: waiting for signal from parent", -1);
/*		system("pause");*/
		
		kill(pidParent, sigParent);
		
		
		_exit(0);
    }
	
	return 0;
}
/****************************************************************************/



/* options handler */
void OptHandler(int _opt, int _options[4])
{
	switch(_opt)
	{
		case 'c':
			_options[0] = atoi(optarg);
			break;
		
		case 'p':
			_options[1] = atoi(optarg);
			break;
		
		case 's':
			_options[2] = atoi(optarg);
			break;
			
		case 'v':
			_options[3] = 1;
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


/* signal handlers for parent and child */

void SigHandlerParent(int sig)
{
	printf("I'm the Parent\n");
}


void SigHandlerChild(int sig)
{
	printf("I'm the Child\n");
}
/****************************************************************************/















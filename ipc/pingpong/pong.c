/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 06.02.14                                                                                              
*    LAST MODIFIED: 06.02.14                                                                                  
*    DESCRIPTION: ping pong	 
*****************************************************************************************************************/
#define _BSD_SOURCE 
#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE 500
/*-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

/*-----------------------------------------------------------------*/

#define PRINT_V(MSG) if(args.m_verbose) printf(MSG)
#define PRINT_S_V(MSG,STR) if(args.m_verbose) printf(MSG,STR)
#define PRINT_D_V(MSG,NUM) if(args.m_verbose) printf(MSG,NUM) 

/*-----------------------------------------------------------------*/
enum{READ, WRITE};
enum{FALSE, TRUE};

enum{ERR_PIPE_CREATE=1};

#define MAX_FILE_NAME 32

#define ARG_NOT_INIT -1

#define DEFAULT_IS_VERBOSE TRUE
#define DEFAULT_FILE "pingpong"
#define DEFAULT_WRK_TIME 1000000

#define MICROSEC_MUL_RES 1000

#define READ_LEN 64

/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_verbose;
	char m_file[MAX_FILE_NAME];
	int m_workTime;
	int m_numOfMsgs;
	int m_isCreate;
	int m_isDelete;
}Args;

/*-----------------------------------------------------------------*/

void GetArgs(int argc ,char** argv, Args* args);

void ReadFromPipe(int _pipefd);

/*-----------------------------------------------------------------*/

Args args = {ARG_NOT_INIT, "", ARG_NOT_INIT} ;

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	int pipefd;

	GetArgs(argc ,argv, &args);
	
	PRINT_D_V("Consumer PID is %d\n",  getpid() );
	
	/*open pipe*/
	PRINT_V("opening pipe for reading...\n");
	pipefd = open(args.m_file, O_RDONLY);
	PRINT_V("pipe was opend for read\n");
    
    /*write to pipe*/
    ReadFromPipe(pipefd);
			
	PRINT_V("PONG ENDED\n");
	
	return 0;
}

/*-----------------------------------------------------------------*/

void ReadFromPipe(int _pipefd)
{
	int readRet;
	char msg[READ_LEN + 1];
	
	PRINT_V("start reading\n");
	
	do {
        if ((readRet = read(_pipefd, msg, READ_LEN)) == -1)
            perror("read");
        else {
            msg[readRet] = '\0';
            PRINT_S_V("%s\n", msg);
            usleep(args.m_workTime);  /*work*/
        }
    } while (readRet > 0);

	PRINT_V("finished reading\n");
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv, Args* _args)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "v:f:s:")) != -1)
	{
		switch(argType)
		{
			case 'v':
				_args->m_verbose = TRUE;
				break;	
			case 'f':
				strcpy(_args->m_file , optarg);
				break;	
			case 's':
				_args->m_workTime = atoi(optarg) * MICROSEC_MUL_RES;
				break;	
			default:				
				break;
		}
	}
	
	/*if wasnt set -> set default values*/
	if(_args->m_verbose ==  ARG_NOT_INIT){
		_args->m_verbose = DEFAULT_IS_VERBOSE;
	}

	if( strcmp (_args->m_file, "") == 0){
		strcpy(_args->m_file , DEFAULT_FILE);
	}

	if(_args->m_workTime ==  ARG_NOT_INIT){
		_args->m_workTime = DEFAULT_WRK_TIME;
	}

	
}

/*-----------------------------------------------------------------*/

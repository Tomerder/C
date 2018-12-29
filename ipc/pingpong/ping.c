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
#define DEFAULT_N_OF_MSG 10
#define DEFAULT_IS_CREATE FALSE
#define DEFAULT_IS_DELETE FALSE

#define MICROSEC_MUL_RES 1000

#define TXT "message num. %d => sender pid %d\n"
#define CRT_MSG(MSG, TXT, NUM, PID) sprintf(MSG, TXT , NUM, PID)
#define MSG_LEN 64

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

void WriteToPipe(int _pipefd);

/*-----------------------------------------------------------------*/

Args args = {ARG_NOT_INIT, "", ARG_NOT_INIT, ARG_NOT_INIT, ARG_NOT_INIT, ARG_NOT_INIT } ;

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	int pipefd;

	GetArgs(argc ,argv, &args);
	
	PRINT_D_V("Producer PID is %d\n",  getpid() );
	
	/*create pipe*/
	if(args.m_isCreate){
		mknod(args.m_file, S_IFIFO | 0666, 0);
		PRINT_S_V("file %s was created\n", args.m_file);
	}

	/*open pipe*/
	PRINT_V("opening pipe for write...\n");
	pipefd = open(args.m_file, O_WRONLY);
	PRINT_V("pipe was opend for write\n");
    
    /*write to pipe*/
    WriteToPipe(pipefd);
		
	/*delete pipe*/
	if(args.m_isDelete){
		if(unlink(args.m_file) == -1){
			 perror("unlink");
		}else{
			PRINT_S_V("file %s was unlinked\n", args.m_file);
		}
	}		
		
	PRINT_V("PING ENDED\n");
	
	return 0;
}

/*-----------------------------------------------------------------*/

void WriteToPipe(int _pipefd)
{
	int i, pid , writeRet;
	char msg[MSG_LEN];
	
	pid = getpid();
	
	PRINT_V("start writing\n");
	
	for(i=0; i < args.m_numOfMsgs; i++){
		usleep(args.m_workTime);  /*work*/
	
		sprintf(msg, TXT , i + 1 , pid);
        
        if ((writeRet = write(_pipefd, msg, strlen(msg))) == -1){
            perror("write");
        }else{
            PRINT_S_V("writing : %s\n", msg);
        }
    }

	PRINT_V("finished writing\n");
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv, Args* _args)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "v:f:s:n:c:d:")) != -1)
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
			case 'n':
				_args->m_numOfMsgs = atoi(optarg);
				break;					
			case 'c':
				_args->m_isCreate = TRUE;
				break;	
			case 'd':
				_args->m_isDelete = TRUE;
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

	if(_args->m_numOfMsgs ==  ARG_NOT_INIT){
		_args->m_numOfMsgs = DEFAULT_N_OF_MSG;
	}

	if(_args->m_isCreate ==  ARG_NOT_INIT){
		_args->m_isCreate = DEFAULT_IS_CREATE;
	}

	if(_args->m_isDelete ==  ARG_NOT_INIT){
		_args->m_isDelete = DEFAULT_IS_DELETE;
	}
	
}

/*-----------------------------------------------------------------*/

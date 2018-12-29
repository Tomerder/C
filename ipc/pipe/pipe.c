/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 06.02.14                                                                                              
*    LAST MODIFIED: 06.02.14                                                                                  
*    DESCRIPTION: buffer	 
*****************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/types.h> 

/*-----------------------------------------------------------------*/

#define PRT_SIG_P "parent sigusr1 Handler!!!!!!!!!\n"
#define PRT_SIG_C "child sigusr2 Handler!!!!!!!!!\n"
#define MSG_LEN strlen(PRT_SIG_P)

#define PRINT_S(MSG) write(0, MSG, MSG_LEN) ; fflush(0);  
#define PRINT(MSG) printf(MSG) ; fflush(0);
#define PRINT_N(MSG,N) printf(MSG) ; fflush(0); printf("%d\n",N) ; fflush(0);
#define PRINT_STR(MSG,STR) printf(MSG) ; fflush(0); printf("%s\n",STR) ; fflush(0);

/*-----------------------------------------------------------------*/
enum{READ, WRITE};
enum{FALSE, TRUE};

enum{ERR_PIPE_CREATE=1};

#define ARG_NOT_INIT -1

#define DEFAULT_VERBOSE TRUE
#define DEFAULT_N_OF_MSG 10
#define DEFAULT_P_WRK_TIME 1000000
#define DEFAULT_C_WRK_TIME 1000000
#define DEFAULT_READ_BUF_SIZE 1

#define MICROSEC_MUL_RES 1000

/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_verbose;
	int m_numOfMsgs;
	int m_workTimeP;
	int m_workTimeC;
	int m_readBufferSize;
}Args;

/*-----------------------------------------------------------------*/

void Parent(int _childId, int* _pipefd);
void Child(int* _pipefd);

void Establish();

void GetArgs(int argc ,char** argv, Args* args);

/*-----------------------------------------------------------------*/

volatile sig_atomic_t parentSigNum;

Args args = {ARG_NOT_INIT, ARG_NOT_INIT, ARG_NOT_INIT, ARG_NOT_INIT, ARG_NOT_INIT} ;

/*-----------------------------------------------------------------*/

void sigusr1Handler(int sig)   /*release parent from wait*/
{
	PRINT_S(PRT_SIG_P);	
}

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	int pid;
	int pipefd[2];

	GetArgs(argc ,argv, &args);
	
	/*create pipe*/
	if (pipe(pipefd) == -1) {
		perror("pipe");
		return ERR_PIPE_CREATE;
	}	

	/*fork*/
    pid = fork();

	if(pid < 0){               
		printf("fork error\n");
	}else if(pid > 0){           /*   n>0  parent    */  
		Parent(pid, pipefd);
	}else{				         /*   n=0  child    */
		Child(pipefd);  		
	}
		
	PRINT("finish prog\n");
	
	return 0;
}

/*-----------------------------------------------------------------*/

void Parent(int _childId, int* _pipefd)
{
	int i;

	PRINT("start parent\n");
	PRINT_N("Parent PID is ",  getpid() );
	
	close(_pipefd[READ]);  	

	for(i='A' ; i < args.m_numOfMsgs + 'A' ; i++){
		usleep(args.m_workTimeP);   /*work*/

		write(_pipefd[WRITE], (char*)&i , 1);	
	}

	close(_pipefd[WRITE]); 
	
	/*wait for child to finish reading - 
      for dealing with situation which child reads faster then parent is writing 
    */
	wait(NULL);

	PRINT("END parent\n");
}

/*-----------------------------------------------------------------*/

void Child(int* _pipefd)
{
	int readRet;
	char buf[args.m_readBufferSize+1];

	PRINT("start child\n");
	PRINT_N("child PID is ",  getpid() );

	close(_pipefd[WRITE]);  
	
	while(TRUE){
		readRet = read(_pipefd[READ], &buf, args.m_readBufferSize);
		/*buf[args.m_readBufferSize] = '\0';*/
		
		if ( readRet > 0){
			PRINT("child read : ");
			/*write(0, buf , args.m_readBufferSize);
			printf("\n");*/
			printf("%s\n", buf);
			
			usleep(args.m_workTimeC);  /*work*/
		}else{ 
			break;
		}
	}

	close(_pipefd[READ]);  

	PRINT("END child\n");
}

/*-----------------------------------------------------------------*/

void Establish(int _sigNum, void* _handler)
{
	struct sigaction sa;

	PRINT_N("Establish ",_sigNum);

	/*initialize sig action*/
    sa.sa_handler = _handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
	
	/*set sig action SIGUSR1 = kill*/
    if (sigaction(_sigNum, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv, Args* _args)
{
	int argType;
	
	/*
	_args->m_parentSigNum =  ARG_NOT_INIT;
	*/

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "v:n:p:c:s:")) != -1)
	{
		switch(argType)
		{
			case 'v':
				_args->m_verbose = TRUE;
				break;	
			case 'n':
				_args->m_numOfMsgs = atoi(optarg);
				break;	
			case 'p':
				_args->m_workTimeC = atoi(optarg) * MICROSEC_MUL_RES;
				break;	
			case 'c':
				_args->m_workTimeP = atoi(optarg) * MICROSEC_MUL_RES;
				break;			
			case 's':
				_args->m_readBufferSize = atoi(optarg);
				break;			
			default:				
				break;
		}
	}
	
	/*if wasnt set -> set default values*/
	if(_args->m_verbose ==  ARG_NOT_INIT){
		_args->m_verbose = DEFAULT_VERBOSE;
	}

	if(_args->m_numOfMsgs ==  ARG_NOT_INIT){
		_args->m_numOfMsgs = DEFAULT_N_OF_MSG;
	}

	if(_args->m_workTimeC ==  ARG_NOT_INIT){
		_args->m_workTimeC = DEFAULT_C_WRK_TIME;
	}

	if(_args->m_workTimeP ==  ARG_NOT_INIT){
		_args->m_workTimeP = DEFAULT_P_WRK_TIME;
	}

	if(_args->m_readBufferSize ==  ARG_NOT_INIT){
		_args->m_readBufferSize = DEFAULT_READ_BUF_SIZE;
	}
	
}

/*-----------------------------------------------------------------*/

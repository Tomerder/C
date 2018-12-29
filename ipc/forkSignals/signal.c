/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 05.02.14                                                                                              
*    LAST MODIFIED: 05.02.14                                                                                  
*    DESCRIPTION: signals - parent release child from pause -> child release parent from pause		 
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

/*-----------------------------------------------------------------*/

#define ARG_NOT_INIT -1

#define PARENT_DEFAULT_SIG_NUM SIGUSR1
#define CHILD_DEFAULT_SIG_NUM SIGUSR2
#define USLEEP_DEFAULT_VAL 1000

/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_childSigNum;
	int m_parentSigNum;
	int m_uSleepVal;
}Args;


/*-----------------------------------------------------------------*/
void Parent(int _childId, Args* args);
void Child(Args* args);

void Establish();

void GetArgs(int argc ,char** argv, Args* args);

/*-----------------------------------------------------------------*/

volatile sig_atomic_t parentSigNum;

/*-----------------------------------------------------------------*/

void sigusr1Handler(int sig)   /*release parent from wait*/
{
	PRINT_S(PRT_SIG_P);	
}

/*-----------------------------------------------------------------*/

void sigusr2Handler(int sig)   /*release child from wait*/ 
{
	PRINT_S(PRT_SIG_C);	

	/*to ensure that parent will be paused before sending signal*/
	PRINT("sleep in child signal handler - to ensure that parent will be paused before sending signal\n");
	usleep(100);

	/*send signal to parent - release parent from wait*/	
	if(  kill( getppid() , SIGUSR1) == 0 ){
		PRINT("signal was sent from child sig handler to parent\n");
	}
}


/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	int id;
	Args args;
	
	GetArgs(argc ,argv, &args);
	
	/*Establish(SIGUSR1, sigusr1Handler);*/
	/*Establish(SIGUSR2, sigusr2Handler);*/

    id = fork();

	if(id < 0){               
		printf("fork error\n");
	}else if(id > 0){           /*   n>0  parent    */  
		Parent(id, &args);
	}else{				      /*   n=0  child    */
		Child(&args);  		
	}
		
	PRINT("finish prog\n");
	
	return 0;
}

/*-----------------------------------------------------------------*/

void Parent(int _childId, Args* _args)
{
	PRINT("start parent\n");
	PRINT_N("Parent PID is ",  getpid() );
	
	/*sleep is neccessery to ensure that child will be paused before signal is sent -> otherwise child will stay paused*/
	/*min sleep is about 75*/ 
	PRINT("sleep in parent - to ensure that child will be paused before signal is sent\n");
	usleep(_args->m_uSleepVal);	
	
	Establish(_args->m_parentSigNum, sigusr1Handler);
	
	/*set global parentSigNum for sending signal from child sig handler*/
	parentSigNum = _args->m_parentSigNum;

	/*send signal to child*/
	if(kill(_childId, _args->m_childSigNum) == 0){
		PRINT("signal was sent from parent to child\n");
	}

	PRINT("parent paused\n");
	pause(); 
	PRINT("parent resumed by signal from child\n");

	PRINT("END parent\n");
}

/*-----------------------------------------------------------------*/

void Child(Args* _args)
{
	PRINT("start child\n");
	PRINT_N("child PID is ",  getpid() );

	Establish(_args->m_childSigNum, sigusr2Handler);

	PRINT("child paused\n");
	pause(); 
	PRINT("child resumed by signal from parent\n");

	/*send signal to parent*/
	/*kill( getppid() , _args->m_parentSigNum);*/

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
	int arg;
	
	_args->m_parentSigNum =  ARG_NOT_INIT;
	_args->m_childSigNum =  ARG_NOT_INIT;
	_args->m_uSleepVal =  ARG_NOT_INIT;
	
	/*set args from agrv*/	
	while ((arg = getopt(_argc, _argv, "c:p:s:")) != -1)
	{
		switch(arg)
		{
			case 'c':
				_args->m_childSigNum = atoi(optarg);
				break;	
			case 'p':
				_args->m_parentSigNum= atoi(optarg);
				break;	
			case 's':
				_args->m_uSleepVal = atoi(optarg);
				break;		
			default:				
				break;
		}
	}
	
	/*if wasnt set -> set default values*/
	if(_args->m_parentSigNum ==  ARG_NOT_INIT){
		_args->m_parentSigNum = PARENT_DEFAULT_SIG_NUM;
	}

	if(_args->m_childSigNum ==  ARG_NOT_INIT){
		_args->m_childSigNum = CHILD_DEFAULT_SIG_NUM;
	}

	if(_args->m_uSleepVal ==  ARG_NOT_INIT){
		_args->m_uSleepVal = USLEEP_DEFAULT_VAL;
	}
}

/*-----------------------------------------------------------------*/

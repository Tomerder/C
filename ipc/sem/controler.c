/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 06.02.14                                                                                              
*    LAST MODIFIED: 06.02.14                                                                                  
*    DESCRIPTION: Message Queue
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
#include <sys/msg.h>
#include <sys/sem.h>

/*-----------------------------------------------------------------*/

#define DEFAULT_N_OF_MSG 20
#define DEFAULT_FILE "sem"
#define DEFAULT_WRK_TIME 1000000

#define FILE_ID 'A'

#define MSGGET_FLAG 0666

enum{FALSE, TRUE};

#define TXT "message num. %d => sender pid %d\n"

#define CHKERR(FUNC_RET) if(FUNC_RET != OK) { printf("ERROR %d\n",FUNC_RET); return FUNC_RET; }

enum{OK , ERR_FTOK , ERR_SEMGET,  ERR_SEMCTL , ERR_SEMOP , ERR_ARGS};

enum{DOWN = -1, UP = 1};
/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_isPause;
	int m_isResume;
}Args;

int GetArgs(int argc ,char** argv);

int DoSemOp(int _semid , int _op);

/*-----------------------------------------------------------------*/

Args args = {FALSE}; 

/*-----------------------------------------------------------------*/

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int semid , err;
    key_t key;
	
	err = GetArgs(argc ,argv);
	CHKERR(err);
	
	/*get sem*/
	if ((key = ftok(DEFAULT_FILE, FILE_ID)) == -1) {
        perror("ftok");
        return ERR_FTOK;
    }

	if ((semid = semget(key, 1 ,  MSGGET_FLAG)) == -1) {
        perror("semget");
        return ERR_SEMGET;
    }
    
    /*execute pause/resume*/
    if(args.m_isResume){
   		err = DoSemOp(semid, UP);
   		CHKERR(err);
   	}
   	
   	if(args.m_isPause){
   		err = DoSemOp(semid, DOWN);
   		CHKERR(err);
   	}
			
	return OK;
}

/*-----------------------------------------------------------------*/

int DoSemOp(int _semid , int _op)
{
	struct sembuf semBuf;
	
	semBuf.sem_num = 0;
    semBuf.sem_flg = 0;
	semBuf.sem_op = _op;
   
    if (semop(_semid, &semBuf, 1) == -1) {
	    perror("semop");
	    return ERR_SEMOP;
	}
	
	 return OK;
}

/*-----------------------------------------------------------------*/

int GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "pr")) != -1)
	{
		switch(argType)
		{
			case 'p':
				args.m_isPause = TRUE;
				break;	
			case 'r':
				args.m_isResume = TRUE;
				break;					
			default:
				return ERR_ARGS;
		}
	}	
	
	return OK;
}

/*-----------------------------------------------------------------*/

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

enum{OK , ERR_FTOK , ERR_SEMGET,  ERR_SEMCTL , ERR_SEMOP };

enum{DOWN = -1, UP = 1};
/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_numOfMsgs;
}Args;

/*-----------------------------------------------------------------*/

void GetArgs(int argc ,char** argv);

int WriteMsgs(int _msqid);

/*-----------------------------------------------------------------*/

Args args = { DEFAULT_N_OF_MSG } ;

/*-----------------------------------------------------------------*/

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int semid ,err;
    key_t key;
    union semun arg;

	GetArgs(argc ,argv);
	
	/*get sem*/
	if ((key = ftok(DEFAULT_FILE, FILE_ID)) == -1) {
        perror("ftok");
        return ERR_FTOK;
    }

	if ((semid = semget(key, 1 ,  MSGGET_FLAG | IPC_CREAT | IPC_EXCL)) == -1) {
        perror("semget");
        return ERR_SEMGET;
    }
    
    /*set sem counter = 1 */
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl");
        return ERR_SEMCTL;
    }

    /*write to screen*/
    err = WriteMsgs(semid);
    CHKERR(err);
		
	/*remove sem*/
	if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        perror("semctl");
        return ERR_SEMCTL;
    }
	
	return OK;
}


/*-----------------------------------------------------------------*/


int WriteMsgs(int _semid)
{
	int i, pid;
	struct sembuf semBuf;
	
	semBuf.sem_num = 0;
    semBuf.sem_flg = 0;
	
	pid = getpid();
	
	for(i=0; i < args.m_numOfMsgs; i++)
	{
		/*sem down*/
		semBuf.sem_op = DOWN;  
		
		if (semop(_semid, &semBuf, 1) == -1) {
		    perror("semop");
		    return ERR_SEMOP;
   		}
	
		/*print to screen*/
		printf(TXT , i + 1 , pid);     
    	
    	/*sem up*/
    	semBuf.sem_op = UP;  
		
		if (semop(_semid, &semBuf, 1) == -1) {
		    perror("semop");
		    return ERR_SEMOP;
   		}
          
        /*sleep*/  
      	usleep(DEFAULT_WRK_TIME);          
    }

	return OK;
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "f:n:")) != -1)
	{
		switch(argType)
		{
			case 'n':
				args.m_numOfMsgs = atoi(optarg);
				break;					
			default:				
				break;
		}
	}
	
	
	
}

/*-----------------------------------------------------------------*/

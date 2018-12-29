/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 09.02.14                                                                                              
*    LAST MODIFIED: 09.02.14                                                                                  
*    DESCRIPTION: shared memory + sem
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
#include <sys/shm.h>

#include "shared.h"

/*-----------------------------------------------------------------*/

#define TXT "message num. %d => sender pid %d\n"

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

void GetArgs(int argc ,char** argv);

int WriteToSharedMem(char* _mem, int _semId);

int RegPing(int _msqid);

int UnRegPing(int _semId, int _msqid);

/*-----------------------------------------------------------------*/

Args args = {DEFAULT_IS_VERBOSE, "", DEFAULT_WRK_TIME, DEFAULT_N_OF_MSG, DEFAULT_IS_CREATE, DEFAULT_IS_DELETE } ;

/*-----------------------------------------------------------------*/

Msg msgSharedMem; 

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int shMemId , semId , shmgetFlags = NO_FLAGS ,err;
	char* mem = NULL;


	GetArgs(argc ,argv);
	
	PRINT_D_V("Producer PID is %d\n",  getpid() );
	
	/*init shared mem*/
	if(args.m_isCreate){
		shmgetFlags = IPC_CREAT | IPC_EXCL;
	}
	err =  InitSharedMem(&shMemId , args.m_file , sizeof(Msg) ,  shmgetFlags );
	CHKERR(err);
	
	/*attach*/
	mem = shmat(shMemId, NULL , NO_FLAGS);
	
	/*first initialzation of numOfMsgs*/
	if(args.m_isCreate){
		MEM_GET_NO_OF_MSGS(mem) = 0;
	}

	/*init sem*/
	err =  InitSem(&semId, args.m_file , IPC_CREAT, args.m_isCreate);
	CHKERR(err);

    
    /*write to shared mem*/
    err = WriteToSharedMem(mem, semId);
    CHKERR(err);
    
		
	/*detach from shared mem*/	
	if (shmdt(mem) == -1) {
        perror("shmdt");
        return ERR_SHMDT;
    }

	/*remove queue and shared mem*/
	if(args.m_isDelete){
		err = RmvSharedMemAndSem(shMemId , semId);
		CHKERR(err);
	}
		
	PRINT_V("PING ENDED\n");
	
	return OK;
}

/*-----------------------------------------------------------------*/

int RegPing(int _semId)
{
	PRINT_V("PING register\n"); 
	
	return OK;
}

/*-----------------------------------------------------------------*/

int UnRegPing(int _semId, int _msqid)
{
	
	PRINT_V("PING unregister\n"); 
	
	return OK;
}

/*-----------------------------------------------------------------*/

int WriteToSharedMem(char* _mem, int _semId)
{
	int err ,i, pid;
	
	pid = getpid();
	
	PRINT_V("start writing...\n");

	/*write Num of Msgs*/
	MEM_GET_NO_OF_MSGS(_mem) += args.m_numOfMsgs; 

	for(i=0; i < args.m_numOfMsgs; i++){
		/*work*/
		usleep(args.m_workTime); 
		
		/*wait until get notification from pong => finished reading last msg*/
		err = SemOp(_semId , NOTIFY_PING_SEM , DOWN , NO_FLAGS );
		CHKERR(err);
	
		/*write msg*/
		sprintf( msgSharedMem.m_text , TXT , i + 1 , pid);
        
        strcpy( MEM_GET_TXT(_mem) , msgSharedMem.m_text);
                    
        PRINT_S_V("writing: %s\n", msgSharedMem.m_text);
        
        /*notify pong can read next msg*/
  		err = SemOp(_semId , NOTIFY_PONG_SEM , UP , NO_FLAGS );
  		CHKERR(err);
    }
    
    
  
    
	PRINT_V("finished writing\n");

	return OK;
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "vf:s:n:cd")) != -1)
	{
		switch(argType)
		{
			case 'v':
				args.m_verbose = TRUE;
				break;	
			case 'f':
				strcpy(args.m_file , optarg);
				break;	
			case 's':
				args.m_workTime = atoi(optarg) * MICROSEC_MUL_RES;
				break;	
			case 'n':
				args.m_numOfMsgs = atoi(optarg);
				break;					
			case 'c':
				args.m_isCreate = TRUE;
				break;	
			case 'd':
				args.m_isDelete = TRUE;
				break;			
			default:				
				break;
		}
	}
	
	/*if wasnt set -> set default values*/
	if( strcmp (args.m_file, "") == 0){
		strcpy(args.m_file , DEFAULT_FILE);
	}
	
}

/*-----------------------------------------------------------------*/

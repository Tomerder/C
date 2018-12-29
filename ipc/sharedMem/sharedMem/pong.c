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

typedef struct Args{
	int m_verbose;
	char m_file[MAX_FILE_NAME];
	int m_workTime;
}Args;

/*-----------------------------------------------------------------*/

void GetArgs(int argc ,char** argv, Args* args);

int ReadFromSharedMem(char* _mem , int _semId);

int RegPong(int _msqid);

int UnRegPong(int _semId, int _msqid);

/*-----------------------------------------------------------------*/

Args args = {DEFAULT_IS_VERBOSE, "", DEFAULT_WRK_TIME} ;

/*-----------------------------------------------------------------*/

Msg msgSharedMem; 

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int shMemId , semId ,err;
    char* mem = NULL;

	GetArgs(argc ,argv, &args);
	
	PRINT_D_V("Consumer PID is %d\n",  getpid() );
	
	/*init shared mem*/
	err =  InitSharedMem(&shMemId , args.m_file , sizeof(Msg) ,  NO_FLAGS );
	CHKERR(err);
	
	/*attach*/
	mem = shmat(shMemId, NULL , NO_FLAGS);
	
	/*init sem*/
	err =  InitSem(&semId, args.m_file ,NO_FLAGS, FALSE);
	CHKERR(err);

    /*read from shared mem*/
    err = ReadFromSharedMem(mem , semId);
    CHKERR(err);
    
    /*detach from shared mem*/	
	if (shmdt(mem) == -1) {
        perror("shmdt");
        return ERR_SHMDT;
    }
				
	PRINT_V("PONG ENDED\n");
	
	return 0;
}

/*-----------------------------------------------------------------*/
int RegPong(int _semId)
{	
	PRINT_V("PONG register\n"); 

	return OK;
}

/*-----------------------------------------------------------------*/

int UnRegPong(int _semId, int _msqid)
{
	PRINT_V("PONG unregister\n"); 

	return OK;
}
/*-----------------------------------------------------------------*/

int ReadFromSharedMem(char* _mem , int _semId)
{
	int err, numOfMsgRcv = 0;
		
	PRINT_V("start reading...\n");
	
	
	while(TRUE)
	{
		/*if no msgs left => exit*/
		if( MEM_GET_NO_OF_MSGS(_mem) == 0 ){
			break;
		}
		
		/* mark msg has been read => numOfMsgs-- */
        --MEM_GET_NO_OF_MSGS(_mem);
	
		/*wait until get notification from ping => finished writing next msg*/
		err = SemOp(_semId , NOTIFY_PONG_SEM , DOWN , NO_FLAGS );
		CHKERR(err);
		
		/*reading*/        
        ++numOfMsgRcv;
        PRINT_S_V("reading: %s\n", MEM_GET_TXT(_mem));  
        
        /*work*/
        usleep(args.m_workTime);      
        
        /*notify ping can write next msg*/
  		err = SemOp(_semId , NOTIFY_PING_SEM , UP , NO_FLAGS );
  		CHKERR(err);
    }

	PRINT_V("finished reading\n");
	PRINT_D_V("%d messages were received\n", numOfMsgRcv);	

	return OK;
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv, Args* _args)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "vf:s:")) != -1)
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
	if( strcmp (_args->m_file, "") == 0){
		strcpy(_args->m_file , DEFAULT_FILE);
	}
	
}

/*-----------------------------------------------------------------*/

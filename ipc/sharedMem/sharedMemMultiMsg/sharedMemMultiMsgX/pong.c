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

int Init(int* _shMemId , char** _mem ,int* _semId);

int ReadFromSharedMem(char* _mem , int _semId);

int Clean( char* _mem);

/*-----------------------------------------------------------------*/

Args args = {DEFAULT_IS_VERBOSE, "", DEFAULT_WRK_TIME} ;

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int shMemId , semId ,err;
    char* mem = NULL;

	GetArgs(argc ,argv, &args);
	
	PRINT_D_V("Consumer PID is %d\n",  getpid() );
	
	/*init*/
	err = Init(&shMemId , &mem ,&semId);
	CHKERR(err);
	
    /*read from shared mem*/
    err = ReadFromSharedMem(mem , semId);
    CHKERR(err);

    /*clean*/
  	err = Clean(mem);
	CHKERR(err);
				
	PRINT_V("PONG ENDED\n");
	
	return 0;
}

/*-----------------------------------------------------------------*/

int Init(int* _shMemId , char** _mem ,int* _semId)
{
	int err;

	/*init shared mem*/
	err =  InitSharedMem(_shMemId , args.m_file , sizeof(Queue) ,  NO_FLAGS );
	CHKERR(err);
	
	/*attach*/
	*_mem = shmat(*_shMemId, NULL , NO_FLAGS);

	/*init sem*/
	err =  InitSem(_semId, args.m_file ,NO_FLAGS, FALSE);
	CHKERR(err);

	return OK;
}

/*-----------------------------------------------------------------*/

int ReadFromSharedMem(char* _mem , int _semId)
{
	int err, numOfMsgRcv = 0 , indexReadFrom = 0;
		
	PRINT_V("start reading...\n");
	
	
	while(TRUE)
	{
		/*if no msgs to read -> exit*/
		err = SemOp(_semId , COUNT_TOT_MSGS_SEM , DOWN , IPC_NOWAIT );
		if(err == -1){
			break;
		}
		
		/*-------------------------------------------------*/
		/*check if empty and dec empty count*/
		err = SemOp(_semId , EMPTY_SEM , DOWN , NO_FLAGS );
		CHKERR(err);

		/*lock*/
		err = SemOp(_semId , LOCK_SEM , DOWN , NO_FLAGS );
		CHKERR(err);		
		/*-------------------------------------------------*/

		/*reading*/  
		indexReadFrom = MEM_GET_HEAD_INDEX(_mem) ;     
      
        PRINT_S_V("reading: %s\n", MEM_GET_MSG(_mem,indexReadFrom)); 
        
        INC_INDEX_CIRCULAR( MEM_GET_HEAD_INDEX(_mem) ); 
 
		++numOfMsgRcv;
		
        /*-------------------------------------------------*/       
        /*unlock*/
        err = SemOp(_semId , LOCK_SEM , UP , NO_FLAGS );
		CHKERR(err);
        
        /*dec full count (up to full sem)*/      
  		err = SemOp(_semId , FULL_SEM , UP , NO_FLAGS );
  		CHKERR(err);
  		/*-------------------------------------------------*/       
        
        /*work*/
        usleep(args.m_workTime);      
    }

	PRINT_V("finished reading\n");
	PRINT_D_V("%d messages were received\n", numOfMsgRcv);	

	return OK;
}

/*-----------------------------------------------------------------*/

int Clean( char* _mem)
{
	/*detach from shared mem*/	
	if (shmdt(_mem) == -1) {
        perror("shmdt");
        return ERR_SHMDT;
    }

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

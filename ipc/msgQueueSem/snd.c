/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 09.02.14                                                                                              
*    LAST MODIFIED: 09.02.14                                                                                  
*    DESCRIPTION: Message Queue + sem
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

#include "msgQueueSem.h"

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

int WriteToMsgQ(int _msqid);

int RegPing(int _msqid);

int UnRegPing(int _semId, int _msqid);

/*-----------------------------------------------------------------*/

Args args = {DEFAULT_IS_VERBOSE, "", DEFAULT_WRK_TIME, DEFAULT_N_OF_MSG, DEFAULT_IS_CREATE, DEFAULT_IS_DELETE } ;

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int msqid , semId , msggetFlags = NO_FLAGS ,err;

	GetArgs(argc ,argv);
	
	PRINT_D_V("Producer PID is %d\n",  getpid() );
	
	/*init queue*/
	if(args.m_isCreate){
		msggetFlags = IPC_CREAT | IPC_EXCL;
	}
	err =  InitMsgQueue(&msqid, args.m_file ,msggetFlags );
	CHKERR(err);

	/*init sem*/
	err =  InitSem(&semId, args.m_file , IPC_CREAT);
	CHKERR(err);

	/*register ping*/
	err = RegPing(semId);
	CHKERR(err);
    
    /*write to queue*/
    err = WriteToMsgQ(msqid);
    CHKERR(err);
    
    /*ping unregister => if last send EOF*/
    err = UnRegPing(semId, msqid);
    CHKERR(err);
		
	/*remove queue and sem*/
	if(args.m_isDelete){
		/*wait until no pongs*/
		SemOp(semId, PONG_SEM, WAIT_FOR_0 , NO_FLAGS);
		err = RmvMsgQueueAndSem(msqid , semId);
		CHKERR(err);
	}
		
	PRINT_V("PING ENDED\n");
	
	return OK;
}

/*-----------------------------------------------------------------*/

int RegPing(int _semId)
{
	PRINT_V("PING register\n"); 
	
	return SemOp(_semId, PING_SEM, UP , NO_FLAGS);
}

/*-----------------------------------------------------------------*/

int UnRegPing(int _semId, int _msqid)
{
	int err, numOfPings ; 
	
	PRINT_V("PING unregister\n"); 
	
	err = SemOp(_semId, PING_SEM, DOWN , NO_FLAGS);
	CHKERR(err);
	
    /*check if this is the last ping => send EOF */
    err = CountOfSem(_semId , PING_SEM ,&numOfPings);
	CHKERR(err);
	
	if(numOfPings == 0)
	{
		PRINT_V("LAST PING SEND EOF!!!!!\n");
		SendEOF(_msqid);
	}
	
	return OK;
}

/*-----------------------------------------------------------------*/

int WriteToMsgQ(int _msqid)
{
	Buf bufSnd, bufAck;
	int i, pid;
	
	pid = getpid();
	
	bufSnd.mtype = SND_TYPE;
	bufAck.mtype = ACK_TYPE;
	
	PRINT_V("start writing...\n");

	for(i=0; i < args.m_numOfMsgs; i++){
		/*work*/
		usleep(args.m_workTime); 
	
		/*send msg - without '\0'*/
		sprintf(bufSnd.mtext , TXT , i + 1 , pid);
        
        if (msgsnd(_msqid, &bufSnd, strlen(bufSnd.mtext) , NO_FLAGS) == -1){ 
            perror("msgsnd");
        }      
          
        PRINT_S_V("writing: %s\n", bufSnd.mtext);
        
        /*wait for ack*/  
	    if (msgrcv(_msqid, &bufAck, sizeof(bufAck.mtext), bufAck.mtype , NO_FLAGS) == -1){
			perror("msgrcv");
		    return ERR_MSGRCV;
	    }  
		PRINT_V("ack recieved\n");   
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

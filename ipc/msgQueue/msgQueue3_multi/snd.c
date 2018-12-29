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

#include "msgQueue.h"

/*-----------------------------------------------------------------*/

enum{FALSE, TRUE};

#define ARG_NOT_INIT -1

#define TXT "message num. %d => sender pid %d\n"

/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_verbose;
	char m_file[MAX_FILE_NAME];
	int m_workTime;
	int m_numOfMsgs;
	int m_isCreate;
	int m_isDelete;
	int m_isMsgEOF;
}Args;

/*-----------------------------------------------------------------*/

void GetArgs(int argc ,char** argv, Args* args);

int WriteToMsgQ(int _msqid);

int RegPing(int _msqid);

int UnRegPing(int _msqid);

/*-----------------------------------------------------------------*/

Args args = {ARG_NOT_INIT, "", ARG_NOT_INIT, ARG_NOT_INIT, ARG_NOT_INIT, ARG_NOT_INIT , ARG_NOT_INIT } ;

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int msqid , msggetFlags = 0;
    key_t key;

	GetArgs(argc ,argv, &args);
	
	PRINT_D_V("Producer PID is %d\n",  getpid() );
	
	/*open queue*/
	if ((key = ftok(args.m_file, FILE_ID)) == -1) {
        perror("ftok");
        return ERR_FTOK;
    }

	if(args.m_isCreate){
		msggetFlags = IPC_CREAT | IPC_EXCL;
		PRINT_V("msg queue was created\n");
	}

	if ((msqid = msgget(key, MSGGET_FLAG | msggetFlags)) == -1) {
        perror("msgget");
        return ERR_MSGGET;
    }

	/*if didnt create the queue - register */
	if(!args.m_isCreate){		
		CHKERR( RegPing(msqid) );
	}
    
    /*write to queue*/
    CHKERR( WriteToMsgQ(msqid) );

	/*unregister ping*/
	CHKERR( UnRegPing(msqid) );
		
	/*remove queue*/
	if(args.m_isDelete){
		if (msgctl(msqid, IPC_RMID, NULL) == -1) {
		    perror("msgctl");
		    return ERR_MSGCTL;
    	}
    	PRINT_V("msg queue was removed\n");
	}		
		
	PRINT_V("PING ENDED\n");
	
	return 0;
}

/*-----------------------------------------------------------------*/

int RegPing(int _msqid)
{
	Buf bufReg;

	bufReg.mtype = PING_CNT_TYPE;

	if (msgsnd(_msqid, &bufReg, strlen(bufReg.mtext) , 0) == -1){ 
        perror("msgsnd");
		return ERR_MSGSND;
    }  

	PRINT_V("PING registered\n"); 
	
	return OK;
}

/*-----------------------------------------------------------------*/

int UnRegPing(int _msqid)
{
	Buf bufSnd, bufReg , bufPong;

	bufReg.mtype = PING_CNT_TYPE;
	bufPong.mtype = PONG_CNT_TYPE;
	bufSnd.mtype = SND_TYPE;
	
	/*unregister : pingCount-- => check if last ping*/
	if( msgrcv(_msqid, &bufReg, sizeof(bufReg.mtext), bufReg.mtype, IPC_NOWAIT) == -1)
	{		
		/*send EOF by number of pongs that are registered to msg queue*/
		while( msgrcv(_msqid, &bufPong, sizeof(bufPong.mtext), bufPong.mtype, IPC_NOWAIT) != -1 )
		{
			PRINT_V("Sending EOF\n"); 
			
			strcpy(bufSnd.mtext , EOF_SIGN );   /*send EOF as special sign */		

			if (msgsnd(_msqid, &bufSnd, strlen(bufSnd.mtext), 0) == -1){
				perror("msgsnd");
			}
		}
	}else{
		PRINT_V("PING unregistered\n"); 
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
	
	PRINT_V("start writing\n");
	
	for(i=0; i < args.m_numOfMsgs; i++){
		/*work*/
		usleep(args.m_workTime); 
	
		/*send msg - without '\0'*/
		sprintf(bufSnd.mtext , TXT , i + 1 , pid);
        
        if (msgsnd(_msqid, &bufSnd, strlen(bufSnd.mtext) , 0) == -1){ 
            perror("msgsnd");
        }      
          
        PRINT_S_V("writing: %s\n", bufSnd.mtext);
        
        /*wait for ack*/  
	    if (msgrcv(_msqid, &bufAck, sizeof(bufAck.mtext), bufAck.mtype , 0) == -1){
			perror("msgrcv");
		    return ERR_MSGRCV;
	    }  
		PRINT_V("ack recieved\n");   
    }
    
	PRINT_V("finished writing\n");

	return OK;
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv, Args* _args)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "vf:s:n:cde")) != -1)
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
			case 'e':
				_args->m_isMsgEOF = TRUE;
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
	
	if(_args->m_isMsgEOF ==  ARG_NOT_INIT){
		_args->m_isMsgEOF = DEFAULT_IS_MSG_EOF;
	}
	
}

/*-----------------------------------------------------------------*/

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

/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_verbose;
	char m_file[MAX_FILE_NAME];
	int m_workTime;
	int m_isMsgEOF;
}Args;

/*-----------------------------------------------------------------*/

void GetArgs(int argc ,char** argv, Args* args);

int ReadFromMsgQ(int _pipefd);

int RegPong(int _msqid);

int UnRegPong(int _msqid);

/*-----------------------------------------------------------------*/

Args args = {ARG_NOT_INIT, "", ARG_NOT_INIT, ARG_NOT_INIT} ;

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int msqid ,err;
    key_t key;

	GetArgs(argc ,argv, &args);
	
	PRINT_D_V("Consumer PID is %d\n",  getpid() );
	
	/*open queue*/
	if ((key = ftok(args.m_file, FILE_ID)) == -1) {
        perror("ftok");
        return ERR_FTOK;
    }

	if ((msqid = msgget(key, MSGGET_FLAG )) == -1) {
        perror("msgget");
        return ERR_MSGGET;
    }
    
	/*register pong*/
	err = RegPong(msqid);
	CHKERR(err);

    /*read from queue*/
    err = ReadFromMsgQ(msqid);
    CHKERR(err);
    
    /*unregister pong*/
    err =  UnRegPong(msqid) ;
	CHKERR(err);
				
	PRINT_V("PONG ENDED\n");
	
	return 0;
}

/*-----------------------------------------------------------------*/

int RegPong(int _msqid)
{
	Buf bufReg;

	bufReg.mtype = PONG_CNT_TYPE;

	if (msgsnd(_msqid, &bufReg, strlen(bufReg.mtext) , 0) == -1){ 
        perror("msgsnd");
		return ERR_MSGSND;
    } 

	PRINT_V("PONG registered\n"); 

	return OK;
}

/*-----------------------------------------------------------------*/

int UnRegPong(int _msqid)
{
	Buf bufReg;

	bufReg.mtype = PONG_CNT_TYPE;
	
	/*unregister : pingCount-- => check if last ping*/
	if( msgrcv(_msqid, &bufReg, sizeof(bufReg.mtext), bufReg.mtype, 0) == -1){		
		perror("msgrcv");
		return ERR_MSGRCV;
	}else{
		PRINT_V("PONG unregistered\n"); 
	}

	return OK;
}

/*-----------------------------------------------------------------*/

int ReadFromMsgQ(int _msqid)
{
	Buf bufRead , bufAck;
	int numOfMsgRcv = 0 , numOfBytesRcv = 0;
	
	bufRead.mtype = 0;
	
	bufAck.mtype = ACK_TYPE;
	strcpy(bufAck.mtext, ACK_SIGN);
	
	PRINT_V("start reading\n");
		
	while(TRUE){
		/*reading - regular channel and special channel*/
		if ( (numOfBytesRcv = (msgrcv(_msqid, &bufRead, sizeof(bufRead.mtext), (-1) * PING_CNT_TYPE, IPC_NOWAIT))) == -1) 
		{
			/*no msgs and no pings => finish work*/
			PRINT_V("no msgs and no pings\n");
            break;
        }
        else if(bufRead.mtype == PING_CNT_TYPE)
        {
        	/*no msgs but there are pings => send back the ping reg msg*/
        	PRINT_V("no msgs push back ping reg msg to queue\n");
        	if (msgsnd(_msqid, &bufRead, strlen(bufRead.mtext) , 0) == -1){
		        perror("msgsnd");
		        return ERR_MSGSND;
        	}  
        	/*sleep to give pong a chance to write msg */
        	usleep(args.m_workTime);
        	continue; 	
        }
        
        ++numOfMsgRcv;
        bufRead.mtext[numOfBytesRcv] = '\0';  /*add null for printing msg*/
        PRINT_S_V("reading: %s\n", bufRead.mtext);
        
        /*work*/
        usleep(args.m_workTime);  
        
        /*send ack*/
        PRINT_V("sending ack\n");
        if (msgsnd(_msqid, &bufAck, strlen(bufAck.mtext) , 0) == -1){
            perror("msgsnd");
            return ERR_MSGSND;
        }
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
	while ((argType = getopt(_argc, _argv, "vf:s:e")) != -1)
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
	
	if(_args->m_isMsgEOF ==  ARG_NOT_INIT){
		_args->m_isMsgEOF = DEFAULT_IS_MSG_EOF;
	}
	
}

/*-----------------------------------------------------------------*/

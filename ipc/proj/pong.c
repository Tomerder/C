/**************************************************************************************
    Author : Tomer Dery
    Creation date :      20.02.14    
    Date last modified : 20.02.14    
    Description : Pong
***************************************************************************************/
#ifndef _XOPEN_SOURCE 
#define _XOPEN_SOURCE 500
#endif
/*------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

/*------------------------------------------------------------------------------------*/

#include "MsgQueue.h"
#include "pingPong.h"

/*------------------------------------------------------------------------------------*/

#define PRINT_ERR if(err!=0){ printf("FAIL: error no. %d  , ",err); }else{printf("PASS ,");}

#define DEBUG TRUE

/*------------------------------------------------------------------------------------*/

typedef struct Args{
	int m_workTime;
	int m_isDebug;
}Args;

/*------------------------------------------------------------------------------------*/

void GetArgs(int argc ,char** argv);

/*------------------------------------------------------------------------------------*/

Args args = {DEFAULT_WORK_TIME , DEBUG} ;

/*------------------------------------------------------------------------------------*/

int Init(MsgQ** _msgQ, MsgQ** _msgQHandshake);
int HandShake(MsgQ* _msgQHandShake);
int Work(MsgQ* _msgQ);
int Clean(MsgQ* _msgQ, MsgQ* _msgQHandShake);

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int err;
	MsgQ* msgQ = NULL;
	MsgQ* msgQHandShake = NULL;

	GetArgs(argc ,argv);

	/*init*/
	err = Init(&msgQ, &msgQHandShake);
	CHK_ERR(err);
	
	/*send handshake*/
	err = HandShake(msgQHandShake);
	CHK_ERR(err);

	/*read msgs from msgQ*/
	err = Work(msgQ);
	CHK_ERR(err);
	
	/*send end msg to ping*/
	MsgQueueSnd (msgQHandShake, PONG_EXIT_SIGN, strlen(PONG_EXIT_SIGN) );	
	
	/*clean*/
	err = Clean(msgQ, msgQHandShake);
	CHK_ERR(err);
	

    return 0; 	
}

/*------------------------------------------------------------------------------------*/

int Init(MsgQ** _msgQ, MsgQ** _msgQHandshake)
{
	/*connect msgQ*/
	*_msgQ = MsgQueueConnect(FILE_NAME);
	CHK_NULL_ERR(*_msgQ, MSGQ_CREATE_E);
	
	/*connect msgQHandshake*/
	*_msgQHandshake = MsgQueueConnect(FILE_NAME_HANDSHAKE);
	CHK_NULL_ERR(*_msgQHandshake, MSGQ_CREATE_E);

	return OK_E;
}

/*------------------------------------------------------------------------------------*/

int HandShake(MsgQ* _msgQHandShake)
{	
	MsgQueueSnd (_msgQHandShake, HANDSHAKE_SIGN, strlen(HANDSHAKE_SIGN) );	
	
	PRINT_D("HandShake sent\n");
	
	return OK_E;
}

/*------------------------------------------------------------------------------------*/

int Work(MsgQ* _msgQ)
{
	char msgToGet[MAX_MSG_SIZE];
	int numOfbytesRead;

	while(TRUE){
		numOfbytesRead = MsgQueueRcv (_msgQ, msgToGet, MAX_MSG_SIZE);
		msgToGet[numOfbytesRead] = '\0';
		printf("msg rcv : %s\n", msgToGet);

		usleep(args.m_workTime);

		if (strcmp(msgToGet , EOF_SIGN) == 0){
			MsgQueueSnd (_msgQ, EOF_SIGN, strlen(EOF_SIGN) );
			break;
		}
	}
	
	return OK_E;
}

/*------------------------------------------------------------------------------------*/

int Clean(MsgQ* _msgQ, MsgQ* _msgQHandShake)
{
	MsgQueueDestroy(_msgQ);
	MsgQueueDestroy(_msgQHandShake);
	
	return OK_E;	
}

/*------------------------------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "s:d?")) != -1)
	{
		switch(argType)
		{
			case 's':
				args.m_workTime = atoi(optarg) * MICROSEC_MUL_RES;
				break;
			case 'd':
				args.m_isDebug = TRUE;
				break;	
			case '?':
				printf("s=workTime\n");
				exit(0);				
			default:				
				break;
		}
	}	
}

/*------------------------------------------------------------------------------------*/


	

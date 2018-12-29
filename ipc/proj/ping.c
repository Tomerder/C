/**************************************************************************************
    Author : Tomer Dery
    Creation date :      20.02.14    
    Date last modified : 20.02.14    
    Description : Ping 
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

#define DEBUG TRUE

/*------------------------------------------------------------------------------------*/

typedef struct Args{
	int m_msgQSize;
	int m_msgQCapacity;
	int m_numOfMsgsPerPong;		
	int m_workTime;
	int m_isDebug;
}Args;

/*------------------------------------------------------------------------------------*/

void GetArgs(int argc ,char** argv);

/*------------------------------------------------------------------------------------*/

Args args = {DEFAULT_TOT_MSGQ_SIZE, DEFAULT_MSGQ_CAPACITY, DEFAULT_NUM_OF_MSGS_PER_PONG, DEFAULT_WORK_TIME, DEBUG} ;

/*------------------------------------------------------------------------------------*/

typedef struct SyncObjs{
	pthread_mutex_t m_mutex;
	sem_t 			m_notifySem;
}SyncObjs;

/*------------------------------------------------------------------------------------*/

typedef struct Params{
	int* m_numOfPongs;
	int* m_numOfMsgsToSnd;
	MsgQ* m_msgQHandShake;
	SyncObjs* m_sync;
}Params;

/*------------------------------------------------------------------------------------*/

int Init(MsgQ** _msgQ, MsgQ** _msgQHandshake, pthread_t** _handShakeThread, SyncObjs* _sync);
void SetParams(Params* _params, MsgQ* _msgQHandShake, int* _numOfPongs, int* _numOfMsgsToSnd, SyncObjs* _sync);
int StartHandShake(pthread_t* _handShakeThread, Params* _params);
int Work(MsgQ* _msgQ, int* _numOfMsgsToSnd, SyncObjs* _sync);
int Clean(MsgQ* _msgQ, MsgQ* _msgQHandshake, pthread_t* _handShakeThread, SyncObjs* _sync);

/*------------------------------------------------------------------------------------*/
void CleanAll();
/*------------------------------------------------------------------------------------*/
/*thread func*/
void* HandShake(void* _param);
/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int err;
	/*sending msgs to pong*/
	MsgQ* msgQ = NULL;
	/*getting handshake and exit msg from pongs (reg/unreg)*/
	MsgQ* msgQHandShake = NULL;
	pthread_t *handShakeThread = NULL;
	int numOfMsgsToSnd = 0, numOfPongs = 0;
	Params params;
	SyncObjs sync;

	GetArgs(argc ,argv);

	/*init*/
	err = Init(&msgQ , &msgQHandShake , &handShakeThread, &sync);
	CHK_ERR(err);

	/*set threads params*/
	SetParams(&params, msgQHandShake, &numOfPongs, &numOfMsgsToSnd, &sync);

	/*start handshake thread*/
	err = StartHandShake(handShakeThread, &params);	
	CHK_ERR(err);
	
	/*start work*/
	err = Work(msgQ, &numOfMsgsToSnd, &sync);	
	CHK_ERR(err);

	/*clean*/
	err = Clean(msgQ , msgQHandShake, handShakeThread, &sync);
	CHK_ERR(err);

    return 0; 	
}

/*------------------------------------------------------------------------------------*/

void SetParams(Params* _params, MsgQ* _msgQHandShake, int* _numOfPongs, int* _numOfMsgsToSnd, SyncObjs* _sync)
{
	_params->m_msgQHandShake = _msgQHandShake;
	_params->m_numOfPongs = _numOfPongs;
	_params->m_numOfMsgsToSnd = _numOfMsgsToSnd;
	_params->m_sync = _sync;
}

/*------------------------------------------------------------------------------------*/

int Init(MsgQ** _msgQ, MsgQ** _msgQHandshake, pthread_t** _handShakeThread, SyncObjs* _sync)
{
	int err;

	/*create msgQ*/
	*_msgQ = MsgQueueCreate(FILE_NAME, DEFAULT_MSGQ_CAPACITY , DEFAULT_TOT_MSGQ_SIZE );
	CHK_NULL_ERR(*_msgQ, MSGQ_CREATE_E);

	/*create msgQHandshake*/
	*_msgQHandshake = MsgQueueCreate(FILE_NAME_HANDSHAKE, DEFAULT_MSGQ_CAPACITY , DEFAULT_TOT_MSGQ_SIZE );
	CHK_NULL_ERR(*_msgQHandshake, MSGQ_CREATE_E);

	/*init handshake thread*/
	*_handShakeThread = malloc( sizeof(pthread_t) );
	CHK_NULL_ERR(*_handShakeThread, ALLOC_E);

	/*init mutex*/
	err = pthread_mutex_init( &(_sync->m_mutex), NULL);  
	CHK_ERR_RET(err, MUTEX_INIT_E);
	
	/*init sem*/
	err = sem_init( &(_sync->m_notifySem) , NOT_SHAREABLE, 0);
    CHK_ERR_RET(err, SEM_INIT_E);
  
	
	return OK_E;
}

/*------------------------------------------------------------------------------------*/

int StartHandShake(pthread_t* _handShakeThread, Params* _params)
{
	int err;
	pthread_attr_t attr;

	err = pthread_attr_init(&attr);
	CHK_ERR(err);

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	CHK_ERR(err);

  	/*start detached HandShake*/
	err = pthread_create(_handShakeThread, &attr, HandShake, (void*)(_params));   
	CHK_ERR(err);

    err = pthread_attr_destroy(&attr);
	CHK_ERR(err);
 		
	return OK_E;
}	

/*------------------------------------------------------------------------------------*/

void* HandShake(void* _param)
{
	int isFirstHandShake = TRUE;
	Params* params = (Params*)_param;
	MsgQ* msgQHandShake = params->m_msgQHandShake;
	int* numOfPongs = params->m_numOfPongs;
	int* numOfMsgsToSnd = params->m_numOfMsgsToSnd;
	SyncObjs* sync = params->m_sync;
	char msgToRcv[MAX_MSG_SIZE];

	PRINT_D("HandShake thread start\n");

	while(TRUE)
	{
		MsgQueueRcv (msgQHandShake, msgToRcv, MAX_MSG_SIZE);
					
		if( strcmp(msgToRcv, HANDSHAKE_SIGN) == 0 )
		{	
			/*-----------------------------*/
			/*protect with mutex*/
			pthread_mutex_lock( &(sync->m_mutex));
	
			PRINT_N_D("(*numOfMsgsToSnd) : %d\n", (*numOfMsgsToSnd) );
	
			(*numOfPongs)++;	
			(*numOfMsgsToSnd) += args.m_numOfMsgsPerPong;
	
			PRINT_N_D("args.m_numOfMsgsPerPong : %d\n", args.m_numOfMsgsPerPong );
			PRINT_N_D("(*numOfMsgsToSnd) : %d\n", (*numOfMsgsToSnd) );
	
			pthread_mutex_unlock( &(sync->m_mutex));
			/*-----------------------------*/
	
			/*notify that first handshake was received and work can start*/
			if(isFirstHandShake){
				sem_post( &(sync->m_notifySem));
				isFirstHandShake = FALSE;	
				PRINT_D("notify first handshake\n");
			} 		
	
			printf("Handshake received\n");
		}
		else /*pong finished*/
		{
			/*when all pongs finish => notify work thread*/
			PRINT_N_D("numOfPongs : %d\n", *numOfPongs );
			(*numOfPongs)--;
			if(*numOfPongs == 0){
				PRINT_D("notify no more pongs on system\n");
				sem_post( &(sync->m_notifySem));
			}	
		}
	}
	
	PRINT_D("HandShake thread end\n");
	
	return (void*)OK_E;
}

/*------------------------------------------------------------------------------------*/

int Work(MsgQ* _msgQ, int* _numOfMsgsToSnd, SyncObjs* _sync)
{
	int i;
	char msg[MAX_MSG_SIZE];
	int pid = getpid();

	PRINT_D("Work thread start\n");
	
	/*wait for notification from handshake thread => for first handshake */
	sem_wait( &(_sync->m_notifySem)  ); 
	PRINT_D("received handshake from first pong\n");

	for(i=0; (*_numOfMsgsToSnd) > 0; i++)
	{
		PRINT_N_D("numOfMsgsToSnd : %d\n", (*_numOfMsgsToSnd) );
		/*work*/
		usleep(args.m_workTime);

		sprintf(msg, "msg num : %d => sender pid : %d", i , pid);
		MsgQueueSnd (_msgQ, msg, strlen(msg) );
		printf("msg sent : %s\n", msg);
		
		/*-----------------------------*/
		/*protect with mutex*/
		pthread_mutex_lock( &(_sync->m_mutex));
		
		(*_numOfMsgsToSnd)--;
		
		pthread_mutex_unlock( &(_sync->m_mutex));
		/*-----------------------------*/
	}

	MsgQueueSnd (_msgQ, EOF_SIGN, strlen(EOF_SIGN) );
	
	PRINT_D("Work thread end\n");
	
	return OK_E;
}

/*------------------------------------------------------------------------------------*/

int Clean(MsgQ* _msgQ, MsgQ* _msgQHandshake, pthread_t* _handShakeThread, SyncObjs* _sync)
{
	/*wait for notify that all pong finished*/
	sem_wait( &(_sync->m_notifySem));
	PRINT_D("notified no more pongs on system\n");

	/*clean*/
	pthread_mutex_destroy( &(_sync->m_mutex) );  
	sem_destroy( &(_sync->m_notifySem) );

	MsgQueueDestroy(_msgQ);	
	MsgQueueDestroy(_msgQHandshake);
	
	free(_handShakeThread);
	
	return OK_E;
}

/*------------------------------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "t:c:m:s:dz?")) != -1)
	{
		switch(argType)
		{
			case 't':
				args.m_msgQSize = atoi(optarg);
				break;	
			case 'c':
				args.m_msgQCapacity = atoi(optarg);
				break;	
			case 'm':
				args.m_numOfMsgsPerPong = atoi(optarg);
				break;	
			case 's':
				args.m_workTime = atoi(optarg) * MICROSEC_MUL_RES;
				break;
			case 'd':
				args.m_isDebug = TRUE;
				break;	
			case 'z':
				CleanAll();
				exit(1);	
			case '?':
				printf("t=totalMsgQsize , c=msgQCapacity, m=numOfMsgsPerPong, s=workTime\n");
				exit(1);	
			default:				
				break;
		}
	}	
}

/*------------------------------------------------------------------------------------*/
/*for debug => clean msgQs in case of error*/
void CleanAll()
{
	MsgQ* msgQ = NULL;
	MsgQ* msgQHandShake = NULL;
	
	msgQ = MsgQueueConnect(FILE_NAME);	
	msgQHandShake = MsgQueueConnect(FILE_NAME_HANDSHAKE);

	MsgQueueDestroy(msgQ);
	MsgQueueDestroy(msgQHandShake);
	
	printf("msg queues removed\n");
}
/*------------------------------------------------------------------------------------*/	

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

#define DEFAULT_N_OF_ITERS 20

#define FILE_NAME "keyFile"
#define PRINT_ERR if(err!=0){ printf("FAIL: error no. %d  , ",err); }else{printf("PASS ,");}

#define DEBUG TRUE

/*------------------------------------------------------------------------------------*/

typedef struct Args{
	int m_numOfIters;	
	int m_workTime;
	int m_isDebug;
}Args;

/*------------------------------------------------------------------------------------*/

void GetArgs(int argc ,char** argv);

/*------------------------------------------------------------------------------------*/

Args args = {DEFAULT_N_OF_ITERS , DEFAULT_WORK_TIME , DEBUG} ;

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int i;
	char msg[50];
	MsgQ* msgQ = NULL;

	GetArgs(argc ,argv);

	/*sets handshake thread -> down sem notify new pong*/

	msgQ = MsgQueueCreate(FILE_NAME, 10 , 100000);
	if(msgQ == NULL){
		printf("msgQ == NULL\n");
		return 0; 
	}

	for(i=0; i < args.m_numOfIters; i++){
		usleep(args.m_workTime);

		sprintf(msg, "msg num : %d", i);
		MsgQueueSnd (msgQ, msg, strlen(msg) );
		printf("msg sent : %s\n", msg);
	}

	MsgQueueSnd (msgQ, EOF_SIGN, strlen(EOF_SIGN) );


	/*wait to sem reg pong == 0*/

	/*kill handshake thread*/	

	MsgQueueDestroy(msgQ);

    return 0; 	
}


/*------------------------------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "i:s:d")) != -1)
	{
		switch(argType)
		{
			case 'i':
				args.m_numOfIters = atoi(optarg);
				break;	
			case 's':
				args.m_workTime = atoi(optarg) * MICROSEC_MUL_RES;
				break;
			case 'd':
				args.m_isDebug = TRUE;
				break;	
			default:				
				break;
		}
	}	
}

/*------------------------------------------------------------------------------------*/


	

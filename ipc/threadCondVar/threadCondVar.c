/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 12.02.14                                                                                              
*    LAST MODIFIED: 12.02.14                                                                                  
*    DESCRIPTION: Thread - producer consumer 
*****************************************************************************************************************/
#define _XOPEN_SOURCE 500
/*-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

/*-----------------------------------------------------------------*/

enum{FALSE , TRUE};

enum{OK_E, ALOC_ERR , SEM_INIT_ERR , THREAD_JOIN_ERR};

#define SLEEP 1000000

#define MSG_SIZE 64

#define TXT_SND "message num. %d => sender tid %u"

#define NUM_OF_PROD 1

#define DEFAULT_N_OF_CONS 5
#define DEFAULT_N_OF_LOOPS 5
#define DEFAULT_IS_WHILE TRUE

#define CHKERR(ERR) if(ERR != OK_E) { printf("ERROR : %d\n",ERR); return ERR; }

/*-----------------------------------------------------------------*/

typedef struct Args{	
	int m_numOfCons;
	int m_numOfLoops;
	int m_isWhile;
}Args;

/*-----------------------------------------------------------------*/

typedef struct SyncObjs{
	/*pthread_mutex_t m_mutex;*/
	pthread_mutex_t m_mutexWrite;
	pthread_mutex_t m_mutexRead;
	pthread_cond_t m_condWriteWait;
	pthread_cond_t m_condReadWait;
}SyncObjs;

/*-----------------------------------------------------------------*/

typedef struct ThreadParams{
	int m_numOfMsg;
	int* m_isMsgReady;
	int* m_isReaderReady;
	SyncObjs* m_syncObjs;
	char m_msg[MSG_SIZE];
}ThreadParams;

/*-----------------------------------------------------------------*/

void GetArgs(int argc ,char** argv);

int Init(pthread_t** _threadsProd, pthread_t** _threadsCons , SyncObjs* _syncObjs);

void Clean(pthread_t* _threadsProd, pthread_t* _threadsCons);

int Work(pthread_t* _threadsProd, pthread_t* _threadsCons, SyncObjs* _syncObjs );



void* cons(void* _parm);
void* prod(void* _parm);

/*-----------------------------------------------------------------*/

Args args = { DEFAULT_N_OF_CONS , DEFAULT_N_OF_LOOPS , DEFAULT_IS_WHILE } ;

/*-----------------------------------------------------------------*/


int main(int argc,char** argv)
{	
    int err;
	pthread_t *threadsProd = NULL , *threadsCons = NULL;
	SyncObjs syncObjs;
		
	GetArgs(argc ,argv);
		
	/*init*/
	err = Init(&threadsProd , &threadsCons , &syncObjs );
	CHKERR(err);
    
    /*start prods and cons - main loop*/
    err = Work(threadsProd, threadsCons,  &syncObjs);
    CHKERR(err);
    
    /*clean*/
    Clean(threadsProd, threadsCons);	
		
	printf("PROG ENDED\n");
	
	return OK_E;
}

/*-----------------------------------------------------------------*/

int Work(pthread_t* _threadsProd, pthread_t* _threadsCons, SyncObjs* _syncObjs )
{
	int i,j , isMsgReady = FALSE, isReaderReady = FALSE;
	ThreadParams params;
	
	/*initialize params*/
	params.m_syncObjs = _syncObjs; 
	params.m_isMsgReady = &isMsgReady;
	params.m_isReaderReady = &isReaderReady;
	
	
	/*main loop*/
	for(i=0; i < args.m_numOfLoops; i++)
	{
		params.m_numOfMsg = i + 1;
		isMsgReady = FALSE;
		isReaderReady = FALSE;
		
		/*start prod*/
		pthread_create(_threadsProd, NULL, prod, (void*)(&params));   

	
		/*start cons*/
		for(j=0; j < args.m_numOfCons; j++){
			 pthread_create(&_threadsCons[j], NULL, cons, (void*)(&params));   
		}


		/*wait for prod to finish*/
		if(pthread_join(*_threadsProd, NULL)) {
			return THREAD_JOIN_ERR;
		}	
		
		/*wait for cons to finish*/
		for(j=0; j < args.m_numOfCons; j++){
			if(pthread_join(_threadsCons[j], NULL)) {
				return THREAD_JOIN_ERR;
			}	
		}

		usleep(SLEEP);

		printf("LOOP %d ENDED\n" , i + 1);
		printf("------------------------------------\n");

	}

	return OK_E;
}

/*-----------------------------------------------------------------*/

int Init(pthread_t** _threadsProd, pthread_t** _threadsCons , SyncObjs* _syncObjs)
{
	int err;
		
	/*init prod*/
	*_threadsProd = malloc( sizeof(pthread_t) );
	if (*_threadsProd == NULL){
		return ALOC_ERR;
	}
		
	/*init arr of cons*/
	*_threadsCons = malloc( args.m_numOfCons * sizeof(pthread_t) );
	if (*_threadsCons == NULL){
		return ALOC_ERR;
	}
    
    /* initialize mutex */
    pthread_mutex_init( &(_syncObjs->m_mutexWrite), NULL);  
    
    pthread_mutex_init( &(_syncObjs->m_mutexRead), NULL);  
  
		
	/* initialize a condition variables*/
	err = pthread_cond_init( &(_syncObjs->m_condWriteWait) , NULL);
	CHKERR(err);

	err = pthread_cond_init( &(_syncObjs->m_condReadWait), NULL);
	CHKERR(err);
	
	
	return OK_E;
}

/*-----------------------------------------------------------------*/

void Clean(pthread_t* _threadsProd, pthread_t* _threadsCons)
{	
	free(_threadsProd);
	
	free(_threadsCons);
}

/*-----------------------------------------------------------------*/

void* prod(void* _parm)
{
	int err;
	ThreadParams* params = (ThreadParams*)_parm;
	SyncObjs* syncObjs = params->m_syncObjs; 
	unsigned tid = (unsigned)pthread_self();
	
	printf("starting prod thread %u\n", tid);	
	
	/*--------------------------------------*/
	/*lock*/
	pthread_mutex_lock(&syncObjs->m_mutexWrite);
		
	while (*(params->m_isReaderReady) == FALSE) 
	{
		/*wait for reader*/
		err = pthread_cond_wait(&syncObjs->m_condWriteWait, &syncObjs->m_mutexWrite);
	}	
	
	/*unlock*/
	pthread_mutex_unlock(&syncObjs->m_mutexWrite);
    /*--------------------------------------*/
    
    /*write msg*/
	sprintf( params->m_msg , TXT_SND , params->m_numOfMsg , tid);                   
    printf("writing: %s\n\n",  params->m_msg);	
    
   	/*broadcast to all readers*/
   	*(params->m_isMsgReady) = TRUE;
    err = pthread_cond_broadcast( &syncObjs->m_condReadWait); 
	
	return (void*)OK_E;
}

/*-----------------------------------------------------------------*/

void* cons(void* _parm)
{
	int err;
	ThreadParams* params = (ThreadParams*)_parm;
	SyncObjs* syncObjs = params->m_syncObjs; 
	char* msg;
	unsigned int tid = (unsigned int)pthread_self();
	
	printf("starting cons thread %u\n", tid);	
	
	/*notify writer*/
	*(params->m_isReaderReady) = TRUE;
	pthread_cond_signal( &syncObjs->m_condWriteWait);

	/*--------------------------------------*/		
	/*lock*/
	pthread_mutex_lock( &syncObjs->m_mutexRead);
	
	while ( *(params->m_isMsgReady) == FALSE) 
	{
		/*wait for writer*/
		err = pthread_cond_wait( &syncObjs->m_condReadWait, &syncObjs->m_mutexRead);
	}
	
	/*unlock*/
	pthread_mutex_unlock( &syncObjs->m_mutexRead);	
	/*--------------------------------------*/
	
	/*reading msg*/	
	printf("reading: %s => receiver : %u\n\n",  params->m_msg , tid);
	
	
	return (void*)OK_E;
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "n:l:w")) != -1)
	{
		switch(argType)
		{
			case 'n':
				args.m_numOfCons = atoi(optarg);
				break;	
			case 'l':
				args.m_numOfLoops = atoi(optarg);
				break;					
			case 'w':
				args.m_isWhile = TRUE;
				break;	
			default:				
				break;
		}
	}	
}

/*-----------------------------------------------------------------*/

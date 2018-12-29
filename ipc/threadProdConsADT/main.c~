/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 12.02.14                                                                                              
*    LAST MODIFIED: 12.02.14                                                                                  
*    DESCRIPTION: Thread - Producer Consumer 
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

#include "cons.h"
#include "prod.h"

/*-----------------------------------------------------------------*/

enum{FALSE , TRUE};

#define MSG_SIZE 64

#define TXT_SND "message num. %d"

#define DEFAULT_N_OF_PROD 5
#define DEFAULT_N_OF_CONS 5
#define DEFAULT_BOX_SIZE 10
#define DEFAULT_N_OF_LOOPS 10
#define DEFAULT_WORK_TIME 100000
#define MICROSEC_MUL_RES 1000

#define CHKERR(ERR) if(ERR != OK_E) { printf("ERROR : %d\n",ERR); return ERR; }

#define DEBUG FALSE

/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_numOfProd;	
	int m_numOfCons;
	int m_boxSize;
	int m_workTime;
	int m_isDebug;
}Args;

/*-----------------------------------------------------------------*/

typedef struct SyncObjs{
	pthread_mutex_t m_mutexProd;
	pthread_mutex_t m_mutexCons;
	pthread_cond_t m_cond;
}SyncObjs;


/*-----------------------------------------------------------------*/

void GetArgs(int argc ,char** argv);

int Init(Produce_t** _prod, Consume_t** _cons, SyncObjs* _syncObjs, pthread_t** _threadsProd, pthread_t** _threadsCons);

int StartWork(Produce_t* _prod, Consume_t* _cons, pthread_t* _threadsProd, pthread_t* _threadsCons);

int WaitFinishWork(Produce_t* _prod, Consume_t* _cons, pthread_t* _threadsProd, pthread_t* _threadsCons);

void Clean(Produce_t* _prod, Consume_t* _cons, pthread_t* _threadsProd, pthread_t* _threadsCons);

void* Cons(void* _parm);
void* Prod(void* _parm);

/*-----------------------------------------------------------------*/

int ChkTotal(int _chkTotal);

/*-----------------------------------------------------------------*/

Args args = {DEFAULT_N_OF_PROD, DEFAULT_N_OF_CONS , DEFAULT_BOX_SIZE, DEFAULT_WORK_TIME , DEBUG} ;

/*-----------------------------------------------------------------*/

typedef struct ProdParams{
	Produce_t* m_prod;
	void* m_item;
}ProdParams;

/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int err;
	pthread_t *threadsProd = NULL , *threadsCons = NULL;
	Produce_t* prod = NULL;
	Consume_t* cons = NULL;
	SyncObjs syncObjs;
	
	
	GetArgs(argc ,argv);
	
	/*init*/
	err = Init(&prod, &cons, &syncObjs, &threadsProd, &threadsCons);
	CHKERR(err);
    
    /*start prods and cons*/
    err = StartWork(prod, cons ,threadsProd, threadsCons);
    CHKERR(err);
    
    /*wait for threads to finish the job*/
    err = WaitFinishWork( prod, cons, threadsProd,  threadsCons);
    CHKERR(err);
    
    /*clean*/
    Clean(prod, cons, threadsProd, threadsCons);	
		
	printf("PROG ENDED\n");
	
	return OK_E;
}

/*-----------------------------------------------------------------*/

int StartWork(Produce_t* _prod, Consume_t* _cons, pthread_t* _threadsProd, pthread_t* _threadsCons)
{
	int i;
	char* msg;
	ProdParams* prodParams = NULL;

	prodParams = malloc(sizeof(ProdParams) * args.m_numOfProd);
	if(prodParams == NULL){
		return ALOC_ERR;
	}
	
	/*start prods*/
	for(i=0; i < args.m_numOfProd; i++){
		prodParams[i].m_prod = _prod;
		
		if(args.m_isDebug){
			msg = malloc(sizeof(char) * MSG_SIZE);
			sprintf( msg , TXT_SND , i + 1); 
		}else{
			msg = (void*)i;
		}           
		prodParams[i].m_item = (void*)msg; 		
		
		pthread_create(&_threadsProd[i], NULL, Prod, (void*)(&prodParams[i]));   
	}
	
	/*start cons*/
	for(i=0; i < args.m_numOfCons; i++){
		pthread_create(&_threadsCons[i], NULL, Cons, (void*)_cons);   
	}


	free(prodParams);	

	return OK_E;
}

/*-----------------------------------------------------------------*/

int WaitFinishWork(Produce_t* _prod, Consume_t* _cons, pthread_t* _threadsProd, pthread_t* _threadsCons)
{
	int i;
	char* msg = NULL;
	int chkTotal = 0;
	
	/*make sure all prod and cond will exit*/
	usleep(args.m_workTime);
	ConsExit(_cons); 
	ProdExit(_prod); 
	
	/*wait for prods to finish writing*/
	for(i=0; i < args.m_numOfProd; i++){
		if(pthread_join(_threadsProd[i], NULL)) {
			return THREAD_JOIN_ERR;
		}	
	}
	
	/*wait for prods to finish writing*/
	for(i=0; i < args.m_numOfCons; i++){
		if(pthread_join(_threadsCons[i], (void**)&msg)) {
			return THREAD_JOIN_ERR;
		}	
		
		if(args.m_isDebug){
			printf("thread %u => read msgs : %s\n", (unsigned)_threadsCons[i] , msg);
			free(msg);
		}else{
			if(msg){
				chkTotal += (int)msg;
			}		
		}
	}

	/*pass/fail*/
	if(!args.m_isDebug){
		if( ChkTotal(chkTotal) ){
			printf("TEST: PASS\n");
		}else{
			printf("TEST: FAIL\n");
		}
	}

	return OK_E;
}

/*-----------------------------------------------------------------*/

int ChkTotal(int _chkTotal)
{
	int i, sum = 0;
	int nItems = (args.m_numOfProd < args.m_numOfCons) ? args.m_numOfProd : args.m_numOfCons; 
		
	for(i=0; i<nItems; i++){
		sum += i;
	}
	
	/* < in case of less cons from prod*/
	if(args.m_numOfProd > args.m_numOfCons){
		return (sum <= _chkTotal);
	}else{
		return (sum == _chkTotal);
	}
}

/*-----------------------------------------------------------------*/

int Init(Produce_t** _prod, Consume_t** _cons, SyncObjs* _syncObjs, pthread_t** _threadsProd, pthread_t** _threadsCons)
{
	int err;
	Queue* queue = NULL;

	/*create queue*/
	queue = QueueCreate(args.m_boxSize);	
	if (queue == NULL){
		return ALOC_ERR;
	}
	
	/* initialize mutex */
    pthread_mutex_init( &(_syncObjs->m_mutexProd), NULL); 

	pthread_mutex_init( &(_syncObjs->m_mutexCons), NULL); 
    
    /* initialize cond var */
    err = pthread_cond_init( &(_syncObjs->m_cond) , NULL);
	CHKERR(err);
		
    /*create prod*/
    *_prod = ProduceInit(queue, &(_syncObjs->m_mutexProd), &(_syncObjs->m_cond), args.m_boxSize);
    if (*_prod == NULL){
		return ALOC_ERR;
	}

    /*create cons*/
    *_cons = ConsumeInit(queue, &(_syncObjs->m_mutexCons), &(_syncObjs->m_cond), args.m_boxSize );
    if (*_cons == NULL){
		return ALOC_ERR;
	}            
	
    /*init arr of prods*/
	*_threadsProd = malloc( args.m_numOfProd * sizeof(_threadsProd) );
	if (*_threadsProd == NULL){
		return ALOC_ERR;
	}
		
	/*init arr of cons*/
	*_threadsCons = malloc( args.m_numOfCons * sizeof(_threadsProd) );
	if (*_threadsCons == NULL){
		return ALOC_ERR;
	}
	
	return OK_E;
}

/*-----------------------------------------------------------------*/

void Clean(Produce_t* _prod, Consume_t* _cons, pthread_t* _threadsProd, pthread_t* _threadsCons)
{

	free(_threadsProd);
	free(_threadsCons);

	pthread_mutex_destroy( ProdGetMutex(_prod));  
	pthread_mutex_destroy( ConsGetMutex(_cons));  

	QueueDestroy( ProdGetQueue(_prod));

	ProduceDestroy(_prod);
	ConsumeDestroy(_cons);

}

/*-----------------------------------------------------------------*/
/*wrapper*/
void* Prod(void* _parm)
{
	ProdParams* prodParams = (ProdParams*)_parm;
	return Produce(prodParams->m_prod, prodParams->m_item);
}

/*-----------------------------------------------------------------*/
/*wrapper*/
void* Cons(void* _parm)
{
	Consume_t* cons = (Consume_t*)_parm;
	return Consume(cons);
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "p:c:n:s:d")) != -1)
	{
		switch(argType)
		{
			case 'p':
				args.m_numOfProd = atoi(optarg);
				break;	
			case 'c':
				args.m_numOfCons = atoi(optarg);
				break;	
			case 'n':
				args.m_boxSize = atoi(optarg) ;
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

/*-----------------------------------------------------------------*/

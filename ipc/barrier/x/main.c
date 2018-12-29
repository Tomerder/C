/*****************************************************************************************************************
*    AUTHOR: 		Tomer Dery                                                                                 
*    DATE: 			16.02.14                                                                                              
*    LAST MODIFIED: 16.02.14                                                                                  
*    DESCRIPTION: 	Barrier
*****************************************************************************************************************/
#ifndef _XOPEN_SOURCE 
#define _XOPEN_SOURCE 500
#endif
/*-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

/*-----------------------------------------------------------------*/

#include "barrier.h"

/*-----------------------------------------------------------------*/

enum{FALSE , TRUE};

#define MSG_SIZE 64

#define TXT_COUNT "thread %u => index %d => counting: %d => iter: %d\n"

#define DEFAULT_N_OF_THREADS 3
#define DEFAULT_N_OF_ITERS 3
#define DEFAULT_WORK_TIME 1000000
#define MICROSEC_MUL_RES 1000
#define DEBUG FALSE

#define CHKERR(ERR , RET) if(ERR != OK_E) { printf("ERROR :%d\n", RET); return RET; }

#define DEBUG FALSE

#define PRINT_D(MSG)  if(DEBUG) printf(MSG)
#define PRINT_U_D(MSG,U)  if(DEBUG) printf(MSG,U)

/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_numOfThreads;
	int m_numOfIters;	
	int m_workTime;
	int m_isDebug;
}Args;

/*-----------------------------------------------------------------*/

void GetArgs(int argc ,char** argv);

int Init(Barrier** _bar, pthread_t** _countingThreads , pthread_t** _sumThread ,int** _countingArr);

int Work(Barrier* _bar, pthread_t* _countingThreads, pthread_t* _sumThread ,int* _countingArr);

int FinishWork(Barrier* _bar, pthread_t* _countingThreads, pthread_t* _sumThread, int* _countingArr);

void Clean(Barrier* _bar, pthread_t* _countingThreads, pthread_t* _sumThread, int* _countingArr);

void* CountThread(void* _parm);
void* SumThread(void* _parm);

/*-----------------------------------------------------------------*/

int calcSum(int* _countingArr);

/*-----------------------------------------------------------------*/

Args args = {DEFAULT_N_OF_THREADS, DEFAULT_N_OF_ITERS , DEFAULT_WORK_TIME , DEBUG} ;

/*-----------------------------------------------------------------*/

typedef struct CountThreadParams{
	Barrier* m_bar;
	int* m_countingArr;
	int m_threadIndex;
}CountThreadParams;

typedef struct SumThreadParams{
	Barrier* m_bar;
	int* m_countingArr;
}SumThreadParams;

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int err;
	pthread_t* countingThreads = NULL;
	pthread_t* sumThread;
	Barrier* bar = NULL;
	int* countingArr = NULL;
	
	GetArgs(argc ,argv);
	
	/*init*/
	err = Init(&bar, &countingThreads, &sumThread, &countingArr);
	CHKERR(err, ERROR);
    
    /*start prods and cons*/
    err = Work(bar, countingThreads, sumThread, countingArr);
    CHKERR(err, ERROR);
    
    /*wait for threads to finish the job*/
    err = FinishWork(bar, countingThreads, sumThread, countingArr);
    CHKERR(err, ERROR);
    
    /*clean*/
    Clean(bar, countingThreads, sumThread, countingArr);
		
	printf("PROG ENDED\n");
	
	return OK_E;
}

/*-----------------------------------------------------------------*/

int Work(Barrier* _bar, pthread_t* _countingThreads, pthread_t* _sumThread ,int* _countingArr)
{
	int i ;
	SumThreadParams sumParams;
	CountThreadParams* countParams = malloc(args.m_numOfThreads * sizeof(CountThreadParams));
	if(countParams == NULL){
		return ALOC_ERR;
	}
	
	/*start counting threads*/
	for(i=0; i < args.m_numOfThreads; i++){
		countParams[i].m_bar = _bar;
		countParams[i].m_countingArr = _countingArr;
		countParams[i].m_threadIndex = i;

		pthread_create(&_countingThreads[i], NULL, CountThread, (void*)(&countParams[i]));   
	}
	
	/*start sum thread*/
	sumParams.m_bar = _bar;
	sumParams.m_countingArr = _countingArr;	
	pthread_create(_sumThread, NULL, SumThread, (void*)(&sumParams)); 


	free(countParams);

	return OK_E;
}

/*-----------------------------------------------------------------*/

int FinishWork(Barrier* _bar, pthread_t* _countingThreads, pthread_t* _sumThread, int* _countingArr)
{
	int err;
	int i;
	
	PRINT_D("ENTERED FinishWork\n");
	
	/*join counting threads*/
	for(i=0; i < args.m_numOfThreads; i++){
		err = pthread_join(_countingThreads[i], NULL);
		CHKERR(err, THREAD_JOIN_ERR);

		PRINT_U_D("THREAD %u JOINED\n", (unsigned)_countingThreads[i]);
	}

	/*join sumThread threads*/
	err = pthread_join(*_sumThread, NULL);
	CHKERR(err, THREAD_JOIN_ERR);
	
	PRINT_D("SUM THREAD JOINED\n");

	PRINT_D("EXITED FinishWork\n");

	return OK_E;
}

/*-----------------------------------------------------------------*/

void* CountThread(void* _parm)
{
	CountThreadParams* params = (CountThreadParams*)_parm; 
	Barrier* bar = params->m_bar;
	int* countingArr = params->m_countingArr;
	int threadIndex = params->m_threadIndex;  
	int iter,count;
	unsigned tid = pthread_self(); 
	
	printf("THREAD STARTED: %u => index: %d\n", tid, threadIndex);
	
	for(iter=0; iter < args.m_numOfIters; iter++){
		/*first barrier*/
		BarrierWait(bar);
		
		PRINT_U_D("thread woke up: %u\n", tid);
		
		/*counting loop*/
		for(count=0; count < threadIndex + 1 ;count++){
			printf(TXT_COUNT, tid, threadIndex, count, iter);
			
			countingArr[threadIndex]++;	

			/*work*/
			usleep(args.m_workTime);
		}

		/*second barrier - after counting*/
		BarrierWait(bar);
		
		printf("END OF ITER %d => %u\n", iter, tid);
	}
	
	return (void*)OK_E;
}

/*-----------------------------------------------------------------*/

void* SumThread(void* _parm)
{
	int iter, totalSum;
	SumThreadParams* params = (SumThreadParams*)_parm; 
	Barrier* bar = params->m_bar;
	int* countingArr = params->m_countingArr;	
	unsigned tid = pthread_self(); 
	
	printf("SUM THREAD STARTED: %u\n", tid);
	
	for(iter=0; iter < args.m_numOfIters; iter++){
		printf("SUM THREAD starting iter: %d\n", iter);
		/*first barrier*/
		BarrierWait(bar);

		/*second barrier - wait for counting threads to finish count*/
		BarrierWait(bar);	

		totalSum = calcSum(countingArr);
		printf("SUM THREAD Iter %d => total sum : %d\n", iter, totalSum);
	}
	
	return (void*)OK_E;
}

/*-----------------------------------------------------------------*/

int calcSum(int* _countingArr)
{
	int i, sum=0;

	for(i=0; i < args.m_numOfThreads; i++){
		sum += _countingArr[i];
		_countingArr[i] = 0;
	}

	return sum;
}
/*-----------------------------------------------------------------*/

int Init(Barrier** _bar, pthread_t** _countingThreads , pthread_t** _sumThread ,int** _countingArr)
{
    /*create barrier counting threads + sum thread*/
    *_bar = BarrierInit(args.m_numOfThreads +1);
    if (*_bar == NULL){
		return ALOC_ERR;
	}           
	
    /*counting threads*/
	*_countingThreads = malloc( args.m_numOfThreads * sizeof(pthread_t) );
	if (*_countingThreads == NULL){
		return ALOC_ERR;
	}
	
	/*sum thread*/
	*_sumThread = malloc(sizeof(pthread_t) );
	if (*_sumThread == NULL){
		return ALOC_ERR;
	}

	 /*counting arr*/
	*_countingArr = malloc( args.m_numOfThreads * sizeof(int) );
	if (*_countingArr == NULL){
		return ALOC_ERR;
	}
	
	return OK_E;
}

/*-----------------------------------------------------------------*/

void Clean(Barrier* _bar, pthread_t* _countingThreads, pthread_t* _sumThread, int* _countingArr)
{
	free(_countingThreads);
	free(_sumThread);
	free(_countingArr);

	BarrierDestroy(_bar);
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "n:i:s:d")) != -1)
	{
		switch(argType)
		{
			case 'n':
				args.m_numOfThreads = atoi(optarg);
				break;	
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

/*-----------------------------------------------------------------*/

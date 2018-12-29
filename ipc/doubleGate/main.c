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

#define DEFAULT_N_OF_TOT_THREADS 6
#define DEFAULT_N_OF_GRP_ITMS 3
#define DEFAULT_N_OF_ITERS 3
#define DEFAULT_WORK_TIME 1000000
#define MICROSEC_MUL_RES 1000


#define CHKERR(ERR , RET) if(ERR != OK_E) { printf("ERROR :%d\n", RET); return RET; }

#define DEBUG TRUE

#define PRINT_D(MSG)  if(DEBUG) printf(MSG)
#define PRINT_U_D(MSG,U)  if(DEBUG) printf(MSG,U)

/*-----------------------------------------------------------------*/

typedef struct Args{
	int m_numOfTotThreads;
	int m_numOfGrpItems;
	int m_numOfIters;	
	int m_workTime;
	int m_isDebug;
}Args;

/*-----------------------------------------------------------------*/

void GetArgs(int argc ,char** argv);

int Init(Barrier** _bar, pthread_t** _countingThreads, int** _countingArr);

int Work(Barrier* _bar, pthread_t* _countingThreads, int* _countingArr);

void Clean(Barrier* _bar, pthread_t* _countingThreads, int* _countingArr);

void* CountThread(void* _parm);

/*-----------------------------------------------------------------*/

void* CalcSum(void* _countingArr);

/*-----------------------------------------------------------------*/

Args args = {DEFAULT_N_OF_TOT_THREADS, DEFAULT_N_OF_GRP_ITMS, DEFAULT_N_OF_ITERS , DEFAULT_WORK_TIME , DEBUG} ;

/*-----------------------------------------------------------------*/

typedef struct CountThreadParams{
	Barrier* m_bar;
	int* m_countingArr;
	int m_threadIndex;
}CountThreadParams;

/*-----------------------------------------------------------------*/

int main(int argc,char** argv)
{	
    int err;
	pthread_t* countingThreads = NULL;
	Barrier* bar = NULL;
	int* countingArr = NULL;
	
	GetArgs(argc ,argv);
	
	/*init*/
	err = Init(&bar, &countingThreads, &countingArr);
	CHKERR(err, ERROR);
    
    /*start threads*/
    err = Work(bar, countingThreads, countingArr);
    CHKERR(err, ERROR);
    
    /*clean*/
    Clean(bar, countingThreads, countingArr);
		
	printf("PROG ENDED\n");
	
	return OK_E;
}

/*-----------------------------------------------------------------*/

int Work(Barrier* _bar, pthread_t* _countingThreads, int* _countingArr)
{
	int i ,err;
	CountThreadParams* countParams = malloc(args.m_numOfTotThreads * sizeof(CountThreadParams));
	if(countParams == NULL){
		return ALOC_ERR;
	}
	
	/*start counting threads*/
	for(i=0; i < args.m_numOfTotThreads; i++){
		countParams[i].m_bar = _bar;
		countParams[i].m_countingArr = _countingArr;
		countParams[i].m_threadIndex = i;

		pthread_create(&_countingThreads[i], NULL, CountThread, (void*)(&countParams[i]));   
	}

	/*join counting threads*/
	for(i=0; i < args.m_numOfTotThreads; i++){
		err = pthread_join(_countingThreads[i], NULL);
		CHKERR(err, THREAD_JOIN_ERR);

		PRINT_U_D("THREAD %u JOINED\n", (unsigned)_countingThreads[i]);
	}
	
	PRINT_D("SUM THREAD JOINED\n");


	free(countParams);

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
	
	for(iter=0; iter < args.m_numOfIters + 1; iter++)
	{	
		/*flood gate*/
		BarrierWait(bar, CalcSum, countingArr);	
		
		PRINT_U_D("THREAD GOT OUT FROM FLOOD GATE: %u\n", tid);

		/*counting loop*/
		for(count=0; count < threadIndex + 1 ;count++){
			printf(TXT_COUNT, tid, threadIndex, count, iter);
		
			countingArr[threadIndex]++;	

			/*work*/
			usleep(args.m_workTime);
		}
		
		printf("END OF ITER %d => %u\n", iter, tid);
	}
	
	return (void*)OK_E;
}

/*-----------------------------------------------------------------*/

void* CalcSum(void* _countingArr)
{
	int i, sum=0;
	int* countingArr = (int*)_countingArr;
	unsigned tid = pthread_self(); 

	for(i=0; i < args.m_numOfTotThreads; i++){
		sum += countingArr[i];
		/*countingArr[i] = 0;*/
	}
	
	printf("*********** %u => TOTAL SUM : %d\n", tid, sum);

	return (void*)sum;
}
/*-----------------------------------------------------------------*/

int Init(Barrier** _bar, pthread_t** _countingThreads, int** _countingArr)
{
    /*create barrier counting threads*/
    *_bar = BarrierInit(args.m_numOfGrpItems);
    if (*_bar == NULL){
		return ALOC_ERR;
	}           
	
    /*counting threads*/
	*_countingThreads = malloc( args.m_numOfTotThreads * sizeof(pthread_t) );
	if (*_countingThreads == NULL){
		return ALOC_ERR;
	}

	 /*counting arr*/
	*_countingArr = malloc( args.m_numOfTotThreads * sizeof(int) );
	if (*_countingArr == NULL){
		return ALOC_ERR;
	}
	
	return OK_E;
}

/*-----------------------------------------------------------------*/

void Clean(Barrier* _bar, pthread_t* _countingThreads, int* _countingArr)
{
	free(_countingThreads);
	free(_countingArr);

	BarrierDestroy(_bar);
}

/*-----------------------------------------------------------------*/

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "t:n:i:s:d")) != -1)
	{
		switch(argType)
		{
			case 't':
				args.m_numOfTotThreads = atoi(optarg);
				break;			
			case 'n':
				args.m_numOfGrpItems = atoi(optarg);
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

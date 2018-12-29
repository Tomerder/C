/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-17    
    Last modified date:		2013-09-17
    Description: 	some test for Sync module
********************************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>


#include "Sync.h"

/*######################################################################*/

#define NSYNC	3

#define ERRPRINT(X)		fprintf(stderr, "error: %s, line: %d\n", X, __LINE__)

enum Synctools
{
	SEMADD,
	SEMMULTI,
	MUTEX
};

typedef struct Params
{
	Sync_t*	m_sync;
	int		m_num;
} Params_t;


int g_number = 1;


void *Add1(void *num);
void *Multiply2(void *num);



/*######################################################################*/
int main(int argc, char* argv[])
{
	void* mem = NULL;
	int adding, multiplying;
	pthread_t add[2], multiply[2];	
	Params_t paramsAdd[2], paramsMulti[2];
	Sync_t* sync = NULL;
	SyncTool_t* semAdd;
	SyncTool_t* semMulti;
	SyncTool_t* mutex;
	int size;
	int init[NSYNC];
	
	adding = atoi(argv[1]);
	multiplying = atoi(argv[2]);

	size = SyncGetSize(NSYNC);
	printf("\nsize of %d-tool sync: %d\n", NSYNC, size);
	
	
	mem = malloc(size);
	if(!mem)
	{
		ERRPRINT("malloc");
		return -1;
	}
	
	init[SEMADD] = 1;
	init[SEMMULTI] = 0;
	init[MUTEX] = 1;
	
	sync = SyncCreate(mem, NSYNC, init);
	if(!sync)
	{
		ERRPRINT("SyncCreate");
		return -1;
	}
	
	SyncPrint(sync);
	
	
	semAdd	 = SyncAttach(sync, SEMADD);
	semMulti = SyncAttach(sync, SEMMULTI);
	mutex	 = SyncAttach(sync, MUTEX);
	
	printf("\nsync created, semaphore values: Add = %d, Multi = %d, mutex: %d\n",
				SyncSemGetVal(semAdd), SyncSemGetVal(semMulti), SyncSemGetVal(mutex));
	
	printf("\nAddresses in main:\tsync:\t\t%p\n\t\t\tsemAdd:\t\t%p\n\t\t\tsemMulti:\t%p\n\t\t\tmutex:\t\t%p\n\n",
		(void*)sync, (void*)semAdd, (void*)semMulti, (void*)mutex);
	
	
	paramsAdd[0].m_sync = sync;
	paramsAdd[0].m_num = adding/2 - 1;
	paramsAdd[1].m_sync = sync;
	paramsAdd[1].m_num = adding - paramsAdd[0].m_num;
	
	paramsMulti[0].m_sync = sync;
	paramsMulti[0].m_num = multiplying/2 - 1;
	paramsMulti[1].m_sync = sync;
	paramsMulti[1].m_num = multiplying - paramsMulti[0].m_num;
		
		
	
	pthread_create(&add[0], 0, Add1, (void *)&paramsAdd[0]);
	printf("created addThread1\n");
	
	pthread_create(&multiply[0], 0, Multiply2, (void *)&paramsMulti[0]);
	printf("created multiThread1\n");
	sleep(1);
	
	pthread_create(&add[1], 0, Add1, (void *)&paramsAdd[1]);
	printf("created addThread2\n");
	
	pthread_create(&multiply[1], 0, Multiply2, (void *)&paramsMulti[1]);
	printf("created multiThread2\n");
	
	
	pthread_join(add[0], 0);
	pthread_join(add[1], 0);
	pthread_join(multiply[0], 0);
	pthread_join(multiply[1], 0);
	
	
	printf("\nfinal result: %d\n\n", g_number);
	
	SyncPrint(sync);
	printf("\n");
	
	free(mem);
	return 0;
}
/*######################################################################*/





/*######################################################################*/
/* Add num */
void *Add1(void *params)
{
	int i;
	int num = (*(Params_t*)params).m_num;
	Sync_t* sync = (*(Params_t*)params).m_sync;
	SyncTool_t* semAdd;
	SyncTool_t* semMulti;
	SyncTool_t* mutex;
	
	semAdd = SyncAttach(sync, SEMADD);
	semMulti = SyncAttach(sync, SEMMULTI);
	mutex = SyncAttach(sync, MUTEX); 
	
	
/*	printf("Addresses in Add: semAdd: %p, semMulti: %p, mutex: %p\n", (void*)semAdd, (void*)semMulti, (void*)mutex);*/
	
printf("line %d: semAdd = %d, semMulti = %d, mutex = %d\n", __LINE__,
	SyncSemGetVal(semAdd), SyncSemGetVal(semMulti), SyncSemGetVal(mutex));
	
	SyncSemDown(semAdd);
	printf("Add: add down\n");
	
	SyncMutexLock(mutex);
	printf("Add: lock\n");
	
printf("line %d: semAdd = %d, semMulti = %d, mutex = %d\n", __LINE__,
	SyncSemGetVal(semAdd), SyncSemGetVal(semMulti), SyncSemGetVal(mutex));
	
	for(i=0; i<num; ++i) {	/* actual action - add 1 to g_number *num times */
		++g_number;
		printf("+1, result: %d\n", g_number);
	}

printf("line %d: semAdd = %d, semMulti = %d, mutex = %d\n", __LINE__,
	SyncSemGetVal(semAdd), SyncSemGetVal(semMulti), SyncSemGetVal(mutex));
	
	SyncMutexUnlock(mutex);
	printf("Add: unlock\n");
	
	SyncSemUp(semMulti);
	printf("Add: multi up\n");
	
printf("line %d: semAdd = %d, semMulti = %d, mutex = %d\n", __LINE__,
	SyncSemGetVal(semAdd), SyncSemGetVal(semMulti), SyncSemGetVal(mutex));
	
	pthread_exit(0);
}
/*######################################################################*/



/*######################################################################*/
/* Add num */
void *Multiply2(void *params)
{
	int i;
	int num = (*(Params_t*)params).m_num;
	Sync_t* sync = (*(Params_t*)params).m_sync;
	SyncTool_t* semAdd;
	SyncTool_t* semMulti;
	SyncTool_t* mutex;
	
	semAdd = SyncAttach(sync, SEMADD);
	semMulti = SyncAttach(sync, SEMMULTI);
	mutex = SyncAttach(sync, MUTEX); 
	
/*	printf("Addresses in Multi: semAdd: %p, semMulti: %p, mutex: %p\n", (void*)semAdd, (void*)semMulti, (void*)mutex);*/
	
printf("line %d: semAdd = %d, semMulti = %d, mutex = %d\n", __LINE__,
	SyncSemGetVal(semAdd), SyncSemGetVal(semMulti), SyncSemGetVal(mutex));
	
	SyncSemDown(semMulti);
	printf("Multi: multi down\n");

	SyncMutexLock(mutex);
	printf("Multi: lock\n");
	
printf("line %d: semAdd = %d, semMulti = %d, mutex = %d\n", __LINE__,
	SyncSemGetVal(semAdd), SyncSemGetVal(semMulti), SyncSemGetVal(mutex));
	
	for(i=0; i<num; ++i) {	/* actual action - multiply g_number by 2 *num times */
		g_number *= 2;
		printf("*2, result: %d\n", g_number);
	}
	
printf("line %d: semAdd = %d, semMulti = %d, mutex = %d\n", __LINE__,
	SyncSemGetVal(semAdd), SyncSemGetVal(semMulti), SyncSemGetVal(mutex));
	
	SyncMutexUnlock(mutex);
	printf("Multi: unlock\n");
	
	SyncSemUp(semAdd);
	printf("Multi: add up\n");
	
printf("line %d: semAdd = %d, semMulti = %d, mutex = %d\n", __LINE__,
	SyncSemGetVal(semAdd), SyncSemGetVal(semMulti), SyncSemGetVal(mutex));
	
	pthread_exit(0);
}
/*######################################################################*/




/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 09.02.14                                                                                              
*    LAST MODIFIED: 09.02.14                                                                                  
*    DESCRIPTION: shared memory + sem
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
#include <sys/sem.h>
#include <sys/shm.h>

/*-----------------------------------------------------------------*/

#include "shared.h"

/*-----------------------------------------------------------------*/

int InitSem(int* _semid, char* _fileName, int _semgetFlags, int _isCreate)
{
	key_t key;
	union semun arg;
	
	/*get sem*/
	if ((key = ftok(_fileName, FILE_ID)) == -1) {
        perror("ftok");
        return ERR_FTOK;
    }

	if ((*_semid = semget(key, NUM_OF_SEMS ,  SEMGET_PERMISIONS | _semgetFlags)) == -1) {
        perror("semget");
        return ERR_SEMGET;
    }
    
    /*only first ping : set NOTIFY_PING_SEM sem counter = 1 */
    if(_isCreate){
		arg.val = 1;
		if (semctl(*_semid, NOTIFY_PING_SEM, SETVAL, arg) == -1) {
		    perror("semctl");
		    return ERR_SEMCTL;
		}
	}

	return OK;
}

/*-----------------------------------------------------------------*/

int InitSharedMem(int* _shMemId, char* _fileName, int _size , int _msggetFlags)
{
	key_t key;
	
	if ((key = ftok(_fileName, FILE_ID)) == -1) {
        perror("ftok");
        return ERR_FTOK;
    }

	if ((*_shMemId = shmget(key, _size , MSGGET_PERMISIONS | _msggetFlags)) == -1) {
        perror("shmget");
        return ERR_SHMGET;
    }
    
    return OK;
}

/*-----------------------------------------------------------------*/


int RmvSharedMemAndSem(int _shMemId, int _semid)
{
	union semun arg;
	 
	/*remove queue*/
	if (shmctl(_shMemId, IPC_RMID, NULL) == -1) {
	    perror("shmctl");
	    return ERR_SHMCTL;
	}
	PRINT_V("msg queue was removed\n");
	
	/*remove sem*/
	if (semctl(_semid, 0, IPC_RMID, arg) == -1) {
	    perror("semctl");
	    return ERR_SEMCTL;
	}
	PRINT_V("sem was removed\n");
	
	return OK;
}	

/*-----------------------------------------------------------------*/

int SemOp(int _semId , int _semNum , int _op , int _flags )
{
	struct sembuf semBuf;
	
	semBuf.sem_num = _semNum;
    semBuf.sem_flg = _flags;
	semBuf.sem_op = _op;  	
	
	if (semop(_semId, &semBuf, 1) == -1) {
	    perror("semop");
	    return ERR_SEMOP;
	}
	
	return OK;
}

/*-----------------------------------------------------------------*/

/*atomic down&up*/
int DownUpSemPing(int _semId, int _semNum, int _flags)
{
	struct sembuf semBuf[2];
	
	semBuf[0].sem_num = _semNum;
    semBuf[0].sem_flg = _flags;
	semBuf[0].sem_op = DOWN; 

	semBuf[1].sem_num = _semNum;
    semBuf[1].sem_flg = _flags;
	semBuf[1].sem_op = UP;   	
	
	if (semop(_semId, semBuf, 2) == -1) {
	    perror("semop");
	    return ERR_SEMOP;
	}
	
	return OK;


}

/*-----------------------------------------------------------------*/

int CountOfSem(int _semId , int _semNum , int* _semCount)
{
	int numOfPings;
	union semun arg;
	
	if ( (numOfPings = semctl(_semId, _semNum , GETVAL, arg)) == -1) {
		perror("semctl");
		return ERR_SEMCTL;
	}
	
	*_semCount = numOfPings;
		
	return OK;
}

/*-----------------------------------------------------------------*/







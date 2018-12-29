/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-29    
    Last modified date:		2013-08-29
    Description: 	memory structure & shared memory objects
****************************************************************************/


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define PERMISSIONS		0666
#define KEYGEN			301
#define SIZE			getpagesize()


const char* filename = "MemSharedTemp.c";

int main()
{
	int shmid;
	key_t key;
	char* data;
	
	/* generate key */
	if( -1 == (key = ftok(filename, KEYGEN)) )
	{
		perror("ftok");
		return -1;
	}
	/* get mem id */
	if( -1 == (shmid = shmget(key, SIZE, PERMISSIONS | IPC_CREAT)) )
	{
		perror("shmget");
		return -1;
	}
	/* attach */
	if( (char *)(-1) == (data = shmat(shmid, (void *)0, 0)) )
	{
		perror("shmat");
		return -1;
	}
	
	
	
	
	
	
	
	
	
	/* detach */
	if( -1 == shmdt(data) )
	{
		perror("shmdt");
		return -1;
	}
	/* close */
	if( -1 == (shmctl(shmid, IPC_RMID, NULL)) )
	{
		perror("shmctl");
		return -1;
	}
	
	return 0;
}






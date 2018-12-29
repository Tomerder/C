/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-18    
    Last modified date:		2013-09-18
    Description: 	some tests for shared memory module
********************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h> 

#include "SharedMem.h"

/*######################################################################*/

#define SIZE			getpagesize()
#define ERRPRINT(X)		fprintf(stderr, "error: %s, line: %d\n", X, __LINE__)


/*######################################################################*/
int main()
{
	void* shmem1 = NULL;
	void* shmem2 = NULL;
	const char* filename = "SharedMemTest.c";
	const int keygen = 42;
	char* msg = "ABCDE 12345";
	char buffer[20];
	int shmid;
	int length;
	int offset = 32;
	int msgInt = 400;
	int bufferInt;
	
	
	shmid = ShmemGet(filename, keygen, SIZE, IPC_CREAT);
	if(-1 == shmid)
	{
		ERRPRINT("ShmemGet");
		return -1;
	}
	printf("\ngot ID: %d\n\n", shmid);
	
	
	shmem1 = ShmemConnect(shmid, 0);
	if(!shmem1)
	{
		ERRPRINT("ShmemConnect");
		return -1;
	}
	
	printf("connected, address: %p\n\n", shmem1);
	
	printf("writing: %s\n", msg);
	length = strlen(msg);
	memcpy(shmem1, msg, length);
	
	memcpy(buffer, shmem1, length);
	buffer[length] = '\0';
	printf("reading: %s\n\n", buffer);
	
	
	printf("writing integer: %d at offset of %d bytes\n", msgInt, offset);
	*(int*)( (char*)shmem1 + offset ) = msgInt;
	
	bufferInt = *(int*)( (char*)shmem1 + offset );
	printf("reading: %d\n\n", bufferInt);
	
	ShmemDisconnect(shmem1, shmid, 0);
	memset(buffer, 0, sizeof(buffer));
	bufferInt = 0;
	
	printf("disconnected & reset all buffers\n\n");
	
	
	
	shmem2 = ShmemConnect(shmid, 0);
	if(!shmem2)
	{
		ERRPRINT("ShmemConnect");
		return -1;
	}
	
	printf("reconnected, address: %p\n\n", shmem1);
	
	memcpy(buffer, shmem1, length);
	buffer[length] = '\0';
	printf("reading: %s\n", buffer);
	
	bufferInt = *(int*)( (char*)shmem1 + offset );
	printf("reading: %d\n\n", bufferInt);
	
	ShmemDisconnect(shmem2, shmid, 1);
	
	return 0;
}
/*######################################################################*/



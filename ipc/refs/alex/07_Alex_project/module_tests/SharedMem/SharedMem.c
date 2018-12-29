/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-17    
    Last modified date:		2013-09-18
    Description: 	shared memory functions
********************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif


#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h>

#include "SharedMem.h"


/*######################################################################*/
/*				DEFINITIONS & FOREWARD DECLARATIONS						*/
/*######################################################################*/

#define		HANDLE_ERRNO(X,S,R)			if(-1 == (X)) { perror(S); return(R); }
#define		HANDLE_ERRNO_VOID(X,S,R)	if((void*)-1 == (X)) { perror(S); return(R); }
#define		HANDLE_ERRNO_NOR(X,S)		if(-1 == (X)) { perror(S); return; }

#define PERMISSIONS		0666

/*######################################################################*/
/*							API FUNCTIONS								*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* get ID of shared memory segment, creating it if doesn't exist and flags include IPC_CREAT */
/* returns ID, or -1 for error */
int		ShmemGet		(const char* _name, int _keygen, size_t _size, int _flags)
{
	key_t key;
	int shmid;
	
	assert(_name && _keygen);
	if( _flags | IPC_CREAT ) {
		assert(_size);
	}
	
	/* generate key */
	key = ftok(_name, _keygen);
	HANDLE_ERRNO(key, "ShmemGet", -1);
	
	/* get ID */
	shmid = shmget(key, _size, PERMISSIONS | _flags);
	HANDLE_ERRNO(shmid, "ShmemGet", -1);
	
	return shmid;
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* connect to shared memory segment and get process specific address, or NULL for errors */
void*	ShmemConnect	(int _shmid, int _flags)
{
	void* shmem;
	
	shmem = shmat(_shmid, NULL, _flags);
	HANDLE_ERRNO_VOID(shmem, "ShmemConnect", NULL);
	
	return shmem;
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* disconnect from shared memory, and destroy if _destroy = true */
/* ( destroy takes effect only after all processes are disconnected ) */
void	ShmemDisconnect	(void* _shmem, int _shmid, int _destroy)
{
	int error;
	
	assert(_shmem);
	
	/* detach */
	error = shmdt(_shmem);
	HANDLE_ERRNO_NOR(error, "ShmemDisconnect");
	
	/* destroy */
	if(_destroy)
	{
		error = shmctl(_shmid, IPC_RMID, NULL);
		HANDLE_ERRNO_NOR(error, "ShmemDisconnect");
	}
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/





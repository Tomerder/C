/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-17    
    Last modified date:		2013-09-30
    Description: 	shared memory functions
********************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

#include <stdlib.h>
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

#define PERMISSIONS		0666
#define KEYGEN			42

/* shared memory data - ID + address */
struct Shmem
{
	int 	m_id;
	void*	m_address;
};


/*######################################################################*/
/*							API FUNCTIONS								*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* create Shmem_t object for shared memory identified by _name */
/* create shared mem. if doesn't exist & _create set to 1 */
/* returns NULL for error */
Shmem_t*	ShmemGet	(const char* _name, size_t _size, int _create)
{
	int flags = PERMISSIONS | ( (_create) ? IPC_CREAT : 0 );
	Shmem_t* shmem = NULL;
	key_t key;
	
	assert(_name);
	if( _create ) {
		assert(_size);
	}
	
	/* allocate Shmem_t struct */
	shmem = (Shmem_t*)malloc(sizeof(Shmem_t));
	if(!shmem) {
		fprintf(stderr, "ShmemGet: malloc failed\n");
		goto MALLOC_FAIL;
	}
	
	/* generate key */
	key = ftok(_name, KEYGEN);
	if(-1 == key) {
		goto OTHER_FAIL;
	}
	
	/* get ID */
	shmem->m_id = shmget(key, _size, flags);
	if(-1 == shmem->m_id) {
		goto OTHER_FAIL;
	}
	
	/* get address */
	shmem->m_address = shmat(shmem->m_id, NULL, 0);
	if( (void*)-1 == shmem->m_address ) {
		goto OTHER_FAIL;
	}
	
	return shmem;
	
	
/* handle errors */
OTHER_FAIL:
	perror("ShmemGet");
	free(shmem);

MALLOC_FAIL:
	return NULL;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* get process-specific address of shared memory */
void*	ShmemConnect	(Shmem_t* _shmem)
{
	assert(_shmem);
	return _shmem->m_address;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* disconnect from shared memory, and destroy if _destroy = true */
/* ( destroy takes effect only after all processes are disconnected ) */
void	ShmemDisconnect	(Shmem_t* _shmem)
{
	int error;
	
	assert(_shmem);
	
	error = shmdt(_shmem->m_address);
	if(-1 == error) {
		perror("ShmemDisconnect");
	}
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* destroy shared memory (takes effect only after all processes are disconnected ) */
void	ShmemDestroy	(Shmem_t* _shmem)
{
	int error;
	
	assert(_shmem);

	error = shmctl(_shmem->m_id, IPC_RMID, NULL);
	if(-1 == error) {
		perror("ShmemDestroy");
	}
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/



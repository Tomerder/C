/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-17    
    Last modified date:		2013-09-30
    Description: 	synchronization tools - mutex & semaphore, for use in shared memory.
									
					currently implemented with POSIX semaphore.
						note: no destroy function b/c posix sem is on shared mem.
						note: link with -pthread flag
********************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <assert.h>

#include "Sync.h"


/*######################################################################*/
/*				DEFINITIONS & FOREWARD DECLARATIONS						*/
/*######################################################################*/


#define		HANDLE_ERRNO(X,S,R)		if(-1 == (X)) { perror(S); return(R); }

/* succes/failure return values */
enum Success
{
	SUCCESS = 0,
	FAIL = -1
};

/* a single tool */
struct SyncTool
{
	int		m_type;
	sem_t	m_tool;
};

/* a "bundle" of tools */
struct Sync
{
	size_t		m_nItems;
	SyncTool_t	m_toolArray[1];
};

/* internal - for debug */
void	ToolPrintData	(SyncTool_t* _tool);


/*######################################################################*/
/*							API FUNCTIONS								*/
/*######################################################################*/


/*######################################################################*/
/*						SYNC GENERAL FUNCTIONS							*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* create sync struct at address, return NULL for errors */
Sync_t*	SyncCreate	(void* _addr, int _nItems)
{
	Sync_t* newSync = (Sync_t*)_addr;
	int i;
	
	assert( _addr && _nItems );
	
	/* "initialize" as uninitialized */
	for(i=0; i<_nItems; ++i)
	{
		newSync->m_toolArray[i].m_type = NONE;
	}
	
	newSync->m_nItems = _nItems;
	
	return newSync;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* destroy - currently empty function */
void	SyncDestroy(Sync_t* _sync)
{
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* return # of bytes needed for Sync */
size_t	SyncGetSize	(int _nItems)
{
	return ( sizeof(Sync_t) + sizeof(SyncTool_t) * (_nItems-1) );
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* initialize tool #_num: type (SEMAPHORE or MUTEX) and initial value */
/* return success (0) / fail (-1) */
int		SyncToolInit	(Sync_t* _sync, int _num, int _type, int _val)
{
	int error;
	
	/* check that tool is uninitialized (type == NONE) */
	assert( _sync && (_sync->m_toolArray[_num].m_type == NONE) );
	
	if( (MUTEX == _type) && (_val > 1) )
	{
		fprintf(stderr, "SyncToolInit: mutex value must be 0 or 1\n");
		return FAIL;
	}
	
	_sync->m_toolArray[_num].m_type = _type;
	
	error = sem_init( &_sync->m_toolArray[_num].m_tool, 1, _val);
	HANDLE_ERRNO(error, "SyncToolInit", FAIL);
		
	return SUCCESS;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* return address of item # _num */
SyncTool_t*	SyncAttach	(Sync_t* _sync, int _num)
{
	return (SyncTool_t*)( (char*)_sync + sizeof(Sync_t) + sizeof(SyncTool_t) * (_num-1) );
}
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								SEMAPHORE								*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* return success (0) / fail (-1) */
int		SyncSemUp		(SyncTool_t* _sem)
{
	assert(_sem && (_sem->m_type == SEMAPHORE) );
	return sem_post(&_sem->m_tool);
}
/*----------------------------------------------------------------------*/
int		SyncSemDown		(SyncTool_t* _sem)
{
	assert(_sem && (_sem->m_type == SEMAPHORE) );
	return sem_wait(&_sem->m_tool);
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* return semaphore value or -1 for error */
int		SyncSemGetVal	(SyncTool_t* _sem)
{
	int value;
	int error;
	
	assert(_sem);
	
	error = sem_getvalue(&_sem->m_tool, &value);
	HANDLE_ERRNO(error, "SyncSemGetVal", FAIL);
	
	return value;
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								MUTEX									*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
int		SyncMutexLock	(SyncTool_t* _mutex)
{
	assert(_mutex && (_mutex->m_type == MUTEX) );
	return sem_wait(&_mutex->m_tool);
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
int		SyncMutexUnlock	(SyncTool_t* _mutex)
{
	assert(_mutex && (_mutex->m_type == MUTEX) );
	return sem_post(&_mutex->m_tool);
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

/*######################################################################*/


/*----------------------------------------------------------------------*/
/*							PRINT - For Debug							*/
/*----------------------------------------------------------------------*/
/* for debug */
void	SyncPrint	(Sync_t* _sync)
{
	int i;
	
	printf("Sync # of items: %d,\t\t\taddress: %p\n", _sync->m_nItems, (void*)_sync);
	
	for(i=0; i < _sync->m_nItems; ++i)
	{
		ToolPrintData( &_sync->m_toolArray[i] );
	}
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
void	ToolPrintData (SyncTool_t* _tool)
{
	printf("Tool type: %s\tvalue: %d\taddress: %p\n",
		(SEMAPHORE==_tool->m_type) ? "Semaphore" : ( (MUTEX==_tool->m_type) ? 
		"Mutex" : "Undefined" ), SyncSemGetVal(_tool), (void*)_tool);
}
/*----------------------------------------------------------------------*/



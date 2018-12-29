/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-17    
    Last modified date:		2013-09-17
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

enum Success
{
	SUCCESS = 0,
	FAIL = -1
};

struct SyncTool
{
	int		m_type;
	sem_t	m_tool;
};


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
/*								SYNC									*/
/*######################################################################*/


/*----------------------------------------------------------------------*/
/* create sync struct at address, return NULL for errors */
Sync_t*	SyncCreate	(void* _addr, int _nItems, int* _initVals)
{
	Sync_t* newSync = (Sync_t*)_addr;
	int i, error;
	
	assert( _addr && _nItems && _initVals );
	
	for(i=0; i<_nItems; ++i)
	{
		if( _initVals[i] < 0 ) {
			return NULL;
		}
	}
	
	for(i=0; i<_nItems; ++i)
	{
		error = sem_init( &newSync->m_toolArray[i].m_tool, 1, _initVals[i]);
		HANDLE_ERRNO(error, "SyncCreate", NULL);
		
		newSync->m_toolArray[i].m_type = UNDEFINED;
	}
	
	newSync->m_nItems = _nItems;
	
	return newSync;
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* destroy - currently empty */
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
/* return address of item # _num */
SyncTool_t*	SyncAttach	(Sync_t* _sync, int _num)
{
	return (SyncTool_t*)( (char*)_sync + sizeof(Sync_t) + sizeof(SyncTool_t) * (_num-1) );
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* set type of tool #_num as either SEMAPHORE or MUTEX */
/* use of this function is optional: type is set automatically
on first use of type-specific function (up/down for semaphore,
lock/unlock for mutex */
int		SyncToolSetType	(Sync_t* _sync, int _num, int _type)
{
	if( _type != SEMAPHORE && _type != MUTEX )	{
		return FAIL;
	}
	
	_sync->m_toolArray[_num].m_type = _type;
	return SUCCESS;
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*								SEMAPHORE								*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* return success (0) / fail (-1) */
int		SyncSemUp		(SyncTool_t* _sem)
{
	assert(_sem);
	
	if( _sem->m_type != SEMAPHORE )
	{
		if( _sem->m_type != UNDEFINED ) {
			return FAIL;
		}
		_sem->m_type = SEMAPHORE;
	}
	
	return sem_post(&_sem->m_tool);
}
/*----------------------------------------------------------------------*/
int		SyncSemDown		(SyncTool_t* _sem)
{
	assert(_sem);
	
	if( _sem->m_type != SEMAPHORE )
	{
		if( _sem->m_type != UNDEFINED ) {
			return FAIL;
		}
		_sem->m_type = SEMAPHORE;
	}
	
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
	assert(_mutex);
	
	if( _mutex->m_type != MUTEX )
	{
		if( _mutex->m_type != UNDEFINED ) {
			return FAIL;
		}
		_mutex->m_type = MUTEX;
	}
	
	return sem_wait(&_mutex->m_tool);
}
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
int		SyncMutexUnlock	(SyncTool_t* _mutex)
{
	assert(_mutex);
	
	if( _mutex->m_type != MUTEX )
	{
		if( _mutex->m_type != UNDEFINED ) {
			return FAIL;
		}
		_mutex->m_type = MUTEX;
	}
	
	return sem_post(&_mutex->m_tool);
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/


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



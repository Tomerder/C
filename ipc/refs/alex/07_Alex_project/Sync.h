/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-17    
    Last modified date:		2013-09-30
    Description: 	synchronization tools - mutex & semaphore, for use in shared memory.
									
					currently implemented with POSIX semaphore.
						note: no destroy function b/c posix sem is on shared mem.
						note: link with -pthread flag
********************************************************************************/

#ifndef __SYNC_H__
#define __SYNC_H__


#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

#include <pthread.h>


typedef  struct Sync 		Sync_t;		/* a "bundle" of tools */
typedef  struct SyncTool	SyncTool_t;	/* a single tool */

/* types of synctools */
enum SyncType
{
	NONE,
	SEMAPHORE,
	MUTEX
};

/*----------------------------------------------------------------------*/
/*							SYNC GENERAL								*/
/*----------------------------------------------------------------------*/

/* create sync struct at address, return NULL for errors */
Sync_t*		SyncCreate	(void* _addr, int _nItems);

/* destroy */
void		SyncDestroy	(Sync_t* _sync);

/* return # of bytes needed for Sync */
size_t		SyncGetSize	(int _nItems);

/* initialize tool #_num: type (SEMAPHORE or MUTEX) and initial value */
/* return success (0) / fail (-1) */
int		SyncToolInit	(Sync_t* _sync, int _num, int _type, int _val);

/* return address of item # _num */
SyncTool_t*	SyncAttach	(Sync_t* _sync, int _num);

/* for debug */
void	SyncPrint	(Sync_t* _sync);

/*----------------------------------------------------------------------*/
/*								SEMAPHORE								*/
/*----------------------------------------------------------------------*/

/* return success (0) / fail (-1) */
int		SyncSemUp		(SyncTool_t* _sem);
int		SyncSemDown		(SyncTool_t* _sem);

/* return sem value or -1 for error */
int		SyncSemGetVal	(SyncTool_t* _sem);

/*----------------------------------------------------------------------*/
/*								MUTEX									*/
/*----------------------------------------------------------------------*/

/* return success (0) / fail (-1) */
int		SyncMutexLock	(SyncTool_t* _mutex);
int		SyncMutexUnlock	(SyncTool_t* _mutex);


#endif /* __SYNC_H__ */

/*******************************************************************************
	Author:				Stav Ofer
	Creation date:		2013-09-16
	Date last modified:	2013-09-19
	Description:		Message Queue over Shared Memory in multi-process interlock
********************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/ipc.h>


#include "QueueNew.h"
#include "MPoolNew.h"
#include "Sync.h"
#include "SharedMem.h"

#include "MsgQueue.h"



/*######################################################################*/
/*				DEFINITIONS & FOREWARD DECLARATIONS						*/
/*######################################################################*/

#define	KEYGEN		42
#define NTOOLS		5

#define OFFSET2ADDR(BASE,OFFSET)	(char*)(BASE) + (OFFSET)
#define HANDLE_ERR(X,R)				fprintf(stderr, "%s:%d Error: %s\n", __FILE__, __LINE__, X); return(R)

enum Tools
{
	SEM_WRITE,
	SEM_READ,
	SEM_FULL,
	SEM_EMPTY,
	MUTEXTOOL
};


/* offset table - internal */
typedef struct OffsetTable
{
	size_t	m_sync;
	size_t	m_queue;
	size_t	m_pool;
	
} OffsetTable_t;


/*** Message Queue ***/
struct MQ
{
	void*		m_shmem;
	Sync_t*		m_sync;
	Queue_t*	m_queue;
	MPool*		m_pool;
	int			m_shmid;
	int			m_createFlag;
};


typedef struct Message
{
	int		m_length;
	char	m_msg[1];
} Message_t;



/*######################################################################*/
/*								API FUNCTIONS							*/
/*######################################################################*/


/*----------------------------------------------------------------------*/
/*					CREATE, CONNECT, DESTROY							*/
/*----------------------------------------------------------------------*/

/* create shared memory, allocate in it requred structures, & create process/thread local metadata */
/* return NULL for errors */
MQ_t*	MsgQueueCreate	(char *_msgQueueName, size_t _msgMax, size_t _memSize)
{
	MQ_t *msgQ;
	OffsetTable_t* table = NULL;
	int syncInit[NTOOLS];
	
	assert( _msgQueueName && _msgMax && _memSize);
	
	/* allocate msgQ */
	msgQ = (MQ_t*)malloc(sizeof(MQ_t));
	if( !msgQ ) {
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	/* connect to shared mem */
	
	msgQ->m_shmid = ShmemGet(_msgQueueName, KEYGEN, _memSize, IPC_CREAT);
	if( -1 == msgQ->m_shmid ) {
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	msgQ->m_shmem = ShmemConnect(msgQ->m_shmid, 0);
	if( !msgQ->m_shmem ) {
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	/* create offset table & start filling */
	table = (OffsetTable_t*)msgQ->m_shmem;
	table->m_sync = sizeof(OffsetTable_t);
	
	/* create sync tools - 5 tools, 4 semaphores and 1 mutex */
	
	syncInit[SEM_WRITE] = 1;
	syncInit[SEM_READ]  = 1;
	syncInit[SEM_FULL]  = _msgMax;
	syncInit[SEM_EMPTY] = 0;
	syncInit[MUTEXTOOL] = 1;
	
	msgQ->m_sync = SyncCreate( OFFSET2ADDR(msgQ->m_shmem, table->m_sync), NTOOLS, syncInit);
	if( !msgQ->m_sync ) {
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	SyncToolSetType(msgQ->m_sync, SEM_WRITE, SEMAPHORE);
	SyncToolSetType(msgQ->m_sync, SEM_READ,  SEMAPHORE);
	SyncToolSetType(msgQ->m_sync, SEM_FULL,  SEMAPHORE);
	SyncToolSetType(msgQ->m_sync, SEM_EMPTY, SEMAPHORE);
	SyncToolSetType(msgQ->m_sync, MUTEXTOOL, MUTEX);
	
	table->m_queue = table->m_sync + SyncGetSize(NTOOLS);
	
	/* create queue of message offsets */
	
	msgQ->m_queue = QueueCreate( OFFSET2ADDR(msgQ->m_shmem, table->m_queue), _msgMax);
	if( !msgQ->m_queue ) {
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	table->m_pool = table->m_queue + QueueGetSize(_msgMax);
	
	/* create MPool in remaining memory */
	
	if( _memSize < table->m_pool ) {						/* ??? keep this? ??? */
		HANDLE_ERR("MsgQueueCreate: not enough memory", NULL);
	}
	
	msgQ->m_pool = MPoolInit( OFFSET2ADDR(msgQ->m_shmem, table->m_pool),  _memSize - table->m_pool );
	
	/* raise create flag */
	msgQ->m_createFlag = 1;
	
	return msgQ;
}
/*----------------------------------------------------------------------*/

/* connect to shared memory & create process/thread local metadata */
/* return NULL for errors */
MQ_t*	MsgQueueConnect	(char *_msgQueueName)
{
	MQ_t *msgQ;
	OffsetTable_t* table = NULL;
	
	assert( _msgQueueName );
	
	/* allocate msgQ */
	msgQ = (MQ_t*)malloc(sizeof(MQ_t));
	if( !msgQ ) {
		HANDLE_ERR("MsgQueueConnect", NULL);
	}
		
	/* connect to shared mem */
	
	msgQ->m_shmid = ShmemGet(_msgQueueName, KEYGEN, 1, 0);
	if( -1 == msgQ->m_shmid ) {
		HANDLE_ERR("MsgQueueConnect", NULL);
	}
	
	msgQ->m_shmem = ShmemConnect(msgQ->m_shmid, 0);
	if( !msgQ->m_shmem ) {
		HANDLE_ERR("MsgQueueConnect", NULL);
	}
	
	/* get table & set msgQ pointers */
	
	table = (OffsetTable_t*)msgQ->m_shmem;
	
	msgQ->m_sync	= (Sync_t*) ( OFFSET2ADDR(msgQ->m_shmem, table->m_sync) );
	msgQ->m_queue	= (Queue_t*)( OFFSET2ADDR(msgQ->m_shmem, table->m_queue) );
	msgQ->m_pool	= (MPool*)  ( OFFSET2ADDR(msgQ->m_shmem, table->m_pool) );
	
	msgQ->m_createFlag = 0;
	
	return msgQ;
}
/*----------------------------------------------------------------------*/

/* disconnect from shared memory & free _msgQue */
/* if shared memory was created in this session, it is destroyed upon
	disconnection of last process */ 
void	MsgQueueDestroy	(MQ_t *_msgQue)
{
	assert( _msgQue );
	ShmemDisconnect( _msgQue->m_shmem, _msgQue->m_shmid, _msgQue->m_createFlag );
	free( _msgQue );
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*				Message Manipulation: Send, Receive						*/
/*----------------------------------------------------------------------*/

/* return number of bytes actually written, 0 if message queue (mpool) full or other error */
/* return value include 1 int of message length */
int		MsgQueueSend	(MQ_t *_msgQue, void* _buffer, size_t _length)
{
	SyncTool_t* semWrite;
	SyncTool_t* semFull;
	SyncTool_t* semEmpty;
	SyncTool_t* mutex;
	void* destination;
	int offset;
	int writing = _length + sizeof(int);
	
	assert( _msgQue && _buffer && _length);
	
	/* connect to sync tools */
	semWrite	= SyncAttach(_msgQue->m_sync, SEM_WRITE);
	semFull		= SyncAttach(_msgQue->m_sync, SEM_FULL);
	semEmpty	= SyncAttach(_msgQue->m_sync, SEM_EMPTY);
	mutex		= SyncAttach(_msgQue->m_sync, MUTEXTOOL);
	
	/* wait if full */
	if( -1 == SyncSemDown(semFull) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	/* lock for writing */
	if( -1 == SyncSemDown(semWrite) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
/********** START mutex-protected section **********/
	
	/* lock mutex */
	SyncMutexLock(mutex);
	
	/* allocate memory */
	destination = MPoolAlloc(_msgQue->m_pool, writing);
	if( !destination )
	{
		SyncMutexUnlock(mutex);
		SyncSemUp(semWrite);
		SyncSemUp(semFull);
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	/* calc & insert offset */
	offset = (unsigned int)destination - (unsigned int)_msgQue->m_pool;
	QueueInsert(_msgQue->m_queue, offset);
	
	/* unlock mutex */
	SyncMutexUnlock(mutex);
	
/********** END mutex-protected section **********/
	
	/* unlock for writing */
	if( -1 == SyncSemUp(semWrite) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	/* write message */
	*(int*)destination = _length;
	memcpy( OFFSET2ADDR(destination, sizeof(int)), _buffer, _length);
	
	/* update empty semaphore */
	if( -1 == SyncSemUp(semEmpty) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	return writing;
}
/*----------------------------------------------------------------------*/

/* return number of bytes actually read, 0 if message queue unavailable */ 
int		MsgQueueRecv	(MQ_t *_msgQue, void* _buffer, size_t _size)
{
	SyncTool_t* semRead;
	SyncTool_t* semFull;
	SyncTool_t* semEmpty;
	SyncTool_t* mutex;
	void* source;
	int offset;
	int reading;
	
	assert( _msgQue && _buffer && _size);
	
	/* connect to sync tools */
	semRead 	= SyncAttach(_msgQue->m_sync, SEM_READ);
	semFull		= SyncAttach(_msgQue->m_sync, SEM_FULL);
	semEmpty	= SyncAttach(_msgQue->m_sync, SEM_EMPTY);
	mutex		= SyncAttach(_msgQue->m_sync, MUTEXTOOL);
	
	/* wait if empty */
	if( -1 == SyncSemDown(semEmpty) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	/* lock for reading */
	if( -1 == SyncSemDown(semRead) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
/********** START mutex-protected section **********/
	
	/* lock mutex */
	SyncMutexLock(mutex);
	
	/* get offset from queue */
	if( -1 == QueueRemove(_msgQue->m_queue, &offset) )
	{		
		SyncMutexUnlock(mutex);
		SyncSemUp(semRead);
		SyncSemUp(semEmpty);
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	/* unlock mutex */
	SyncMutexUnlock(mutex);										/* !!!!!!! here? !!!!! */
	
/********** END mutex-protected section **********/
	
	/* unlock for reading */
	if( -1 == SyncSemUp(semRead) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	/* address */
	source = OFFSET2ADDR(_msgQue->m_pool, offset);
	
	/* get message length, then message */
	reading = (*(int*)source > _size) ? _size : *(int*)source;
	memcpy(_buffer, OFFSET2ADDR( source, sizeof(int) ), reading);
	
	/* free memory */
	MPoolFree(_msgQue->m_pool, source);
	
	/* update full semaphore */
	if( -1 == SyncSemUp(semFull) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	return reading;
}
/*----------------------------------------------------------------------*/



/*----------------------------------------------------------------------*/
/*								IsEmpty									*/
/*----------------------------------------------------------------------*/
int		IsMsgQueueEmpty (MQ_t *_msgQue)
{
	return IsQueueEmpty(_msgQue->m_queue);
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/* Utility Functions for debugging and unit testing */
/*----------------------------------------------------------------------*/
void	MsgQueuePrint	(MQ_t* _msgQue)
{
	/**/
}
/*----------------------------------------------------------------------*/



/*######################################################################*/
/*							INTERNAL FUNCTIONS							*/
/*######################################################################*/





/*******************************************************************************
	Author:				Stav Ofer
	Creation date:		2013-09-16
	Date last modified:	2013-09-30
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

#define NTOOLS			3

#define MIN_POOL_SIZE	32

#define OFFSET2ADDR(BASE,OFFSET)	(char*)(BASE) + (OFFSET)
#define HANDLE_ERR(X,R)				fprintf(stderr, "%s:%d Error: %s\n", __FILE__, __LINE__, X); return(R)

/* sync tool IDs */
enum Tools
{
	SEM_FULL,
	SEM_EMPTY,
	MUTEXTOOL
};

/* succes/failure return values */
enum Success
{
	SUCCESS = 0,
	FAIL = -1
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
	Shmem_t*	m_shmem;		/* shared mem object */
	void*		m_address;		/* address of shared mem */
	Queue_t*	m_queue;		/* queue of message offsets */
	MPool*		m_pool;			/* memory pool for dynamic allocation of messages */
	SyncTool_t* m_semFull;		/* semaphore for full queue */
	SyncTool_t* m_semEmpty;		/* semaphore for empty queue */
	SyncTool_t* m_mutex;		/* mutex */
	int			m_createFlag;	/* 1 if this process created the shared mem, 0 otherwise */
};


/* create msgQ, create/connect to shared memory, return NULL for error */
static MQ_t*	MsgQMemCreate	(char *_msgQueueName, size_t _memSize, int _create);
/* attach semaphores and mutex to messageQ, return success (0) / fail (-1) */
static int 		MsgQSyncAttach	(MQ_t* _msgQ, Sync_t* _sync);

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
	MQ_t *msgQ = NULL;
	OffsetTable_t* table = NULL;
	Sync_t* sync = NULL;
	int tableSize, syncSize, queueSize;
	
	assert( _msgQueueName && _msgMax );
	
	/* calculate sizes, check that _memSize is enough */
	tableSize = sizeof(OffsetTable_t);
	syncSize  = SyncGetSize(NTOOLS);
	queueSize = QueueGetSize(_msgMax);
	
	assert( _memSize >= tableSize + syncSize + queueSize + MIN_POOL_SIZE );
	
	/* create messageQ & shared memory */
	msgQ = MsgQMemCreate(_msgQueueName, _memSize, 1);
	if( !msgQ ) {
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	/* create & populate offset table */
	table = (OffsetTable_t*)msgQ->m_address;
	table->m_sync  = tableSize;
	table->m_queue = table->m_sync + syncSize;
	table->m_pool  = table->m_queue + queueSize;
	
	/* create sync tools: 2 semaphores and 1 mutex */
	sync = SyncCreate( OFFSET2ADDR(msgQ->m_address, table->m_sync), NTOOLS);
	if( !sync )
	{
		MsgQueueDestroy(msgQ);
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	/* init synctools */
	if( -1 == SyncToolInit(sync, SEM_FULL,  SEMAPHORE, _msgMax) ||
		-1 == SyncToolInit(sync, SEM_EMPTY, SEMAPHORE, 0)	||
		-1 == SyncToolInit(sync, MUTEXTOOL, MUTEX, 	 1) )
	{
		MsgQueueDestroy(msgQ);
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	/* attach synctools */
	if( -1 == MsgQSyncAttach(msgQ, sync) )
	{
		MsgQueueDestroy(msgQ);
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	/* create queue of message offsets */
	msgQ->m_queue = QueueCreate( OFFSET2ADDR(msgQ->m_address, table->m_queue), _msgMax);
	if( !msgQ->m_queue )
	{
		MsgQueueDestroy(msgQ);
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	/* create MPool in remaining memory */
	msgQ->m_pool = MPoolInit( OFFSET2ADDR(msgQ->m_address, table->m_pool),  _memSize - table->m_pool );
	if( !msgQ->m_pool )
	{
		MsgQueueDestroy(msgQ);
		HANDLE_ERR("MsgQueueCreate", NULL);
	}
	
	/* recalculate pool offset */
	table->m_pool = (char*)msgQ->m_pool - (char*)msgQ->m_address;
	
	return msgQ;
}
/*----------------------------------------------------------------------*/

/* connect to shared memory & create process/thread local metadata */
/* return NULL for errors */
MQ_t*	MsgQueueConnect	(char *_msgQueueName)
{
	MQ_t *msgQ = NULL;
	Sync_t* sync = NULL;
	OffsetTable_t* table = NULL;
	int error;
	
	assert( _msgQueueName );
	
	/* create messageQ & connect to shared memory */
	msgQ = MsgQMemCreate(_msgQueueName, 1, 0);
	if( !msgQ ) {
		HANDLE_ERR("MsgQueueConnect", NULL);
	}
	
	/* get table & sync and set msgQ pointers */
	table = (OffsetTable_t*)msgQ->m_address;
	msgQ->m_queue	 = (Queue_t*)( OFFSET2ADDR(msgQ->m_address, table->m_queue) );
	msgQ->m_pool	 = (MPool*)  ( OFFSET2ADDR(msgQ->m_address, table->m_pool) );
	
	sync = (Sync_t*) ( OFFSET2ADDR(msgQ->m_address, table->m_sync) );
	error = MsgQSyncAttach(msgQ, sync);
	
	if( -1 == error || !msgQ->m_queue || !msgQ->m_pool )
	{
		MsgQueueDestroy(msgQ);
		HANDLE_ERR("MsgQueueConnect", NULL);
	}
	
	return msgQ;
}
/*----------------------------------------------------------------------*/

/* disconnect from shared memory & free _msgQue */
/* if shared memory was created in this session, it is set to be destroyed
	upon disconnection of last process */ 
void	MsgQueueDestroy	(MQ_t *_msgQue)
{
	assert( _msgQue );
	
	ShmemDisconnect( _msgQue->m_shmem );
	
	if( _msgQue->m_createFlag ) {
		ShmemDestroy( _msgQue->m_shmem );
	}
	
	free( _msgQue );
}
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*				Message Manipulation: Send, Receive						*/
/*----------------------------------------------------------------------*/

/* return number of bytes actually written, 0 if message queue (mpool) full or other error */
/* return value DOES NOT include 1 int of message length */
int		MsgQueueSend	(MQ_t *_msgQue, void* _buffer, size_t _length)
{
	void* destination;
	int offset;
	int writing = _length + sizeof(int);
	
	assert( _msgQue && _buffer && _length);
	
	/* wait if full */
	if( -1 == SyncSemDown(_msgQue->m_semFull) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
/*********** START mutex-protected section ***********/
	if( -1 == SyncMutexLock(_msgQue->m_mutex) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	/* allocate memory */
	if( NULL == ( destination = MPoolAlloc(_msgQue->m_pool, writing) ) )
	{
		SyncMutexUnlock(_msgQue->m_mutex);
		SyncSemUp(_msgQue->m_semFull);
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	if( -1 == SyncMutexUnlock(_msgQue->m_mutex) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
/*********** END mutex-protected section ***********/
	
	/* write message */
	*(int*)destination = _length;
	memcpy( OFFSET2ADDR(destination, sizeof(int)), _buffer, _length);
	
	/* calculate offset */
	offset = (unsigned int)destination - (unsigned int)_msgQue->m_pool;
	
/********** START mutex-protected section **********/
	if( -1 == SyncMutexLock(_msgQue->m_mutex) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	/* insert offset to queue */
	if( -1 == QueueInsert(_msgQue->m_queue, offset) )
	{	
		SyncMutexUnlock(_msgQue->m_mutex);
		SyncSemUp(_msgQue->m_semFull);
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
	if( -1 == SyncMutexUnlock(_msgQue->m_mutex) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
/********** END mutex-protected section **********/
	
	/* update empty semaphore */
	if( -1 == SyncSemUp(_msgQue->m_semEmpty) ) {
		HANDLE_ERR("MsgQueueSend", 0);
	}
	
/*	return written _length;*/
	return _length;
}
/*----------------------------------------------------------------------*/

/* return number of bytes actually read, 0 if message queue unavailable */ 
int		MsgQueueRecv	(MQ_t *_msgQue, void* _buffer, size_t _size)
{
	void* source;
	int offset;
	int reading;
	
	assert( _msgQue && _buffer && _size);
	
	/* wait if empty */
	if( -1 == SyncSemDown(_msgQue->m_semEmpty) ) {
		HANDLE_ERR("MsgQueueRecv", 0);
	}
	
/********** START mutex-protected section **********/
	if( -1 == SyncMutexLock(_msgQue->m_mutex) ) {
		HANDLE_ERR("MsgQueueRecv", 0);
	}
	
	/* get offset from queue */
	if( -1 == QueueRemove(_msgQue->m_queue, &offset) )	
	{		
		SyncMutexUnlock(_msgQue->m_mutex);
		SyncSemUp(_msgQue->m_semEmpty);
		HANDLE_ERR("MsgQueueRecv", 0);
	}
	
	if( -1 == SyncMutexUnlock(_msgQue->m_mutex) ) {
		HANDLE_ERR("MsgQueueRecv", 0);
	}
/********** END mutex-protected section **********/
	
	/* address */
	source = OFFSET2ADDR(_msgQue->m_pool, offset);
	
	/* get message length, then message */
	reading = (*(int*)source > _size) ? _size : *(int*)source;
	memcpy(_buffer, OFFSET2ADDR( source, sizeof(int) ), reading);
	
/********** START mutex-protected section **********/
	if( -1 == SyncMutexLock(_msgQue->m_mutex) ) {
		HANDLE_ERR("MsgQueueRecv", 0);
	}
	
	/* free memory (no return value - can't check errors */
	MPoolFree(_msgQue->m_pool, source);		
	
	if( -1 == SyncMutexUnlock(_msgQue->m_mutex) ) {
		HANDLE_ERR("MsgQueueRecv", 0);
	}
/********** END mutex-protected section **********/

	/* update full semaphore */
	if( -1 == SyncSemUp(_msgQue->m_semFull) ) {
		HANDLE_ERR("MsgQueueRecv", 0);
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
/* Utility Function for debugging and unit testing */
/*void	MsgQueuePrint	(MQ_t* _msgQue)*/
/*{*/
	/**/
/*}*/
/*----------------------------------------------------------------------*/


/*######################################################################*/
/*							INTERNAL FUNCTIONS							*/
/*######################################################################*/

/*----------------------------------------------------------------------*/
/* create msgQ, create/connect to shared memory, return NULL for error */
MQ_t*	MsgQMemCreate(char *_msgQueueName, size_t _memSize, int _create)
{
	MQ_t* msgQ = NULL;
	
	/* allocate msgQ & initialize to all-0 */
	msgQ = (MQ_t*)malloc(sizeof(MQ_t));
	if( !msgQ ) {
		return NULL;
	}
	memset(msgQ, 0, sizeof(MQ_t));
	
	/* set create flag */
	msgQ->m_createFlag = _create;
	
	/* connect to shared mem */
	msgQ->m_shmem = ShmemGet(_msgQueueName, _memSize, _create);
	if( !msgQ->m_shmem )
	{
		MsgQueueDestroy(msgQ);
		return NULL;
	}
	
	/* get shmem address */
	msgQ->m_address = ShmemConnect(msgQ->m_shmem);
	if( !msgQ->m_address )
	{
		MsgQueueDestroy(msgQ);
		return NULL;
	}
	
	return msgQ;
}
/*----------------------------------------------------------------------*/


/* attach semaphores and mutex to messageQ, return success (0) / fail (-1) */
int MsgQSyncAttach(MQ_t* _msgQ, Sync_t* _sync)
{
	_msgQ->m_semFull  = SyncAttach(_sync, SEM_FULL);
	_msgQ->m_semEmpty = SyncAttach(_sync, SEM_EMPTY);
	_msgQ->m_mutex	  = SyncAttach(_sync, MUTEXTOOL);
	
	return ( _msgQ->m_semFull && _msgQ->m_semEmpty && _msgQ->m_mutex ) ? SUCCESS : FAIL;
}
/*----------------------------------------------------------------------*/



/*******************************************************************************
	Author:			(Stav Ofer)
	Creation date:		2013-09-16
	Date last modified:	2013-09-18
	Description:		Message Queue over Shared Memory in multi-process interlock
********************************************************************************/

#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

typedef struct MQ MQ_t;

/*** Message Queue API ***/

/* create shared memory, allocate in it requred structures, & create process/thread local metadata */
/* return NULL for errors */
MQ_t*	MsgQueueCreate	(char *_msgQueueName, size_t _msgMax, size_t _memSize); 

/* connect to shared memory & create process/thread local metadata */
/* return NULL for errors */
MQ_t*	MsgQueueConnect	(char *_msgQueueName);

/* disconnect from shared memory & free _msgQue */
/* if shared memory was created in this session, it is destroyed upon
	disconnection of last process */ 
void	MsgQueueDestroy	(MQ_t *_msqQue);

/* Message Manupulation */

/* return number of bytes actually written, 0 if message queue full	or other error	*/
int		MsgQueueSend	(MQ_t *_msgQue, void* _buffer, size_t _length);
/* return number of bytes actually read, 0 if message queue unavailable */ 
int		MsgQueueRecv	(MQ_t *_msgQue, void* _buffer, size_t _size);

int		IsMsgQueueEmpty (MQ_t *_msgQue);

/* Utility Functions for debugging and unit testing */
void	MsgQueuePrint	(MQ_t* _msgQue);

#endif /* __MSG_QUEUE_H__ */

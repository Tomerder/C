/**************************************************************************************
    Author :				
    Creation date :      	10.1.12
    Date last modified :
    Description : 			Message Queue over Shared Memory in multi-process interlock
***************************************************************************************/
#ifndef __MSG_QUEUE_H_
#define __MSG_QUEUE_H_

/*-----------------------------------------------------------------*/

typedef struct MsgQ MsgQ;

/*-----------------------------------------------------------------*/

/* Message Queue API */
MsgQ*	MsgQueueCreate	(char* _msgQueueName, size_t _MaxNumOfMsgs, size_t _memSize);  


MsgQ*	MsgQueueConnect	(char* _msgQueueName);


void	MsgQueueDestroy	(MsgQ* _msqQue);

/* Message Manupulation */
/* return number of bytes actually written, 0 if message queue full		*/
int		MsgQueueSnd	(MsgQ*_msgQue, void* _buffer, size_t _length);


/* return number of bytes actually read, 0 if message queue unavailable */ 
int		MsgQueueRcv	(MsgQ*_msgQue, void* _buffer, size_t _size);

int		IsMsgQueueEmpty (MsgQ*_msgQue);

/* Utility Functions for debugging and unit testing */
void	MsgQueuePrint	(MsgQ* _msgQue);

/*-----------------------------------------------------------------*/

#endif /* __MSG_QUEUE_H_ */

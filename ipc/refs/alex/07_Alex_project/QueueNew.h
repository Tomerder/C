/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-16    
    Last modified date:		2013-09-17
    Description:	header file for queue handling functions.
    				update 16/9: modified for use as part of message queue.
********************************************************************************/

#ifndef __QUEUE_NEW_H__
#define __QUEUE_NEW_H__

typedef struct Queue Queue_t;


/* create queue of size _nPlaces, starting at given memory address */
/* return NULL for failure */
Queue_t*	QueueCreate		(void* _memory, size_t _nPlaces);

/* destroy queue */
void	QueueDestroy	(Queue_t* _q);

/* return required memory size for given _nPlaces */
size_t	QueueGetSize	(size_t _nPlaces);

/* insert to queue tail, return success (0) / fail (-1) */
int		QueueInsert		(Queue_t* _q, int _item);

/* remove from queue head, return success (0 / fail (-1) */
int		QueueRemove		(Queue_t* _q, int* _item);

/* return true (1) / false (0) */
int		IsQueueEmpty	(Queue_t* _q);
int		IsQueueFull		(Queue_t* _q);

/* for testing purposes */
void	QueuePrint		(Queue_t* _q);

#endif /* __QUEUE_NEW_H__ */

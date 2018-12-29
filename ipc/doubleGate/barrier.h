/*****************************************************************************************************************
*    AUTHOR: Tomer Dery                                                                                 
*    DATE: 16.02.14                                                                                              
*    LAST MODIFIED: 16.02.14                                                                                  
*    DESCRIPTION: Barrier
*****************************************************************************************************************/
#ifndef __BARRIER_H__
#define __BARRIER_H__

enum{ERROR = -1 , OK_E, ALOC_ERR , BROADCAST_ERR , THREAD_JOIN_ERR , WAIT_ERR , LOCK_ERR , UNLOCK_ERR};
#define BARRIER_OPENED 1

typedef void* (*CallBack)(void*);

typedef struct Barrier Barrier;

Barrier* BarrierInit(int _capacity);

void BarrierDestroy(Barrier* _bar);

int BarrierWait(Barrier* _bar, CallBack _CallBack, void* _param);


#endif /* __BARRIER_H__ */

#ifndef __CONS_H__
#define __CONS_H__

#include "prodCons.h"

typedef struct Consume_t Consume_t;

Consume_t* ConsumeInit(Queue* _queue, pthread_mutex_t* _mutex, pthread_cond_t* _cond, size_t _maxQueueSize);
						
void ConsumeDestroy(Consume_t* _consume);

void* Consume(Consume_t* _consume);

pthread_mutex_t* ConsGetMutex(Consume_t* _consume);

int ConsExit(Consume_t* _consume);

int ConsGetNum(Consume_t* _consume);

#endif /* #ifndef __CONS_H__ */

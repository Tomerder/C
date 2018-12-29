/**
 * ProdCons.h	Producer/Consumer ADT
 * 
 * Author:			Alex Katz
 * Creation Date:	16.02.14
 * Last Modified:	
 * Description:	Producer and Consumer unified ADT for use in pthreads
 * 		hide pthreads synchronization tools
 */
#ifndef _PRODCONS_H_
#define _PRODCONS_H_

#include "queue.h"

enum{OK_E, ALOC_ERR , THREAD_JOIN_ERR};

typedef struct ProdCons	ProdCons;

/**
 * Create Producer-Consumer box of the specified size
 * and allocated all necessary synchronization tools
 */
ProdCons* PCCreate (int _boxSize);
void PCDestroy(ProdCons *);

/**
 * Put new item into the box 
 * return value -- same item if success, NULL otherwise
 */
void* PCProduceItem(ProdCons*, void* _item);

/**
 * Get an item from the box 
 * return value -- item if success, NULL otherwise
 */
void* PCConsumeItem(ProdCons* _prodCons);

/**
 * Initiate shutdown mode -- wake all waiting producers and/or consumers
 */
void PCSetShutdownMode(ProdCons*);

#endif /* _PRODCONS_H_ */

/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-13    
    Last modified date:		2013-09-13
    Description: 	conditional variables
****************************************************************************/

#ifndef __BARRIER_PH2_H__
#define __BARRIER_PH2_H__

typedef struct Barrier_t Barrier;

typedef void (*Func) (void* _params);

Barrier* 	BarrierInit		(size_t _capacity);

void 		BarrierDestroy	(Barrier* _barrier);

int 		BarrierWait		(Barrier* _barrier, Func _callback, void* _params);


#endif /* __BARRIER_PH2_H__ */

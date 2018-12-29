/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-09    
    Last modified date:		2013-09-09
    Description: 	conditional variables
****************************************************************************/

#ifndef __BARRIER_H__
#define __BARRIER_H__

typedef struct Barrier_t Barrier;



Barrier* 	BarrierInit		(size_t _capacity);

void 		BarrierDestroy	(Barrier* _barrier);

int 		BarrierWait		(Barrier* _barrier);


#endif /* __BARRIER_H__ */

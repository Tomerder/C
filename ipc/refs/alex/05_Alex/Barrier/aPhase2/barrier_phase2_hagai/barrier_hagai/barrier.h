/**************************************************************************************
    Author: Hagai Moshe
    Creation date :      
    Last modified date:  
    Description : .
***************************************************************************************/

#ifndef __PPCOMMON_H__
#define __PPCOMMON_H__

#include <semaphore.h>  /*sem_init, sem_wait   : link with -pthread*/



typedef struct Barrier_t Barrier;

				/*numOfThreads must be grater then 0*/
Barrier*		BarrierInit( size_t _numOfThreads );
				
void			BarrierDestroy(Barrier* _barrier);
				/*put a thread on hold untill the numOfThreads waiting has been reached, return 0 on success or an error num on failer */
int				BarrierWait(Barrier* _barrier);



#endif/*__PPCOMMON_H__*/ 









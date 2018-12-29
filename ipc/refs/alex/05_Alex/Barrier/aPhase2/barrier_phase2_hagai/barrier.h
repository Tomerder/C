/**************************************************************************************
    Author: Hagai Moshe
    Creation date :      
    Last modified date:  
    Description : .
***************************************************************************************/

#ifndef __PPCOMMON_H__
#define __PPCOMMON_H__





typedef struct Barrier_t Barrier;
				/**/
typedef int 	(*CallBack)( void* _params);

				/*numOfThreads must be grater then 0*/
Barrier*		BarrierInit( size_t _numOfThreads );
				
void			BarrierDestroy(Barrier* _barrier);
				/*put a thread on hold untill the numOfThreads waiting has been reached, return 0 on success or an error num on failer */
int				BarrierWait(Barrier* _barrier, CallBack _CallBack , void* _params);



#endif/*__PPCOMMON_H__*/ 









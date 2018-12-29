/**************************************************************************************
    Author: Hagai Moshe
    Creation date :      
    Last modified date:  
    Description : .
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         /*getopt*/
#include <string.h>
#include <pthread.h>        /*-pthread*/
#include "barrier.h"


#define	NUM_OF_THREADS 5
#define	NUM_OF_ITERATIONS 4




typedef struct Params_t
{
	Barrier* 	m_barrier;
	int*		m_arrCell; /*each thread being given an array cell to do its work on */
	int			m_iterNum;

}Params;



    /*global declerations*/
int 		g_check = 0;



void*		ThreadCommon(void* _params)
{
	int i;
	Params* params = (Params*)_params;	

	for(i=0; i<NUM_OF_ITERATIONS; ++i){
		/*pre-wait work/mark*/
		++ *params->m_arrCell;
	
		BarrierWait( params->m_barrier, NULL, NULL);

		/*pos-wait work/mark*/
		++ *params->m_arrCell;
		/*allowing the chek of the post-wait work*/
		sleep(1);
	}
	
	pthread_exit(0);
	return NULL;
}/*ThreadCommon*/




void*		ThreadLast(void* _params)
{
	Params* params = (Params*)_params;	
	int i;

		/*checking if the threads had done their pre-wait work*/
	for(i=0; i<NUM_OF_THREADS; ++i ){
		g_check = ((params->m_iterNum*2)+1 == params->m_arrCell[i]) ? g_check:__LINE__ ;
	}
	
		/*wait for releasing the threads*/
	BarrierWait( params->m_barrier, NULL, NULL);

	pthread_exit(0);
	return NULL;	
}/*ThreadLast*/




int     main( )
{
	
	int 		i,j;
	int			testArr[NUM_OF_THREADS]={0};
	Barrier		*barrier;
	Params 		params[NUM_OF_THREADS+1];
	pthread_t 	trd[NUM_OF_THREADS+1];
	
	barrier = BarrierInit( NUM_OF_THREADS+1 );
    
    	/*staring regular threads*/
	for( i=0; i< NUM_OF_THREADS ; ++i ){
		params[i].m_barrier = barrier;
		params[i].m_arrCell = testArr+i;
		pthread_create (trd+i, NULL, &ThreadCommon, params+i);
	}
	
	for(i=0; i<NUM_OF_ITERATIONS; ++i ){ 
			/*allowing the regular threads to enter wait*/
		sleep(1);
		
			/*starting the "last thread" (the one who opens the barrier )*/
		params[NUM_OF_THREADS].m_barrier = barrier;
		params[NUM_OF_THREADS].m_arrCell = testArr;
		params[NUM_OF_THREADS].m_iterNum = i;
		pthread_create (trd+NUM_OF_THREADS, NULL, &ThreadLast, params+NUM_OF_THREADS);
			/*destoying  the "last thread" */
		pthread_join(trd[NUM_OF_THREADS],NULL);

			/*checking if the threads had done their pos-wait work */
		for(j=0; j<NUM_OF_THREADS; ++j ){
			g_check = ( ((2*i)+2) == testArr[j] ) ? g_check:__LINE__ ;
		}

			/*allowing the regular threads to re-enter wait */
		sleep(2);
			
    }/*for (NUM_OF_ITERATIONS)*/


	for(i=0; i< NUM_OF_THREADS-1; i++){
		pthread_join(trd[i],NULL);
	}  		
	BarrierDestroy(barrier);

	if(g_check){
		printf("error	%d\n", g_check);
	}
    return 0;
}/*main*/

/*
printf("print: %d\n",__LINE__);
printf("print: %d\n",*params->m_arrCell);
*/









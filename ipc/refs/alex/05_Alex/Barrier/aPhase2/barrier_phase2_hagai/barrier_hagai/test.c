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
#define	NUM_OF_ITERATIONS 1

#define VERBOSE(X) 	if(options.m_verbose){	    \
						printf("%s\n", (X));    \
					}


typedef struct Params_t
{
	Barrier* 	m_barrier;
	int*		m_arrCell;

}Params;



    /*global declerations*/
int			g_testArr[NUM_OF_THREADS]={0};
int 		g_check = 0;



void*		ThreadCommon(void* _params)
{
	int i;
	Params* params = (Params*)_params;	

	for(i=0; i<NUM_OF_ITERATIONS; ++i){
		/*pre-wait work/mark*/
		++ *params->m_arrCell;
		
		BarrierWait( params->m_barrier);
printf("print: %d\n",__LINE__);

		/*pos-wait work/mark*/
		++ *params->m_arrCell;
	}
	
	pthread_exit(0);
	return NULL;
}/*ThreadCommon*/

/*
printf("print: %d\n",*params->m_arrCell);
*/
void*		ThreadLast(void* _params)
{
	Params* params = (Params*)_params;	
	int i;
printf("print: %d\n",__LINE__);	

		/*pre-wait work/mark*/
	++ *params->m_arrCell;

		/*checking if the threads had done their pre-wait work*/
	for(i=0; i<NUM_OF_THREADS; ++i ){
		g_check = (1 == g_testArr[i]) ? g_check:__LINE__ ;
	}
	
	BarrierWait( params->m_barrier);
		/*pos-wait work/mark*/
	++ *params->m_arrCell;

	pthread_exit(0);
	return NULL;	
}/*ThreadLast*/




int     main( )
{
	
	int 		i;
	Barrier* 	barrier;
	Params 		params[NUM_OF_THREADS];
	pthread_t 	trd[NUM_OF_THREADS];
	
	barrier = BarrierInit( NUM_OF_THREADS );
    
    	/*staring regular threads*/
	for( i=0; i< (NUM_OF_THREADS-1) ; ++i ){
		params[i].m_barrier = barrier;
		params[i].m_arrCell = g_testArr+i;
		pthread_create (trd+i, NULL, &ThreadCommon, params+i);
	}
	
	for(i=0; i<NUM_OF_ITERATIONS; ++i ){ 
			/*allowing the regular threads to enter wait*/
		sleep(1);
		
			/*starting the "last thread" (the one who opens the barrier )*/
		params[NUM_OF_THREADS-1].m_barrier = barrier;
		params[NUM_OF_THREADS-1].m_arrCell = g_testArr+(NUM_OF_THREADS-1);
		pthread_create (trd+(NUM_OF_THREADS-1), NULL, &ThreadLast, params+(NUM_OF_THREADS-1));

			/*allowing the regular threads to re-enter wait */
		sleep(1);
			
			/*destoying  the "last thread" */
		pthread_join(trd[NUM_OF_THREADS-1],NULL);

			/*checking if the threads had done their pos-wait work */
		for(i=0; i<NUM_OF_THREADS; ++i ){
			g_check = (2 == g_testArr[i]) ? g_check:__LINE__ ;
		}
    }/*for (NUM_OF_ITERATIONS)*/


	for(i=0; i< NUM_OF_THREADS-1; i++){
		pthread_join(trd[i],NULL);
	}  		

	if(g_check){
		printf("error	%d\n", g_check);
	}
    return 0;
}/*main*/

/*
printf("print: %d\n",__LINE__);
*/









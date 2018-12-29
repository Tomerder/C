/**************************************************************************************
    Author: Dor Kopp
    Creation date :      	9/9/13
    Last modified date:  	10/9/13
    Description : barrier system test.
***************************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "barrier.h"

/* testing struct */
typedef struct params
{
	Barrier_t*	m_barrier;
	Barrier_t*	m_bar;
	int			m_iterations;
	int			m_index;
	int*		m_arr;
} Params;

/*************************************************************************
							COUNTER
*************************************************************************/
void*	Counter(void* _var)
{
	int i = 0, j = 0, index = ((Params*)_var)->m_index;
	
	while ( i < ((Params*)_var)->m_iterations )
	{
		j = 0;
		printf("Counter #%d start.\n",index+1);
		while ( j <= index )
		{
			++((Params*)_var)->m_arr[index];
			usleep( index * 50 );
			++j;
		}
		printf("Counter #%d stop. count:%d. \n",index+1,((Params*)_var)->m_arr[index]);
		BarrierWait( ((Params*)_var)->m_barrier );
		printf("Counter #%d waiting for the sum.\n",index+1);
		BarrierWait( ((Params*)_var)->m_bar );
		++i;
		printf("counter #%d end of iteration #%d.\n",index+1, i );
	}
	pthread_exit(0);
}
/*************************************************************************
								SUMMER
*************************************************************************/
void*	Summer(void* _var)
{
	int 		i = 0, sum = 0;
	static int	it = 1;
	
	BarrierWait( ((Params*)_var)->m_barrier );
	printf("summer release barrier & start summing.\n");
	while ( i < ((Params*)_var)->m_index )
	{
		sum+= ( (Params*)_var)->m_arr[i];
		++i;
	}
	printf("sum #%d: %d.\n",it,sum);
	++it;
	BarrierWait( ((Params*)_var)->m_bar );
	printf("summer release barrier & stop summing.\n");
	pthread_exit(0);
}
/*************************************************************************
								MAIN
*************************************************************************/
int main(int argc, char **argv)
{
	Params		params;
	int			threads = 0, i = 0;
	pthread_t	*arr = 0;	
		
	/* first parameter is number of iterations */
	params.m_iterations = ( 2 > argc )? 5 : atoi(argv[1]);
	/* second parameter is number of threads in barrier */
	threads = ( 3 > argc )? 3 : atoi(argv[2]);
	
	params.m_arr = (int*)calloc( threads, sizeof(int) );
	if ( 0 == params.m_arr )
	{ return -1; }
	arr = (pthread_t*)malloc( threads * sizeof(pthread_t) );
	if ( 0 == arr )
	{ goto PTHREAD_MALLOC_FAIL; }
	
	/* sets the barriers */
	if ( NULL == (params.m_barrier = BarrierInit(threads) ) )
	{	goto BARRIER_INIT_FAIL;	}
	if ( NULL == (params.m_bar = BarrierInit(threads) ) )
	{	goto BARRIER_INIT_FAIL;	}		

	/* creates "treads - 1" threads */
	params.m_index = 0;
	while ( params.m_index < threads - 1 )
	{
		if ( 0 > pthread_create(arr + params.m_index,0,Counter,&params) )
		{	goto THREAD_CREATE_FAIL;	}
		usleep(500);
		++params.m_index;
	}
	/* creates "iterations" times last summer thread */
	i = 0;
	while ( i < params.m_iterations )
	{
		if ( 0 > pthread_create(arr + params.m_index,0,Summer,&params) )
		{	goto THREAD_CREATE_FAIL;	}
		++i;
		if ( 0 > pthread_join(arr[params.m_index],0) )
		{	goto THREAD_JOIN_FAIL; }
	}
	i = 0;
	while ( i < threads-1 )
	{
		if ( 0 > pthread_join(arr[i],0) )
		{	goto THREAD_JOIN_FAIL; }
		++i;
	}
		
	/*BarrierDestroy(params.m_barrier);*/
	return 0;
	
/* GOTO's */
INT_MALLOC_FAIL:
	free(params.m_arr);
	return -1;
THREAD_CREATE_FAIL:
	perror("thread_create");
	goto INT_MALLOC_FAIL;
THREAD_JOIN_FAIL:
	perror("thread_join");
	goto INT_MALLOC_FAIL;
BARRIER_INIT_FAIL:
	perror("barrier_init");
	goto INT_MALLOC_FAIL;
PTHREAD_MALLOC_FAIL:
	perror("pthread_malloc");
	goto INT_MALLOC_FAIL;

}

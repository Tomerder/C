/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-09    
    Last modified date:		2013-09-09
    Description: 	conditional variables
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


typedef struct Args
{
	int m_counter;
	int m_nThreads;
	int m_nRounds;
} Args;



/*######################################################################*/

pthread_mutex_t myMutex;
pthread_cond_t condRead, condWrite;
int g_lock = 0;
char buffer[] = "Aaaaaaaaaaaaaaaaa";

/*######################################################################*/

void WriterFunc();
void ReaderFunc();
void Writer(void* _args);
void Reader(void* _args);

/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char * argv[])
{
	pthread_t writer, *readers;
	Args args;
	int N, I;
	int j, k;
	
	/* input arguments */
	I = atoi(argv[1]);
	N = atoi(argv[2]);
	
	args.m_counter = 1;
	args.m_nThreads = N;
	args.m_nRounds = 1;
	
	readers = (pthread_t*)malloc(N*sizeof(pthread_t));
	
	/* initialize */
	pthread_mutex_init(&myMutex, 0);
	
	pthread_cond_init(&condRead, 0);
	pthread_cond_init(&condWrite, 0);
	
	
	pthread_create(&readers[0], 0, (void*)Reader, (void*)&args);	/* 1st reader */
	pthread_create(&writer, 0, (void*)Writer, 0);					/* writer */
	
	for(k=1; k<N; ++k)												/* rest of readers */
	{
		pthread_create(&readers[k], 0, (void*)Reader, (void*)&args);
	}
		
		
/*	for(j=0; j<I; ++j)*/
/*	{*/
/*		printf("Round #%d\n", j+1);*/
/*		*/
/*		args.m_counter = 1;*/
/*	}*/
	
	
	pthread_join(writer, 0);
	for(k=0; k<N; ++k)	
	{
		pthread_join(readers[k], 0);
	}
	
	/* close/free/destroy */
	free(readers);
	pthread_cond_destroy(&condRead);
	pthread_cond_destroy(&condWrite);
	pthread_mutex_destroy(&myMutex);
	
	return 0;
}
/*######################################################################*/


void WriterFunc()
{
	++buffer[0];
}
/*######################################################################*/


void ReaderFunc(int _i)
{
	printf("%d - %s\n", _i, buffer);
}
/*######################################################################*/


void Writer(void* _args)
{
	pthread_mutex_lock(&myMutex);				/* lock mutex */
	while(g_lock==0) {							/* or wait until lock is changed */
		pthread_cond_wait(&condWrite, &myMutex);		
	}
	
	printf("Writer here\n");
	WriterFunc();
	
	g_lock = 0;								/* change lock */
	pthread_cond_signal(&condRead);			/* wake up Reader */
	pthread_mutex_unlock(&myMutex);			/* unlock mutex */
	
	
	
	pthread_exit(0);
}
/*######################################################################*/


void Reader(void* _args)
{
	int *counter = &((Args*)_args)->m_counter;
	int N = ((Args*)_args)->m_nThreads;
	int *nRounds = &((Args*)_args)->m_nRounds;
	
	pthread_mutex_lock(&myMutex);				/* lock mutex */
	while(g_lock==1) {							/* or wait until lock is changed */
		pthread_cond_wait(&condRead, &myMutex);		
	}
	
	printf("Reader #%d here\n", *counter);
	
	if( *counter == 1 )
	{
		g_lock = 1;									/* change lock */
		pthread_cond_signal(&condWrite);			/* wake up Writer */
		pthread_mutex_unlock(&myMutex);				/* unlock mutex */
		
		pthread_mutex_lock(&myMutex);				/* wait for writer */
		while(g_lock==1) {							
			pthread_cond_wait(&condRead, &myMutex);
		}
	}
	
	ReaderFunc(*counter);
	++ *counter;
	
	pthread_cond_signal(&condRead);
	
	
	if( *counter == N )
	{
		*counter = 1;
		++ *nRounds;
		
		g_lock = 1;									/* change lock */
		pthread_cond_signal(&condWrite);			/* wake up Writer */
		pthread_mutex_unlock(&myMutex);				/* unlock mutex */
	}
	
	if(*nRounds == I) {
		pthread_exit(0);
	}
		
	return;
}
/*######################################################################*/



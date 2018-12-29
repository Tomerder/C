/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-09    
    Last modified date:		2013-09-09
    Description: 	conditional variables
****************************************************************************/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define SIZE	100
enum Bool {FALSE, TRUE};


/*######################################################################*/

pthread_mutex_t myMutex;
pthread_cond_t condRead, condWrite;
int readerOnLine = FALSE;
int writerWrote = FALSE;
char buffer[] = "Aaaaaaaaaaaaaaaaa";

/*######################################################################*/

void WriterFunc();
void ReaderFunc();
void Writer(void* _num);
void Reader(void* _num);

void Wait(int *_flag, pthread_cond_t *_cond, pthread_mutex_t *_mutex);
void Wake(int *_flag, pthread_cond_t *_cond, pthread_mutex_t *_mutex);


/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char * argv[])
{
	pthread_t writer, *readers;
	int N, I;
	int j, k;
	int nums[SIZE];
	
	/* input arguments */
	I = atoi(argv[1]);
	N = atoi(argv[2]);
	
	readers = (pthread_t*)malloc(N*sizeof(pthread_t));
	
	/* initialize */
	pthread_mutex_init(&myMutex, 0);
	
	pthread_cond_init(&condRead, 0);
	pthread_cond_init(&condWrite, 0);
	
		
	for(j=0; j<I; ++j)
	{
		readerOnLine = FALSE;
		writerWrote = FALSE;
		
		printf("Round #%d\n", j+1);
		
		/* create threads */
		pthread_create(&writer, 0, (void*)Writer, 0);					/* writer */
		sleep(1);
		for(k=0; k<N; ++k)												/* readers */
		{
			nums[k] = k+1;
			pthread_create(&readers[k], 0, (void*)Reader, (void*)&nums[k]);
		}
		
		
		/* join */
		pthread_join(writer, 0);
		for(k=0; k<N; ++k)	
		{
			pthread_join(readers[k], 0);
		}
		
	}
	

	/* close/free/destroy */
	free(readers);
	pthread_cond_destroy(&condRead);
	pthread_cond_destroy(&condWrite);
	pthread_mutex_destroy(&myMutex);
	
	return 0;
}
/*######################################################################*/


void Wait(int *_flag, pthread_cond_t *_cond, pthread_mutex_t *_mutex)
{
	pthread_mutex_lock(&myMutex);
	while(!_flag)
	{
		pthread_cond_wait(_cond, _mutex);
	}
	return;
}
/*######################################################################*/

void Wake(int *_flag, pthread_cond_t *_cond, pthread_mutex_t *_mutex)
{
	*_flag = !*_flag;
	pthread_cond_broadcast(_cond);
	pthread_mutex_unlock(_mutex);
	return;
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


void Writer(void* _num)
{
	Wait(&readerOnLine, &condWrite, &myMutex);
	
	printf("Writer here\n");
	WriterFunc();
	
	Wake(&writerWrote, &condRead, &myMutex);
	pthread_exit(0);
}
/*######################################################################*/

void Reader(void* _num)
{
	if(!readerOnLine) {
		readerOnLine = TRUE;
		Wake(&readerOnLine, &condWrite, &myMutex);
	}
	Wait(&writerWrote, &condRead, &myMutex);
	
	printf("Reader here\n");
	ReaderFunc(*(int*)_num);
	pthread_exit(0);
}
/*######################################################################*/




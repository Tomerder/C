/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-25    
    Last modified date:		2013-08-25
    Description: 	interprocess communication excercise - semaphore
****************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "SemTest.h"


#define NSEMS		1
#define SEMFLAGS	IPC_CREAT


/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char *argv[])
{
	Options options;		/* runtime options	*/
	semun semUn;			/* *** */
	int sid;				/* semaphore id		*/
	int i;
	struct sembuf sops;
	
	/* initialize & handle runtime options */
	OptHandler(argc, argv, &options);
	
	/* create semaphore */
	if( -1 == (sid = SemConnect(NSEMS, SEMFLAGS)) )
	{
		fprintf(stderr, "%s:%d Error: SemConnect\n", __FILE__, __LINE__-2);
		return errno;
	}
	
	/* initialize */
	semUn.val = 1;
	if( -1 == semctl(sid, 0, SETVAL, semUn) )
	{
		perror("semctl");
		return errno;
	}
	
	sops.sem_num = 0;
	sops.sem_op  = 0;
	sops.sem_flg = 0;
	
	/* printing N times */
	for(i=0; i < options.m_nLines; ++i)
	{
		/* semaphore - down */
		sops.sem_op = -1;
		if( -1 == semop(sid, &sops, 1) )
		{
			perror("semop");
			return errno;
		}
		
		/* action */
		printf("Line #%d\n", i+1);
		
		/* semaphore - up */
		sops.sem_op = 1;
		if( -1 == semop(sid, &sops, 1) )
		{
			perror("semop");
			return errno;
		}
		
		/* sleep */
		if(-1 == usleep(options.m_usleep) )
		{
			perror("usleep");
			return errno;
		}
	}
	
	/* delete */
	if(-1 == semctl(sid, 0, IPC_RMID) )
	{
		perror("semctl");
	}
	
	return errno;
}
/*######################################################################*/


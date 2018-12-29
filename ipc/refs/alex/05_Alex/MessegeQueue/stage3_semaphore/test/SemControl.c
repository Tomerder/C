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


#define NSEMS		0
#define SEMFLAGS	0



/*######################################################################*/
/*									MAIN								*/
/*######################################################################*/
int main(int argc, char *argv[])
{
	Options options;		/* runtime options	*/
	int sid;				/* semaphore id		*/
	struct sembuf sops;
	
	
	/* initialize & handle runtime options */
	OptHandler(argc, argv, &options);
	
	/* connect to semaphore */	
	if( -1 == (sid = SemConnect(NSEMS, SEMFLAGS)) )
	{
		fprintf(stderr, "%s:%d Error: SemConnect\n", __FILE__, __LINE__-2);
		return errno;
	}
	
printf("%d\n", sid);
	
	sops.sem_num = 0;
	sops.sem_op  = 0;
	sops.sem_flg = 0;
	
	/* pause - down */
	if(options.m_pause)
	{
		sops.sem_op = -1;
		if( -1 == semop(sid, &sops, 1) )
		{
			perror("semop");
			return errno;
		}
	}
		
	/* resume - up */
	if(options.m_resume)
	{
		sops.sem_op = 1;
		if( -1 == semop(sid, &sops, 1) )
		{
			perror("semop");
			return errno;
		}
	}
	
	/* delete */
	if(options.m_destroy)
	{
		if(-1 == semctl(sid, 0, IPC_RMID) )
		{
			perror("semctl");
		}
	}
	
	return errno;
}
/*######################################################################*/



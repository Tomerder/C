/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-25    
    Last modified date:		2013-08-25
    Description: 	interprocess communication excercise - semaphore
****************************************************************************/

#ifndef __SEM_TEST_H__
#define __SEM_TEST_H__

/* runtime options */
typedef struct Options
{
	int		m_nLines;		/* # of lines to print 		*/
	int		m_usleep;		/* sleep time in microsec 	*/
	int		m_pause;		/* pause printer			*/
	int		m_resume;		/* resume printer			*/
	int 	m_destroy;		/* destroy semaphore 		*/
} Options;


/* requred for semctl */
typedef union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
} semun;


enum Bool {FALSE, TRUE};


void 	OptHandler	(int _argc, char *_argv[], Options *_options);
int 	SemConnect	(int _num, int _flags);


#endif /* __SEM_TEST_H__*/



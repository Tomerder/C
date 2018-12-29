/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-25    
    Last modified date:		2013-08-28
    Description: 	interprocess communication excercise - named pipe - functions
    				multiple processes from 2 files - ping (sending) and pong (receiving)
    				controlled by semaphores.
****************************************************************************/

#ifndef __PINGPONG_H__
#define __PINGPONG_H__


#define FILE_NAME_LEN	16
#define MAX_MSG_LEN		128

#define CH_PING			1
#define CH_PONG			(CH_PING+1)


/* messege structure */
typedef struct Messege
{
	long	m_type;
	char	m_msg[MAX_MSG_LEN];
} Messege;


/* runtime options */
typedef struct Options
{
	int		m_verbose;					/* print more info 					*/
	int 	m_create;					/* create MsgQue & semaphore set	*/
	int 	m_destroy;					/* destroy MsgQue & semaphore set	*/
	int		m_usleep;					/* sleep time in microsec 			*/
	int		m_nMesseges;				/* # of messeges to send 			*/
	char 	m_filename[FILE_NAME_LEN];	/* filename							*/
} Options;


/* integer arguments for messege send and receive */
typedef struct SendRecvArg
{
	int		m_qid;
	int		m_length;
	int		m_flags;
	int		m_counter;
	int 	m_from;
	int		m_pid;
} SendRecvArg;


/* requred for semctl */
typedef union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
} semun;


enum Bool {FALSE, TRUE};
enum Semaphores{PING, PONG};


/* options handler */
void 	OptHandler		(int _argc, char *_argv[], Options *_options);

/*** SEMAPHORES ***/

/* create/connect to semaphore set, return id, -1 for errors */
int 	SemConnect	(int _num, int _flags);

/*** MESSAGES ***/

/* create/open MsgQue, returns identifier, -1 for error */
int 	MsgQueCreate		(Options *_options);
/* create and send a messege, return messege length */
int 	MsgSend			(SendRecvArg *_args, Messege *_messege);
/* fetch a messege from queue, return messege length */
int 	MsgRecv			(SendRecvArg *_args, Messege *_messege);


/* print in verbose mode */
void 	PrintV			(Options *_options, char* _text, int _num);
/* print semaphore values */
void 	SemValuePrint	(int _semid, Options *_options);

#endif /* __PINGPONG_FUNC_H__ */

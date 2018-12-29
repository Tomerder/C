/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-31    
    Last modified date:		2013-09-05
    Description: 	ping pong with shared memory
    				stage 1 - single ping & single pong
    				
    				runtime options:
    					-s: sleep in msec
    					-f: filename
    					-n: # of messeges to send
    					-c: create (semaphore, shared mem)
    					-d: destroy
    					-v:	verbose mode
    					
****************************************************************************/

#ifndef __PINGPONG_H__
#define __PINGPONG_H__

/*######################################################################*/
/*								DEFINITIONS								*/
/*######################################################################*/

#define FILE_NAME_LEN	16
#define MAX_MSG_LEN		128

#define COUNTER(X)		*(int*)(X)
#define LENGTH(X)		*(int*)( (char*)X + sizeof(int) )
#define MSG_START(X)	( (char*)X + 2*sizeof(int) )

#define	UP		1
#define DOWN	-1

/* structure of shared mem data */
typedef struct Message
{
	int		m_counter;
	int		m_length;
	char	m_msg[MAX_MSG_LEN];
} Message;


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
	int			m_counter;	/* # of messages sent/received					*/
	int 		m_from;		/* sender										*/
	int			m_pid;		/* process id									*/
	Message*	m_msg;		/* pointer to start of shared memory segment	*/
} SendRecvArg;


/* required for semctl */
typedef union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */            
} semun;


enum Bool {FALSE, TRUE};
enum Semaphores {PING, PONG, PING_LOCK};


/*######################################################################*/
/*								FUNCTIONS								*/
/*######################################################################*/

/* options handler */
void 	OptHandler		(int _argc, char *_argv[], Options *_options);

/*** SEMAPHORES ***/

/* create/connect to semaphore set, return id, -1 for errors */
int 	SemConnect		(int _num, int _flags);
/* print semaphore values */
void 	SemValuePrint	(int _semid, Options *_options);


/*** SHARED MEMORY ***/

/* create/connect to shared memory, return id, -1 for errors */
int		ShmConnect	(int _size, int _flags);
/* write a messege to shared memory, return length */
int 	MsgSend		(SendRecvArg *_args);
/* read message from shared memory, return message length */
int 	MsgRecv		(SendRecvArg *_args, char* _buffer);


/* print in verbose mode */
void 	PrintV			(Options *_options, char* _text, int _num);


#endif /* __PINGPONG_H__ */


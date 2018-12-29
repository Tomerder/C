/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-22    
    Last modified date:		2013-08-22
    Description: 	interprocess communication excercise - named pipe - functions
    				multiple processes from 2 files - ping (sending) and pong (receiving)
    				
    				Each ping enters an initial message into a dedicated channel ("registrar")
    				and deletes a message there on exit.
    				When a pong checks messages, if the messege channel is empty it checks
    				the registrar channel. If not empty, sends back in the same message.
    				If empty, exits.
    				(Note: possibility of race conditions).
    				
****************************************************************************/

#ifndef __PINGPONG_H__
#define __PINGPONG_H__


#define FILE_NAME_LEN	16
#define MAX_MSG_LEN		128

#define CH_PING			1
#define CH_CHCK_IN		(CH_PING+1)
#define CH_PONG			(CH_PING+2)


/* messege structure */
typedef struct Messege
{
	long	m_type;
	char	m_msg[MAX_MSG_LEN];
} Messege;


/* runtime options */
typedef struct Options
{
	int		m_verbose;					/* print more info 			*/
	int		m_usleep;					/* sleep time in microsec 	*/
	char 	m_filename[FILE_NAME_LEN];	/* filename					*/
	int		m_nMesseges;				/* # of messeges to send 	*/
	int 	m_create;					/* create MsgQue 			*/
	int 	m_destroy;					/* destroy MsgQue 	 		*/
	int		m_typeEOF;					/* pass EOF as msg type (channel) */
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

enum Bool {FALSE, TRUE};


/* options handler */
void 	OptHandler		(int _argc, char *_argv[], Options *_options);

/* create/open MsgQue, returns identifier, -1 for error */
int 	MsgQueCreate		(Options *_options);

/* create and send a messege, return messege length */
int 	MsgSend			(SendRecvArg *_args, Messege *_messege, int _isEOF);
/* fetch a messege from queue, return messege length */
int 	MsgRecv			(SendRecvArg *_args, Messege *_messege);

/* check if messege is from registrar channel */
int 	CheckPings		(Messege *_msg);

/* print in verbose mode */
void 	PrintV			(Options *_options, char* _text, int _num);



#endif /* __PINGPONG_FUNC_H__ */

/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-21    
    Last modified date:		2013-08-21
    Description: 	interprocess communication excercise - named pipe - functions
    				2 files - ping (sending) and pong (receiving)
    				
    				pong active until receives EOF messege from ping.
    				default: empty messege (just type) on regular channel
    					if -e flag used: messege sent on EOF channel.
    				
****************************************************************************/

#ifndef __PINGPONG_H__
#define __PINGPONG_H__


#define FILE_NAME_LEN	16
#define MAX_MSG_LEN		128

#define CH_PING			1
#define CH_EOF			(CH_PING+1)
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
} SendRecvArg;

enum Bool {FALSE, TRUE};

/*
enum Channels
{
	PING,
	PONG,
	END
};
*/

/* options handler */
void 	OptHandler		(int _argc, char *_argv[], Options *_options);

/* create/open MsgQue, returns identifier, -1 for error */
int 	MsgQueCreate		(Options *_options);
/* destroy MsgQue, returns errno  */
/* int 	MsgQueDestroy	(Options *_options, int _qid); */

/* create and send a messege, return messege length */
int 	MsgSend			(SendRecvArg *_args, Messege *_messege, int _isEOF);
/* fetch a messege from queue, return messege length */
int 	MsgRecv			(SendRecvArg *_args, Messege *_messege);

/* check if a messege signals EOF */
int 	CheckEOF		(Options *_options, Messege *_msg);

/* print in verbose mode */
void 	PrintV			(Options *_options, char* _text, int _num);



#endif /* __PINGPONG_FUNC_H__ */

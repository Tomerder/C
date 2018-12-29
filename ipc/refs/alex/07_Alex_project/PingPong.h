/*******************************************************************************
    Author:					Stav Ofer
    Creation date:  		2013-09-20    
    Last modified date:		2013-09-20
    Description: 	Test for Message Queue over Shared Memory - functions
    
    				runtime options:
    					common:
    						-s		sleep time [sec]
    						-f		filename
    						-h		filemane for handshake queue
    						-v		verbose mode
    					
    					Ping:
    						-m		# of messages per pong
    						-q		capacity of queue
    									
********************************************************************************/

#ifndef __PINGPONG_H__
#define __PINGPONG_H__

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

/*######################################################################*/
/*								DEFINITIONS								*/
/*######################################################################*/

#define FILE_NAME_LEN	16
#define MAX_MSG_SIZE	32

#define NMSG_DEFAULT		24
#define QSIZE_DEFAULT		10
#define SLEEP_DEFAULT		2

#define FILE_MAINQUEUE		"PingPong.c"
#define FILE_HANDSHAKEQ		"QueueNew.c"
#define HANDSHAKE_MSG		"Hello"
#define BYE_MSG				"Bye"


/* runtime options */
typedef struct Options
{
	char 	m_filename[FILE_NAME_LEN];		/* filename					*/
	char	m_fileHandshake[FILE_NAME_LEN];	/* filename for handshake	*/
	int		m_verbose;						/* print more info 			*/
	int		m_sleep;						/* sleep time sec 			*/
	int		m_nMessages;					/* # of messeges to send 	*/
	int		m_queueSize;					/* capacity of queue		*/
} Options_t;


enum boolean { FALSE, TRUE };


/* options handler */
void 	OptHandler		(int _argc, char *_argv[], Options_t *_options);



#endif /* __PINGPONG_H__ */

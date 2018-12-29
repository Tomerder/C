/***************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-21    
    Last modified date:		2013-08-21
    Description: 	interprocess communication excercise - named pipe - functions
    				2 files - ping (sending) and pong (receiving)
****************************************************************************/

#ifndef __PINGPONG_FUNC_H__
#define __PINGPONG_FUNC_H__


#define FILE_NAME_LEN		16

/* runtime options */
typedef struct Options
{
	int		m_usleep;					/* sleep time in microsec */
	char 	m_filename[FILE_NAME_LEN];	/* filename */
	int		m_nMesseges;				/* # of messeges to send */
	int 	m_create;					/* create fifo file */
	int 	m_destroy;					/* destroy fifo file */
	int		m_verbose;					/* print more info */
	int		m_bufferLen;				/* read buffer length */
} Options;



/* options handler */
void 	OptHandler		(int _argc, char *_argv[], Options *_options);

/* create FIFO file, returns errno  */
int 	FifoCreate		(Options *_options);
/* destroy (by unlinking) FIFO file, returns errno  */
int 	FifoDestroy		(Options *_options);

/* open pipe in required mode (read/write), return descriptor or -1 for error */
int		PipeOpen		(Options *_options, int _mode);
/* close pipe, input is file descriptor, returns errno */
int 	PipeClose		(Options *_options, int _desc);

/* print in verbose mode */
void 	PrintV			(Options *_options, char* _text, int _num);


#endif /* __PINGPONG_FUNC_H__ */

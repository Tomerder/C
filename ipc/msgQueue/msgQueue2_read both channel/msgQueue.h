/**************************************************************************************
    Author : Tomer Dery
    Creation date :      06.02.14    
    Date last modified : 06.02.14    
    Description : Message Queue
***************************************************************************************/
#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

/*----------------------------------------------------------------------------------*/

#define FILE_ID 'A'

#define MSGGET_FLAG 0644
#define BUF_SIZE 256

enum{ SND_TYPE=1 ,EOF_TYPE=2 , ACK_TYPE=3 };

#define EOF_SIGN "###EOF###"
#define ACK_SIGN "ACK"

enum{ERR_FTOK=1 , ERR_MSGGET, ERR_MSGCTL};

/*----------------------------------------------------------------------------------*/
#define MAX_FILE_NAME 32

#define DEFAULT_IS_VERBOSE TRUE
#define DEFAULT_FILE "msgQueue"
#define DEFAULT_WRK_TIME 1000000
#define DEFAULT_N_OF_MSG 10
#define DEFAULT_IS_CREATE FALSE
#define DEFAULT_IS_DELETE FALSE
#define DEFAULT_IS_MSG_EOF FALSE

#define MICROSEC_MUL_RES 1000
/*----------------------------------------------------------------------------------*/

#define PRINT_V(MSG) if(args.m_verbose) printf(MSG)
#define PRINT_S_V(MSG,STR) if(args.m_verbose) printf(MSG,STR)
#define PRINT_D_V(MSG,NUM) if(args.m_verbose) printf(MSG,NUM) 

/*----------------------------------------------------------------------------------*/

typedef struct msgbuf{
    long mtype;
    char mtext[BUF_SIZE];
}Buf;

/*----------------------------------------------------------------------------------*/

#endif /* #ifndef __MSG_QUEUE_H__ */

/**************************************************************************************
    Author : Tomer Dery
    Creation date :      09.02.14    
    Date last modified : 09.02.14    
    Description : Message Queue
***************************************************************************************/
#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

/*----------------------------------------------------------------------------------*/

enum{FALSE, TRUE};

#define FILE_ID 'A'

#define NO_FLAGS 0

#define SEMGET_PERMISIONS 0666
#define MSGGET_PERMISIONS 0644
#define BUF_SIZE 256

/*msg types : channels*/
enum{ SND_TYPE=1 ,ACK_TYPE=2};

#define EOF_SIGN "###EOF###"
#define ACK_SIGN "ACK"

/*Errors types*/
enum{OK , ERR_FTOK , ERR_MSGGET,  ERR_MSGCTL , ERR_MSGSND ,ERR_MSGRCV , ERR_SEMGET,  ERR_SEMCTL , ERR_SEMOP};

/*semaphores */
enum{PING_SEM, PONG_SEM , NUM_OF_SEMS };

enum{DOWN = -1, WAIT_FOR_0 = 0 ,UP = 1};

/*----------------------------------------------------------------------------------*/
#define MAX_FILE_NAME 32

#define DEFAULT_IS_VERBOSE TRUE
#define DEFAULT_FILE "keyFile"
#define DEFAULT_WRK_TIME 1000000
#define DEFAULT_N_OF_MSG 10
#define DEFAULT_IS_CREATE FALSE
#define DEFAULT_IS_DELETE FALSE
#define DEFAULT_IS_MSG_EOF FALSE

#define MICROSEC_MUL_RES 1000

#define MIN_SLEEP 10
/*----------------------------------------------------------------------------------*/

#define PRINT_V(MSG) /*if(args.m_verbose)*/ printf(MSG)
#define PRINT_S_V(MSG,STR) if(args.m_verbose) printf(MSG,STR)
#define PRINT_D_V(MSG,NUM) if(args.m_verbose) printf(MSG,NUM) 

#define ERROR -1
#define CHKERR(FUNC_RET) if(FUNC_RET != OK) { PRINT_V("ERROR\n"); return FUNC_RET; }

/*----------------------------------------------------------------------------------*/

typedef struct msgbuf{
    long mtype;
    char mtext[BUF_SIZE];
}Buf;

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

/*----------------------------------------------------------------------------------*/

int InitMsgQueue(int* _msqid, char* _fileName, int _msggetFlags);

int InitSem(int* _semid, char* _fileName, int _semgetFlags);

int RmvMsgQueueAndSem(int _msqid, int _semid);

int SemOp(int _semId , int _semNum , int _op , int _flags );

int DownUpSemPing(int _semId, int _semNum, int _flags );

int SendEOF(int _msqid);

int CountOfSem(int _semId , int _semNum , int* _semCount);

/*----------------------------------------------------------------------------------*/









#endif /* #ifndef __MSG_QUEUE_H__ */

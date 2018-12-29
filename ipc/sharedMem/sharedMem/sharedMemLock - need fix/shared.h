/**************************************************************************************
    Author : Tomer Dery
    Creation date :      09.02.14    
    Date last modified : 09.02.14    
    Description : shared memory + sem
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

#define MSG_SIZE 256

/*msg types : channels*/
enum{ SND_TYPE=1 ,ACK_TYPE=2};

#define EOF_SIGN "###EOF###"
#define ACK_SIGN "ACK"

/*Errors types*/
enum{OK , ERR_FTOK, ERR_SEMGET,  ERR_SEMCTL , ERR_SEMOP , ERR_SHMGET , ERR_SHMAT , ERR_SHMDT , ERR_SHMCTL};

/*semaphores */
enum{NOTIFY_PING_SEM, NOTIFY_PONG_SEM , LOCK ,NUM_OF_SEMS };

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

typedef struct Msg{
    int m_numOfMsgs;
    char m_text[MSG_SIZE];
}Msg;

#define MEM_GET_NO_OF_MSGS(MEM) (*(int*)MEM)
#define MEM_GET_TXT(MEM) (MEM + sizeof(int))
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

int InitSharedMem(int* _shMemId, char* _fileName, int _size , int _msggetFlags);

int InitSem(int* _semid, char* _fileName, int _semgetFlags, int _isCreate);

int RmvSharedMemAndSem(int _shMemId, int _semid);

int SemOp(int _semId , int _semNum , int _op , int _flags );

int DownUpSemPing(int _semId, int _semNum, int _flags );

int CountOfSem(int _semId , int _semNum , int* _semCount);

/*----------------------------------------------------------------------------------*/









#endif /* #ifndef __MSG_QUEUE_H__ */

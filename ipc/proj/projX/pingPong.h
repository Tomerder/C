/**************************************************************************************
    Author : Tomer Dery
    Creation date :      18.02.14    
    Date last modified : 18.02.14    
    Description : Ping Pong
***************************************************************************************/
#ifndef __PING_PONG_H__
#define __PING_PONG_H__

/*----------------------------------------------------------------------------------*/

enum{FALSE, TRUE};

#define FILE_ID 'A'

#define NO_FLAGS 0

#define SEMGET_PERMISIONS 0666
#define MSGGET_PERMISIONS 0644

#define EOF_SIGN "###EOF###"

/*Errors types*/
enum{OK , ERR_FTOK , ERR_MSGGET,  ERR_MSGCTL , ERR_MSGSND ,ERR_MSGRCV , ERR_SEMGET,  ERR_SEMCTL , ERR_SEMOP};

/*semaphores */
enum{SEM_REG_PONG, SEM_NOTIFY_NEW_PONG , NUM_OF_SEMS_PP };

enum{DOWN = -1, WAIT_FOR_0 = 0 ,UP = 1};

/*----------------------------------------------------------------------------------*/
#define MAX_FILE_NAME 32

#define FILE_NAME "keyFile"
#define DEFAULT_WORK_TIME 1000000
#define DEFAULT_N_OF_MSG_PER_PONG 20

#define MICROSEC_MUL_RES 1000

#define MIN_SLEEP 10
/*----------------------------------------------------------------------------------*/

#define ERROR -1
#define CHKERR(FUNC_RET) if(FUNC_RET != OK) { PRINT_V("ERROR\n"); return FUNC_RET; }

/*----------------------------------------------------------------------------------*/
/*
union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};
*/
/*----------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------*/

#endif /* #ifndef __PING_PONG_H__ */

#ifndef __UDP_COMMOM_H__
#define __UDP_COMMOM_H__

/*-----------------------------------------------------------*/
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5060

#define BUF_SIZE 64

/*-----------------------------------------------------------*/
#define DEBUG 1

#define PRT_DBG(STR)  if(DEBUG) printf("%s\n", STR); 

#define CHK_ERR(ERR, STR)  if(ERR == -1) {  fprintf(stderr, STR); return -1;}		
#define CHK_ERR_SOCK(ERR, SOCK, STR)  if(ERR == -1) {  fprintf(stderr, STR); close(SOCK); return -1;}		

#define CHK_IF_SOCK(IF, SOCK ,STR)  if(IF) { fprintf(stderr, STR); close(SOCK); return -1;}		

/*-----------------------------------------------------------*/

#define FAMILY AF_INET

#define SOCK_TYPE SOCK_STREAM

#define SINS_QUEUE_SIZE 5

/*-----------------------------------------------------------*/

#endif



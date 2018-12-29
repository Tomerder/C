/* tcpCommon.h
*
* Common definitions for the simple TCP client/server
*
* Author: Vladimir Krapp
* Created on: 20/3/14
* Last modified:
*
*/
#ifndef __TCP_CLIENT_SERVER_H__
#define __TCP_CLIENT_SERVER_H__

/************************************************************/
/*** Defaults and Definitions *******************************/
/************************************************************/
#define IP_ADDRESS "127.0.0.1"
#define PORT 5060
#define BUFF_SIZE 128

/************************************************************/
/*** Data structures ****************************************/
/************************************************************/

/************************************************************/
/*** Macros *************************************************/
/************************************************************/

#ifndef DEBUG
#define DEBUG
#endif


#ifdef DEBUG
#define PRINT_DBG(ARGS) printf ARGS
#else
#define PRINT_DBG(ARGS) 
#endif

#define CHECK(COND, CLEANUP) while((COND)){goto CLEANUP;}


#define RETURN_IF(EXPR, STR) if((EXPR))									\
						 	 {											\
						 		 fprintf(stderr, STR) ;					\
	   							 return -1;								\
							 }											\

#define RETURN_IF_1A(EXPR, STR, ARG1) if((EXPR))						\
						 	 {											\
						 		 fprintf(stderr, (STR), (ARG1));		\
	   							 return -1;								\
							 }											\

#define RETURN_IF_2A(EXPR, STR, ARG1, ARG2) if((EXPR))					\
						 	 {											\
						 		 fprintf(stderr, (STR), (ARG1), (ARG2));\
	   							 return -1;								\
							 }											\

#define RETURN_IF_ERROR(ACT) if(errno)													\
		{	fprintf(stderr, "Error while %s. Details: %s.\nExiting.\n",					\
				   	(ACT), strerror(errno));											\
			return -1;}

/*** Common Output Strings **********************************/
/* STR_ERR - Non recoverable errors. Program will end.*/
/* STR_WARN - Program will attempt to continue. */
#define STR_ERR_SOCK_CREATE "Error creating socket\n"
#define STR_ERR_SEND "Error sending message\n"
#define STR_ERR_RECV "Error receiving message\n"
#define STR_ERR_BIND "Error binding to port\n"
#define STR_ERR_SRV_CONNECT "Error connecting to server\n"
#define STR_ERR_LISTEN "Error attempting to listen\n"
#define STR_ERR_CONNECTION_CLOSED "Error: Peer has closed the connection\n"

#endif /*__TCP_CLIENT_SERVER_H__ */

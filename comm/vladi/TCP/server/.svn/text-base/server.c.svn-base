/* server.c
* Basic UDP Server 
* Author: Vladimir Krapp
* Created on: 20/3/14
* Last modified:
*/

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>


#include "tcpCommon.h"

#define NUM_SYN_WAITING 5


int main(void)
{
    int err = 0;

    char* msg = "Welcome to Server!";
    const size_t msgLen = strlen(msg) + 1;

    struct sockaddr_in srvAddr;
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    inet_pton(AF_INET, IP_ADDRESS, &(srvAddr.sin_addr));
    srvAddr.sin_port = htons(PORT);


    int sockListen = socket(AF_INET, SOCK_STREAM, 0);
        RETURN_IF(sockListen == -1, STR_ERR_SOCK_CREATE);
    PRINT_DBG(("Got socket: %d\n", sockListen));


    err = bind(sockListen, (const struct sockaddr*)&srvAddr, sizeof(srvAddr)); 
    PRINT_DBG(("sin_addr: %ld, port: %ld\n",
                    *(long*)&srvAddr.sin_addr,
                    *(long*)&srvAddr.sin_port ));
    RETURN_IF(err, STR_ERR_BIND);

    
    err = listen(sockListen, NUM_SYN_WAITING);
    RETURN_IF(err, STR_ERR_LISTEN);


    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t clientAddrLen = sizeof(clientAddr);
    int sockClient = accept(sockListen, (struct sockaddr*)&clientAddr, &clientAddrLen);


//RECV
    char buff[BUFF_SIZE];

    int bytesnum = recv(sockClient, buff, sizeof(buff), 0);
        RETURN_IF(bytesnum == -1, STR_ERR_RECV);
        RETURN_IF(bytesnum == 0, STR_ERR_CONNECTION_CLOSED);

    printf("Got request: %s\n", buff);

    PRINT_DBG(("addrLen: %d, sin_addr: %ld, port: %ld\n",
                clientAddrLen,
                *(long*)&clientAddr.sin_addr,
                *(long*)&clientAddr.sin_port ));


//SEND
    bytesnum = send(sockClient, msg, msgLen, 0);
    RETURN_IF(bytesnum == 0, STR_ERR_CONNECTION_CLOSED);
    RETURN_IF(bytesnum != msgLen, STR_ERR_SEND);

    close(sockClient);
    close(sockListen);

	return 0;
}

/************************************************************/

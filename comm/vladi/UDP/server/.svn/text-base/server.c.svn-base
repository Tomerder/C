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


#include "udpCommon.h"

int main(void)
{
   char* msg = "Welcome to Server!";
    const size_t msgLen = strlen(msg) + 1;

    struct sockaddr_in srvAddr;
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    inet_pton(AF_INET, IP_ADDRESS, &(srvAddr.sin_addr));
    srvAddr.sin_port = htons(PORT);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
        RETURN_IF(sock == -1, STR_ERR_SOCK_CREATE);
    PRINT_DBG(("Got socket: %d\n", sock));

    int err = bind(sock, (const struct sockaddr*)&srvAddr, sizeof(srvAddr));
        RETURN_IF(err, STR_ERR_BIND);

//RECV
    char buff[BUFF_SIZE];
    struct sockaddr_in peerAddr;
    memset(&peerAddr, 0, sizeof(peerAddr));
    socklen_t peerAddrLen = sizeof(peerAddr);

    int bytesnum = recvfrom(sock, buff, sizeof(buff), 0,
            (struct sockaddr*)&peerAddr, &peerAddrLen);
        RETURN_IF(bytesnum == -1, STR_ERR_RECV);

    printf("Got request: %s\n", buff);

    PRINT_DBG(("addrLen: %d, sin_addr: %ld, port: %ld\n",peerAddrLen, *(long*)&peerAddr.sin_addr, *(long*)&peerAddr.sin_port ));
//SEND
    bytesnum = sendto(sock, msg, msgLen, 0, 
        (const struct sockaddr*)&peerAddr, sizeof(peerAddr));
    RETURN_IF(bytesnum != msgLen, STR_ERR_SEND);


    close(sock);

	return 0;
}

/************************************************************/

/* client.c
* Basic UDP client
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
    char* msg = "Hello, Server!";
    const size_t msgLen = strlen(msg) + 1;
    int bytesnum = 0;

    struct sockaddr_in srvAddr;
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    inet_pton(AF_INET, IP_ADDRESS, &(srvAddr.sin_addr));
    srvAddr.sin_port = htons(PORT);

    PRINT_DBG(("sin_addr: %ld, port: %ld\n",*(long*)&srvAddr.sin_addr, *(long*)&srvAddr.sin_port ));

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
        RETURN_IF(sock == -1, STR_ERR_SOCK_CREATE);
    PRINT_DBG(("Got socket: %d\n", sock));
       

// SEND
    bytesnum = sendto(sock, msg, msgLen, 0, 
            (const struct sockaddr*)&srvAddr, sizeof(srvAddr));
        RETURN_IF(bytesnum != msgLen, STR_ERR_SEND);


// RECV
   char buff[BUFF_SIZE];
    struct sockaddr_in peerAddr;
    memset(&peerAddr, 0, sizeof(peerAddr));
    socklen_t peerAddrLen = 0;

    bytesnum = recvfrom(sock, buff, sizeof(buff), 0,
            (struct sockaddr*)&peerAddr, &peerAddrLen);
        RETURN_IF(bytesnum == -1, STR_ERR_RECV);

    printf("Got response: %s\n", buff);

    close(sock);

	return 0;
}

/************************************************************/

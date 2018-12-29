/* client.c
* Basic TCP client
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


int main(void)
{
    int err = 0;

    char* msg = "Hello, Server!";
    const size_t msgLen = strlen(msg) + 1;
    int bytesnum = 0;

    struct sockaddr_in srvAddr;
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    inet_pton(AF_INET, IP_ADDRESS, &(srvAddr.sin_addr));
    srvAddr.sin_port = htons(PORT);

    PRINT_DBG(("sin_addr: %ld, port: %ld\n",*(long*)&srvAddr.sin_addr, *(long*)&srvAddr.sin_port ));



    int sock = socket(AF_INET, SOCK_STREAM, 0);
        RETURN_IF(sock == -1, STR_ERR_SOCK_CREATE);
    PRINT_DBG(("Got socket: %d\n", sock));
    

    err = connect(sock, (const struct sockaddr*)&srvAddr, sizeof(srvAddr));
        RETURN_IF(err, STR_ERR_SRV_CONNECT);


// SEND
    bytesnum = send(sock, msg, msgLen, 0);
        RETURN_IF(bytesnum != msgLen, STR_ERR_SEND);



// RECV
    char buff[BUFF_SIZE];

    bytesnum = recv(sock, buff, sizeof(buff), 0);
        RETURN_IF(bytesnum == -1, STR_ERR_RECV);
        RETURN_IF(bytesnum == 0, STR_ERR_CONNECTION_CLOSED);

    printf("Got response: %s\n", buff);

    close(sock);

	return 0;
}

/************************************************************/

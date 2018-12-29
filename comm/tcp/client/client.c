#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#include "../common.h"

/*-----------------------------------------------------------*/

int main(void)
{
	char msgSnd[BUF_SIZE]  = "msg from client"; 
	size_t msgSndLen = strlen(msgSnd) + 1;
	
	char bufRcv[BUF_SIZE];
	
	/*--------------init------------------------------------*/
	//open socket
	int sock = socket(FAMILY, SOCK_TYPE, 0);
    CHK_ERR(sock, "socket\n");
    
    /*----------------connect----------------------------------*/
    //server address struct init - from common
    struct sockaddr_in serverAdr;
    
    memset(&serverAdr, 0, sizeof(serverAdr));
   
    serverAdr.sin_family = FAMILY;
   
    int err = inet_pton(FAMILY, SERVER_IP, &(serverAdr.sin_addr));
    CHK_ERR_SOCK(err, sock,"inet_pton\n");
    
    serverAdr.sin_port = htons(SERVER_PORT);
    
    int conn = connect(sock, (const struct sockaddr*)&serverAdr, sizeof(serverAdr)); 
    CHK_IF_SOCK(conn !=0, sock, "connect\n");
    
    /*----------------send to server----------------------------------*/
    //send msg to server
    size_t bytesSnd = send(sock, msgSnd, msgSndLen , 0);
    CHK_IF_SOCK(bytesSnd != msgSndLen, sock, "send\n");

	/*----------------receive from server----------------------------------*/
	
    size_t bytesRcv = recv(sock, bufRcv, sizeof(bufRcv), 0);
    CHK_ERR_SOCK(bytesRcv, sock,"recv\n");

    printf("msg received from server: %s\n", bufRcv);

	/*-------------cleanup------------------------------------*/
	//close sock
    close(sock);
    
 	return 0;   
}    
    
    



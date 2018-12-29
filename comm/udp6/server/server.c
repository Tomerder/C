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
	int err;
	char msgSnd[BUF_SIZE]  = "msg from server"; 
	size_t msgSndLen = strlen(msgSnd) + 1;
	
	char bufRcv[BUF_SIZE];
	
	/*--------------init------------------------------------*/
	//open socket
	int sock = socket(AF_INET6, SOCK_DGRAM, 0);
    CHK_ERR(sock, "socket");
    
    //bind to server address - from common
    struct sockaddr_in6 serverAdr;
    
    memset(&serverAdr, 0, sizeof(serverAdr));
   
    serverAdr.sin6_family = FAMILY;
   
    err = inet_pton(AF_INET6, SERVER_IP, &(serverAdr.sin6_addr));
    CHK_ERR_SOCK(err, sock,"inet_pton\n");
    serverAdr.sin6_port = htons(SERVER_PORT);
    
    int bindRes = bind(sock, (const struct sockaddr*)&serverAdr, sizeof(serverAdr));
    CHK_ERR_SOCK(bindRes, sock,"bind\n");
  
	/*----------------receive----------------------------------*/
	
	//client address struct init - kernel will write the address to it 
    struct sockaddr_in6 clientAdr;
    socklen_t clientAdrLen = sizeof(clientAdr);
    
    memset(&clientAdr, 0, sizeof(clientAdr));

    size_t bytesRcv = recvfrom(sock, bufRcv, sizeof(bufRcv), 0, (struct sockaddr*)&clientAdr, &clientAdrLen);
    CHK_ERR_SOCK(bytesRcv, sock,"recvfrom\n");

	printf("client ip : %lu , client port : %hu\n" , (long)clientAdr.sin6_addr.s6_addr , clientAdr.sin6_port);
    printf("msg received from client: %s\n", bufRcv);

	/*----------------send----------------------------------*/
    
    //send msg back to client to server
    size_t bytesSnd = sendto(sock, msgSnd, msgSndLen, 0, (const struct sockaddr*)&clientAdr, clientAdrLen );
    
    CHK_IF_SOCK(bytesSnd != msgSndLen, sock,"bytesSnd\n");


	/*-------------cleanup------------------------------------*/
	//close sock
    close(sock);
    
 	return 0;   
}    
    
    



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
	int sock = socket(AF_INET6, SOCK_DGRAM, 0);
    CHK_ERR(sock, "socket\n");
    
    /*----------------send----------------------------------*/
    //server address struct init - from common
    struct sockaddr_in6 serverAdr;
    
    memset(&serverAdr, 0, sizeof(serverAdr));
   
    serverAdr.sin6_family = FAMILY;
   
    inet_pton(AF_INET6, SERVER_IP, &(serverAdr.sin6_addr));
    serverAdr.sin6_port = htons(SERVER_PORT);
    
    //send msg to server
    size_t bytesSnd = sendto(sock, msgSnd, msgSndLen , 0, (const struct sockaddr*)&serverAdr, sizeof(serverAdr));
    CHK_IF_SOCK(bytesSnd != msgSndLen, sock, "bytesSnd\n");

	/*----------------receive----------------------------------*/
	
	//peer address struct init - kernel will write the address to it 
    struct sockaddr_in6 peerAdr;
    
    memset(&peerAdr, 0, sizeof(peerAdr));
    
    socklen_t peerAdrLen = 0;

    size_t bytesRcv = recvfrom(sock, bufRcv, sizeof(bufRcv), 0, (struct sockaddr*)&peerAdr, &peerAdrLen);
    CHK_ERR_SOCK(bytesRcv, sock,"recvfrom\n");


	printf("peer ip : %lu , peer port : %hu\n" , (long)peerAdr.sin6_addr.s6_addr , peerAdr.sin6_port);	
    printf("msg received from server: %s\n", bufRcv);

	/*-------------cleanup------------------------------------*/
	//close sock
    close(sock);
    
 	return 0;   
}    
    
    



#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#include "../common.h"

/*-----------------------------------------------------------*/

volatile sig_atomic_t exitFlag = 0; 

/*-----------------------------------------------------------*/

int SigInit();

void sigint_handler(int sig)
{
    write(0, "ByeBye !!!!\n", 14);   //write to stdout   - async-safe (not like printf)
    exitFlag = 1; 
}

/*-----------------------------------------------------------*/

int main(void)
{
	int err;
	char msgSnd[BUF_SIZE]  = "msg from server"; 
	size_t msgSndLen = strlen(msgSnd) + 1;
	
	char bufRcv[BUF_SIZE];
	
	/*--------------init------------------------------------*/
	err = SigInit();
	CHK_ERR( err , "SigInit()\n");
	
	//open socket
	int listenSock = socket(FAMILY, SOCK_TYPE, 0);
    CHK_ERR(listenSock, "socket\n");
     
    //set socket for not being on "wait" state after socket is closed (by def will be on wait state for 40 secs ans re bind will fail)  
  	int yes = 1;
	err = setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ;
	CHK_ERR(err , "setsockopt");
    
    //bind to server address - from common
    struct sockaddr_in serverAdr;
    
    memset(&serverAdr, 0, sizeof(serverAdr));
   
    serverAdr.sin_family = FAMILY;
   
    err = inet_pton(FAMILY, SERVER_IP, &(serverAdr.sin_addr));
    CHK_ERR_SOCK(err, listenSock,"inet_pton\n");
    
    serverAdr.sin_port = htons(SERVER_PORT);
    
    int bindRes = bind(listenSock, (const struct sockaddr*)&serverAdr, sizeof(serverAdr));
    CHK_ERR_SOCK(bindRes, listenSock,"bind\n");
  
  	err = listen(listenSock, SINS_QUEUE_SIZE);
  	CHK_ERR_SOCK(err, listenSock,"listen\n");
  
  	/*------------------------main loop--------------------------------*/
  	while(!exitFlag)
	{  
	  	/*----------------client sock----------------------------------*/
	  	//client address struct init - kernel will write the address to it 
	  	
	  	struct sockaddr_in clientAdr;
		socklen_t clientAdrLen = sizeof(clientAdr);
		
		memset(&clientAdr, 0, sizeof(clientAdr));

		int clientSock = accept(listenSock, (struct sockaddr*)&clientAdr, &clientAdrLen);
	  
		/*----------------receive from client----------------------------------*/	
	   
		size_t bytesRcv = recv(clientSock, bufRcv, sizeof(bufRcv), 0);
		if(exitFlag){
			close(clientSock);
			break;
		}
		CHK_ERR_SOCK(bytesRcv, clientSock,"recv\n");

		//printf("client ip : %lu , client port : %hu\n" , (long)clientAdr.sin_addr.s_addr , clientAdr.sin_port);
		printf("msg received from client: %s\n", bufRcv);

		/*----------------send to client----------------------------------*/
		
		//send msg back to client to server
		size_t bytesSnd = send(clientSock, msgSnd, msgSndLen, 0);  
		CHK_IF_SOCK(bytesSnd != msgSndLen, clientSock,"send\n");

		/*------------------close client sock-------------------------*/
	
		close(clientSock);
	
	}
	/*------------------------main loop end--------------------------------*/
	
	
	/*-------------cleanup------------------------------------*/
	//close listenSock
    close(listenSock);
    
    printf("listenSock is closed!!!!!!!!!!!!!!!!!!\n");
    
 	return 0;   
}    
    
    
/*---------------------------------------------------------------------------*/ 
    
int SigInit()
{
    void sigint_handler(int sig); /* prototype */
    char s[200];
    struct sigaction sa;

	/*-------------------------------------*/
	/*initialize sig action*/
    sa.sa_handler = sigint_handler;     /*handler function address*/
    /*sa.sa_flags = 0; // or SA_RESTART*/
	sa.sa_flags = SA_NOMASK;
    sigemptyset(&sa.sa_mask);
	/*-------------------------------------*/
	/*replace signal handler number SIGINT (ctrl+c) in current process signal table*/
    int err = sigaction(SIGINT, &sa, NULL) ;  
    CHK_ERR(err, "sigaction\n");
}
   
/*---------------------------------------------------------------------------*/  
    



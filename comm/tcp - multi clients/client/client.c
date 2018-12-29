#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#include "../common.h"

#include "list.h"

/*-----------------------------------------------------------*/

int numOfClients = 1;

void GetArgs(int _argc ,char** _argv);

/*-------------client API------------------------------------*/

int ClientCreate();

void ClientDestroy(int _clientSock);

int ClientSend(int _clientSock, char* _msgSnd, size_t _msgSndLen);

int ClientRcv(int _clientSock);

/*--------------main flow--------------------------------------*/

int Init(List* _list);
int SendAndRcv(List* _list);
void CleanUp(List* _list);

/*-----------------------------------------------------------*/

int main(int argc,char** argv)
{
	int i, err;
	
	GetArgs(argc ,argv);
		
	/*-----------init---------*/
	
	List* list = ListCreate();
   
    err = Init(list);
    CHK_ERR(err, "Init()\n"); 
   
    /*------------------------*/
   
    int cont = 1;			
	unsigned option;
	
	while (cont) {
		printf("Choose option: \n");
		printf("1: send & rcv  \n");
		printf("Any another number - stop \n");

		scanf("%d", &option);
		switch (option) {
			case 1:
				err = SendAndRcv(list);  
				CHK_ERR(err, "SendAndRcv()\n"); 	
				break;

			default: cont = 0;	break;
		}
	}
    
    CleanUp(list);
    
 	return 0;   
}    

/************************************main flow*************************************/
/*--------------------------------------------------------------------------------*/    

int Init(List* _list)
{	
	int i, clientSock;
	
	for(i=0; i < numOfClients; i++){
		clientSock = ClientCreate();
		CHK_ERR(clientSock, "ClientCreate()\n");
		
		ListPushTail(_list, (Data)clientSock);
		
		printf("Init : %d\n", clientSock);
	}

	return 0;
}

/*--------------------------------------------------------------------------------*/    

int SendAndRcv(List* _list)
{
	int i, err, clientSock;
	
	ListItr itr = ListBegin(_list);
	
	for(i=0; i < numOfClients; i++){
		
		clientSock = (int)ListGetData(itr);
	
		printf("SendAndRcv : %d\n", clientSock);
	
	 	/*----------send to server-------------*/
	 
	 	char msgSnd[BUF_SIZE];
	 	
	 	sprintf(msgSnd, "%s => from client socket : %d\n" , "msg from client" , clientSock);
	 	
		size_t msgSndLen = strlen(msgSnd) + 1;
		
		err = ClientSend(clientSock, msgSnd, msgSndLen);
		CHK_ERR(err, "ClientSend()\n");
		
		/*----------rcv from server-------------*/
		
		err = ClientRcv(clientSock);
		CHK_ERR(err, "ClientRcv()\n");
		
		/*---to next client----*/
		itr = ListNext(itr);
		
	}
	
	return 0;
}

/*--------------------------------------------------------------------------------*/    

void CleanUp(List* _list)
{
	int i, clientSock;	
	
	for(i=0; i < numOfClients; i++){
		ListPopTail(_list, (Data*)&clientSock);
		
		ClientDestroy(clientSock);
	}
    
    ListDestroy(_list);
}

/************************************client api*************************************/
/*--------------------------------------------------------------------------------*/    

int ClientCreate()
{
	/*--------init-------*/
	//open socket
	int clientSock = socket(FAMILY, SOCK_TYPE, 0);
    CHK_ERR(clientSock, "socket\n");
    
    /*-------connect-----*/
    //server address struct init - from common
    struct sockaddr_in serverAdr;
    
    memset(&serverAdr, 0, sizeof(serverAdr));
   
    serverAdr.sin_family = FAMILY;
   
    int err = inet_pton(FAMILY, SERVER_IP, &(serverAdr.sin_addr));
    CHK_ERR_SOCK(err, clientSock,"inet_pton\n");
    
    serverAdr.sin_port = htons(SERVER_PORT);
    
    int conn = connect(clientSock, (const struct sockaddr*)&serverAdr, sizeof(serverAdr)); 
    CHK_IF_SOCK(conn !=0, clientSock, "connect\n");
    
    return clientSock;
}

/*-----------------------------------------------------------*/

void ClientDestroy(int _clientSock)
{
	//close sock
    close(_clientSock);
}

/*-----------------------------------------------------------*/

int ClientSend(int _clientSock, char* _msgSnd, size_t _msgSndLen)
{
	//send msg to server
    size_t bytesSnd = send(_clientSock, _msgSnd, _msgSndLen , 0);
    
    CHK_IF_SOCK(bytesSnd != _msgSndLen, _clientSock, "send\n");

	return 0;
}

/*-----------------------------------------------------------*/

int ClientRcv(int _clientSock)
{
	char bufRcv[BUF_SIZE];

	size_t bytesRcv = recv(_clientSock, bufRcv, sizeof(bufRcv), 0);
    CHK_ERR_SOCK(bytesRcv, _clientSock,"recv\n");

    printf("msg received from server: %s\n", bufRcv);

	return 0;
}

/************************************args******************************************/
/*--------------------------------------------------------------------------------*/    

void GetArgs(int _argc ,char** _argv)
{
	int argType;

	/*set args from agrv*/	
	while ((argType = getopt(_argc, _argv, "n:")) != -1)
	{
		switch(argType)
		{
			case 'n':
				numOfClients = atoi(optarg);
				break;	
			default:				
				break;
		}
	}	
}

/*-----------------------------------------------------------------*/
 

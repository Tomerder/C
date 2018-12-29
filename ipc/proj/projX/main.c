#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/*------------------------------------------------------------------------------------*/

#include "MsgQueue.h"

/*------------------------------------------------------------------------------------*/

#define FILE_NAME "keyFile"
#define PRINT_ERR if(err!=0){ printf("FAIL: error no. %d  , ",err); }else{printf("PASS ,");}


/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	char msg[50] = "abct";
	char msgToGet[50] ;

	MsgQ* msgQ = MsgQueueCreate(FILE_NAME, 10 , 100000);
	if(msgQ == NULL){
		printf("msgQ == NULL\n");
		return 0; 
	}

	MsgQueueSnd (msgQ, msg, 5);

	MsgQueueRcv (msgQ, msgToGet, 5);

	printf("msg to get : %s\n", msgToGet);

	MsgQueueDestroy(msgQ);

    return 0; 	
}

/*------------------------------------------------------------------------------------*/	
	

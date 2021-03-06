/*************************************************************/
/** 	Project:	Logger                 				    **/
/** 	Author:		Tomer Dery								**/
/** 	Created:	28/1/2112								**/
/** 	Updated:	28/1/2112								**/
/*************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------------------------------------------*/

#include "log.h"

/*------------------------------------------------------------------------------------*/

enum {FALSE, TRUE};
enum {FAIL, PASS};

char* resultArr[2] = {"FAIL","PASS"};

#define SUCCESS 1

#define DEBUG 0

#define WRT_LOG(MODE, STR) ToLog(MODE, STR, __FILE__, __func__ );

/*------------------------------------------------------------------------------------*/

void func();

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	if(!LogCreate("log.txt", LOG_DEB)){
		printf("error\n");
	}
		
	WRT_LOG(LOG_DEB, "testttt");	
	
	func();
	
	LogDestroy();
		
	printf("/*-----------------------------------------------------------*/\n");	
    
    return 0; 	
}

/*------------------------------------------------------------------------------------*/

void func()
{
	WRT_LOG(LOG_DEB, "testttt2");	
}

/*------------------------------------------------------------------------------------*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


#define NULL 0
#define MAX_COMMAND_LEN 1024
#define MAX_ARG_LEN 64
#define MAX_PARAM_NUM 16


void TypePrompt();
int runCommandProcessor(input, errOut);


typedef struct Command {
		char   command[MAX_COMMAND_LEN];
		char*  argv[MAX_PARAM_NUM ];		
} Command;


/*---------------------------------------------------------------------------------------*/

int main(void)
{		
	return runCommandProcessor(stdin, stderr);
	

	return 0;
}

/*---------------------------------------------------------------------------------------*/


int runCommandProcessor(input, errOut){
	Command* pCmd = calloc(1, sizeof(struct Command));
	
	getCommand(input, pCmd));
	
	while( !isQuitCommand(pCmd) ){
		if( executeCommand(pCmd, &error) == CMD_ERR)
			handleError(errOut, error);
		
		getCommand(stdin, pCmd));				
	}	
	
	free(pCmd);
	
	return 0;
}


/*---------------------------------------------------------------------------------------*/

void ReadCommand(string* command, strSeq* params)
{
	int i = 0;
	string buff;
    strSeq args;
    string tok;
   
    buff = (string) malloc(MAX_COMMAND_LEN);
    
	 /* get atmost MAX_COMMAND_LEN chars from  input line */
	fgets(buff, MAX_COMMAND_LEN, stdin);
	
    args = malloc(sizeof(string*) * MAX_PARAM_NUM);
    args = calloc(MAX_PARAM_NUM,  sizeof(string*));


    
    tok = strtok(buff, " \n");
    while(tok && i < MAX_PARAM_NUM){
        args[i] = malloc(strlen(tok)+1);
        strcpy(args[i], tok);
        tok = strtok(NULL, " \n");
        ++i;
    }
    
    args[i] = NULL;
    *params = args;
    *command = args[0];
    return;
}

/*---------------------------------------------------------------------------------------*/

void TypePrompt()
{
	printf("$~>:");
}

/*---------------------------------------------------------------------------------------*/


/*

	
	
		camelCase();
		PascalCase();
		

		TypePrompt();
		ReadCommand(&command, &params);
		
		if ((strncmp(command, "exit", strlen("exit") - 1) == 0)){
			break;

		}
		
		pid = fork();
		

		if (pid > 0){
			waitpid(-1, &status, 0);
		} else if(pid == 0){
			execve(command, params, 0);

		} else{
			printf("Unable to fork\n");
		}
    }
    

	return 0;
}
*/





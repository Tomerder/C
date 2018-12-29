#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define COMMAND_LEN 50
#define PARAMETERS_LEN 10



char* type_prompt();
/*read_command(char* command , char* parameters);*/



int main(int argc,char** argv)
{	
	int status , id,i;
	char* command; 
	char* parameters = NULL;

	command = type_prompt();

	i=0;
	while(1){
		type_prompt();
		/*read_command(command,parameters);*/
		if((id = fork()) != 0){       				   /*parent*/
			printf("waitpid %d\n", id);
			waitpid(-1,&status,0);
			printf("status= %d\n", status);
		}else{            				   /*chilld*/
			printf("execve %d\n", id);
			execve(command, parameters, 0);
		}
		i++;
	}
	
	
	printf("finish\n");

	return 0;
}



char* type_prompt(){
	char* command;

	command = (char*)malloc( COMMAND_LEN * sizeof(char));  /*"/c/process";*/

	printf("please enter command: ");
	scanf("%s", command);

	return command;	

}








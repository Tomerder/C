#include<stdio.h>
#include <math.h>
#include "strings2.h"

static void enterBetweenWordApi();


static void decBinApi();


static void bigIntsApi();
 

static void squeezeApi();
 

static void anyApi();


/*------------------------------------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	

	int cont = 1;			
	unsigned int option;
	
	while (cont) {
		printf("Choose option: \n");
		printf("1: enterBetweenWord  \n");
		printf("2: DecBin    \n");
		printf("3: BigInts  \n");
        printf("4: Squeeze  \n");
		printf("5: Any  \n");
		printf("Any another number - stop \n");

		scanf("%d", &option);
		switch (option) {
			case 1:
				enterBetweenWordApi();	break;
			case 2:
				decBinApi();		break;
			case 3:
				bigIntsApi();	break;
			case 4:
				squeezeApi();	break;
			case 5:
				anyApi();	break;

			default: cont = 0;	break;
		}
	}
	

	return 0;
}


/*-----------------------------------------------------11111111111111111---------------------------------------------*/

static void enterBetweenWordApi(){
	char str[256];	
	printf("Enter a sentences : ");
	getchar();        /*disables enter from menu*/
  	gets (str);  				/* get sentence from user*/
	enterBetweenWord(str);                       
	printf ("\n");
}


/*----------------------decBin-------------------------------22222222222222222---------------------------------------------*/

static void decBinApi(){
	int num=0;	
	char c;	
	char resultStr[33];        /*      32 bits to int + '\0'       */
	char numStr[33];    

	while(1){
		printf("Enter a number : ");
		scanf("%s",&numStr);
		if(numStr[0] = '-')
			break;
		
		printf("press 'b' if you wish to convert from decimal to binary and any other key for converting binary to decimal : ");	
			
		fflush(stdout);	
		while(c=getchar()!='\n');
			

		if(c=getchar()=='b'){
			if(strlen(numStr) > 9 || strtok(numStr,"0123456789")!=NULL){  /*max number of int || not number*/
				printf("input error\n\n");
				continue;
			}
			num = atoi(numStr);
			printf("%s\n\n",decToBin(num,resultStr));
		}else{
			if(strtok(numStr,"01")!=NULL){
				 printf("number should contain only 0,1 \n\n");
				 continue;
			}
			printf("%d\n\n",binarToDec(numStr));
		}				
	}
}





/*---------------------BigInt--------------------------------333333333333---------------------------------------------*/

static void bigIntsApi(){
	
	char num1[500];
	char num2[500];
	char result[501];
	
	
	printf("Enter first number : ");
	scanf("%s",&num1);
	printf("Enter second number : ");
	scanf("%s",&num2);

	printf("%s\n\n",bigInts(num1,num2,result));

}


/*-----------------------------------------------------4444444444444---------------------------------------------*/


static void squeezeApi(){
	char num1[500];
	char num2[500];
	
	
	printf("Enter first word : ");
	scanf("%s",&num1);
	printf("Enter second word : ");
	scanf("%s",&num2);

	printf("%s\n\n",squeeze(num1,num2));	

}




/*-----------------------------------------------------555555555555555---------------------------------------------*/

static void anyApi(){
	
	char str1[500];
	char str2[500];
	
	
	printf("Enter first word : ");
	scanf("%s",&str1);
	printf("Enter second word : ");
	scanf("%s",&str2);

	printf("index is: %d\n\n",any(str1,str2));	

}






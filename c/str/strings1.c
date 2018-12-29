#include<stdio.h>
#include <math.h>

void enterBetweenWordApi();
static void enterBetweenWord(char* sentence);

void decBinApi();
static int binarToDec(int num);
static char* decToBin(int num,char* resultStr);  

void bigIntsApi();
static char*  bigInts(char* num1,char* num2, char* result);  

void squeezeApi();
static char* squeeze(char* s1,char* s2);  

void anyApi();
static int any(char* str1,char* str2);

static char* reverseStr(char* str);    
static void leftShift(char* str , int i);
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

void enterBetweenWordApi(){
	char str[256];	
	printf("Enter a sentences : ");
	getchar();        /*disables enter from menu*/
  	gets (str);  				/* get sentence from user*/
	enterBetweenWord(str);                       
	printf ("\n");
}

static void enterBetweenWord(char* sentence){     /*  return is the same address we got by parameter :  sentence */
	
	char str[500];
	int len; 
	int i = 0;
	char c;
	char* w;

	w = strtok (sentence," ,.-");
  	while (w != NULL)
  	{
   		printf ("%s\n",w);
   		w = strtok (NULL, " ,.-");
  	}
	
	

}

/*----------------------decBin-------------------------------22222222222222222---------------------------------------------*/

void decBinApi(){
	int num=0;	
	char c;	
	int cont=1;
	char resultStr[33];        /*      32 bits to int + '\0'       */

	while(cont){
		printf("Enter a number : ");
		scanf("%d",&num);
		if(num<0)
			cont=0;
		else{
			printf("press 'b' if you wish to convert from decimal to binary and any other key for converting binary to decimal : ");	
			
			fflush(stdout);	
			while(c=getchar()!='\n');
			

			if(c=getchar()=='b')
				printf("%s\n\n",decToBin(num,resultStr));
			else 
				printf("%d\n\n",binarToDec(num));				
		}
	}

}


static int binarToDec(int binarNum){
	int num=0;
	int i=0;
	int mul=0;

	while(binarNum>0)
	{
		mul = pow(2,i);
		num+=(binarNum%10)*mul;
		binarNum = binarNum/10;
		i++;
	} 
	
	return num;
	
}




static char* decToBin(int num,char* resultStr){     /*  return is the same address we got by parameter :  resultStr */
	char bit;
	int i=0;	

	while(num>0){
		bit=(num % 2)+'0';
		num = num/2;	
		resultStr[i]=bit;	
		i++;
	}
	resultStr[i]='\0';
	
	resultStr=reverseStr(resultStr);
	
	return resultStr;

}



static char* reverseStr(char* str){  /*return value is to the same address as str parameter*/
	int i=0;
	char c;	
	int len = strlen(str) - 1;

	while(i <= len/2){
		c=str[len-i];
		str[len-i] = str[i];
		str[i] = c;
		i++;
	}

	return str;
}

/*---------------------BigInt--------------------------------333333333333---------------------------------------------*/

void bigIntsApi(){
	
	char num1[500];
	char num2[500];
	char result[501];
	
	
	printf("Enter first number : ");
	scanf("%s",&num1);
	printf("Enter second number : ");
	scanf("%s",&num2);

	printf("%s\n\n",bigInts(num1,num2,result));

}


static char* bigInts(char* num1 , char* num2, char* res){   /*return value is to the same address as res parameter*/

	int len1,len2,min,max,first;
	int i=0;
	int sum;
	int plus=0;
	char c;

	len1 = strlen(num1);
	len2 = strlen(num2);
	
	min = (len1>len2)?(len2):(len1);
	max = (len1>len2)?(len1):(len2);

	

	while(i<max){
		if(i<min){  /*small number didnt end*/ 
			sum = (num1[len1-i-1]-'0') + (num2[len2-i-1]-'0') + plus; 
		}else{      /* small number ended */
			if(len1>len2)
				sum = (num1[len1-i-1]-'0')	+ plus;
			else if(len2>len1)	
				sum =(num2[len2-i-1]-'0') + plus; 		
		}
	
		if(sum>9){
			first = sum - 10;
			plus = 1;
		}else{
			first = sum;
			plus = 0;
		}
		
		c=first+'0';
		/*printf("%c\n",c);*/
		res[i]= c;
		i++;
	}

	res[i] = '\0';
	
	reverseStr(res);

	return res;	
}

/*-----------------------------------------------------4444444444444---------------------------------------------*/


void squeezeApi(){
	char num1[500];
	char num2[500];
	
	
	printf("Enter first word : ");
	scanf("%s",&num1);
	printf("Enter second word : ");
	scanf("%s",&num2);

	printf("%s\n\n",squeeze(num1,num2));	

}


static char* squeeze(char* s1,char* s2){   /*return value is to the same address as s1 parameter*/
	int i=0,j=0;	
	int hashTable[256] = {0};		
		
	while(s2[i]!='\0'){
		hashTable[(s2[i])] = 1;
		i++;
	}
	
	i=0;
	while(s1[i]!='\0'){
		if(hashTable[(s1[i])] == 1){
			leftShift(s1,i);
		}	
		i++;
	}
			
		
	return s1;

} 



static void leftShift(char* str , int i){
	while(str[i]!='\0'){
		str[i]=str[i+1];
		i++;	
	}
}




/*-----------------------------------------------------555555555555555---------------------------------------------*/

void anyApi(){
	
	char str1[500];
	char str2[500];
	
	
	printf("Enter first word : ");
	scanf("%s",&str1);
	printf("Enter second word : ");
	scanf("%s",&str2);

	printf("index is: %d\n\n",any(str1,str2));	

}



static int any(char* str1,char* str2){
	int i=0,j=0;
	int toRet = -1;	
	
	while(str1[i] != '\0'){
		for(j=0;str2[j]!='\0';j++){
			if(str1[i+j] != str2[j]){
				break;			
			}
		}
		if(str2[j] == '\0'){
			toRet = i;
			break;
		}
		i++;
	}
	
	return toRet;
}







#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------------------------*/

enum{FALSE , TRUE};


#define HEXA_DIGITS_IN_INT 8
#define BITS_1111 15 
#define BITS_OF_HEXA 4 


/*------------------------------------------------------------------------------------*/

void num2Hex(int num, char* hexaNum);
char* reverseStr(char* str);
/*------------------------------------------------------------------------------------*/

int main()
{
	int num = 99756218;
	char hexaNum[HEXA_DIGITS_IN_INT] = {'0'};
	
	num2Hex(num, hexaNum);
	
	printf("decimal number : %d  == hexa number : %s\n", num , hexaNum);
	
	return 0;

}

/*------------------------------------------------------------------------------------*/

void num2Hex(int num, char* hexaNum)
{
	/*will be build only on first function call*/
	static char lookUpHexa[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; 
	int hexaCurdigit=0 , i=0;

	while(num != 0   &&   i<HEXA_DIGITS_IN_INT){
		hexaCurdigit = num & BITS_1111;  /*get current hexa digit*/
		
		/*hexaNum[i] = lookUpHexa[hexaCurPart];  =>   if we want this function to work for types other then int*/  
		hexaNum[HEXA_DIGITS_IN_INT - i - 1] = lookUpHexa[hexaCurdigit];
		
		num = num >> BITS_OF_HEXA;	  /* >>4 bits => to next hexa digit*/	
		++i;
	}

	/*reverseStr(hexaNum);   =>   if we want this function to work for types other then int*/
}

/*------------------------------------------------------------------------------------*/

char* reverseStr(char* str){ 
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

/*------------------------------------------------------------------------------------*/





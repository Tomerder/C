#include<stdio.h>
#include <math.h>

int atoi2(char* str);

int main(int argc,char** argv)
{	
	char str1[20]= "100";
	char str2[20]= "    1234    15";
	char str3[20]= " -1234";
	char str4[20]= " +1234";
	char str5[20]= "123four";
	char str6[20]= "invalid123";
	printf("%s : %d\n", str1,atoi2(str1));	
	printf("%s : %d\n",str2,atoi2(str2));	
	printf("%s : %d\n",str3,atoi2(str3));	
	printf("%s : %d\n",str4,atoi2(str4));	
	printf("%s : %d\n",str5,atoi2(str5));	
	printf("%s : %d\n",str6,atoi2(str6));	
}


int atoi2(char* str){
	int i=0;
	int num=0;
    char* numStr;
	int startNumIndex;
	int lenNum;  
	int m1,m2;
	int mul = 1; 	
	int len = strlen(str);
	
	numStr = (char*)malloc(len*sizeof(char));		
	
	while(str[i]==' '){   /*delete spaces from left*/  
		i++; 
	}
    
	if(str[i]=='-'){	     /*minus*/
		mul=-1;
		i++;
	}else if(str[i]=='+'){   /*   ignore +   */
		i++;	
	}

	startNumIndex = i;

	while(isdigit(str[i])){
		numStr[i-startNumIndex] = str[i];   /*getting the number*/
		i++;	
	}
	

	lenNum = strlen(numStr);  
		

	for(i=lenNum-1;i>=0;i--){
			m1= (pow(10,lenNum-i-1)) ;
			m2= (numStr[i] - '0');
			/*printf("%d * %d \n", m1 , m2 );*/
			/*num += (pow(10,lenNum-i-1)) * (numStr[i] - '0'); */ 
			num += m1 * m2 * mul;   
	}
	
	return num;


}













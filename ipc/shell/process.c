#include<stdio.h>
#include<stdlib.h>



int var1=0;

int main(int argc,char** argv)
{	
	int i,n,var2=0;
	
    n=fork();

	if(n<0){               
		printf("error\n");
	}else if(n>0){           /*   n>0  parent    */  
		for(i=0;i<5;i++){
			var1++;
			var2++;
			printf("%d parent - var1: %d  , var2: %d\n",n,var1,var2);
			sleep(1);
		}
	}else{
		for(i=0;i<5;i++){    /*   n=0  child    */
			var1--; 
			var2--;
			printf("%d child - var1: %d  , var2: %d\n",n,var1,var2);
			sleep(1);
		}
		
	}
	

	printf("finish\n");
	return 0;
}





#include<stdio.h>
#include<stdlib.h>


int checkParms(char* parm1,char* parm2);


int main(int argc,char** argv)
{	
	int i=0;
	int j=0;
	int temp;
	
	int arr[24]={3,7,4,1,63,756,123,875,1,5,8,3,6,1324,8,4,112,86,6534,24,321,4,6,8};

	for(i=0;i<24;i++){
		for(j=i;j<24;j++){
			if(arr[i]>arr[j]){
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}

	i=0;
	while(i<24){
		printf("%d , ",arr[i]);
		i++;
	}
	
	
	printf("\nAll good.\n\n");	
	return 0;
}





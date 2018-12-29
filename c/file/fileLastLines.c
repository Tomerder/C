#include<stdio.h>
#include<stdlib.h>



int printLastNSentences(char* fileName,int n);


int main(int argc,char** argv)
{	
	char* filename;   
	int n=5;
/*
    printf("enter file name: ");
	scanf("%s",filename); 

	printf("enter number of lines to print: ");
	scanf("%d",&n);
*/
	 
	
	if(argc<2){
		printf("file name wasnt entered");
		return -1;
	}
	
	filename = (char*)argv[1]; 
	if(argc>2){
		n=atoi(argv[2]);
	}	

	if(printLastNSentences(filename,n)!=0){
		printf("failed");
	}
	
	return 0;

}


int printLastNSentences(char* fileName,int n){ 
	FILE *fp;
	char sentence[256];
	int posOfSetencesBegin[100];      /*   change to posOfSetencesBegin[N]   */
	int posToBegin;
	int i=1;
	

	if (n==0){
		n=5;
	}	

	if( (fp=fopen(fileName,"r"))==NULL){
		return -1;
	}

	posOfSetencesBegin[0]=1;
	while( fgets(sentence , 100 , fp) != NULL){		
		posOfSetencesBegin[i] = ftell(fp);
		i++;
	}

	if(i>n-1){
		posToBegin = posOfSetencesBegin[i-n-1];	
	}else{
		posToBegin = 1;
	}	

	fseek(fp,posToBegin,0);

	while( fgets(sentence , 100 , fp) != NULL){
		printf("%s",sentence);
	}
	
	return 0;
}


#include<stdio.h>
#include<stdlib.h>


int checkParms(char* parm1,char* parm2);


int main(int argc,char** argv)
{	
	
	
	if(argc<3){
		printf("wrong number of parmaeters.\n\n");
		return -1;
	}
	

	if(checkParms(argv[1],argv[2]) == -1){
		printf("Errorrrr.\n\n");		
		return -1;
	}
	
	printf("All good.\n\n");	
	return 0;
}




int checkParms(char* parm1,char* parm2){ 
	int flag =0;
	int i=1;

	if(isdigit(parm1[0]))
		flag = 1;

	while(parm1[i] != '\0'){
		if(   (flag && !isdigit(parm1[i])) || (!flag && isdigit(parm1[i]))  ){	
			return -1;
		}
		i++;
	}

	i=0;
	while(parm2[i] != '\0'){
		if(   (!flag && !isdigit(parm2[i])) || (flag && isdigit(parm2[i]))  ){
			return -1;
		}
		i++;
	}


}


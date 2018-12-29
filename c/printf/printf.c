#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<math.h>

#define MAX_INT_DIGITS 10
#define MAX_DOUBLE_DIGITS 16
#define DOUBLE_ACCURACY 5

typedef struct OutputStr{
	int len;
	char* str;
}OutputStr;


/*---------------------------------------------------------------------------------*/

int printF(const char* format,...);
int concatInt(OutputStr* decodedStr,int intFromArg,int index);
int concatDouble(OutputStr* decodedStr,double doubleFromArg,int index);
int concatStr(OutputStr* decodedStr,char* strFromArg,int index);
int realloc2(OutputStr* decodedStr,int j,int toCheck);

/*---------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	char* par;
	printF("test1 %d test2 %d test3 test4 %s test5 %f test6 ",12345,6789,"test char*",123.456);
	
	/*printf("please enter printf format :");		
	scanf("&s",&par);
	printF(par);*/	

	return 0;
}


/*---------------------------------------------------------------------------------*/


int printF(const char* format,...){
	OutputStr decodedStr;
	int i=0, j=0;
	int intFromArg=0;
	double doubleFromArg=0;
	char* strFromArg;
	int formatLen = strlen(format);
	
	
	decodedStr.len = formatLen*2;
	decodedStr.str = (char*)malloc(sizeof(char)*decodedStr.len);
	if(decodedStr.str==NULL){
		return -1;
	}


	va_list arg;
	va_start (arg, format);

  	while (format[i] != '\0')
  	{

		if(realloc2(&decodedStr,j,1)==-1){
			return -1;
		}
		

		if(format[i] == '%'){
			if(format[i+1] == '%'){
				decodedStr.str[j] = format[i];
			}else if(format[i+1] == 'd'){
				intFromArg = va_arg(arg,int);          /* get int from parameter    */												
				j=concatInt(&decodedStr,intFromArg,j);
			}else if(format[i+1] == 'f'){
				doubleFromArg = va_arg(arg,double);    /* get double from parameter */					
				j=concatDouble(&decodedStr,doubleFromArg,j);
			}else if(format[i+1] == 's'){
				strFromArg = va_arg(arg,char*);        /* get char* from parameter */					
				j=concatStr(&decodedStr,strFromArg,j);
			}else{
				return -1;    /*not legal*/
			}
			i++;   /* advance after next letter after %    */
		}else{
			decodedStr.str[j] = format[i];
		}	
		i++;
		j++;
  	}

	va_end (arg);

	decodedStr.str[j] = '\0';
	printf("\n%s\n\n",decodedStr.str);
	free(decodedStr.str);

}



/*---------------------------------------------------------------------------------*/

int concatInt(OutputStr* decodedStr,int intFromArg,int index){
	int curIndex = index;
	int i=0;
    char* word;
	
	if(realloc2(decodedStr,index,MAX_INT_DIGITS)==-1){
		return -1;
	}		

	if(intFromArg==0){
		decodedStr->str[curIndex]='0';
		return curIndex;
	}

	word = (char*)malloc(sizeof(char)*MAX_INT_DIGITS);

	while(intFromArg != 0){
		word[i] = (intFromArg % 10) + '0';
		intFromArg=intFromArg/10;
		i++;
	}


	for(i=i-1;i>=0;i--){
		decodedStr->str[curIndex]=word[i];
		curIndex++;
	}

	curIndex--;

	free(word);
	return curIndex;

}


/*---------------------------------------------------------------------------------*/


int concatDouble(OutputStr* decodedStr,double doubleFromArg,int index){
	
	int intpart;
 	int decpart;
	

	if(realloc2(decodedStr,index,MAX_DOUBLE_DIGITS)==-1){
		return -1;
	}	

	intpart = doubleFromArg / 1;
	decpart = (int)((doubleFromArg - intpart)*pow(10,DOUBLE_ACCURACY));
	while(decpart%10==0 && decpart!=0)
		decpart/=10;

	index = concatInt(decodedStr,intpart,index);
	index++;
	decodedStr->str[index++] = '.';
	index = concatInt(decodedStr,decpart,index);
	
	return index;

}

/*---------------------------------------------------------------------------------*/


int concatStr(OutputStr* decodedStr,char* strFromArg,int index){
	int i=0;

	if(realloc2(decodedStr,index,strlen(strFromArg)+1)==-1){
		return -1;
	}		

	while(strFromArg[i]!='\0'){
		decodedStr->str[index + i] = strFromArg[i];
		i++;
	}
	
	return index+i-1;

}

/*---------------------------------------------------------------------------------*/


int realloc2(OutputStr* decodedStr,int j,int toCheck){

	if(j >= (decodedStr->len + toCheck)){            
			decodedStr->len = (decodedStr->len + toCheck)*2;
			decodedStr->str = (char*)realloc(decodedStr->str, sizeof(char)*decodedStr->len);
			if(decodedStr->str==NULL)
				return -1;
			/*else
				printf("realloc\n");*/
	}	

	return decodedStr->len;
}

/*---------------------------------------------------------------------------------*/





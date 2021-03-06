/*-----------------------------------------Tomer Dery-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MyString.h"


typedef enum {FAIL,PASS} Result;
char* resultArr[2] = {"FAIL","PASS"};

#define SUCCESS 1

#define DEBUG 0
/*------------------------------------------------------------------------------------*/

#define STR_LEN_CHK(S) if(MyStrlen(S) != strlen(S)  ) resultInd = FAIL;

#define STR_LEN_R_CHK(S) if(MyStrlenRec(S) != strlen(S)  ) resultInd = FAIL;

/*------------------------------------------------------------------------------------*/

#define STR_CMP_CHK(S1,S2) if( (MyStrcmp(S1, S2) * strcmp(S1, S2)) < 0 ) resultInd = FAIL;

#define STR_CMP_R_CHK(S1,S2) if( (MyStrcmpRec(S1, S2) * strcmp(S1, S2)) < 0 ) resultInd = FAIL;

/*------------------------------------------------------------------------------------*/

#define STR_CPY_CHK(S1, S2) if(  (strcmp(MyStrcpy(S1, S2) , strcpy(S1, S2)) )   ) resultInd = FAIL;

#define STR_CPY_R_CHK(S1, S2) if(  (strcmp((char*)MyStrcpyRec(S1, S2) , strcpy(S1, S2)) )   ) resultInd = FAIL;

/*------------------------------------------------------------------------------------*/

#define STR_N_CPY_CHK(S1, S2, N) if(  (strcmp(MyStrncpy(S1, S2,N) , strncpy(S1, S2,N)) )  ) resultInd = FAIL;

#define STR_N_CPY_R_CHK(S1, S2, N) if(  (strcmp((char*)MyStrncpyRec(S1, S2,N) , strncpy(S1, S2,N)) )  ) resultInd = FAIL;

/*------------------------------------------------------------------------------------*/

#define STR_CAT_CHK(S1, S2) if( ( strcmp(MyStrcat(S1, S2) , strcat(S1, S2)) )  ) resultInd = FAIL;

/*------------------------------------------------------------------------------------*/

int strstrChk(char* _str1, char* _str2, int _isRec );

/*------------------------------------------------------------------------------------*/

static void MyStrlenChk();
static void MyStrlenRecChk();

static void MyStrcmpChk();
static void MyStrcmpRecChk();

static void MyStrcpyChk();
static void MyStrcpyRecChk();

static void MyStrncpyChk();
static void MyStrncpyRecChk();

static void MyStrcatChk();

static void MyStrstrChk(int _isRec);

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{

	printf("/------------------------------------------------------/\n");

	MyStrlenChk();
	MyStrlenRecChk();		

	printf("/------------------------------------------------------/\n");
	
	MyStrcmpChk();
	MyStrcmpRecChk();

	printf("/------------------------------------------------------/\n");

	MyStrcpyChk();
	MyStrcpyRecChk();

	printf("/------------------------------------------------------/\n");

	MyStrncpyChk();
	MyStrncpyRecChk();

	printf("/------------------------------------------------------/\n");

	MyStrcatChk();

	printf("/------------------------------------------------------/\n");

	MyStrstrChk(0);
	MyStrstrChk(1);

	printf("/------------------------------------------------------/\n");




    return 0; 	

    
}

/*------------------------------------------------------------------------------------*/


static void MyStrlenChk(){
	int resultInd = PASS;

	printf("MyStrlenChk()\n");

	STR_LEN_CHK("");
	STR_LEN_CHK("hello");
	STR_LEN_CHK("trtgervbd");

	
	printf("%s\n", resultArr[resultInd]);

}


static void MyStrlenRecChk(){
	int resultInd = PASS;

	printf("MyStrlen REC Chk()\n");

	STR_LEN_R_CHK("");
	STR_LEN_R_CHK("hello");
	STR_LEN_R_CHK("trtgervbd");
	
	printf("%s\n", resultArr[resultInd]);
}
/*------------------------------------------------------------------------------------*/

static void MyStrcmpChk(){
	int resultInd = PASS;

	printf("MyStrcmpChk()\n");

	STR_CMP_CHK("abc", "a");
	STR_CMP_CHK("abc", "abc");
	STR_CMP_CHK("a", "abc");
	STR_CMP_CHK("tttta", "atttt");
	STR_CMP_CHK("a", "b");
	STR_CMP_CHK("ttaa", "ttat");
	STR_CMP_CHK("attt", "a");
	STR_CMP_CHK("taaa", "t");
	STR_CMP_CHK("a", "a");
	STR_CMP_CHK("tt", "tta");
	STR_CMP_CHK("tta", "tt");
	STR_CMP_CHK("", "tt");
	STR_CMP_CHK("tt", "");
	STR_CMP_CHK("", "");
	
	printf("%s\n", resultArr[resultInd]);
}



static void MyStrcmpRecChk(){
	int resultInd = PASS;

	printf("MyStrcmp REC Chk()\n");

	STR_CMP_R_CHK("abc", "a");
	STR_CMP_R_CHK("abc", "abc");
	STR_CMP_R_CHK("a", "abc");
	STR_CMP_R_CHK("tttta", "atttt");
	STR_CMP_R_CHK("a", "b");
	STR_CMP_R_CHK("ttaa", "ttat");
	STR_CMP_R_CHK("attt", "a");
	STR_CMP_R_CHK("taaa", "t");
	STR_CMP_R_CHK("a", "a");
	STR_CMP_R_CHK("tt", "tta");
	STR_CMP_R_CHK("tta", "tt");
	STR_CMP_R_CHK("", "tt");
	STR_CMP_R_CHK("tt", "");
	STR_CMP_R_CHK("", "");

	printf("%s\n", resultArr[resultInd]);
}

/*------------------------------------------------------------------------------------*/


static void MyStrcpyChk(){
	int resultInd = PASS;
	const char src[10] = "abc"; 	
	char dst[10]; 	

	printf("MyStrcpy()\n");

	STR_CPY_CHK(dst, src);
	STR_CPY_CHK(dst, "abc");
	STR_CPY_CHK(dst, "abc");
	STR_CPY_CHK(dst, "");
	STR_CPY_CHK(dst, "tt");
	STR_CPY_CHK(dst, "b");
	STR_CPY_CHK(dst, "");

	printf("%s\n", resultArr[resultInd]);

}




static void MyStrcpyRecChk(){
	int resultInd = PASS;	
	const char src[10] = "abc"; 	
	char dst[10]; 	

	printf("MyStrcpy REC \n");
	
	STR_CPY_R_CHK(dst, src);
	STR_CPY_R_CHK(dst, "abc");
	STR_CPY_R_CHK(dst, "");
	STR_CPY_R_CHK(dst, "tt");
	STR_CPY_R_CHK(dst, "b");
	STR_CPY_R_CHK(dst, "");

	printf("%s\n", resultArr[resultInd]);

}


/*------------------------------------------------------------------------------------*/

static void MyStrncpyChk(){
	int resultInd = PASS;
	char src[10] = "abc"; 	
	char dst[10] ; 	

	printf("MyStrNcpy()\n");

	STR_N_CPY_CHK(dst, src , 2);
	STR_N_CPY_CHK(dst, "abc", 3);
	STR_N_CPY_CHK(dst, "" , 4);
	STR_N_CPY_CHK(dst, "tt" , 0);
	STR_N_CPY_CHK(dst, "b" , 1);
	STR_N_CPY_CHK(dst, "" , 1);
	STR_N_CPY_CHK(dst, "ttttt" , 4);
	
	printf("%s\n", resultArr[resultInd]);

}



static void MyStrncpyRecChk(){
	int resultInd = PASS;
	char src[10] = "abc"; 	
	char dst[10] ; 	

	printf("MyStrNcpy()\n");


	STR_N_CPY_R_CHK(dst, src , 2);
	STR_N_CPY_R_CHK(dst, "abc", 3);
	STR_N_CPY_R_CHK(dst, "" , 4);
	STR_N_CPY_R_CHK(dst, "tt" , 0);
	STR_N_CPY_R_CHK(dst, "b" , 1);
	STR_N_CPY_R_CHK(dst, "" , 1);
	STR_N_CPY_R_CHK(dst, "ttttt" , 4);
	
	printf("%s\n", resultArr[resultInd]);

}


/*------------------------------------------------------------------------------------*/

static void MyStrcatChk(){
	int resultInd = PASS;
	char src[10] = "abc"; 	
	char dst[100] = "fgdfsdf"; 	

	printf("MyStrcatChk()\n");

	STR_CAT_CHK(dst, src );
	STR_CAT_CHK(dst, "abc");
	STR_CAT_CHK(dst, "");
	STR_CAT_CHK(dst, "tt");
	STR_CAT_CHK(dst, "b");
	STR_CAT_CHK(dst, "jh");
	STR_CAT_CHK(dst, "ttttt");
	
	printf("%s\n", resultArr[resultInd]);

}

/*------------------------------------------------------------------------------------*/



static void MyStrstrChk(int _isRec){
	int resultInd = PASS;	
	
	char str[10] = "abcdefg"; 	

	printf("MyStrstrChk()\n");

	/*printf("Strcpy : %s \n" , strstr(str, "gr"));
	printf("MyStrcpyRec : %s\n" , MyStrstr(str, "gr"));
	*/

	resultInd = ((strstrChk(str, "abc", _isRec) == FAIL) ? FAIL : resultInd);
	resultInd = ((strstrChk(str, "", _isRec)  == FAIL) ? FAIL : resultInd);
	resultInd = ((strstrChk(str, "b", _isRec)  == FAIL) ? FAIL : resultInd);
	resultInd = ((strstrChk(str, "bc", _isRec)  == FAIL) ? FAIL : resultInd);
	resultInd = ((strstrChk(str, "g", _isRec)  == FAIL) ? FAIL : resultInd);
	resultInd = ((strstrChk(str, "gr", _isRec)== FAIL) ? FAIL : resultInd);
	resultInd = ((strstrChk(str, "fdsgd", _isRec)== FAIL) ? FAIL : resultInd);
	resultInd = ((strstrChk(str, "defg" , _isRec)== FAIL) ? FAIL : resultInd);
	resultInd = ((strstrChk("", "defg" , _isRec)== FAIL) ? FAIL : resultInd);
	resultInd = ((strstrChk("", "" , _isRec)== FAIL) ? FAIL : resultInd);
	
	printf("%s\n", resultArr[resultInd]);
}



int strstrChk(char* _str1, char* _str2 , int _isRec)
{
	char* res2;	
	char* res1= strstr(_str1, _str2);
	
	if(_isRec){
		res2= MyStrstrRec(_str1, _str2);
	}else{
		res2= MyStrstr(_str1, _str2);
	}

	if   (!    (  (res1 == res2) && (res1 == NULL) )    ){
		if( strcmp(res1,res2) ){
			return FAIL;
		}
	}	

	return PASS;
}







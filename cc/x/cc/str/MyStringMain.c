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

#define STR_CMP_CHK(S1,S2) if(MyStrcmp(S1, S2) * strcmp(S1, S2) < 0 ) resultInd = FAIL;

#define STR_CPY_CHK(S1, S2) if(MyStrcpy(S1, S2) != strcpy(S1, S2)  ) resultInd = FAIL;

#define STR_N_CPY_CHK(S1, S2, N) if(MyStrncpy(S1, S2, N) != strncpy(S1, S2, N)  ) resultInd = FAIL;

#define STR_CAT_CHK(S1, S2) if(MyStrcat(S1, S2) != strcat(S1, S2)  ) resultInd = FAIL;

#define STR_STR_CHK(S1, S2) if(MyStrstr(S1, S2) != strstr(S1, S2)  ) resultInd = FAIL;

/*------------------------------------------------------------------------------------*/



static void MyStrlenChk();
static void MyStrlenRecChk();
static void MyStrcmpChk();
static void MyStrcpyChk();
static void MyStrncpyChk();
static void MyStrcatChk();
static void MyStrstrChk();

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{

	printf("/------------------------------------------------------/\n");

	MyStrlenChk();
	MyStrlenRecChk();		

	printf("/------------------------------------------------------/\n");
	
	MyStrcmpChk();

	printf("/------------------------------------------------------/\n");

	MyStrcpyChk();

	printf("/------------------------------------------------------/\n");

	MyStrncpyChk();

	printf("/------------------------------------------------------/\n");

	MyStrcatChk();

	printf("/------------------------------------------------------/\n");

	MyStrstrChk();

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

	printf("MyStrlenRecChk()\n");

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

	/*printf("MyStrcmp(abc,a) : %d \n" , MyStrcmp("abc","ab") );
	printf("MyStrcmp(abc,a) : %d \n" , strcmp("abc","ab") );*/

	printf("%s\n", resultArr[resultInd]);
}

/*------------------------------------------------------------------------------------*/


static void MyStrcpyChk(){
	int resultInd = PASS;
	char src[10] = "abc"; 	
	char dst[10]; 	

	printf("MyStrcpy()\n");


	STR_CPY_CHK(dst, src);
	STR_CPY_CHK(dst, "abc");
	STR_CPY_CHK(dst, "");
	STR_CPY_CHK(dst, "tt");
	STR_CPY_CHK(dst, "b");
	STR_CPY_CHK(dst, "");

	printf("%s\n", resultArr[resultInd]);

}

/*------------------------------------------------------------------------------------*/

static void MyStrncpyChk(){
	int resultInd = PASS;
	char src[10] = "abc"; 	
	char dst[10]; 	

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



/*------------------------------------------------------------------------------------*/

static void MyStrcatChk(){
	int resultInd = PASS;
	char src[10] = "abc"; 	
	char dst[100] = "\0"; 	

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

static void MyStrstrChk(){
	int resultInd = PASS;	
	
	char str[10] = "abcdefg"; 	

	printf("MyStrstrChk()\n");

	STR_STR_CHK(str, "abc");
	STR_STR_CHK(str, "");
	STR_STR_CHK(str, "b");
	STR_STR_CHK(str, "bc");
	STR_STR_CHK(str, "g");
	STR_STR_CHK(str, "gr");
	STR_STR_CHK(str, "fdsgd");
	STR_STR_CHK(str, "defg");
	
	printf("%s\n", resultArr[resultInd]);
}










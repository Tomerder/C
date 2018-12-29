/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-08-01
    Date last modified :	2013-08-01
    Description : implementation of some string.h functions
***************************************************************************************/

#include <stdio.h>
/*
#include <string.h>
*/

#include "MyString.h"




int main()
{
	int len1, len2;
	int cmp1, cmp2, cmp3;
	
	const char str1[] = "abcde";
	const char str2[] = "Ab #cdE f";
	const char str3[] = "abcfg";
	const char strfind[] = "cde";
	const char blank10[] = "        .";
	
	char str4[10]="\0", str5[10]="\0", str6[10]="\0", str7[10]="\0";
	char str8[20] = "xyz";
	char *strPtr1, *strPtr2;
	
	
	
	/****** STRLEN ******/
	
	len1 = StrlenIter(str1);
	len2 = StrlenRec(str1);
	printf("lenIter: %d\nlenRec:  %d\n", len1, len2);
	
	len1 = StrlenIter(str2);
	len2 = StrlenRec(str2);
	printf("lenIter: %d\nlenRec:  %d\n", len1, len2);
	
	
	/****** STRCMP ******/
	
	cmp1 = StrcmpIter( str1, str3 );
	cmp2 = StrcmpIter( str3, str1 );
	cmp3 = StrcmpIter( str2, str2 );
	printf("CmpIter: %d, %d, %d\n", cmp1, cmp2, cmp3);
	
	cmp1 = StrcmpRec( str1, str3 );
	cmp2 = StrcmpRec( str3, str1 );
	cmp3 = StrcmpRec( str2, str2 );
	printf("CmpRec:  %d, %d, %d\n", cmp1, cmp2, cmp3);
	
	
	/****** STRCPY ******/
	
	strPtr1 = StrcpyIter(str4, str1);
	StrcpyIter(str5, str2);
	printf("CpyIter: %s,  %s,  %s\n", str4, strPtr1, str5);
	
	strPtr2 = StrcpyRec(str6, str1);
	StrcpyRec(str7, str2);
	printf("CpyRec:  %s,  %s,  %s\n", str6, strPtr2, str7);
	
	StrcpyIter(str4, blank10);
	StrcpyIter(str5, blank10);
	StrcpyIter(str6, blank10);
	StrcpyIter(str7, blank10);
	
	/****** STRNCPY ******/
	
	strPtr1 = StrncpyIter(str4, str1, 7);
	StrncpyIter(str5, str2, 5);
	printf("nCpyIter: %s,  %s,  %s\n", str4, strPtr1, str5);
	
	strPtr2 = StrncpyRec(str6, str1, 7);
	StrncpyRec(str7, str2, 5);
	printf("nCpyRec:  %s,  %s,  %s\n", str6, strPtr2, str7);
	
	StrcpyIter(str4, blank10);
	StrcpyIter(str5, blank10);
	StrcpyIter(str6, blank10);
	StrcpyIter(str7, blank10);
		
	/****** STRCAT ******/
	
	strPtr1 = MyStrcat(str8, str2);
	printf("Strcat: %s, %s\n", str8, strPtr1);
	
	/****** STRSTR ******/
	
	strPtr1 = MyStrstr(str1, strfind);
	strPtr2 = MyStrstr(str2, strfind);
	printf("StrStr: %d, %c\n", (int)(strPtr1-str1), (strPtr2==NULL)?'+':'-');
	
	return 0;
}



#include <stdio.h>
#include <stdlib.h>

#include "MyString.h"

/*-------------------------------------------------------------------------*/
size_t MyStrlen(const char* _str){
	int i=0;
	
	while( _str[i] != '\0'){
		i++;
	}
	
	return i;
}

size_t MyStrlenRec(const char* _str){
	
	if(*_str == '\0'){
		return 0;
	}
	
	return (MyStrlenRec(++_str) + 1);
}

/*-------------------------------------------------------------------------*/

int MyStrcmp(const char* _str1, const char* _str2){
	
	while( *_str1 && *_str2  && ( *(_str1++) == *(_str2++) )   ) {} ;

	return (*_str1 - *_str2);
}

/*-------------------------------------------------------------------------*/

char* MyStrcpy(char* _dst, const char* _src){
	char* toRet =  _dst;

	while(  (*(_dst++) = *(_src++))    )  {} ;   
	
	*(_dst) = '\0';
	
	return toRet;
	
}

/*-------------------------------------------------------------------------*/

char* MyStrncpy(char* _dst, const char* _src, size_t _num){
	int i=0;
	char* toRet =  _dst;
	
	while(  ( (*(_dst++) = *(_src++) )  )   &&    i++ < _num ){};
	
	while( i++ < _num  ){    /*could have been done with memset instead*/
		*(_dst++) = '\0'; 
	}
	
	return toRet;
}

/*-------------------------------------------------------------------------*/

char* MyStrcat(char* _dst, const char* _src){
	char* toRet =  _dst;	
		
	while( !(*(_dst++)) );
	_dst--;
	
	while(   !(*(_dst++) = *(_src++))   ) ;

	return toRet;
}

/*-------------------------------------------------------------------------*/

char* MyStrstr(const char* _str, const char* _substr){
	char* startPtr = _str;
	char* subStrPtr = _substr;		

	if(!(*_str) ){
		if(!(*_substr)){
			return _str;
		}else{
			return NULL;
		}
	}

	while(  *subStrPtr && ( *(_str++) == *(subStrPtr++) )  );

	if( !(*subStrPtr) && ( *(_str - 1) == *(subStrPtr - 1) ) ){
		return startPtr;
	}else{
		return MyStrstr(++startPtr, _substr);
	}
	
}

/*-------------------------------------------------------------------------*/















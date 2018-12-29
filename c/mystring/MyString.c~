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

int MyStrcmp(const char* _str1, const char* _str2)
{	
	while( *_str1  &&   ( *_str1 == *_str2 )   ) 
	{
		++_str1; 
		++_str2; 
	}    

	return (*_str1 - *_str2);  
}


int MyStrcmpRec(const char* _str1, const char* _str2)
{	
	if( !(*_str1) || (*_str1 != *_str2)){
		return (*_str1 - *_str2);
	}
	
	return MyStrcmpRec(++_str1, ++_str2);
}

/*-------------------------------------------------------------------------*/

char* MyStrcpy(char* _dst, const char* _src){
	char* toRet =  _dst;

	while(  (*(_dst++) = *(_src++))    )  {} ;    /*copy from src to dst*/ 
	
	return toRet;	
}


char* MyStrcpyRec(char* _dst, const char* _src)
{
	char* toRet = _dst;
	
	if(!(*_src)){
		return _dst;
	}
	
	*_dst = *_src;
	MyStrcpyRec(++_dst, ++_src);
	
	return toRet;
}

/*-------------------------------------------------------------------------*/

char* MyStrncpy(char* _dst, const char* _src, size_t _num){
	int i=0;
	char* toRet =  _dst;
	
	while(  ( (*(_dst++) = *(_src++) )  )   &&    i < _num ){
		++i;
	};    
	
	while( i++ < _num  ){    /*could have been done with memset instead*/
		*(_dst++) = '\0';    /*padding with '\0'*/
	}	

	return toRet;
}


char* MyStrncpyRec(char* _dst, const char* _src, size_t _num)
{
	char* toRet =  _dst;

	if( _num == 0 ){
		return toRet;
	}

	*_dst = *_src;		

	if (!(*_src)){
		MyStrncpyRec(++_dst, _src , --_num);
	}else{
		MyStrncpyRec(++_dst, ++_src , --_num);
	}
	 
	
	return toRet;
}

/*-------------------------------------------------------------------------*/

char* MyStrcat(char* _dst, const char* _src){
	char* toRet =  _dst;	
		
	while( (*(_dst++)) );  /*to the end of dst*/
	_dst--;
	
	while(   (*(_dst++) = *(_src++))   ) ;  /*append src to dst*/ 

	return toRet;
}

/*-------------------------------------------------------------------------*/

char* MyStrstrImp(const char* _str, const char* _subStr){
	const char* startPtr = _str;
	const char* subStrPtr = _subStr;		

	while(*startPtr){
		_str = startPtr ;
		_subStr = subStrPtr;
		while(*_subStr && (*_str++ == *_subStr++));
	
		if( (!(*_subStr))  && ( *(_str - 1) == *(_subStr - 1) )   ){
			return (char*)startPtr;
		}
		++startPtr;
	}

	return NULL;
}

char* MyStrstr(const char* _str, const char* _subStr){
	if(!(*_str) && !(*_subStr)){
		return "";
	}else if( !(*_str)){
		return NULL;
	}else if(!(*_subStr)){
		return (char*)_str;		
	}
	
	return MyStrstrImp(_str, _subStr);
}


char* MyStrstrRecImp(const char* _str, const char* _substr){
	const char* startPtr = _str;
	const char* subStrPtr = _substr;		

	if(!(*_str) ){   	  /*rec stop condition*/
		return NULL;
	}

	while(  *subStrPtr && ( *(_str++) == *(subStrPtr++) )  );

	if( !(*subStrPtr) && ( *(_str - 1) == *(subStrPtr - 1) ) ){   /*if substr finished and equal*/ 
		return (char*)startPtr;
	}else{
		return MyStrstrRecImp(++startPtr, _substr);    /*rec check from next position*/
	}
	
}


char* MyStrstrRec(const char* _str, const char* _substr){
	if(!(*_str) && !(*_substr)){
		return "";
	}else if( !(*_str)){
		return NULL;
	}else if(!(*_substr)){
		return (char*)_str;		
	}
	
	return (MyStrstrRecImp(_str, _substr)); /*recursive call*/

}
/*-------------------------------------------------------------------------*/















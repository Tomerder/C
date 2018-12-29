/**************************************************************************************
    Author : Tal Rais
    Creation date :      24.12.13
    Date last modified : 24.12.13
    Description : string.h self implementation.
***************************************************************************************/
#include "MyStringRec.h"

size_t MyStrlen(const char* _str)
{
	return *_str? MyStrlen(_str+1)+1: 0;
}

int MyStrcmp(const char* _str1, const char* _str2)
{
	return (*_str1 != *_str2 || !*_str1)? *_str1 - *_str2: MyStrcmp(_str1+1, _str2+1);
}

char* MyStrcpy(char* _destination, const char* _source)
{
	*_destination = *_source;
	if (*_source) {
		MyStrcpy(_destination+1, _source+1);
	}

	return _destination;
}

char* MyStrncpy(char* _destination, const char* _source, size_t _num)
{
	if (_num == 0) {
		return _destination;
	}

	*_destination = *_source;
	MyStrncpy(_destination+1, *_source? _source+1: _source, _num-1);

	return _destination;
}



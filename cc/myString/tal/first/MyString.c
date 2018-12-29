/**************************************************************************************
    Author : Tal Rais
    Creation date :      24.12.13
    Date last modified : 24.12.13
    Description : string.h self implementation.
***************************************************************************************/
#include "MyString.h"

size_t MyStrlen(const char* _str)
{
	size_t len;

	for (len = 0; *(_str++); ++len);

	return len;
}

int MyStrcmp(const char* _str1, const char* _str2)
{
	while (*(_str1++) == *(_str2++) && *_str1 && *_str2);

	return *(_str1-1) - *(_str2-1);
}

char* MyStrcpy(char* _destination, const char* _source)
{
	char* retval = _destination;

	while ((*(_destination++) = *(_source++)));
	
	return retval;
}

char* MyStrncpy(char* _destination, const char* _source, size_t _num)
{
	char* retval = _destination;

	while (_num-- > 0) {
		_source += ((*(_destination++) = *_source))? 1: 0;
	}

	return retval;
}

char* MyStrcat(char* _destination, const char* _source)
{
	char* retval = _destination;

	while (*_destination && ++_destination);
	
	while ((*(_destination++) = *(_source++)));

	return retval;
}

char* MyStrstr(const char* _str, const char* _substr)
{
	const char *occurrence = _str, *substring;

	do {
		substring = _substr;
		while (*occurrence && *(occurrence++) == *substring && *(substring++));
	
	} while (*substring && *occurrence && (occurrence = ++_str));
	
	return (*substring)? 0: (char*)_str;
}


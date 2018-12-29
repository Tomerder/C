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

	for (len = 0; *_str; ++len) {
		++_str;
	}

	return len;
}

int MyStrcmp(const char* _str1, const char* _str2)
{
	while (*_str1 == *_str2 && *_str1) {
		++_str1;
		++_str2;
	}

	return *_str1 - *_str2;
}

char* MyStrcpy(char* _destination, const char* _source)
{
	char* retval = _destination;
	
	while ((*_destination = *_source)) {
		++_destination;
		++_source;
	}

	return retval;
}

char* MyStrncpy(char* _destination, const char* _source, size_t _num)
{
	char* retval = _destination;

	while (_num > 0) {
		*_destination = *_source;
		_source += (*_source)? 1: 0;
		++_destination;
		--_num;
	}

	return retval;
}

char* MyStrcat(char* _destination, const char* _source)
{
	char* retval = _destination;

	if (*_destination) {
		while (*++_destination) {}
	}
	
	while ((*_destination = *_source)) {
		++_destination;
		++_source;
	}

	return retval;
}

char* MyStrstr(const char* _str, const char* _substr)
{
	const char *str_pos, *substr_pos;

	do {
		str_pos = _str;
		substr_pos = _substr;
		while (*str_pos && *str_pos == *substr_pos) {
			++str_pos;
			++substr_pos;
		}
		
	} while (*substr_pos && *str_pos && (str_pos = ++_str));	/* ++_str is conditioned by *str_pos. */
	
	return (*substr_pos)? 0: (char*) _str;
}




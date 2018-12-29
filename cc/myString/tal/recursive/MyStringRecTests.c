/**************************************************************************************
    Author : Tal Rais
    Creation date :      24.12.13
    Date last modified : 24.12.13
    Description : MyString.h unit tests.
***************************************************************************************/
#include <stdio.h>
#include <string.h>

#include "MyStringRec.h"

#define SIGN(X) (((X)>0)? 1: (((X)<0)? -1: 0))

int MyStrlenTest(const char* _input)
{
	size_t output, expected;
	
	output = MyStrlen(_input);
	expected = strlen(_input);

	if (output != expected) {
		printf("MyStrlen Test FAILED:\n\tMyStrlen(\"%s\") = %u, and not %u as expected.\n\n", _input, output, expected);
		return 0;
	}
	
	return 1;
}

int MyStrcmpTest(const char* _str1, const char* _str2)
{
	int output, expected;
	
	output = SIGN(MyStrcmp(_str1, _str2));
	expected = SIGN(strcmp(_str1, _str2));

	if (output != expected) {
		printf("MyStrcmp Test FAILED:\n\tSIGN(MyStrcmp(\"%s\", \"%s\")) = %d, and not %d as expected.\n\n", _str1, _str2, output, expected);
		return 0;
	}
	
	return 1;
}

int MyStrcpyTest(char* _destination, const char* _source)
{
	int pass = 1;
	char* output;

	output = MyStrcpy(_destination, _source);
	
	if (output != _destination) {
		printf("MyStrcpy Test FAILED:\n\tThe return value points to %p instead of %p.\n\n", (void*)output, (void*)_destination);
		pass = 0;
	} else if (strcmp(_destination, _source) != 0) {
		printf("MyStrcpy Test FAILED:\n\tThe destination is \"%s\" instead of \"%s\".\n\n", _destination, _source);
		pass = 0;
	}
	
	return pass;
}

int MyStrncpyTest(char* _destination, const char* _source, size_t _num)
{
	char* output;
	char* expected;
	int pass = 1;

	expected = (char*)calloc(strlen(_destination)+_num+1, sizeof(char));
	strcpy(expected, _destination);
	strncpy(expected, _source, _num);
	
	output = MyStrncpy(_destination, _source, _num);
	
	if (output != _destination) {
		printf("MyStrncpy Test FAILED:\n\tThe return value points to %p instead of %p.\n\n", (void*)output, (void*)_destination);
		pass = 0;
	} else if (strcmp(_destination, expected) != 0) {
		printf("MyStrncpy Test FAILED:\n\tThe destination is \"%s\" instead of \"%s\".\n\n", _destination, expected);
		pass = 0;
	}

	return pass;
}

void RunMyStringTests(void)
{
	int i, j, n=5;
	const char* strings[] = { "Hello world!", "o world!", " How goes it?", "o .", "" }; 
	char str[32];
	
	for (i = 0; i < n; ++i)
	{
		MyStrlenTest(strings[i]);
	}
	
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			MyStrcmpTest(strings[i], strings[j]);
		}
	}

	for (i = 0; i < n; ++i)
	{
		MyStrcpyTest(str, strings[i]);
	}
	
	for (i = 0; i < n; ++i)
	{
		strncpy(str, strings[(i+1)%n], 32);
		MyStrncpyTest(str, strings[i], i);
	}
}

int main(void)
{
	RunMyStringTests();
	return 0;
}


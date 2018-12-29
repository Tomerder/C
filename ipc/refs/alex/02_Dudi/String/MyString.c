/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-08-01
    Date last modified :	2013-08-01
    Description : implementation of some string.h functions
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
/*
#include <string.h>
*/

#include "MyString.h"




/*----------------------------------------------------------*/
/*							STRLEN							*/
/*----------------------------------------------------------*/

/*
Returns the length of the C string str.

The length of a C string is determined by the terminating null-character: A C
string is as long as the number of characters between the beginning of the
string and the terminating null character (without including the terminating
null character itself).

This should not be confused with the size of the array that holds the string.
For example:

char mystr[100]="test string";

defines an array of characters with a size of 100 chars, but the C string with
which mystr has been initialized has a length of only 11 characters. Therefore,
while sizeof(mystr) evaluates to 100, strlen(mystr) returns 11.

Parameters

str
    C string.

Return Value
The length of string.
*/


size_t StrlenIter ( const char * str )
{
	size_t len;
	
	for( len=0; str[len] != '\0'; ++len );
	return len;
}
/*----------------------------------------------------------*/

size_t StrlenRec  ( const char * str )
{

	if( str[0] == '\0' )
	{
		return 0;
	}
	
	return StrlenRec( str+1 ) + 1;
}
/*##########################################################*/


/*----------------------------------------------------------*/
/*							STRCMP							*/
/*----------------------------------------------------------*/
/*
 Compares the C string str1 to the C string str2.

This function starts comparing the first character of each string. If they are
equal to each other, it continues with the following pairs until the characters
differ or until a terminating null-character is reached.

This function performs a binary comparison of the characters. For a function
that takes into account locale-specific rules, see strcoll.

Parameters

str1
    C string to be compared.
str2
    C string to be compared.


Return Value
Returns an integral value indicating the relationship between the strings:
A zero value indicates that both strings are equal.
A value greater than zero indicates that the first character that does not match
has a greater value in str1 than in str2; And a value less than zero indicates
the opposite.
*/


int StrcmpIter ( const char * str1, const char * str2 )
{
	int i;

	for( i=0; str1[i] - str2[i] == 0 && str1[i] != '\0' && str2[i] != '\0'; ++i);
	
	return 	str1[i] - str2[i];
}
/*----------------------------------------------------------*/

int StrcmpRec  ( const char * str1, const char * str2 )
{
	if( str1[0]-str2[0] != 0 || str1[0] == '\0' || str2[0] == '\0')
	{
		return str1[0] - str2[0];
	}
	
	return StrcmpRec( str1+1, str2+1);
}
/*##########################################################*/



/*----------------------------------------------------------*/
/*							STRCPY							*/
/*----------------------------------------------------------*/
/*
 Copies the C string pointed by source into the array pointed by destination,
 including the terminating null character (and stopping at that point).

To avoid overflows, the size of the array pointed by destination shall be long
enough to contain the same C string as source (including the terminating null
character), and should not overlap in memory with source.

Parameters

destination
    Pointer to the destination array where the content is to be copied.
source
    C string to be copied.

Return Value
destination is returned.

*/


char * StrcpyIter ( char * destination, const char * source )
{
	int i;
	
	for( i=0; source[i] != '\0'; ++i )
	{
		destination[i] = source[i];
	}
	destination[i] = '\0';
	
	return destination;
}
/*----------------------------------------------------------*/

char * StrcpyRec  ( char * destination, const char * source )
{
	if( source[0] == '\0')
	{
		destination[0] = '\0';
		return destination;
	}
	
	StrcpyRec( destination+1, source+1 );
	
	destination[0] = source[0];
	return destination;
}
/*##########################################################*/



/*----------------------------------------------------------*/
/*							STRNCPY							*/
/*----------------------------------------------------------*/
/*
 Copies the first num characters of source to destination. If the end of the
 source C string (which is signaled by a null-character) is found before num
 characters have been copied, destination is padded with zeros until a total
 of num characters have been written to it.

No null-character is implicitly appended at the end of destination if source
is longer than num. Thus, in this case, destination shall not be considered a
null terminated C string (reading it as such would overflow).

destination and source shall not overlap (see memmove for a safer alternative
when overlapping).

Parameters

destination
    Pointer to the destination array where the content is to be copied.
source
    C string to be copied.
num
    Maximum number of characters to be copied from source.
    size_t is an unsigned integral type.

Return Value
destination is returned.
*/


char * StrncpyIter ( char * destination, const char * source, size_t num )
{
	int i;
	
	for( i=0; source[i] != '\0' && i < num ; ++i )
	{
		destination[i] = source[i];
	}
	for( ; i<num; ++i)
	{
		destination[i] = '\0';
	}
	
	return destination;
}
/*----------------------------------------------------------*/

char * StrncpyRec  ( char * destination, const char * source, size_t num )
{
	if( num == 0 ) {
		return destination;
	}
	
	if( source[0] == '\0' )
	{
		StrncpyRec( destination+1, "\0", num-1 );
		
	} else {
		StrncpyRec( destination+1, source+1, num-1 );
	}
	
	destination[0] = source[0];
	return destination;
}
/*##########################################################*/



/*----------------------------------------------------------*/
/*							STRCAT							*/
/*----------------------------------------------------------*/
/*
 Appends a copy of the source string to the destination string. The terminating
 null character in destination is overwritten by the first character of source,
 and a null-character is included at the end of the new string formed by the
 concatenation of both in destination.

destination and source shall not overlap.

Parameters

destination
    Pointer to the destination array, which should contain a C string, and be
    large enough to contain the concatenated resulting string.
source
    C string to be appended. This should not overlap destination.

Return Value
destination is returned.
*/

char * MyStrcat ( char * destination, const char * source )
{
	size_t len = StrlenIter(destination);
	int i;
	
	for( i = 0; source[i] != '\0'; ++i )
	{
		destination[i+len] = source[i];
	}
	
	return destination;
}
/*##########################################################*/



/*----------------------------------------------------------*/
/*							STRSTR							*/
/*----------------------------------------------------------*/
/*
 Returns a pointer to the first occurrence of str2 in str1, or a null pointer
 if str2 is not part of str1.

The matching process does not include the terminating null-characters, but it
stops there.

Parameters

str1
    C string to be scanned.
str2
    C string containing the sequence of characters to match.

Return Value
A pointer to the first occurrence in str1 of the entire sequence of characters
specified in str2, or a null pointer if the sequence is not present in str1.
*/

char * MyStrstr ( const char *str1, const char *str2 )
{
	size_t len1 = StrlenIter(str1);
	size_t len2 = StrlenIter(str2);
	int i, j=0, place=-1;
	
	for(i=0; i <= len1-len2; ++i)
	{
		if(str1[i] == str2[0])
		{
			place = i;
			for(j=1; j<len2; ++j)
			{
				if( str2[j] != str1[place+j] )
				{
					place = -1;
					break;
				}
			} /* end for j */
		}
	} /* end for i */
	
	return ( place != -1 ) ? &str1[place] : NULL;
}
/*##########################################################*/



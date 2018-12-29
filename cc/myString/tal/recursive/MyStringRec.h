/**************************************************************************************
    Author : Dudi Kahana
    Creation date :      30.8.10
    Date last modified : 24.12.13	Tal Rais
    Description : string.h self implementation.
***************************************************************************************/
#ifndef __STR_LIB_H__
#define __STR_LIB_H__

#include <stdlib.h>

/**************************************************************************
MyStrlen() calculates the length of the string str, excluding the
terminating null byte ('\0').
**************************************************************************/
size_t MyStrlen(const char* _str);

/**************************************************************************
 MyStrcmp() compares the string "s1" to the string "s2".
 Both strings must be terminated by the usual '\0' character.
 Gives the results of the comparison:
     "i" is zero if the strings are identical.
     "i" is positive if string "s1" is greater than string "s2",
     "i" is negative if string "s2" is greater than string "s1".
**************************************************************************/
int     MyStrcmp(const char* _str1,   const char* _str2);

/**************************************************************************
 MyStrcpy() copies the string source into the area pointed to by destination.
 This area must be big enough to hold the contents of source.
 Since strcpy() moves the '\0' at the end of source, the new string
 destination will have the usual terminating '\0'.
**************************************************************************/
char*   MyStrcpy(char*    _destination, const char* _source);

/**************************************************************************
 MyStrncpy() copies at most N characters from the string source into the
 area of memory pointed to by destination.
 If it encounters a '\0' before it has copied N characters,
 it pads destination to N characters by adding '\0' characters.
 If source is more than N-1 characters long, the first N characters
 will be copied and the string destination will NOT receive the usual
 terminating '\0'.
**************************************************************************/
char*   MyStrncpy(char*   _destination, const char* _source, size_t _num);

#endif /*__STR_LIB_H__*/

/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-08-01
    Date last modified :	2013-08-01
    Description : implementation of some string.h functions
***************************************************************************************/

#ifndef __MYSTRING_H__
#define __MYSTRING_H__

size_t StrlenIter ( const char * str );
size_t StrlenRec  ( const char * str );

int StrcmpIter ( const char * str1, const char * str2 );
int StrcmpRec  ( const char * str1, const char * str2 );

char * StrcpyIter ( char * destination, const char * source );
char * StrcpyRec  ( char * destination, const char * source );

char * StrncpyIter ( char * destination, const char * source, size_t num );
char * StrncpyRec  ( char * destination, const char * source, size_t num );

char * MyStrcat ( char * destination, const char * source );

char * MyStrstr ( const char *str1, const char *str2 );

#endif  /* __MYSTRING_H__ */


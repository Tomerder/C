/**************************************************************************************
    Author: Liron Sherman
    Creation date:  		1.8.13    
    Last modified date:		1.8.13
    Description: String handling functions.
***************************************************************************************/
#include <stdio.h>
#include <string.h>

size_t StrlenItr ( const char * _str )
{
	register char *s = _str;
	
	while(*s) 
	{
		++s;
	}
	
	return (s - _str);
} 

size_t StrlenRec ( const char * _str )
{	
	return (*_str++) ? 1+StrlenRec(_str) : 0;
}

int StrcmpItr ( const char *_str1, const char *_str2 )
{
	while (*_str1 == *_str2 && *_str1)
	{
		++_str1;
		++_str2;
	}
	
	return *_str1 - *_str2;
}

int StrcmpRec ( const char *_str1, const char *_str2 )
{
	if (*_str1 != *_str2 || !*_str1)
	{
		return *_str1 - *_str2;
	}
	
	return StrcmpRec(++_str1,++_str2);
}

char * StrcpyItr ( char * _destination, const char * _source )
{
	char * d = _destination;

	while ((*d++ = *_source++)) ;

	return 	_destination;
}

char * StrcpyRec ( char * _destination, const char * _source )
{
	return ((*_destination = *_source)) ?
				 StrcpyRec(++_destination,++_source) :
				 _destination;
}

char * StrncpyItr ( char * _destination, const char * _source, size_t _num )
{
	int i,j;
	for (i=0;i<_num && *_source;++i)
	{
		*_destination = *_source;
		++_destination;
		++_source;
	}
	for (j=i;j<_num;++j)
	{
		*_destination = 0;
		++_destination;
	}
	*_destination = 0;
	
	return 	_destination;
}

char * StrncpyRec ( char * _destination, const char * _source, size_t _num )
{

	*_destination = *_source;
	if (!_num)
	{
		return _destination;
	}

	return StrncpyRec(++_destination, (*_source) ? ++_source : _source, --_num) - 1;
}

char * Mystrcat ( char * _destination, const char * _source )
{
	int len = StrlenItr(_destination);	
	while (*_source)
	{
		_destination[len] = *_source;
		++_source;
		++len;
	}
	_destination[len] = 0;
	return _destination;	
}

const char * Mystrstr ( const char * _str1, const char * _str2 )
{
	int len = StrlenItr(_str2);	
	const char* tempStr2 = _str2;
	
	if (!*_str2)
	{
		return NULL;
	}
	
	while(*_str1 && *_str2 && *(_str1+len) != 0 )
	{
		if (*_str1 == *_str2)
		{
			++_str1;
			++_str2;
		}
		else
		{
			_str2 = tempStr2;
			++_str1;
		}
	}
	if (!*_str2)
	{
		return _str1-len;
	}
	
	return NULL;
}
int main()
{
	char string1[30] = "dfdfgfgfgfg";
	char string2[30] = "gfgfgfgfg";
	
	printf("%d \n",StrlenItr(string1));
	
	printf("%d \n",StrlenRec(string1));
	
	printf("%d \n",StrcmpItr(string1,string2));

	printf("%d \n",StrcmpRec(string1,string2));
	/*	
	StrcpyItr(string1,string2);
	printf("%s \n",string1);
	
	StrcpyRec(string1,string2);
	printf("%s \n",string1);
	

	StrncpyItr(string1,string2,3);
	printf("%s \n",string1);
	
	StrncpyRec(string1,string2,10);
	printf("%s \n",string1);
	
	Mystrcat(string1,string2);
	printf("%s \n",string1);
*/	
	printf("%s \n",Mystrstr(string1,string2));
	
	return 1;
}
	

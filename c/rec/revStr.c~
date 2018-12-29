#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 10

void RevStr(char* _str, int _beginInd, int _endInd);
void Swap(char* _c1, char* _c2);

/*--------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	char str[LEN] = "abcdefg";

	printf("str = %s\n", str);

	RevStr(str, 0, strlen(str) - 1);

	printf("rev str = %s\n", str);

	return 0;
}


/*--------------------------------------------------------------------*/

void RevStr(char* _str, int _beginInd, int _endInd)
{
	if( _beginInd >= _endInd){
		return;
	}

	/*swap*/
	Swap( &_str[_beginInd] , &_str[_endInd]  );

	/*rec*/
	RevStr(_str, ++_beginInd, --_endInd);
	
}

/*--------------------------------------------------------------------*/

void Swap(char* _c1, char* _c2)
{
	char tmp;

	tmp = *_c1;
	*_c1 = *_c2;
	*_c2 = tmp;	
}

/*--------------------------------------------------------------------*/

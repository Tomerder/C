/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-00-00    
    Last modified date:		2013-00-00
    Description: 
***************************************************************************************/

#include <stdio.h>
#include <string.h>

typedef union uni
{
	char m_buff[30];
	
	struct
	{
		unsigned m_a:2;
		unsigned m_b:6;
		unsigned m_c:3;
	} m_fields;
} uni;


int main()
{
	uni x, y;
	
	printf("size of uni: %d\n", sizeof(uni) );
	
	x.m_fields.m_a=2;
	x.m_fields.m_b=16;
	x.m_fields.m_c=0;
	
	strcpy(y.m_buff, x.m_buff);

	printf("buffer: %s\n", y.m_buff);
	printf("a: %d, b: %d, c: %d\n", y.m_fields.m_a, y.m_fields.m_b, y.m_fields.m_c);
	printf("buff[0] = %d\n", (int)y.m_buff[0] );
	
	return 0;
}

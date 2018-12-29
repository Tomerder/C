


#include <stdio.h>
#include <string.h>

#define STR_SIZE 50

void StrReverse(char* str);

int main()
{
	char string[STR_SIZE];

	printf("enter your string:\n");
	/*getchar();*/
	scanf("%s", string);

	StrReverse(string);

	printf("your string, reversed:\n%s\n", string);

	return 0;
}

void StrReverse(char* str)
{
	int len;
	char temp = str[0];

	len = strlen(str);

	if (len<=1) {
		return;
	}
	
	str[0] = str[len-1];
	str[len-1] = '\0';

	StrReverse( str+1 );
	str[len-1] = temp;
}








#include <stdio.h>
#include <string.h>

#define STR_SIZE 50

int IsPalindrome(char* str);

int main()
{
	char string[STR_SIZE];
	int isPal;

	printf("enter your string:\n");
	/*getchar();*/
	scanf("%s", string);

	isPal = IsPalindrome(string);
	
	switch(isPal) {
		case 1:
			printf("It's a palindrome!\n");
			break;
		default:
			printf("Not a palindrome\n");
			break;
	}

	return 0;
}


int IsPalindrome(char* str)
{
	int len;
	int isPal;
	char str_tmp[STR_SIZE], str_tmp2[STR_SIZE];

	strcpy(str_tmp, str);
	strcpy(str_tmp2, str_tmp);

	len = strlen(str_tmp);
	
	if(len < 2) {
		return 1;
	}
	
	str_tmp[len-1] = '\0';
	isPal = IsPalindrome( str_tmp+1 );
	
	if( isPal == 0 ) {
		return 0;
	} else if (str_tmp2[len-1]-str_tmp2[0] == 0) {
		str_tmp2[len-1] = '\0';
		return 1;
	} else {
		return 0;
	}
}


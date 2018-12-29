#include <string.h>




char* fDeborah(char* dest, char* src)
{
	int i=0;
	int len=0;
	
	len = strlen(dest);
	
	for(i=0; src[i]!='\0'; ++i)
	{
		dest[i+len] = src[i];
	}
	
	dest[i+len] = '\0';
	return dest;
}



int main()
{
	char str1[50] ="d";
	char str2[50] = "Deborah";
	
	str1 = fDeborah(str1, str2);
	
	return 0;
}



#include <stdio.h>
#include <string.h>
#include <errno.h>


int main()
{
	FILE* fp = fopen("stam.txt", "a+");
	if(!fp)
	{
		perror("fopen");
		return -1;
	}
	
	
	char buff[] = "This is my buff\n";
	
	const int bufflen = strlen(buff);
	
	int bytes_written = fwrite(buff, sizeof(char), bufflen + 1, fp);
	
	
	if( bytes_written != bufflen )
	{
		perror("fclose");
		return -2;
	}
	
	if (fp)
		fclose(fp);
	
	return 0;
}



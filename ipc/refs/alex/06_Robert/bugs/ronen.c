#include <stdlib.h>


int main()
{
	int i;
	char *arr, *tempArr, *back, *tempBack;
	
	arr = malloc(10*sizeof(char));
	if(!arr)
		return -1;
	
	tempArr = arr;
	back = arr;
	back = back-5;
	tempBack = back;
	
	for(i=0; i<14; ++i, ++arr)
	{
		*arr = 'a';
	}
	
	free(tempArr);
	
	for(i=0; i<10; ++back, ++i)
	{
		*back = 'b';
	}
	
	return 0;
}



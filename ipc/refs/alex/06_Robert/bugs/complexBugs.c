/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-08-15    
    Last modified date:		2013-08-15
    Description: causing complicated bugs
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>


double* createDoubleArr(int _size)
{
	double *arr;
	
	arr = calloc(_size, sizeof(double*));
	if(!arr)
		return NULL;
	
	return arr;
}



int main()
{
	double *array;
	int size = 5;
	int i;
	
	double seventh = 1.0/7;
	
	array = createDoubleArr(size);
	if(!array)
	{
		fprintf(stderr, "Error: calloc\n");
		return 1;
	}
	
	for(i=0; i<size; ++i)
	{
		array[i] = (double)i + seventh;
	}
	
	printf("\n");
	for(i=0; i<size; ++i)
	{
		printf("%.12f ", array[i]);
	}
	printf("\n\n");
	
	free(array);
	
	return 0;
}


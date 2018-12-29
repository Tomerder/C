

#include <stdio.h>
#include <math.h>

int MaxArray(int* array, int n);

int main()
{
	int N=8;
	int array[8]={20, 8, 4, 0, 12, -10, 15, 4};
	int max;
	
	max = MaxArray(array, N);

	printf("max = %d\n", max);
	
	
	
	
	return 0;
}


int MaxArray(int* array, int n)
{
	int _max;	

	if (n<3) {
		return fmax( array[0], array[1] );
	}

	if( fmax( array[0], array[n-1] ) == array[0] ) {
		_max = MaxArray ( array, n-1 );
	} else {
		_max = MaxArray ( array+1, n-1 );
	}
	
	return _max;
}



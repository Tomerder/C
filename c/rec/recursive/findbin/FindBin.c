


#include <stdio.h>

int FindBin(int* array, int size, int what);

int main()
{
	int N=7;
	int array[7] = {-2,1,5,11,13,14,20};
	int what;
	int isThere;

	printf("enter the number you want to find: ");
	scanf("%d", &what);

	isThere = FindBin( array, N, what);

	if(isThere) {
		printf("it's there!\n");
	} else {
		printf("it's not there\n");
	}

	return 0;
}


/* returns 1 if "what" is part of the array, 0 if not */
int FindBin(int* array, int size, int what)	
{
	if (size <= 1) {
		return ( what == array[0] );
	}

	if ( what == array[ size/2 ] ) {
		return 1;
	} else if ( what > array[ size/2 ] ) {
		return FindBin( array+size/2+1, (size-1)/2, what );
	} else {
		return FindBin( array, size/2, what );
	}
	
}


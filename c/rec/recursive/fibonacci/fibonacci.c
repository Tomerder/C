
#include <stdlib.h>
#include <stdio.h>


void FibIter(int _n);
int FibRecur(int _n);


int main()
{
	int N=15;

	int num;

	FibIter(N);

	num = FibRecur(N);
	printf("f(%d) = %d\n\n", N, num);
	

	return 0;
}


void FibIter(int _n)
{
	int i;
	int a=0, b=1, sum;

	printf("\n");
	
	for (i=0; i<_n; i++) {
		
		printf("%d ", b);

		sum = a+b;
		a = b;
		b = sum;
	}

	printf("\n\n");
}


int FibRecur(int _n)
{	
	if ( _n<1 ) {
		return 0;
	} else if (_n==1) {
		return 1;
	}
		return ( FibRecur(_n-1) + FibRecur(_n-2) );
}

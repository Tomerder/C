/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-17    
    Last modified date:		2013-06-17
    Description: solve Hanoi Towers recursively.
***************************************************************************************/

#include <stdio.h>

void HanoiTowers(int _n, char _from, char _to, char _via);

int main()
{
	int N;
	char a='A', b='B', c='C';

	printf("Enter the number: ");
	scanf("%d", &N);
	printf("Steps to solve Towers of Hanoi:\n");

	HanoiTowers(N, a, b, c);

	return 0;
}


void HanoiTowers(int _n, char _from, char _to, char _via)
{
	if(!_n) {
		return;
	}

	HanoiTowers(_n-1, _from, _via, _to );			/* step in until n=0, start stepping out */
	printf("#%d from %c to %c\n", _n, _from, _to);	/* print move */
	HanoiTowers(_n-1, _via, _to, _from );			/* on the way out, step in while printing moves */
}


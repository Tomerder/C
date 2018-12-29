/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-17    
    Last modified date:		2013-06-17
    Description: put 8 queens on chess board so that none threatens the others.
***************************************************************************************/

#include <stdio.h>

#define BOARD_SIZE 5
#define ARR_SIZE BOARD_SIZE+1
#define NUM_QUEENS 2


void PrintBoard(int _board[ARR_SIZE][ARR_SIZE])
{
	int i, j;

	printf("\n");
	for(i=1; i<ARR_SIZE; ++i) {
		for(j=1; j<ARR_SIZE; ++j) {
			printf("%3d ", _board[i][j]);
			
		}
		printf("\n\n");
	}
}

/* given a queen at row _r, column _c, change all threatend squares from 0 to 1 */
/* queen is -1 */
void Threat(int _board[ARR_SIZE][ARR_SIZE], int _r, int _c)
{
	int i, j;

	for(i=1; i<ARR_SIZE; i++) {
		for(j=1; j<ARR_SIZE; j++) {
			if ( i==_r || j==_c || i+j == _r+_c || _r-i==_c-j ) {
				if (i==_r && j==_c) {
					continue;
				}
				_board[i][j] = 1;
			}
		}
	}
}



/* insert queen at coordinates, change value to -1, change threatened square to 1 */
void InsertQueen(int _board[ARR_SIZE][ARR_SIZE], int _r, int _c)
{
	_board[_r][_c] = -1;
	Threat(_board, _r, _c);
}

/* fine next unthreatend place for a queen. return 1 if for success, 0 for failure */
int FindPlace(int _board[ARR_SIZE][ARR_SIZE], int *_r, int *_c)
{
	int i, j;

	for(i=1; i<ARR_SIZE; ++i) {
		for(j=1; j<ARR_SIZE; ++j) {
			if(_board[i][j]==0) {
				*_r=i;
				*_c=j;
				return 1;
			}
		}
	}
	return 0;
}


/* recursive function for solving the 8-queen problem. _n - number of queens */
void Queens(int _board[ARR_SIZE][ARR_SIZE], int _n)
{
	int r, c;	

	if(!_n) {
		return;
	}
	
	
	
	Queens(_board,  _n-1)
	
}




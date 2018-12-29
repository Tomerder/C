/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-17    
    Last modified date:		2013-06-17
    Description: put 8 queens on chess board so that none threatens the others.
***************************************************************************************/

#include <stdio.h>
#include <string.h>

#define BOARD_SIZE 4
#define ARR_SIZE BOARD_SIZE+1

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

/* populate board with the queens using the coordinates in the solution array,
and mark the threatened squares using the Threat function called by InsertQueen.
solution[0][0] contains the cuurent # of queens */

void Populate(int _board[ARR_SIZE][ARR_SIZE] ,int _solution[2][ARR_SIZE])
{
	int i;

	for(i=1; i<=_solution[0][0]; ++i) {
		InsertQueen(_board, _solution[0][i], _solution[1][i]);
	}
}


/* fine next unthreatend place for a queen. return 1 if for success, 0 for failure */
/* repeat is 1 if we went back to find another solution, 0 otherwise. */
int FindPlace(int _board[ARR_SIZE][ARR_SIZE], int _solution[2][ARR_SIZE], int *_repeat)
{
	int i, j, k;
	int *n;

	n = &_solution[0][0];

	for( i = _solution[0][*n+*_repeat] + (*n==0) , k=0; i<ARR_SIZE ; ++i , ++k) {
		for( j = (!k) ? ( (_solution[1][*n+*_repeat] + *_repeat) % ARR_SIZE ) : 1 ; j<ARR_SIZE ; ++j ) {

			if( 0==j ) {
				i++;
				j++;
			}
			if ( 0 == _board[i][j] ) {

				_solution[0][0] ++;
				_solution[0][*n] = i;	/* n was increased in previous line */
				_solution[1][*n] = j;
				return 1;
			}
		}
	}
	return 0;
}


void ResetBoard(int _board[ARR_SIZE][ARR_SIZE])
{
	int i,j;

	for(i=1; i<ARR_SIZE; ++i) {
		for(j=1; j<ARR_SIZE; ++j) {
			_board[i][j]=0;
		}
	}
}

/* recursive function for solving the 8-queen problem. _n - number of queens */
void Queens(int _board[ARR_SIZE][ARR_SIZE], int _n, int _solution[2][ARR_SIZE])
{
	int repeat;

	if(!_n) {
		return;
	}
	
	if( _solution[0][2]==0 ) {
		repeat=0;
	}

	Queens(_board,  _n-1, _solution);

	if( 1 == _n ) {
		FindPlace(_board, _solution, &repeat);
	}

	Populate(_board, _solution);
	PrintBoard(_board);
							/* !!! find a way to keep value of repeat across recursions */
	repeat = ( !FindPlace(_board, _solution, &repeat) );

	if( repeat ) {
		_solution[0][0] -= 2;
		ResetBoard(_board);
		Populate(_board, _solution);

	}
	
}




/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-17    
    Last modified date:		2013-06-17
    Description: put 8 queens on chess board so that none threatens the others.
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#define BOARD_SIZE 8
#define ARR_SIZE BOARD_SIZE+1
#define NUM_QUEENS 2

void PrintBoard(int _board[ARR_SIZE][ARR_SIZE]);
void Threat(int _board[ARR_SIZE][ARR_SIZE], int _r, int _c);
int FindPlace(int _board[ARR_SIZE][ARR_SIZE], int *_r, int *_c);


int main()
{

	int r=1, c=1;
	int board[ARR_SIZE][ARR_SIZE]={0};
																			/* create board: board[i][j] is the square at row i, column j */
/*	int **board = (int **) malloc( sizeof(int*) * arrSize );				

	for(i=1; i<arrSize; ++i) {
		board[i] = (int *) calloc(arrSize, sizeof(int));
	}
*/

	board[1][1] = -1;
	Threat(board, 1,1);

	PrintBoard(board);

	FindPlace(board, &r, &c);

	printf("r=%d, c=%d\n", r, c);

	board[r][c] = -1;
	Threat(board, r,c);
	PrintBoard(board);




	return 0;
}

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

	for(i=1; i<ARR_SIZE; ++i) {
		for(j=1; j<ARR_SIZE; ++j) {
			if ( i==_r || j==_c || i+j == _r+_c || _r-i==_c-j ) {
				if (i==_r && j==_c) {
					continue;
				}
				_board[i][j] = 1;
			}
		}
	}
}

/* fine next unthreatend place for a queen. return 1 if for success, 0 for failure */
int FindPlace(int _board[ARR_SIZE][ARR_SIZE], int *_r, int *_c)
{
	int i, j;

	for(j=*_c; j<ARR_SIZE; ++j) {
		for(i=*_r; i<ARR_SIZE; ++i) {
			if(_board[i][j]==0) {
				*_c=i;
				*_r=j;
				return 1;
			}
		}
	}
	return 0;
}













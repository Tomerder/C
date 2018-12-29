/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-17    
    Last modified date:		2013-06-17
    Description: put 8 queens on chess board so that none threatens the others.
***************************************************************************************/

#include <stdio.h>
#include <string.h>

#include "EightQueensFunc.h"

#define BOARD_SIZE 5
#define ARR_SIZE BOARD_SIZE+1


/*	int **board = (int **) malloc( sizeof(int*) * arrSize );				

	for(i=1; i<arrSize; ++i) {
		board[i] = (int *) calloc(arrSize, sizeof(int));
	}
*/

int main()
{

	int r=1, c=1;
	int board[ARR_SIZE][ARR_SIZE];								/* create board: board[i][j] is the square at row i, column j */
	int isPlace;

	memset ( board, 0, sizeof(board) );

	InsertQueen(board, 1, 1);
	PrintBoard(board);

	isPlace = FindPlace(board, &r, &c);
	if ( isPlace ) {
		printf("r=%d, c=%d\n", r, c);
		InsertQueen(board, r, c);
	}

	PrintBoard(board);

	isPlace = FindPlace(board, &r, &c);

	if ( isPlace ) {
		printf("r=%d, c=%d\n", r, c);
		InsertQueen(board, r, c);
	}

	PrintBoard(board);



	return 0;
}



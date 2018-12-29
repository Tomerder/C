/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-17    
    Last modified date:		2013-06-17
    Description: put N queens on an NxN chess board so that none threatens the others.
		method: store locations in an array, if there isn't place for all queens, delete 
		the last location and try again.
***************************************************************************************/

#include <stdio.h>
#include <string.h>

#include "EightQueensFunc.h"

#define BOARD_SIZE 4
#define ARR_SIZE BOARD_SIZE+1

/* board[i][j] is the square at row i, column j.
solution[0][n], solution[1][n] are the coordinates i,j of the n-th queen. 
solution[0][0] contains # of queens cuurently in the solution array */


int main()
{
	int board[ARR_SIZE][ARR_SIZE];				
	int solution[2][ARR_SIZE];

	memset ( board, 0, sizeof(board) );
	memset ( solution, 0, sizeof(solution) );

	Queens(board, ARR_SIZE, solution);

	printf("  ____________\n\n");
	PrintBoard(board);

	return 0;
}



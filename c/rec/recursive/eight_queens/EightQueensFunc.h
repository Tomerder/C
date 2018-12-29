/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-17    
    Last modified date:		2013-06-17
    Description: put 8 queens on chess board so that none threatens the others.
***************************************************************************************/

#ifndef __QUEENS__
#define __QUEENS__

#define BOARD_SIZE 4
#define ARR_SIZE BOARD_SIZE+1

void PrintBoard(int _board[ARR_SIZE][ARR_SIZE]);
void Threat(int _board[ARR_SIZE][ARR_SIZE], int _r, int _c);
void InsertQueen(int _board[ARR_SIZE][ARR_SIZE], int _r, int _c);
int FindPlace(int _board[ARR_SIZE][ARR_SIZE] , int _solution[2][ARR_SIZE], int *_repeat);
void Populate(int _board[ARR_SIZE][ARR_SIZE] , int _solution[2][ARR_SIZE]);
void ResetBoard(int *_board[ARR_SIZE][ARR_SIZE]);

void Queens(int _board[ARR_SIZE][ARR_SIZE], int _n, int _solution[2][ARR_SIZE]);

#endif /* __QUEENS__ */

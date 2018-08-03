#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 25
#define NPLAYINGBOARD 9

/*
char board[] = 
{
	=,=,=,=,=,
	=,-,-,-,=,
	=,-,-,-,=,
	=,-,-,-,=,
	=,=,=,=,=,
}
*/

const char NOUGHTS = 'O';
const char CROSSES = 'X';
const char BOARD = ':';
const char EMPTY = '-';

const int PLAYERWIN = 0;
const int BOTWIN = 1;
const int DRAW = 2;

const int directions[] = { 1, 5, 4, 6 };


const char convertTo25[] =
{
	6,7,8,
	11,12,13,
	16,17,18
};

void printBoard(const char *board)
{
	int i;
	printf("Board: \n");
	for (i = 0; i < N; i++)
	{
		if (i % 5 == 0)
			printf("\n");

		printf("%4c", board[i]);
	}
	printf("\n");
}

int hasEmpty(const char *board)
{
	int i;
	for (i = 0; i < NPLAYINGBOARD; i++)
		if (board[convertTo25[i]] == EMPTY)
			return 1;
	return 0;
}

void makeMove(char *board, const int position, const char side)
{
	board[position] = side;
}

int getPlayerInput(const char *board)
{

	int input;
	int fineMove = 0;
	
	while (!fineMove)
	{
		printf("Enter a position from 1 to 9:");
		scanf_s("%d", &input);

		if (input < 1 || input > 9)
		{
			printf("Invalid range, try agane \n");
			continue;
		}

		if (board[convertTo25[input-1]] != EMPTY)
		{
			printf("Square is occupied, try agane \n");
			continue;
		}
		fineMove = 1;
	}
	return convertTo25[--input];
}

int getBotInput(const char *board)
{
	int i, free=0, availableMoves[9];

	for (i = 0; i < NPLAYINGBOARD; ++i)
		if (board[convertTo25[i]] == EMPTY)
			availableMoves[free++] = convertTo25[i];
	
	return availableMoves[rand() % free];
}

 int getNumberForDirection (int startingSquare, const int direction, 
							const char *board, const char turn)
{
	 int found = 0;

	 while (board[startingSquare] != BOARD)
	 {
		 if (board[startingSquare] != turn)
			 break;
		 found++;
		 startingSquare += direction;
	 }
	 return found;
}

 int checkWin(const char *board, const int i, const char turn)
 {
	 int directionIndex, direction, countOfThreeInARow = 1;
	 int win = 0;
	 for (directionIndex = 0; directionIndex < 4; ++directionIndex)
	 {
		 direction = directions[directionIndex];
		 countOfThreeInARow += getNumberForDirection(i + direction, direction, board, turn);
		 countOfThreeInARow += getNumberForDirection(i + direction * -1, direction * -1, board, turn);
		 if (countOfThreeInARow == 3)
		 {
			 win = 1;
			 break;
		 }			
		 countOfThreeInARow = 1;
	 }
	 return win;
 }

void game()
{
	int i, gameOver = 0, lastMove;
	char board[N], turn = NOUGHTS, checkWinSymbol;

	for (i = 0; i < N; i++)
		board[i] = BOARD;
	for (i = 0; i < NPLAYINGBOARD; i++)
		board[convertTo25[i]] = EMPTY;
	printBoard(board);

	while (!gameOver)
	{
		if (turn == NOUGHTS)
		{
			checkWinSymbol = NOUGHTS;
			lastMove = getPlayerInput(board);
			makeMove(board, lastMove, turn);
			turn = CROSSES;
		}
		else
		{
			checkWinSymbol = CROSSES;
			lastMove = getBotInput(board);
			makeMove(board, lastMove, turn);
			turn = NOUGHTS;
			printBoard(board);
		}

		if (checkWin(board, lastMove, checkWinSymbol))
		{
			printBoard(board);
			printf("Game Over!");
			gameOver = 1;
			if (checkWinSymbol == NOUGHTS)
				printf("Player Wins! \n");
			else
				printf("Bot Wins! \n");

		}

		if (!hasEmpty(board))
		{
			printf("DRAW! Game Over \n");
			gameOver = 1;
		}
	}
}

int main()
{
	srand(time(NULL));
	game();
	getchar();
	system("PAUSE");
	return 0;

}
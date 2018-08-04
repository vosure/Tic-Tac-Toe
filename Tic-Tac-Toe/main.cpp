#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PVP 1
#define PVB 2
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
enum { NOUGHTS , CROSSES, BOARD, EMPTY };

const int directions[] = { 1, 5, 4, 6 };

const int middleSquare = 4;
const int corners[] = {0, 2, 6, 8};

int ply = 0, positions = 0, maxPly = 0;


const char convertTo25[] =
{
	6,7,8,
	11,12,13,
	16,17,18
};

void printBoard(const char *board)
{
	int i;
	char characters[] = "OX=-";
	printf("Board: \n");
	for (i = 0; i < N; i++)
	{
		if (i % 5 == 0)
			printf("\n");

		printf("%4c", characters[board[i]]);
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

		if (board[convertTo25[input - 1]] != EMPTY)
		{
			printf("Square is occupied, try agane \n");
			continue;
		}
		fineMove = 1;
	}
	return convertTo25[--input];
}



int getNumberForDirection(int startingSquare, const int direction,
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

int checkWinAllBoard(const char *board, const char turn)
{
	int i, winFound = 0;

	for (i = 0; i < NPLAYINGBOARD; ++i)
	{
		if (board[convertTo25[i]] == turn)
			if (checkWin(board, convertTo25[i], turn))
			{
				winFound = 1;
				break;
			}
	}
	return winFound;
}

int evaluateWinForEachSides(const char *board, const char turn)
{
	if (checkWinAllBoard(board, turn) != 0)
		return 1;
	if (checkWinAllBoard(board, turn^1) != 0)
		return -1;

	return 0; // Draw!
}

int minMax(char *board, char turn)
{

	int moveList[9];
	int moveCount = 0;
	int score = -2;
	int bestScore = -2;
	int move;
	int bestMove = -1;
	int i;

	if (ply > maxPly)
		maxPly = ply;
	positions++;

	if (ply > 0)
	{
		score = evaluateWinForEachSides(board, turn);
		if (score != 0)
			return score;
	}

	for (i = 0; i < NPLAYINGBOARD; ++i)
		if (board[convertTo25[i]] == EMPTY)
			moveList[moveCount++] = convertTo25[i];
		
	for (i = 0; i < moveCount; ++i)
	{
		move = moveList[i];
		board[move] = turn;

		ply++;
		score = -minMax(board, turn^1);
		if (score > bestScore)
		{
			bestScore = score;
			bestMove = move;
		}
		board[move] = EMPTY;
		ply--;
	}

	if (!moveCount)
		bestScore = checkWinAllBoard(board, turn);

	if (ply != 0)
		return bestScore;
	else
		return bestMove;
}

int findWinningMove(char *board, const char turn)
{
	int i, move = 0, win=0;

	for (i = 0; i < NPLAYINGBOARD; ++i)
	{
		if (board[convertTo25[i]] == EMPTY)
		{
			move = convertTo25[i];
			board[move] = turn;

			if (checkWin(board, move, turn))
				win = 1;
			board[move] = EMPTY;
			if (win)
				break;
			move = 0;
		}
	}
	return move;
}

int findBestMove(const char *board)
{
	int move = convertTo25[middleSquare];
	if (board[move] == EMPTY)
		return move;
	
	int i;
	move = 0;
	for (i = 0; i < 4; i++)
	{
		move = convertTo25[corners[i]];
		if (board[move] == EMPTY)
			break;
		move = 0;
	}
	return move;
}

int getStupidBotMove(char *board, const char turn)
{
	int i, free = 0, availableMoves[9];
	
	int move = findWinningMove(board, turn);
	if (move != 0)
		return move;

	move = findWinningMove(board, turn^1); //enemy 
		if (move != 0)
			return move;

	move = findBestMove(board);
	if (move != 0)
		return move;



	for (i = 0; i < NPLAYINGBOARD; ++i)
		if (board[convertTo25[i]] == EMPTY)
			availableMoves[free++] = convertTo25[i];

	return availableMoves[rand() % free];
}

int getCoolBotMove(char *board, const char turn)
{
	ply = 0;
	positions = 0;
	maxPly = 0;
	int best = minMax(board, turn);
	printf("Positions checked: %d | Max Depth: %d | Best Move: %d \n", positions, maxPly, best);

	return best;

}

void game(int gameType)
{
	int i, gameOver = 0, lastMove, side, correctInput = 0;
	char board[N], turn;
;
	system("cls");
	if (gameType == PVP)
	{
		printf("You choose player vs player");
	}
	else
	{
		printf("You choos player vs bot. How move first?\n");
		printf("1 - You\n2 - Bot");
		while (!correctInput)
		{
			scanf_s("%d", &side);
			if (side < 1 || side > 2)
				printf("Incorrect input, try agane \n");
			else
				correctInput = 1;
		}
	}
	if (side == 1)
	{
		printf("You are playing NOUGHT and move first");
		turn = NOUGHTS;
	}
	else
	{
		printf("Bot is playing Crosses and move first");
		turn = CROSSES;
	}

	for (i = 0; i < N; i++)
		board[i] = BOARD;
	for (i = 0; i < NPLAYINGBOARD; i++)
		board[convertTo25[i]] = EMPTY;
	printBoard(board);

	while (!gameOver)
	{
		if (turn == NOUGHTS)
		{
			lastMove = getPlayerInput(board);
			makeMove(board, lastMove, turn);
			turn = CROSSES;
		}
		else
		{
			lastMove = getCoolBotMove(board, turn);
			makeMove(board, lastMove, turn);
			turn = NOUGHTS;
			printBoard(board);
		}

		if (checkWin(board, lastMove, turn^1))
		{
			printBoard(board);
			printf("Game Over!");
			gameOver = 1;
			if (turn == CROSSES)
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

void menu()
{

	printf("Weclome to Tic-Tac-Toe game\nChose type of game:\n");
	printf("1 - Player vesus Player\n2 - Player vesus Bot\n");
	int gameType, correctInput = 0;
	while (!correctInput)
	{
		scanf_s("%d", &gameType);
		if (gameType < 1 || gameType > 2)
			printf("Incorrect input, try agane \n");
		else
			correctInput = 1;
	}

	game(gameType);
}

int main()
{
	srand(time(NULL));
	menu();
	system("PAUSE");
	return 0;

}
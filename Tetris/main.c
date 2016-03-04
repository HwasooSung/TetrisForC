
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>

/* array size */
#define TETRIS_SIZE 4

/* arrow key */
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

/* */
#define SPACE 32
#define ESC 27

#define HEIGHT 20+1
#define WIDTH 10+2

#define PRINT_BLANCK printf("  ");
#define PRINT_BLOCK printf("��");

/* Tetromino  */
// I, J, L, O, S, T, Z
int tetromino[7][TETRIS_SIZE][TETRIS_SIZE] = {
	// I
	{
		{ 0, 0, 1, 0 }, 
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 }
	},
	// J
	{
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	// L
	{
		{ 0, 0, 1, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	// O
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	},
	// S
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	},
	// T
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	// Z
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 0, 0 }
	}
};

// TESTED
void rotateBlock(int block[][TETRIS_SIZE]) {
	int tempBlock[TETRIS_SIZE][TETRIS_SIZE] = { 0 };
	int i = 0,  j = 0;

	/* �� �����صα� */
	for (i = 0; i < TETRIS_SIZE; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			tempBlock[i][j] = block[i][j];
		}
	}

	
	/* ȸ�� */
	for (i = 0; i < TETRIS_SIZE; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			block[j][3 - i] = tempBlock[i][j];
		}
	}

	return;
}

// TESTED
int oneLineFilled(int line[WIDTH]) {
	int i = 0;

	for (i = 0; i < WIDTH; i++) {
		if (line[i] != 1) {
			return 0; // false
		}
	}
	return 1; // true
}

// TESTED
int returnBlockRandomly() {

	srand((unsigned)time(NULL));

	return rand() % 7 ;
}

// TESTED
void printBoard(int gameBoard[][WIDTH]) {

	int i = 0, j = 0;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (gameBoard[i][j] == 1) {
				PRINT_BLOCK;
			}
			else {
				PRINT_BLANCK;
			}
		}
		printf("\n");
	}
	return;
}

void monitorUserInput(int arrowKey, int block[][TETRIS_SIZE]) {
	while (1) {
		switch (arrowKey) {
		case UP:
			rotateBlock(block);
			break;

		case DOWN:
			break;

		case RIGHT:
			break;

		case LEFT:
			break;

		case SPACE:
			break;
		}
	}
}


// ���¿� �� �� ������ �� �ִ��� Ȯ���ϴ� �Լ� �ʿ�
//void nearByRightEdge(int gameBoard[][WIDTH], int block[][TETRIS_SIZE], int* downPosition, int* rightPosition) {
//	int i = 0, j = 0;
//	int columnIndexWhichIsOne = -1;
//
//	// block���� �� ������ 1�� index ã��
//	for (i = 0; i < TETRIS_SIZE; i++) {
//		for (j = 0; j < TETRIS_SIZE; j++) {
//			if (block[i][TETRIS_SIZE - j - 1] == 1) {
//				columnIndexWhichIsOne = TETRIS_SIZE - j - 1;
//				break;
//			}
//		}
//		if (columnIndexWhichIsOne != -1) {
//			break;
//		}
//	}
//	if(columnIndexWhichIsOne)
//	//gameBoard[*downPosition][columnIndexWhichIsOne]
//}

// FUNCTION FOR TEST
void functionForTest() {
	printf("************\n");
	printf("THIS IS TEST\n");
	printf("************\n");



	printf("************\n");
	return;
}

/* main function */
int main(void) {

	int i = 0, j = 0;
	
	 functionForTest();

	int* leftPosition = 0;
	int* rightPosition = 0;
	int* downPosition = 0;

	int gameBoard[HEIGHT][WIDTH] = { 0, };
	while (1) {
		
		printBoard(gameBoard);

		/* ���� ������ */


		/* ����� ���� */

		/* �� �ٴڿ� ���� */

		/* �� �� ������ ���� üũ */
		for (i = 0; i < HEIGHT; i++) {
			if (oneLineFilled(gameBoard[i])) {

			}
			else {

			}
		}
	}
	


	return 0;
}
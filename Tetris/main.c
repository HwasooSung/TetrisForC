#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

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

#define HEIGHT 20
#define WIDTH 10

#define PRINT_BLANCK printf("  ");
#define PRINT_BLOCK printf("■");

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

	/* 값 복사해두기 */
	for (i = 0; i < TETRIS_SIZE; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			tempBlock[i][j] = block[i][j];
		}
	}

	
	/* 회전 */
	for (i = 0; i < TETRIS_SIZE; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			block[j][3 - i] = tempBlock[i][j];
		}
	}

	return;
}

//void doWork(int arrowKey, int** block) {
//	switch (arrowKey) {
//	case UP:
//		rotateBlock(block);
//		break;
//
//	case DOWN:
//		break;
//
//	case RIGHT:
//		break;
//
//	case LEFT:
//		break;
//
//	case SPACE:
//		break;
//	}
//}

int checkOneLineFilled(int line[WIDTH]) {
	int i = 0;

	for (i = 0; i < WIDTH; i++) {
		if (line[i] != 1) {
			return 0; // false
		}
	}
	return 1; // true
}

int main(void) {

	int i = 0, j = 0;
	int gameBoard[HEIGHT][WIDTH] = { 0 };
	
	while (1) {
		/* 블럭이 내려옴 */

		/* 사용자 조작 */

		/* 블럭 떨구기 */

		/* 한 줄 지울지 말지 체크 */
		for (i = 0; i < HEIGHT; i++) {
			if (checkOneLineFilled(gameBoard[i])) {

			}
			else {

			}
		}
	}
	


	return 0;
}

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <string.h>
#include <Windows.h>

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

#define HEIGHT 21 // 20+1
#define WIDTH 12 // 10+2

#define PRINT_BLANCK printf("  ");
#define PRINT_BLOCK printf("��");


// TODO: 
// 1�� �Ѿ��� �� �׷��� >> ������ؾ���
// SPACE �� ��� �Է�?
// ���� ��

/* Tetromino  */
// I, J, L, O, S, T, Z
int tetromino[7][TETRIS_SIZE][TETRIS_SIZE] = {
	// I
	{
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 }
	},
	/*{
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 }
	},*/
	// L
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	// J
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 0, 1, 1, 1 },
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

/************************* Functions *************************/

// TESTED
void rotateBlock(int block[][TETRIS_SIZE]) {
	int tempBlock[TETRIS_SIZE][TETRIS_SIZE] = { 0 };
	int i = 0, j = 0;

	/* �� �����صα� */
	memcpy(tempBlock, block, sizeof(tempBlock));

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
int returnBlockIndexRandomly() {

	srand((unsigned)clock());

	return rand() % 7;
}

// TESTED
void printBoard(int gameBoard[][WIDTH]) {
	int i = 0, j = 0;
	COORD initialConsoleCursorPosition = { 0, 3 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), initialConsoleCursorPosition);

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

// TESTED
void makeBasicGameBoard(int gameBoard[][WIDTH]) {
	int i = 0, j = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if ((j == 0 || j == WIDTH - 1) || i == HEIGHT - 1) {
				gameBoard[i][j] = 1;
			}
			else {
				gameBoard[i][j] = 0;
			}
		}
	}
}

// TESTED
int checkWhetherAbleToMove(int gameBoard[][WIDTH], int* downPosition, int* leftAndRightPosition) {
	int i = 0, j = 0;

	for (i = 0; i < TETRIS_SIZE; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			if (gameBoard[*downPosition - i][*leftAndRightPosition + j] > 1) {
				return 0; // can NOT move
			}
			else {
				continue;
			}
		}
	}
	return 1;

	/* TEST code of this function */
	//memcpy(tempBoard, gameBoard, sizeof(gameBoard));
	//memcpy(fallingBlock, tetromino[0], sizeof(fallingBlock));
	//rotateBlock(fallingBlock);
	//addBlockDataToGameBoardData(tempBoard, fallingBlock, downPos, leftAndRightPos);
	//if (checkWhetherAbleToMove(tempBoard, downPos, leftAndRightPos) == 1) {
	//	memcpy(gameBoard, tempBoard, sizeof(gameBoard));
	//}
	//else {
	//	memcpy(tempBoard, gameBoard, sizeof(tempBoard));
	//}
	//printBoard(gameBoard);
	/******************************/
}

// TESTED
void subtractBlockFromGameBoard(int gameBoard[][WIDTH], int block[][TETRIS_SIZE], int* downPosition, int* leftAndRightPosition) {
	int i = 0, j = 0;
	int rowCount = 0;

	// �� �Ϻθ� ���� �� (�� �������� �ʱ�)
	if (*downPosition + 1 < TETRIS_SIZE) {
		rowCount = *downPosition + 1;
	}
	// �� ���� �� ���� ��
	else {
		rowCount = TETRIS_SIZE;
	}
	// gameboard �� block�� 1�� �κ� 0���� �ٲٱ�
	for (i = 0; i < rowCount; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			if (block[TETRIS_SIZE - (i + 1)][j] == 1) {
				gameBoard[*downPosition - i][*leftAndRightPosition + j] = 0;
			}
		}
	}
}

// TESTED
void addBlockDataToGameBoardData(int gameBoard[][WIDTH], int block[][TETRIS_SIZE], int* downPosition, int* leftAndRightPosition) {

	int i = 0, j = 0;
	int rowCount = 0;

	// �� �Ϻθ� ���� �� (�� �������� �ʱ�)
	if (*downPosition + 1 < TETRIS_SIZE) {
		rowCount = *downPosition + 1;
	}
	// �� ���� �� ���� ��
	else {
		rowCount = TETRIS_SIZE;
	}

	// gameboard data, block data ���ϱ�
	for (i = 0; i < rowCount; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			gameBoard[*downPosition - i][*leftAndRightPosition + j] += block[TETRIS_SIZE - (i + 1)][j];
		}
	}
	/* This is TEST code of this function*/
	//int i = 0, j = 0;
	//int gameBoard[HEIGHT][WIDTH];
	//int* downPos;
	//int* leftAndRightPos;
	//int down = 7, leftAndRight = 6;
	//downPos = &down;
	//leftAndRightPos = &leftAndRight;
	//for (i = 0; i < HEIGHT; i++) {
	//	for (j = 0; j < WIDTH; j++) {
	//		if ((j == 0 || j == WIDTH - 1) || i == HEIGHT - 1) {
	//			gameBoard[i][j] = 1;
	//		}
	//		else {
	//			gameBoard[i][j] = 0;
	//		}
	//	}
	//}
	//addBlockDataToGameBoardData(gameBoard, tetromino[0], downPos, leftAndRightPos);
	//printBoard(gameBoard);
	/********************/
	return;
}

// NOT TESTED
int oneInFirstRow(int tempblock[][TETRIS_SIZE]) { // block[0][i]�� 1�� �ֳ� // ������ ���� ���� ��
	int i = 0;
	int isOne = 0;

	for (i = 0; i < TETRIS_SIZE; i++) {
		if (tempblock[0][i] == 1) {
			return isOne = 1;
		}
	}

	return isOne;
}
int oneInLastRow(int tempblock[][TETRIS_SIZE]) { // block[3][i]�� 1�� �ֳ�
	int i = 0;
	int isOne = 0;

	for (i = 0; i < TETRIS_SIZE; i++) {
		if (tempblock[TETRIS_SIZE - 1][i] == 1) {
			return isOne = 1;
		}
	}

	return isOne;
}

// NOT TESTED
int veryNearByRightWall(int tempBoard[][WIDTH], int* downPosition, int* leftAndRightPosition) {
	int i = 0, j=0;
	int sum = 0;
	int isWall = 1;
	int oneNotInSecondColumnInI = 0;
	int tempLeftAndRightPos = *leftAndRightPosition;
	int gapIsZero = 0;


	for (i = 0; i < TETRIS_SIZE; i++) {
		if (tempBoard[*downPosition - i][tempLeftAndRightPos + 1] == 0) {
			oneNotInSecondColumnInI = 1;
		}
	}
	for (i = 0; i < TETRIS_SIZE; i++) {
		if (tempBoard[*downPosition - i][tempLeftAndRightPos + 2] == 0) {
			gapIsZero = 1;
		}
		else {
			gapIsZero = 0;
			break;
		}
	}

	// �� �� block
	if (oneNotInSecondColumnInI || gapIsZero) {
		tempLeftAndRightPos += 3;
	}
	//"I" block�� [][1]�� ���� �϶�
	else {
		tempLeftAndRightPos += 2;
	}

	for (i = 0; i < TETRIS_SIZE; i++) {
		if (tempBoard[*downPosition - i][tempLeftAndRightPos] == 0) {
			return isWall = 0;
		}
	}
	return isWall;

}
int veryNearByLeftWall(int tempBoard[][WIDTH], int* downPosition, int* leftAndRightPosition) {
	int i = 0;
	int isWall = 1;
	int tempLeftAndRightPos = *leftAndRightPosition;
	int oneNotInThirdColumnInI = 0;

	for (i = 0; i < TETRIS_SIZE; i++) {
		if (tempBoard[*downPosition - i][tempLeftAndRightPos + 2] == 0) {
			oneNotInThirdColumnInI = 1;
		}
	}

	if (oneNotInThirdColumnInI) {
		//tempLeftAndRightPos ;
	}
	//"I" block�� [][2]�� ���� �϶�
	else {
		tempLeftAndRightPos += 1;
	}
	for (i = 0; i < TETRIS_SIZE; i++) {
		if (tempBoard[*downPosition - i][tempLeftAndRightPos] == 0) {
			return isWall = 0;
		}
	}

	return isWall;
}


// NOT COMPLETED
int processUserInput(int arrowKey, int gameBoard[][WIDTH], int block[][TETRIS_SIZE], int* downPosition, int* leftAndRightPosition) {
	int forChangedPosition = 0;
	int* changedPosition = &forChangedPosition;
	int tempBoard[HEIGHT][WIDTH];
	int tempBlock[TETRIS_SIZE][TETRIS_SIZE];

	int i = 0, j = 0;

	/*
	 * ������ print�� gameBoard�θ� �Ѵ�
	 * ������ data ������ tempBoard�θ� �Ѵ�
	 */
	memcpy(tempBoard, gameBoard, sizeof(tempBoard));
	memcpy(tempBlock, block, sizeof(tempBlock));

	switch (arrowKey) {

	case UP: /////////////////////////////////////////////////////////////// 
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, tempBlock, downPosition, leftAndRightPosition);
		}

		// I block�� ���� ����ó��
		if (*leftAndRightPosition == -1 || *leftAndRightPosition == 0) {
			while (veryNearByLeftWall(tempBoard, downPosition, leftAndRightPosition) && oneInLastRow(tempBlock)) {
				*leftAndRightPosition += 1;
				addBlockDataToGameBoardData(tempBoard, tempBlock, downPosition, leftAndRightPosition);
				if (checkWhetherAbleToMove(tempBoard, downPosition, leftAndRightPosition) == 1) {
					memcpy(gameBoard, tempBoard, sizeof(tempBoard));
					memcpy(block, tempBlock, sizeof(tempBlock));
				}
				else {
					memcpy(tempBoard, gameBoard, sizeof(tempBoard));
				}
				subtractBlockFromGameBoard(tempBoard, tempBlock, downPosition, leftAndRightPosition);
			}
		}
		else if (*leftAndRightPosition == WIDTH - TETRIS_SIZE || *leftAndRightPosition == WIDTH - TETRIS_SIZE + 1) {
			while (veryNearByRightWall(tempBoard, downPosition, leftAndRightPosition) && oneInFirstRow(tempBlock)) {
				*leftAndRightPosition -= 1;
				addBlockDataToGameBoardData(tempBoard, tempBlock, downPosition, leftAndRightPosition);
				if (checkWhetherAbleToMove(tempBoard, downPosition, leftAndRightPosition) == 1) {
					memcpy(gameBoard, tempBoard, sizeof(tempBoard));
					memcpy(block, tempBlock, sizeof(tempBlock));
				}
				else {
					memcpy(tempBoard, gameBoard, sizeof(tempBoard));
				}
				subtractBlockFromGameBoard(tempBoard, tempBlock, downPosition, leftAndRightPosition);
			}
		}

		rotateBlock(tempBlock);

		addBlockDataToGameBoardData(tempBoard, tempBlock, downPosition, leftAndRightPosition);
		if (checkWhetherAbleToMove(tempBoard, downPosition, leftAndRightPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			memcpy(block, tempBlock, sizeof(tempBlock));
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
		}


		break;

	case DOWN:
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, tempBlock, downPosition, leftAndRightPosition);
		}
		*changedPosition = *downPosition + 1;
		addBlockDataToGameBoardData(tempBoard, tempBlock, changedPosition, leftAndRightPosition);
		if (checkWhetherAbleToMove(tempBoard, changedPosition, leftAndRightPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			memcpy(block, tempBlock, sizeof(tempBlock));
			*downPosition = *changedPosition;
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
			return 1; // �� �� ��������(true) => I think this is not good
		}

		break;

	case RIGHT:
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, tempBlock, downPosition, leftAndRightPosition);
		}

		*changedPosition = *leftAndRightPosition + 1;
		addBlockDataToGameBoardData(tempBoard, tempBlock, downPosition, changedPosition);
		if (checkWhetherAbleToMove(tempBoard, downPosition, changedPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			memcpy(block, tempBlock, sizeof(tempBlock));
			*leftAndRightPosition = *changedPosition;
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
		}
		break;

	case LEFT:
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, tempBlock, downPosition, leftAndRightPosition);
		}
		*changedPosition = *leftAndRightPosition - 1;
		addBlockDataToGameBoardData(tempBoard, tempBlock, downPosition, changedPosition);
		if (checkWhetherAbleToMove(tempBoard, downPosition, changedPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			memcpy(block, tempBlock, sizeof(tempBlock));
			*leftAndRightPosition = *changedPosition;
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
		}
		break;

		//case SPACE:
		//	while (1) {
		//		if (*downPosition != 0) {
		//			subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
		//		}
		//		*changedPosition = *downPosition + 1;
		//		addBlockDataToGameBoardData(tempBoard, block, changedPosition, leftAndRightPosition);
		//		if (checkWhetherAbleToMove(tempBoard, changedPosition, leftAndRightPosition) == 1) {
		//			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
		//			*downPosition = *changedPosition;
		//		}
		//		else {
		//			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
		//			break;
		//		}
		//	}
		//	break;

	default:
		break;
	}

	return 0; // �� �� �ȶ�����, ��� ���� ������ ����
}

////////////////////////////
//int gameOver(int gameBoard[][WIDTH], int* downPosition) {
//	int i = 0, j = 0;
//
//}

// FUNCTION FOR TEST
void functionForTest() {
	int i = 0, j = 0;
	int gameBoard[HEIGHT][WIDTH];
	int indexDown = 0, indexLeftAndRight = 4;
	int* downPosition;
	int* leftAndRightPosition;


	int fallingBlock[TETRIS_SIZE][TETRIS_SIZE];
	int tempBoard[HEIGHT][WIDTH]; // oneLineFilled �϶� ���

	int userInputKeyCode;

	int mustReturnNewBlock = 1;

	downPosition = &indexDown;
	leftAndRightPosition = &indexLeftAndRight;

	makeBasicGameBoard(gameBoard);

	printf("************\n");
	printf("THIS IS TEST\n");
	printf("************\n\n");

	while (1) {
		printBoard(gameBoard);

		/* ���� ������ */
		if (mustReturnNewBlock) {

			memcpy(fallingBlock, tetromino[returnBlockIndexRandomly()], sizeof(fallingBlock));
			//memcpy(fallingBlock, tetromino[0], sizeof(fallingBlock)); 
			mustReturnNewBlock = 0;
			*downPosition = 0;
			*leftAndRightPosition = 4;
		}

		/* ����� ���� */
		if (_kbhit()) {
			if (_getch() != 0xE0) {}
			userInputKeyCode = _getch();

			mustReturnNewBlock = processUserInput(userInputKeyCode, gameBoard, fallingBlock, downPosition, leftAndRightPosition);
		}

		/* �ٽ� �׸��� */
		printBoard(gameBoard);

		/* if => �� �ٴڿ� ���� */
		if (mustReturnNewBlock == 1) {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));

			for (i = 0; i < HEIGHT - 1; i++) {
				/* �� �� ������ ���� üũ */
				if (oneLineFilled(tempBoard[i]) == 1) {
					for (j = 0; (i - j) > 0; j++) {
						memcpy(tempBoard[i - j], gameBoard[i - j - 1], sizeof(int) * WIDTH);
					}
					memset(tempBoard[0], 0, sizeof(int) * WIDTH);
					tempBoard[0][0] = 1;
					tempBoard[0][11] = 1;
					memcpy(gameBoard, tempBoard, sizeof(tempBoard));
					printBoard(gameBoard);
				}
			}
			/* GAME OVER ���� */
			if (*downPosition <= 0) {
				return;
			}
		}

		userInputKeyCode = -1;
	}

	printf("\n************\n");
	return;
}

/* main function */
int main(void) {

	int i = 0, j = 0;

	functionForTest();

	//int* leftAndRightPosition = 0;
	//int* downPosition = 0;

	//int gameBoard[HEIGHT][WIDTH] = { 0, };
	//while (1) {
	//	
	//	printBoard(gameBoard);

	//	/* ���� ������ */


	//	/* ����� ���� */

	//	/* �ٽ� �׸��� */

	//	/* if => �� �ٴڿ� ���� */

	//	/* �� �� ������ ���� üũ */
	//	//for (i = 0; i < HEIGHT; i++) {
	//	//	if (oneLineFilled(gameBoard[i])) {
	//	//	}
	//	//	else {
	//	//	}
	//	//}
	//}



	return 0;
}
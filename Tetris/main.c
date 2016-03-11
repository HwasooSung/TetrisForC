
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
		{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	// L
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
	int i = 0,  j = 0;

	/* 값 복사해두기 */
	memcpy(tempBlock, block, sizeof(tempBlock));

	/* 회전 */
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

	return rand() % 7 ;
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

	// 블럭 일부만 나올 때 (블럭 떨어지는 초기)
	if (*downPosition + 1 < TETRIS_SIZE) {
		rowCount = *downPosition + 1;
	}
	// 블럭 전부 다 나올 때
	else {
		rowCount = TETRIS_SIZE;
	}
	// gameboard 내 block의 1인 부분 0으로 바꾸기
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

	// 블럭 일부만 나올 때 (블럭 떨어지는 초기)
	if (*downPosition + 1 < TETRIS_SIZE) {
		rowCount = *downPosition + 1;
	}
	// 블럭 전부 다 나올 때
	else {
		rowCount = TETRIS_SIZE;
	}

	// gameboard data, block data 더하기
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


// TODO: 
// 1초 넘었을 때 그려줌

// NOT COMPLETED
int processUserInput(int arrowKey, int gameBoard[][WIDTH], int block[][TETRIS_SIZE], int* downPosition, int* leftAndRightPosition) {
	int forChangedPosition = 0;
	int* changedPosition = &forChangedPosition;
	int tempBoard[HEIGHT][WIDTH];
	
	/*
	 * 게임판 print는 gameBoard로만 한다
	 * 게임판 data 조작은 tempBoard로만 한다
	 */
	memcpy(tempBoard, gameBoard, sizeof(tempBoard));

	switch (arrowKey) {

	case UP: /////////////////////////////////////////////////////////////// 벽에 딱 붙어서 rotate 하면 벽에 딱 달라 붙음 ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
		}
		rotateBlock(block);
		addBlockDataToGameBoardData(tempBoard, block, downPosition, leftAndRightPosition);
		if (checkWhetherAbleToMove(tempBoard, downPosition, leftAndRightPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
		}
		break;

	case DOWN:
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
		}
		*changedPosition = *downPosition + 1;
		addBlockDataToGameBoardData(tempBoard, block, changedPosition, leftAndRightPosition);
		if (checkWhetherAbleToMove(tempBoard, changedPosition, leftAndRightPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			*downPosition = *changedPosition;
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
			return 1; // 블럭 다 떨어졌음(true) => I think this is not good
		}

		break;

	case RIGHT:
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
		}
		*changedPosition = *leftAndRightPosition + 1;
		addBlockDataToGameBoardData(tempBoard, block, downPosition, changedPosition);
		if (checkWhetherAbleToMove(tempBoard, downPosition, changedPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			*leftAndRightPosition = *changedPosition;
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
		}
		break;

	case LEFT:
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
		}
		*changedPosition = *leftAndRightPosition - 1;
		addBlockDataToGameBoardData(tempBoard, block, downPosition, changedPosition);
		if (checkWhetherAbleToMove(tempBoard, downPosition, changedPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
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

	default :
		break;
	}

	return 0; // 블럭 다 안떨어짐, 계속 조작 받을수 있음
}



// FUNCTION FOR TEST
void functionForTest() {
	int i = 0, j = 0;
	int gameBoard[HEIGHT][WIDTH];
	int indexDown = 0, indexLeftAndRight = 4;
	int* downPosition;
	int* leftAndRightPosition;

	int fallingBlock[TETRIS_SIZE][TETRIS_SIZE];
	int tempBoard[HEIGHT][WIDTH]; // oneLineFilled 일때 사용

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
		
		/* 블럭이 내려옴 */
		if (mustReturnNewBlock) {
			memcpy(fallingBlock, tetromino[returnBlockIndexRandomly()], sizeof(fallingBlock));
			//memcpy(fallingBlock, tetromino[0], sizeof(fallingBlock)); // tetromino 1, 2 ERROR // 맨밑일때 // J L 딱 그 모양일때만 ////////////////////////////////////////////////////
			mustReturnNewBlock = 0;
			*downPosition = 0;
			*leftAndRightPosition = 4;
		}
		
		/* 사용자 조작 */
		// TODO: SPACE 바 어떻게 입력?
		if (_kbhit()) {
			if (_getch() != 0xE0) {}
			userInputKeyCode = _getch();
			
			mustReturnNewBlock = processUserInput(userInputKeyCode, gameBoard, fallingBlock, downPosition, leftAndRightPosition);
		}
		
		/* 다시 그리기 */
		printBoard(gameBoard);

		/* if => 블럭 바닥에 도착 */
		if (mustReturnNewBlock == 1) {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));

			for (i = 0; i < HEIGHT-1; i++) {
				/* 한 줄 지울지 말지 체크 */
				if (oneLineFilled(tempBoard[i]) == 1) {
					for (j = 0; (i - j) > 0 ; j++) {
						memcpy(tempBoard[i - j], gameBoard[i - j - 1], sizeof(int) * WIDTH);
					}
					memset(tempBoard[0], 0, sizeof(int) * WIDTH);
					tempBoard[0][0] = 1;
					tempBoard[0][11] = 1;
					memcpy(gameBoard, tempBoard, sizeof(tempBoard));
					printBoard(gameBoard);
				}
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

	//	/* 블럭이 내려옴 */


	//	/* 사용자 조작 */

	//	/* 다시 그리기 */

	//	/* if => 블럭 바닥에 도착 */

	//	/* 한 줄 지울지 말지 체크 */
	//	//for (i = 0; i < HEIGHT; i++) {
	//	//	if (oneLineFilled(gameBoard[i])) {
	//	//	}
	//	//	else {
	//	//	}
	//	//}
	//}
	


	return 0;
}
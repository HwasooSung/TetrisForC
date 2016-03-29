
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

/* Game Board */
#define HEIGHT 22 // 20+1 +1
#define WIDTH 12 // 10+2

#define TRUE 1
#define FALSE 0

#define ONE_SECOND_MILSEC 1000
#define ONE_MINIUTE_MILSEC 60000
#define ONE_TENTH_OF_SECOND_MILSEC 100

#define PRINT_BLANCK printf("  ");
#define PRINT_BLOCK printf("■");


// TODO: 
// 1초 넘었을 때 그려줌 >> 얘먼저해야함
	// TODO : Draw 함수
	// TODO : 게임 data랑 출력이랑 구분해야함
// 맨 위에서 벽으로 갔을 때 블럭 멈춰지는 버그


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
			return FALSE; // false
		}
	}
	return TRUE; // true
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

	// gameBoard[0]은 game over 용
	// gameBoard[1]부터 그림
	for (i = 1; i < HEIGHT; i++) {
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
				return FALSE; // can NOT move
			}
			else {
				continue;
			}
		}
	}
	return TRUE;

	/* TEST code of this function */
	//memcpy(tempBoard, gameBoard, sizeof(gameBoard));
	//memcpy(fallingBlock, tetromino[0], sizeof(fallingBlock));
	//rotateBlock(fallingBlock);
	//addBlockDataToGameBoardData(tempBoard, fallingBlock, downPos, leftAndRightPos);
	//if (checkWhetherAbleToMove(tempBoard, downPos, leftAndRightPos) == TRUE) {
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

// NOT TESTED
int oneInFirstRow(int tempblock[][TETRIS_SIZE]) { // block[0][i]에 1이 있나 // 오른쪽 벽에 있을 때
	int i = 0;
	int isOne = 0;

	for (i = 0; i < TETRIS_SIZE; i++) {
		if (tempblock[0][i] == 1) {
			return isOne = 1;
		}
	}

	return isOne;
}
int oneInLastRow(int tempblock[][TETRIS_SIZE]) { // block[3][i]에 1이 있나
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
	int i = 0, j = 0;
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

	// 그 외 block
	if (oneNotInSecondColumnInI || gapIsZero) {
		tempLeftAndRightPos += 3;
	}
	//"I" block이 [][1]에 있을 일때
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
	//"I" block이 [][2]에 있을 일때
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
	 * 게임판 print는 gameBoard로만 한다
	 * 게임판 data 조작은 tempBoard로만 한다
	 */
	memcpy(tempBoard, gameBoard, sizeof(tempBoard));
	memcpy(tempBlock, block, sizeof(tempBlock));

	switch (arrowKey) {

	case UP: /////////////////////////////////////////////////////////////// 
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, tempBlock, downPosition, leftAndRightPosition);
		}

		// I block에 대한 예외처리
		if (*leftAndRightPosition == -1 || *leftAndRightPosition == 0) {
			while (veryNearByLeftWall(tempBoard, downPosition, leftAndRightPosition) && oneInLastRow(tempBlock)) {
				*leftAndRightPosition += 1;
				addBlockDataToGameBoardData(tempBoard, tempBlock, downPosition, leftAndRightPosition);
				if (checkWhetherAbleToMove(tempBoard, downPosition, leftAndRightPosition) == TRUE) {
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
				if (checkWhetherAbleToMove(tempBoard, downPosition, leftAndRightPosition) == TRUE) {
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
		if (checkWhetherAbleToMove(tempBoard, downPosition, leftAndRightPosition) == TRUE) {
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
		if (checkWhetherAbleToMove(tempBoard, changedPosition, leftAndRightPosition) == TRUE) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			memcpy(block, tempBlock, sizeof(tempBlock));
			*downPosition = *changedPosition;
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
			return TRUE; // 블럭 다 떨어졌음(true) => I think this is not good
		}

		break;

	case RIGHT:
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, tempBlock, downPosition, leftAndRightPosition);
		}

		*changedPosition = *leftAndRightPosition + 1;
		addBlockDataToGameBoardData(tempBoard, tempBlock, downPosition, changedPosition);
		if (checkWhetherAbleToMove(tempBoard, downPosition, changedPosition) == TRUE) {
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
		if (checkWhetherAbleToMove(tempBoard, downPosition, changedPosition) == TRUE) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			memcpy(block, tempBlock, sizeof(tempBlock));
			*leftAndRightPosition = *changedPosition;
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
		}
		break;

	case SPACE:
		while (1) {
			if (*downPosition != 0) {
				subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
			}
			*changedPosition = *downPosition + 1;
			addBlockDataToGameBoardData(tempBoard, block, changedPosition, leftAndRightPosition);
			if (checkWhetherAbleToMove(tempBoard, changedPosition, leftAndRightPosition) == TRUE) {
				memcpy(gameBoard, tempBoard, sizeof(tempBoard));
				*downPosition = *changedPosition;
			}
			else {
				memcpy(tempBoard, gameBoard, sizeof(tempBoard));
				return TRUE;
			}
		}
		break;

	default:
		break;
	}

	return FALSE; // 블럭 다 안떨어짐, 계속 조작 받을수 있음
}


//////////////////////////
void WINAPI increaseDownPositionValuePerTime(void* argList) {
	int** argumentList = (int**)argList;

	int* downPosition = argumentList[0];
	int* mustDrawBoard = argumentList[1];
	int* sleepTime = argumentList[2];

	while (1) {
		/* 조작 */
		*downPosition += 1;
		*mustDrawBoard = TRUE;
		/* 쉬기 */
		Sleep(*sleepTime);
	}
}

void WINAPI decreaseSleepTime(void* argList) {
	int** argumentList = (int**)argList;
	int* sleepTime = argumentList[0];

	while (1) {
		Sleep(*sleepTime);
		*sleepTime -= ONE_TENTH_OF_SECOND_MILSEC;
	}
}

//void WINAPI drawBoardAgain(void* argList) {
//
//	int** argumentList = (int**)argList;
//	int* downPosition = argumentList[0];
//	int** block = argumentList[1];
//	int** gameBoard = argumentList[2];
//	int* leftAndRightPosition = argumentList[3];
//	int* mustDrawNewBoard = argumentList[4];
//	int* changedPosition;
//
//	/*
//	* 게임판 print는 gameBoard로만 한다
//	* 게임판 data 조작은 tempBoard로만 한다
//	*/
//	int tempBoard[HEIGHT][WIDTH];
//	int tempBlock[TETRIS_SIZE][TETRIS_SIZE];
//
//	while (*mustDrawNewBoard == TRUE) {
//		memcpy(tempBlock, block, sizeof(tempBlock));
//		memcpy(tempBoard, gameBoard, sizeof(tempBoard));
//		*changedPosition = *downPosition;
//
//		if (*changedPosition != 0) {
//			subtractBlockFromGameBoard(tempBoard, tempBlock, downPosition, leftAndRightPosition);
//		}
//		
//		addBlockDataToGameBoardData(tempBoard, tempBlock, changedPosition, leftAndRightPosition);
//		if (checkWhetherAbleToMove(tempBoard, changedPosition, leftAndRightPosition) == TRUE) {
//			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
//			memcpy(block, tempBlock, sizeof(tempBlock));
//			printBoard(gameBoard);
//			*mustDrawNewBoard = FALSE;
//		}
//		else {
//			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
//			*downPosition -= 1;
//			//return TRUE; // 블럭 다 떨어졌음(true) => I think this is not good
//		}
//	}
//	//downPosition, fallingBlock, gameBoard, leftAndRightPosition, mustDrawNewBoard
//}
//////////////////////////////




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
	int tempKeyCode;

	int mustReturnNewBlock = 1;

	downPosition = &indexDown;
	leftAndRightPosition = &indexLeftAndRight;

	/* thread variables */
	//HANDLE threadHandle;

	//int mustDrawNewBoardMemSection = 0;
	//int sleepTimeMemSection = ONE_SECOND_MILSEC;

	//int* mustDrawNewBoard = &mustDrawNewBoardMemSection;
	//int* sleepTime = &sleepTimeMemSection; // 초기값 1초 //1초마다 내려감

	//int* increaseDownPositionValArgList[] = { downPosition, mustDrawNewBoard, sleepTime };
	//int* decreaseSleepTimeArgList[] = { sleepTime };
	//// TODO : 게임 data랑 출력이랑 구분해야함
	//int* drawBoardAgainArgList[] = { downPosition, fallingBlock, gameBoard, leftAndRightPosition, mustDrawNewBoard  };

	//threadHandle = (HANDLE)_beginthreadex(NULL, 0, increaseDownPositionValuePerTime, (void*)increaseDownPositionValArgList, CREATE_SUSPENDED, NULL);
	/********************/

	makeBasicGameBoard(gameBoard);

	printf("************\n");
	printf("THIS IS TEST\n");
	printf("************\n\n");

	while (1) {
		printBoard(gameBoard);

		/* 블럭이 내려옴 */
		if (mustReturnNewBlock) {
			
			memcpy(fallingBlock, tetromino[returnBlockIndexRandomly()], sizeof(fallingBlock));
			//memcpy(fallingBlock, tetromino[3], sizeof(fallingBlock)); 
			mustReturnNewBlock = 0;
			*downPosition = 0;
			*leftAndRightPosition = 4;

			////thread
			//ResumeThread(threadHandle);
		}

		/* 사용자 조작 */
		if (_kbhit()) {
			tempKeyCode = _getch();
			if (tempKeyCode == 0xE0) {
				userInputKeyCode = _getch();
			}
			else if (tempKeyCode == SPACE) {
				userInputKeyCode = SPACE;
			}
			
			mustReturnNewBlock = processUserInput(userInputKeyCode, gameBoard, fallingBlock, downPosition, leftAndRightPosition);
		}

		/* 다시 그리기 */
		printBoard(gameBoard);

		/* if => 블럭 바닥에 도착 */
		if (mustReturnNewBlock == TRUE) {
			//// thread
			//SuspendThread(threadHandle);

			memcpy(tempBoard, gameBoard, sizeof(tempBoard));

			for (i = 0; i < HEIGHT - 1; i++) {
				/* 한 줄 지울지 말지 체크 */
				if (oneLineFilled(tempBoard[i]) == TRUE) {
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
			/* GAME OVER */
			for (i = 0; i < WIDTH - 2; i++) {
				if (gameBoard[0][i + 1] == 1) {
					printf("\nGAME OVER\n\n");
					return;
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
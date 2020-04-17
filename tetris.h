#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>


#define bWidth 15
#define bHeight 25
#define BLOCKSIZE 4




extern char board[bHeight][bWidth];
extern int timeBtw;
extern int gameState;
extern int score;
extern int level;



typedef struct Block
{
    int xPos;
    int yPos;
    int nextType;
	int color;
    char matrix[BLOCKSIZE][BLOCKSIZE];
    char newMatrix[BLOCKSIZE][BLOCKSIZE];
} Block;


extern Block block;
extern void rotateMatrix();
extern void createBlock();
extern void copyBlockToBoard();
extern void init();
extern int overLap();
extern void deleteAndShift();
extern void moveBlock(int moveTo);



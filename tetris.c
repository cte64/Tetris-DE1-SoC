#include "address_map_nios2.h"
#include "tetris.h"
#include "math.h"

char board[bHeight][bWidth];
timeBtw = 30000000;
gameState = 0;
score = 0;
level = 1;
Block block;

void rotateMatrix()
{
    for(int y = 0; y< BLOCKSIZE/2; y++)
    {
        int siz = BLOCKSIZE - 2*y;
        int perimeter = 4*siz - 4;

        for(int z = 0; z<siz - 1; z++)
        {
            int hold = block.matrix[y + 1][y];
            for(int x = 0; x<perimeter; x++)
            {
                int xIndex = 0;
                int yIndex = 0;

                if(0 <= x  && x <= siz - 1) {xIndex = x + y; yIndex = y; }
                if(siz - 1 < x  && x < 2*siz - 2) {xIndex = BLOCKSIZE - 1 - y; yIndex = x - siz + 1 + y; }
                if(2*siz - 2 <= x && x <= 3*siz - 3){xIndex = 3*siz - 3 - x + y; yIndex = BLOCKSIZE - 1 - y; }
                if(3*siz - 3 < x && x < 4*siz - 4){xIndex = y; yIndex = 4*siz - 4 - x + y; }

                int temp = block.matrix[yIndex][xIndex];
                block.matrix[yIndex][xIndex] = hold;
                hold = temp;
            }
        }
    }
}


void createBlock()
{

	for(int y = 0; y<BLOCKSIZE; y++)
        for(int x = 0; x<BLOCKSIZE; x++)
            block.matrix[y][x] = block.newMatrix[y][x];
	
	unsigned int newType = rand() % 7;


    if (newType == 0)
    {
       char newMatrix[BLOCKSIZE][BLOCKSIZE] =
       { ' ', ' ', ' ', ' ',
         ' ', 'M', 'M', ' ',
         ' ', 'M', 'M', ' ',
         ' ', ' ', ' ', ' '};
        for(int y = 0; y<BLOCKSIZE; y++)
            for(int x = 0; x<BLOCKSIZE; x++)
                block.newMatrix[y][x] = newMatrix[y][x];
    }

    if (newType == 1)
    {
       char newMatrix[BLOCKSIZE][BLOCKSIZE] =
       { ' ', ' ', ' ', ' ',
         ' ', ' ', ' ', ' ',
         'N', 'N', 'N', 'N',
         ' ', ' ', ' ', ' '};
        for(int y = 0; y<BLOCKSIZE; y++)
            for(int x = 0; x<BLOCKSIZE; x++)
                block.newMatrix[y][x] = newMatrix[y][x];
    }
	
    if (newType == 2)
    {
       char newMatrix[BLOCKSIZE][BLOCKSIZE] =
       { ' ', ' ', ' ', ' ',
         ' ', ' ', 'O', ' ',
         'O', 'O', 'O', ' ',
         ' ', ' ', ' ', ' '};
        for(int y = 0; y<BLOCKSIZE; y++)
            for(int x = 0; x<BLOCKSIZE; x++)
                block.newMatrix[y][x] = newMatrix[y][x];
    }
	

    if (newType == 3)
    {
       char newMatrix[BLOCKSIZE][BLOCKSIZE] =
       { ' ', ' ', ' ', ' ',
         ' ', 'P', ' ', ' ',
         ' ', 'P', 'P', 'P',
         ' ', ' ', ' ', ' '};
        for(int y = 0; y<BLOCKSIZE; y++)
            for(int x = 0; x<BLOCKSIZE; x++)
                block.newMatrix[y][x] = newMatrix[y][x];
    }

	
    if (newType == 4)
    {
       char newMatrix[BLOCKSIZE][BLOCKSIZE] =
       { ' ', ' ', ' ', ' ',
         'Q', 'Q', 'Q', ' ',
         ' ', 'Q', ' ', ' ',
         ' ', ' ', ' ', ' '};
        for(int y = 0; y<BLOCKSIZE; y++)
            for(int x = 0; x<BLOCKSIZE; x++)
                block.newMatrix[y][x] = newMatrix[y][x];
    }

    if (newType == 5)
    {
       char newMatrix[BLOCKSIZE][BLOCKSIZE] =
       { ' ', ' ', ' ', ' ',
         ' ', 'R', 'R', ' ',
         ' ', ' ', 'R', 'R',
         ' ', ' ', ' ', ' '};
        for(int y = 0; y<BLOCKSIZE; y++)
            for(int x = 0; x<BLOCKSIZE; x++)
                block.newMatrix[y][x] = newMatrix[y][x];
    }

    if (newType == 6)
    {
       char newMatrix[BLOCKSIZE][BLOCKSIZE] =
       { ' ', ' ', ' ', ' ',
         ' ', ' ', 'S', 'S',
         ' ', 'S', 'S', ' ',
         ' ', ' ', ' ', ' '};
        for(int y = 0; y<BLOCKSIZE; y++)
            for(int x = 0; x<BLOCKSIZE; x++)
                block.newMatrix[y][x] = newMatrix[y][x];
    }

    block.xPos = bWidth/2 - BLOCKSIZE/2 - 1;
    block.yPos = 0;
}


void copyBlockToBoard()
{
	for(int y = block.yPos; y < block.yPos + BLOCKSIZE; y++)
	{
		for(int x = block.xPos; x < block.xPos + BLOCKSIZE; x++)
		{
			if(block.matrix[y - block.yPos][x - block.xPos] != ' ')
				board[y][x] = block.matrix[y - block.yPos][x - block.xPos];

		}

	}
}


     
void init()
{
	//srand(time(NULL));

    for(int y = 0; y<bHeight; y++)
    {
        for(int x = 0; x<bWidth; x++)
        {
            if(x == 0 || x == bWidth - 1  || y == 0 || y == bHeight - 1)
                board[y][x] = 'T';
            else
                board[y][x] = ' ';
        }
    }
    block.xPos = 1;
    block.yPos = 7;
    createBlock();
    createBlock();
	score = 0;
	gameState = 0;
	timeBtw = 30000000;
}


int overLap()
{
    int overlap = 0;
    for(int y = block.yPos; y < block.yPos + BLOCKSIZE; y++)
    {
        for(int x = block.xPos; x < block.xPos + BLOCKSIZE; x++)
        {
            if( board[y][x] != ' ' && block.matrix[y - block.yPos][x - block.xPos] != ' ')
                overlap = 1;
        }
    }
    return overlap;
}


void deleteAndShift()
{

	int levelBefore = level;

    for(int z = bHeight - 2; z != 0; z--)
    {
		a: ;
    	bool flag = true;
    	for(int x = 1; x < bWidth - 1; x++)
    		if( board[z][x] == ' ') 
    			flag = false;

    	if(flag)
    	{
    		score += 100;
    		level = score / 1000 + 1;

			//MAKE THE GAME GO FASTER
			timeBtw = timeBtw*0.9;

    		for(int x = 1; x < bWidth - 1; x++)
    			for(int y = z; y != 0; y--)
                    board[y][x] = (y > 1) ? board[y - 1][x] : ' ';
    		goto a;
    	}

    }

    if(levelBefore != level)
    	timeBtw *= 0.9;

}


void moveBlock(int moveTo)
{

	if(gameState == 0 && moveTo == 112)
	{
		gameState = 2;
		return;
	}

	if(gameState == 2)
	{
		if(moveTo == 112)
			gameState = 0;
		
		return;
	}
		
	
	//RESTART THE GAME
	if(gameState == 1)
	{
		if(moveTo == 121)
			init();
		return;
	}


    if(moveTo == 97)
    {
        block.xPos--;
        if( overLap(block) ) block.xPos++;
    }
    

    if(moveTo == 115)
    {
        block.yPos++;
        if( overLap(block) )
        {
            block.yPos--;
            copyBlockToBoard(block);
            deleteAndShift();
            createBlock(block);
            if(overLap(block)) gameState = 1;
        }
    }

    if(moveTo == 100)
    {
        block.xPos++;
        if( overLap(block) ) block.xPos--;
    }

    if(moveTo == 119)
    {
    	rotateMatrix(block);

    	if( overLap(block) )
    	{
    		for(int x = 0; x<3; x++)
    			rotateMatrix(block);
    	}
    	
    } 

    if(moveTo == 32)
    {
    	while(1)
    	{
    		block.yPos++;
    		if( overLap(block) )
	        {
	            block.yPos--;
	            copyBlockToBoard(block);
	            deleteAndShift();
	            createBlock(block);
	            break;
	        }

    	}

    }
}

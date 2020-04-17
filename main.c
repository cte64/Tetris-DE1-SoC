#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"					
#include "tetris.h"

const unsigned int WIDTH = 640;
const unsigned int HEIGHT = 480;

				//Purple  Red     Green   Yellow   Blue     Orange  Cyanc  Grey
int colors[8] =  {0xD01F, 0xF800, 0x07E3, 0xE7E0,  0x019F,  0xFC60, 0x07FC, 0xAD75};

void write_pixel(int x, int y, short color) {
  volatile short *vga_addr=(volatile short*)(0x08000000 + (y<<10) + (x<<1));
  *vga_addr=color;
}

void clearEverything() {

	for(int x = 0; x<79; x++)
	{
		for(int y = 0; y<59; y++)
		{	
			volatile char * character_buffer = (char *) (0x09000000 + (y<<7) + x);
			*character_buffer = ' ';	
		}
	}

  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
	  write_pixel(x,y,0);
	}
  }
}

void write_char(int x, int y, char c) {
  // VGA character buffer
  volatile char * character_buffer = (char *) (0x09000000 + (y<<7) + x);
  *character_buffer = c;
}


void drawSquare(int i, int j, int color)
{
	int hMargin = 60;
	int vMargin = 21;
	int tileWidth = 6;
	int trim = 1;

	int leftEdge = hMargin + (1.0 + j)*trim + tileWidth*j;
	int topEdge =  vMargin + (1.0 + i)*trim + tileWidth*i;
	for(int y = topEdge; y < (topEdge + tileWidth); y++)
	{
		for(int x = leftEdge; x < (leftEdge + tileWidth); x++)
			write_pixel(x, y, color);
	}
}


void drawBoard()
{
	//CLEAR CHAR BUFFER ===========================================================

	for(int x = 0; x<79; x++)
	{
		for(int y = 0; y<59; y++)
		{	
			volatile char * character_buffer = (char *) (0x09000000 + (y<<7) + x);
			*character_buffer = ' ';	
		}
	}

	//BANNER ==========================================
	char *hw;
	int charIndex = 0;

	if(gameState == 0) 
		hw = "ECE 178 Tetris:";	
	else if(gameState == 1) 
		hw = "GAMEOVER PLAY AGAIN? Y/N";
	else if(gameState == 2) 
		hw = "PAUSED:";

	while(*hw)
	{
		write_char(charIndex + 15, 2, *hw);
		charIndex++;
		hw++;
	}
	
	//DISPLAY THE SCORE ===============================
	
    hw = "Score:";
    int leftOffset = 43;
	charIndex = 0;

	while(*hw)
	{
		write_char(charIndex + leftOffset, 5, *hw);
		charIndex++;
		hw++;
	}

		
    char *str;
	sprintf(str, "%d", score);
	
	charIndex = 0;
	while(*str)
	{
		write_char(charIndex + leftOffset, 7, *str);
		charIndex++;
		str++;
	}
	

	//NEXT BLOCK ==========================================
   hw = "NEXT BLOCK:";
	for(int x = 0; x<11; x++)
		write_char(x + leftOffset, 10, hw[x]);


	//DISPLAY THE NEXT BLOCK =========================================
	for(int y = 0; y<4; y++)
	{
		for(int x = 0; x<4; x++)
		{
			char c = block.newMatrix[y][x];
			if(c != ' ')
				drawSquare(y + 4, x + 16, colors[(int)c - 77]);
			else
				drawSquare(y + 4, x + 16, 0x0000);
		}
	}
	
	for(int y = 0; y<25; y++)
	{
		for(int x = 0; x<15; x++)
		{
			drawSquare(y, x, 0x0000); 			

			if( y >= block.yPos && y < block.yPos + BLOCKSIZE && x >= block.xPos && x < block.xPos + BLOCKSIZE)
	        {
	        	char c = block.matrix[y - block.yPos][x - block.xPos];

	            if(c != ' ')
	            	drawSquare(y, x, colors[(int)c - 77]);
	            else
	            {
	            	if(board[y][x] != ' ')
	                	drawSquare(y, x, colors[(int)board[y][x] - 77]);     
	            }
	         }
	         else
	         {
	         	if(board[y][x] != ' ')
	            	drawSquare(y, x, colors[(int)board[y][x] - 77]);
	         }
		}
	}
}


int main(void)
{
	volatile int * interval_timer_ptr = (int *) TIMER_BASE;	
	volatile int * jtag = (int *) JTAG_UART_BASE;
	volatile int * KEY_ptr = (int *) KEY_BASE;		

	//CLEAR THE BOARD JUST IN CASE ==================================================
	clearEverything();	
	
    //SET TIMER =====================================================================
	int counter = 30000000;				
	*(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
	*(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;
	
	//SET INTERRUPTS ================================================================
	*(interval_timer_ptr + 1) = 0x7;	
	*(KEY_ptr + 2) = 0xF; 				
	NIOS2_WRITE_IENABLE( 0x3 );
	NIOS2_WRITE_STATUS( 1 );			

	//SET UP GAME ===================================================================
	init();
	createBlock();
	drawBoard();

	//GAMELOOP ======================================================================
	while(1)
	{
		if( ((*jtag) & 0x8000) != 0x0000)
			pushbutton_ISR();
	}						
}

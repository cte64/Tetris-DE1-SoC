#include "address_map_nios2.h"
#include "tetris.h"

void timerISR( )
{
	volatile int * timerPtr = (int *) TIMER_BASE;

	//CLEAR INTERRUPT 	
	*(timerPtr) = 0; 			

	//UPDATE THE TIMER VALUE
	*(timerPtr + 0x2) = (timeBtw & 0xFFFF);
	*(timerPtr + 0x3) = (timeBtw >> 16) & 0xFFFF;
	*(timerPtr + 1) = 0x7;

	moveBlock(115);
	drawBoard();

	return;
}

#include "address_map_nios2.h"
#include "tetris.h"
#include "address_map_nios2.h"

void pushbutton_ISR( void )
{
	volatile int * KEY_ptr = (int *) KEY_BASE;
  	volatile int * slider_switch_ptr = (int *) SW_BASE;
	int press;
	press = *(KEY_ptr + 3);		// read the pushbutton interrupt register
	*(KEY_ptr + 3) = press;		// Clear the interrupt

	volatile int * jtag = (int *) JTAG_UART_BASE;
	int value = ((*jtag) & 0xff);

	moveBlock(value);
	drawBoard();

	return;
}
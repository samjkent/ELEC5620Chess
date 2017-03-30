/**
 * PS/2 Keyboard Driver
 * Sam Kent
 *
 * Table 1. DE1-SOC COMPUTER SYSTEM WITH ARM CORTEX-A9
 * DE1-SoC Interrupt ID: 79
 *
 * FPGA implements a PS/2 controller and data can be read from it's buffer
 *
 * Requires DATA and CLOCK signals. Key's are encoded as ASCII values.
 * Each data packet contains 11 bits:
 * Start bit:   0
 * 8 data bits: xxxxxxxx
 * Parity bit:  1 or 0
 * Stop bit :   1
 *
 */

#include "kbd_drv.h"

void board_move(int x);

extern int cursor_xy[2];
<<<<<<< HEAD
=======
volatile int break_code = 0;
>>>>>>> refs/remotes/origin/main-dev

void board_move(int direction) {
	// Update cursor position
	switch(direction){
		case 0:
			if(cursor_xy[1] != 0) cursor_xy[1]--;
			break;
		case 1:
			if(cursor_xy[1] < 7) cursor_xy[1]++;
			break;
		case 2:
			if(cursor_xy[0] != 0) cursor_xy[0]--;
			break;
		case 3:
<<<<<<< HEAD
			if(cursor_xy[1] < 7) cursor_xy[0]++;
=======
			if(cursor_xy[0] < 7) cursor_xy[0]++;
>>>>>>> refs/remotes/origin/main-dev
			break;
	}
}

/**
 * Set up PS/2 interface and enable interrupts
 */
void kbd_setup(void){
	*PS2_Control = 0x01;	// Enable interrupts
}

/**
 * Keyboard interrupt
 * When a PS/2 interrupt is detected move cursor
 *
 */
void kbd_interrupt(void){
<<<<<<< HEAD
	char key_pressed = *PS2_Data & 0xFF;

=======
	unsigned char key_pressed = *PS2_Data & 0xFF;

	// Check if previous value was break code
	if(break_code){
		break_code = 0;
		return;
	}

	// Check for break code
	// Ignore next key press
	if(key_pressed == 0xF0){
		break_code = 1;
		return;
	}

	// Key -> Movement
>>>>>>> refs/remotes/origin/main-dev
	switch(key_pressed){
		case KEY_UP:
		case KEY_W:
			board_move(0);
			break;
		case KEY_DOWN:
		case KEY_S:
			board_move(1);
			break;
		case KEY_LEFT:
		case KEY_A:
			board_move(2);
			break;
		case KEY_RIGHT:
		case KEY_D:
			board_move(3);
			break;
		case KEY_SPACE:
		case KEY_ENTER:
		// case KEY_ENTER2:
			// board_select();
			break;
		default:
			// Do nothing
			break;
	}


}

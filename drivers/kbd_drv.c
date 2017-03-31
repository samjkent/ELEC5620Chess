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
extern int selected_xy[2];
volatile int break_code = 0;

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
			if(cursor_xy[0] < 7) cursor_xy[0]++;
			break;
	}
}

/**
 *  Highlight selected square
 */
void board_select(void){
	// Copy current xy position to selected position
	selected_xy[0] = cursor_xy[0];
	selected_xy[1] = cursor_xy[1];
}

/**
 *  Remove selection
 */
void board_deselect(void){
	// Copy current xy position to selected position
	selected_xy[0] = -1;
	selected_xy[1] = -1;
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
		case KEY_ENTER: // Both Enter Keys will work. RHS Enter sends E05A so will still match this case with the second byte
			board_select();
			break;
		case KEY_ESC:
			board_deselect();
		default:
			// Do nothing
			break;
	}


}

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

/**
 * Set up PS/2 interface and enable interrupts
 */
void kbd_setup(void){
	*PS2_Control |= 0x01;	// Enable interrupts
}

/**
 * Keyboard interrupt
 * When a signal is detected at the PS/2 input read in an 11 bit packet
 *
 */
void kbd_interrupt(void){
	char key_pressed = *PS2_Data & 0xFF;

	switch(key_pressed){
		case KEY_UP:
			board_move(UP);
			break;
		case KEY_DOWN:
			board_move(DOWN);
			break;
		case KEY_LEFT:
			board_move(LEFT);
			break;
		case KEY_RIGHT:
			board_move(RIGHT);
			break;
		case KEY_ENTER:
			board_select();
			break;
		default:
			// Do nothing
			break;
	}


}

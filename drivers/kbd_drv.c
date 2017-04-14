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
#include "../graphics_chess.h"

void board_move(int x);

extern int cursor_xy[2];
extern int cursor_menu;
extern int mode;
extern int game_mode;

extern int input_mode;

extern char board_highlight[8];
int refresh_display = 0;
volatile int break_code = 0;

extern int menu_begin;
extern int game_begin;

extern int enter_pressed;

extern int promotion_no;

void board_move(int direction) {
	// Update cursor position
	switch (direction) {
	case 0:
		if (cursor_xy[1] != 0)
			cursor_xy[1]--;
		if (cursor_menu != 0)
			cursor_menu--;
		break;
	case 1:
		if (cursor_xy[1] < 7)
			cursor_xy[1]++;
		if (cursor_menu != 3)
			cursor_menu++;
		break;
	case 2:
		if (cursor_xy[0] != 0)
			cursor_xy[0]--;
		break;
	case 3:
		if (cursor_xy[0] < 7)
			cursor_xy[0]++;
		break;
	}

	// Update board
	// LCD_DrawBoard(board);

}

/**
 *  Remove selection
 */
void board_deselect(void) {
	board_highlight[0] = 0;
	board_highlight[1] = 0;
	board_highlight[2] = 0;
	board_highlight[3] = 0;
	board_highlight[4] = 0;
	board_highlight[5] = 0;
	board_highlight[6] = 0;
	board_highlight[7] = 0;
}

/**
 *  Highlight selected square
 */
void board_select(void) {
	if (mode == 0) {
		// Set mode
		mode = 1;
		game_mode = cursor_menu;

		// Reset cursors
		cursor_xy[0] = 0;
		cursor_xy[1] = 0;
		cursor_menu = 0;

		// Reset game
		game_begin = 1;

	} else if (mode == 1) {
		enter_pressed = 1;

		if (input_mode == 0) {
			input_mode = 1;
		}
		board_deselect();
		// Copy current xy position to selected position
		// Do not update highlight if input_mode == INPUT_END
		if (input_mode != 2) {
			board_highlight[cursor_xy[0]] = 0x80 >> cursor_xy[1];
		}
	}
}

/**
 * Set up PS/2 interface and enable interrupts
 */
void kbd_setup(void) {
	*PS2_Control = 0x01; // Enable interrupts
}

/**
 * Keyboard interrupt
 * When a PS/2 interrupt is detected move cursor
 *
 */
void kbd_interrupt(void) {
	unsigned char key_pressed = *PS2_Data & 0xFF;

	// Check if previous value was break code
	if (break_code) {
		break_code = 0;
		return;
	}

	// Check for break code
	// Ignore next key press
	if (key_pressed == 0xF0) {
		break_code = 1;
		return;
	}

	// Key -> Movement
	switch (key_pressed) {
	case KEY_1:
		promotion_no = 1;
		break;
	case KEY_2:
		promotion_no = 2;
		break;
	case KEY_3:
		promotion_no = 3;
		break;
	case KEY_4:
		promotion_no = 4;
		break;
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
		break;
	case KEY_Q:
		//Quit
		mode = 0;
		// Reset menu
		menu_begin = 1;
		break;
	default:
		// Do nothing
		break;
	}

	// Update display flag
	refresh_display = 1;

}

void key_IRQ_set(unsigned int state){
	*PS2_Control ^= (-state ^ *PS2_Control) & (1 << 0); // nth bit to x
}

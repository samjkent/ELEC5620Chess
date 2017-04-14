#include "DE1-SoC_LT24.h"
#include "Delay.h"
#include "graphics_chess.h"
#include "address_map_arm.h"
#include "font8x8_basic.h"
#include "stdio.h"
#include "string.h"
<<<<<<< HEAD

struct time
{
	unsigned int seconds;
	unsigned int minutes;
	unsigned int hours;
};

struct time time1 = {0,5,0};
//struct time time2 = {0,0,0);

char board[8][8] =
{
	{ROO_B, KNI_B, BIS_B, QUE_B, KIN_B, BIS_B, KNI_B, ROO_B},
	{PAW_B, PAW_B, PAW_B, PAW_B, PAW_B, PAW_B, PAW_B, PAW_B},
	{BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
	{BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
	{BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
	{BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
	{PAW_W, PAW_W, PAW_W, PAW_W, PAW_W, PAW_W, PAW_W, PAW_W},
	{ROO_W, KNI_W, BIS_W, QUE_W, KIN_W, BIS_W, KNI_W, ROO_W}
};

void configure_interrupts(void);

struct time tick(struct time time);
struct time tock(struct time time);
=======
#include "drivers/vga_drv.h"
#include "ChessLogic/ChessInput.h"

struct time {
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
};

struct time time1 = { 0, 0, 0 };
struct time time2 = { 0, 0, 0 };
//struct time time2 = {0,0,0);

/*
 char board[8][8] =
 {
 {ROO_B, KNI_B, BIS_B, QUE_B, KIN_B, BIS_B, KNI_B, ROO_B},
 {PAW_B, PAW_B, PAW_B, PAW_B, PAW_B, PAW_B, PAW_B, PAW_B},
 {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
 {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
 {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
 {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
 {PAW_W, PAW_W, PAW_W, PAW_W, PAW_W, PAW_W, PAW_W, PAW_W},
 {ROO_W, KNI_W, BIS_W, QUE_W, KIN_W, BIS_W, KNI_W, ROO_W}
 };
 */

extern int cursor_xy[2];
extern char board_highlight[8];
extern char last_move_highlight[8];

void configure_interrupts(void);

void tick(struct time *time);
void tock(struct time *time);
>>>>>>> thomas-dev

void decrease_time1(void);
void generate_end_message(char *str_1, char *str_2, struct ChessBoard *board);

// Display Updates
void display_menu(void);
void display_game(void);

// Refresh when something changes
extern int refresh_display;
int mode = 1; // Init to menu
int cursor_menu;

// Menu
#define MENU 0
unsigned char* menu_str;
int menu_begin = 1;

// Game
unsigned char* time1_str;
unsigned char* time2_str;
unsigned char* turn_str;

unsigned char* endgame_str_1;
unsigned char* endgame_str_2;

#define GAME 1
int game_begin = 1;

// Chess input state machine

#define NO_INPUT		0
#define INPUT_START 	1
#define INPUT_END 		2
#define INPUT_PROMOTION 3
int input_mode = 0;
int move_made = 0;
int promotion_no = 0;
struct BoardCoordinate start_coordinate;
struct BoardCoordinate end_coordinate;
struct MoveCoordinateList end_move_list;

int enter_pressed = 0;

// IR Test
void send_ir_byte(char);

//Serial comms test
void send_data(int data);
struct ChessBoard chess_board;

int main() {

	// ARM A9 Private Timer load, value, control and interrupt addresses
	//volatile unsigned int *private_timer_load       = (unsigned int *) MPCORE_PRIV_TIMER;
	//volatile unsigned int *private_timer_value		= (unsigned int *) MPCORE_PRIV_TIMER + 0x4;
	//volatile unsigned int *private_timer_control    = (unsigned int *) MPCORE_PRIV_TIMER + 0x8;
	//volatile unsigned int *private_timer_interrupt  = (unsigned int *) MPCORE_PRIV_TIMER + 0xC;
	//volatile unsigned int *LED_ptr					= (unsigned int *) LEDR_BASE;

	configure_interrupts();

	//Clear watchdog
	ResetWDT(); //watchdog reset function. Cleaner than lots of wdt_csr writes

	// Initialise LCD
	LCD_Init();
	ResetWDT();
	vga_draw_test();
	// Initialise timer
	utimer_irq(1000000); // set 1s timer w/ auto reload

<<<<<<< HEAD
	LCD_DrawBoard(board);

	while (1)
	{
		Delay_Ms(1000); //1s delay

		sprintf(time_str,"%02d:%02d:%02d",time1.hours,time1.minutes,time1.seconds);

		LCD_PutStr(1,1,time_str,LCD_WHITE,LCD_BLACK);

		time1 = tock(time1);
=======
	while (1) {
		switch (mode) {
		case MENU:
			display_menu();
			break;
		case GAME:
			display_game();
			break;
		default:
			// Should be unreachable
			while (1)
				;
			break;
		}

		// Sleep until PS/2 or timer interrupt
		__asm("WFE");

		ResetWDT();

	}

	return 0; //Unreachable
}

void display_menu(void) {
	if (refresh_display) {
		if (menu_begin) {
			vga_draw_test();
			LCD_Clear(LCD_BLACK);
			menu_begin = 0;
		}

		// Write menu
		sprintf(menu_str, "DE1-SoC Chess");
		LCD_PutStr(20, 20, menu_str, LCD_BLACK, LCD_WHITE);

		sprintf(menu_str, "1P vs AI");
		LCD_PutStr(20, 30, menu_str, LCD_BLACK, LCD_WHITE);
>>>>>>> thomas-dev

		sprintf(menu_str, "2P - Local");
		LCD_PutStr(20, 40, menu_str, LCD_BLACK, LCD_WHITE);

		sprintf(menu_str, "2P - Serial");
		LCD_PutStr(20, 50, menu_str, LCD_BLACK, LCD_WHITE);

		sprintf(menu_str, "Highscores");
		LCD_PutStr(20, 60, menu_str, LCD_BLACK, LCD_WHITE);

		// Clear cursor
		sprintf(menu_str, " ");
		LCD_PutStr(10, 30, menu_str, LCD_BLACK, LCD_WHITE);
		LCD_PutStr(10, 40, menu_str, LCD_BLACK, LCD_WHITE);
		LCD_PutStr(10, 50, menu_str, LCD_BLACK, LCD_WHITE);
		LCD_PutStr(10, 60, menu_str, LCD_BLACK, LCD_WHITE);

		// Draw cursor
		sprintf(menu_str, ">");
		LCD_PutStr(10, 30 + (cursor_menu * 10), menu_str, LCD_BLACK, LCD_WHITE);
	}
}

<<<<<<< HEAD
struct time tick(struct time time)
{
	time.seconds++;
	if (time.seconds > 59) { time.seconds = 0; time.minutes++; }
	if (time.minutes > 59) { time.minutes = 0; time.hours++; }
	if (time.hours > 23) { time.hours = 0; }

	ResetWDT();

	return time;
}

struct time tock(struct time time)
{
	time.seconds--;
	if (time.seconds > 59) { time.seconds = 59; time.minutes--; }
	if (time.minutes > 59) { time.minutes = 59; time.hours--; }
	if (time.hours > 23) { time.hours = 23; }
=======
void display_game(void) {
	int i;

	// INIT
	// Initial run
	if (game_begin) {
		// Initialise chess board
		initChessBoard(&chess_board);
		for (i = 0; i < 8; i++)
		{
			last_move_highlight[i] = 0;
		}
		// Clear LCD
		LCD_Clear(LCD_BLACK);
		vga_draw_test();

		// Set timer
		time1.hours = 0;
		time1.minutes = 5; // 5 minute countdown
		time1.seconds = 0;
		time2.hours = 0;
		time2.minutes = 5; // 5 minute countdown
		time2.seconds = 0;

		// Clear game_begin flag
		game_begin = 0;

		send_data(0xF0F0);

		//send_ir_byte(0x00);
		//send_ir_byte(0x44);
	}

	// UPDATE
	// Check for input + test logic

	switch (input_mode) {
	case NO_INPUT:
		break;
	case INPUT_START:
		if (enter_pressed == 1) {
			enter_pressed = 0;
			//NOTE: The chess board's (0,0) = square A1
			start_coordinate = BoardCoordinateConstructor(cursor_xy[0],
					7 - cursor_xy[1]);

			inputMoveStart(&chess_board, start_coordinate, &end_move_list);

			if (end_move_list.move_count > 0) {
				for (i = 0; i < end_move_list.move_count; i++) {
					board_highlight[end_move_list.moves[i].x] |= 0x1 << end_move_list.moves[i].y;
				}

				input_mode = INPUT_END;
			}
		}

		break;

	case INPUT_END:
		if (enter_pressed == 1) {
			enter_pressed = 0;

			end_coordinate = BoardCoordinateConstructor(cursor_xy[0],7 - cursor_xy[1]);
			if (isMoveInMoveList(end_move_list, end_coordinate)) {
				inputEndMove(&chess_board, start_coordinate, end_coordinate);

				// Check for pawn promotion
				if (chess_board.promotion & 0x88) {
					input_mode = INPUT_PROMOTION;
				} else {
					move_made = 1;
				}
			}
		}
		break;
	case INPUT_PROMOTION:
		// promotion_no = (convert keyboard input to a number)
		//chess_board = inputPawnPromotion(chess_board, promotion_no);
		move_made = 1;
		break;
	default:
		// Should be unreachable
		while (1)
			;
		break;
	}

	if (move_made) {
		move_made = 0;
		for (i = 0; i < 8; i++)
		{
			last_move_highlight[i] = 0x00;
		}
		last_move_highlight[start_coordinate.x] |= 0x1 << start_coordinate.y;
		last_move_highlight[end_coordinate.x] 	|= 0x1 << end_coordinate.y;
		input_mode = NO_INPUT;
		endGameCheck(&chess_board);
	}

	// DRAW
	// Refresh the display only when something changes
	if (refresh_display) {
		sprintf(time1_str, "%02d:%02d:%02d", time1.hours, time1.minutes, time1.seconds);
		LCD_PutStr(1, 1, time1_str, LCD_WHITE, LCD_BLACK);
		sprintf(time2_str, "%02d:%02d:%02d", time2.hours, time2.minutes, time2.seconds);
		LCD_PutStr((LCD_WIDTH - 1) - (strlen(time2_str)*8), 1, time2_str, LCD_WHITE, LCD_BLACK);
>>>>>>> thomas-dev

		if (chess_board.white_turn) { sprintf(turn_str,"White to move"); }
		else { sprintf(turn_str,"Black to move"); }
		LCD_PutStr((240/2) - ((strlen(turn_str)/2) * 8),240,turn_str,LCD_WHITE,LCD_BLACK);

		if (chess_board.end_game)
		{
			generate_end_message(endgame_str_1, endgame_str_2, &chess_board);
			sprintf(turn_str,"             ");
			LCD_PutStr((240/2) - ((strlen(turn_str)/2) * 8),240,turn_str,LCD_WHITE,LCD_BLACK); //clear turn state message
			LCD_PutStr((240/2) - ((strlen(endgame_str_1)/2) * 8),232,endgame_str_1,LCD_WHITE,LCD_BLACK);
			LCD_PutStr((240/2) - ((strlen(endgame_str_2)/2) * 8),240,endgame_str_2,LCD_WHITE,LCD_BLACK);
		}

		LCD_DrawBoard(chess_board.board);
		refresh_display = 0;
	}
}

void tick(struct time *time) {
	time->seconds++;
	if (time->seconds > 59) {
		time->seconds = 0;
		time->minutes++;
	}
	if (time->minutes > 59) {
		time->minutes = 0;
		time->hours++;
	}
	if (time->hours > 23) {
		time->hours = 0;
	}

	ResetWDT();

}

void tock(struct time *time) {
	time->seconds--;
	if (time->seconds > 59) {
		time->seconds = 59;
		time->minutes--;
	}
	if (time->minutes > 59) {
		time->minutes = 59;
		time->hours--;
	}
	if (time->hours > 23) {
		time->hours = 23;
	}

	ResetWDT();
}

void decrease_time1(void) {
	// Clear interrupt
	*(unsigned int *) 0xFFFEC60C = 0x1;

	if (chess_board.white_turn == 1) //update white timer if white to move
	{
		tock(&time1);
	}
	else if (chess_board.white_turn == 0) //update black timer if black to move
	{
		tock(&time2);
	}

	refresh_display = 1;
	ResetWDT();

<<<<<<< HEAD
	return time;
=======
>>>>>>> thomas-dev
}

void generate_end_message(char *str_1, char *str_2, struct ChessBoard *board)
{
	if (board->end_game & 0x01)
	{
		sprintf(str_1,"Checkmate!");
		sprintf(str_2,"Black wins!");
	}
	else if (board->end_game & 0x02)
	{
		sprintf(str_1,"Checkmate!");
		sprintf(str_2,"White wins!");
	}
	else if (board->end_game & 0x04)
	{
		sprintf(str_1,"Stalemate!");
		sprintf(str_2,"Game is drawn!");
	}
	else if (board->end_game & 0x08)
	{
		sprintf(str_1,"Insufficient mating material!");
		sprintf(str_2,"Game is drawn!");
	}
	else if (board->end_game & 0x10)
	{
		sprintf(str_1,"50 move rule!");
		sprintf(str_2,"Game is drawn!");
	}
	else if (board->end_game & 0x20)
	{
		sprintf(str_1,"Threefold Repetition!");
		sprintf(str_2,"Game is drawn!");
	}
	else
	{
		if (board->king_info & 0x08)
		{
			sprintf(str_1,"White in check");
		}
		else if (board->king_info & 0x80)
		{
			sprintf(str_1,"Black in check");
		}

		if (board->white_turn)
		{
			sprintf(str_2,"White to move");
		}
		else
		{
			sprintf(str_2,"Black to move");
		}
	}
}


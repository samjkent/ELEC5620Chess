
#include "DE1-SoC_LT24.h"
#include "Delay.h"
#include "graphics_chess.h"
#include "address_map_arm.h"
#include "font8x8_basic.h"
#include "stdio.h"
#include "string.h"
#include "drivers/vga_drv.h"
#include "ChessLogic/ChessInput.h"


struct time
{
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
};

struct time time1 = {0,0,0};
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


void configure_interrupts(void);

struct time tick(struct time time);
struct time tock(struct time time);

void decrease_time1(void);

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
unsigned char* time_str;
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


// IR Test
void send_ir_byte(char);

//Serial comms test
void send_data(int data);
struct ChessBoard chess_board;

int main()
{

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

	while (1)
	{
		switch(mode){
			case MENU:
				display_menu();
				break;
			case GAME:
				display_game();
				break;
			default:
				// Should be unreachable
				while(1);
				break;
		}

		// Sleep until PS/2 or timer interrupt
		__asm("WFE");

		ResetWDT();

	}
    
	return 0; //Unreachable
}

void display_menu(void){
	if(refresh_display){
		if(menu_begin){
			vga_draw_test();
			LCD_Clear(LCD_BLACK);
			menu_begin = 0;
		}

		// Write menu
		sprintf(menu_str,"DE1-SoC Chess");
		LCD_PutStr(20,20,menu_str,LCD_BLACK,LCD_WHITE);

		sprintf(menu_str,"1P vs AI");
		LCD_PutStr(20,30,menu_str,LCD_BLACK,LCD_WHITE);

		sprintf(menu_str,"2P - Local");
		LCD_PutStr(20,40,menu_str,LCD_BLACK,LCD_WHITE);

		sprintf(menu_str,"2P - Serial");
		LCD_PutStr(20,50,menu_str,LCD_BLACK,LCD_WHITE);

		sprintf(menu_str,"Highscores");
		LCD_PutStr(20,60,menu_str,LCD_BLACK,LCD_WHITE);

		// Clear cursor
		sprintf(menu_str," ");
		LCD_PutStr(10,30 ,menu_str,LCD_BLACK,LCD_WHITE);
		LCD_PutStr(10,40 ,menu_str,LCD_BLACK,LCD_WHITE);
		LCD_PutStr(10,50 ,menu_str,LCD_BLACK,LCD_WHITE);
		LCD_PutStr(10,60 ,menu_str,LCD_BLACK,LCD_WHITE);

		// Draw cursor
		sprintf(menu_str,">");
		LCD_PutStr(10,30 + (cursor_menu * 10),menu_str,LCD_BLACK,LCD_WHITE);
	}
}

void display_game(void){
	int i;

	// INIT
	// Initial run
	if(game_begin){
		// Initialise chess board
		initChessBoard(&chess_board);

		// Clear LCD
		LCD_Clear(LCD_BLACK);
		vga_draw_test();

		// Set timer
		time1.hours = 0;
		time1.minutes = 5; 	 // 5 minute countdown
		time1.seconds = 0;

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

			//start_coordinate = (convert keyboard input to board coordinate here)
			//NOTE: Use 'BoardCoordinateConstructor()' found in 'ChessBoard.c'
			//NOTE: The chess board's (0,0) = square A1
			start_coordinate = BoardCoordinateConstructor(cursor_xy[0],7 - cursor_xy[1]);

			inputMoveStart(&chess_board, start_coordinate, &end_move_list);

			if (end_move_list.move_count > 0)
			{
				for (i = 0; i < end_move_list.move_count; i++)
				{
					board_highlight[end_move_list.moves[i].x] = 0x1 << end_move_list.moves[i].y;
				}

				input_mode = INPUT_END;
			}
			break;
		case INPUT_END:
			//end_coordinate = (convert keyboard input to board coordinate here)
			if (isMoveInMoveList(end_move_list, end_coordinate))
			{
				inputEndMove(&chess_board, start_coordinate, end_coordinate);

				// Check for pawn promotion
				if (chess_board.promotion & 0x88)
				{
					input_mode = INPUT_PROMOTION;
				}
				else
				{
					move_made = 1;
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
			while (1);
			break;
		}

		if (move_made)
		{
			move_made = 0;
			input_mode = NO_INPUT;
			//chess_board = endGameCheck(chess_board);
		}

	// DRAW
	// Refresh the display only when something changes
	if(refresh_display){
		sprintf(time_str,"%02d:%02d:%02d",time1.hours,time1.minutes,time1.seconds);
		LCD_PutStr(1,1,time_str,LCD_WHITE,LCD_BLACK);
		LCD_DrawBoard(chess_board.board);
		refresh_display = 0;
	}
}

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

	ResetWDT();

	return time;
}

void decrease_time1(void)
{
	// Clear interrupt
	*(unsigned int *) 0xFFFEC60C = 0x1;

	time1.seconds--;
	if (time1.seconds > 59) { time1.seconds = 59; time1.minutes--; }
	if (time1.minutes > 59) { time1.minutes = 59; time1.hours--; }
	if (time1.hours > 23) { time1.hours = 23; }

	refresh_display = 1;
	ResetWDT();

}


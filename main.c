<<<<<<< HEAD
#include "DE1-SoC_LT24.h"
#include "Delay.h"
#include "graphics_chess.h"
#include "address_map_arm.h"
#include "font8x8_basic.h"
#include "string.h"


#define TILE 24

enum chess_pieces {
	BLANK,
	PAW_W,
	KNI_W,
	BIS_W,
	ROO_W,
	QUE_W,
	KIN_W,
	PAW_B,
	KNI_B,
	BIS_B,
	ROO_B,
	QUE_B,
	KIN_B,
	ERROR
};

enum chess_pieces board[8][8] =
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

char board_highlight[8]= { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

int cursor_xy[2] = {0, 0};

void LCD_DrawTile(int x, int y, unsigned char graphics_tile[ROWS_24_24][COLS_24_24], unsigned short bg_colour, unsigned short fg_colour);
void LCD_DrawBoard(enum chess_pieces board[8][8]);

void LCD_PutChar(int x, int y, unsigned char ch, unsigned short bg_colour, unsigned short fg_colour);
void LCD_PutStr(int x, int y, unsigned char * string, unsigned short bg_colour, unsigned short fg_colour);

void LCD_DrawLine(int x, int y, int dx, int dy, unsigned short colour);
void LCD_DrawRect(int x, int y, int dx, int dy, unsigned short colour);
void DrawCursor(int x, int y);

void configure_interrupts(void);

int main()
{

    // ARM A9 Private Timer load, value, control and interrupt addresses
    //volatile unsigned int *private_timer_load       = (unsigned int *) MPCORE_PRIV_TIMER;
    //volatile unsigned int *private_timer_value		= (unsigned int *) MPCORE_PRIV_TIMER + 0x4;
    //volatile unsigned int *private_timer_control    = (unsigned int *) MPCORE_PRIV_TIMER + 0x8;
    //volatile unsigned int *private_timer_interrupt  = (unsigned int *) MPCORE_PRIV_TIMER + 0xC;
    //volatile unsigned int *LED_ptr					= (unsigned int *) LEDR_BASE;

    //unsigned char * cal = "CALUM";

    configure_interrupts();

    //Clear watchdog
    ResetWDT(); //watchdog reset function. Cleaner than lots of wdt_csr writes

	// Initialise LCD
	LCD_Init();
	ResetWDT();



	while (1)
	{
		usleep(20000);

		LCD_DrawBoard(board);
		//LCD_DrawRect(24,24,23,23,LCD_CYAN);
		//DrawCursor(cursor_xy[0],cursor_xy[1]);
		//LCD_PutStr(10,10,cal,LCD_WHITE,LCD_BLACK);
		//LCD_PutStr(0,280,"AaBbCcDdEeFfGgHhIiJjKkLlMm",LCD_BLUE,LCD_RED);
		//LCD_PutStr(0,289,"NnOoPpQqRrSsTtUuVvWwXxYyZz",LCD_GREEN,LCD_YELLOW);
		//LCD_PutStr(0,298,"0123456789.,/?!#@()+-*=   ",LCD_CYAN,LCD_MAGENTA);

		ResetWDT();

	}
    

}


void LCD_DrawTile(int x, int y, unsigned char graphics_tile[ROWS_24_24][COLS_24_24], unsigned short bg_colour, unsigned short fg_colour)
{
	int i, j, k;
	unsigned short buffer[576];



	for (j = 0; j < TILE; j++)
	{
		for (i = 0; i < 3; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if (graphics_tile[j][i] & (0x80 >> k))
				{
					buffer[(j*TILE)+((i*8)+k)] = fg_colour;
					//LCD_WR_DATA(fg_colour);
				}
				else
				{
					buffer[(j*TILE)+((i*8)+k)] = bg_colour;
					//LCD_WR_DATA(bg_colour);
				}
			}
		}
	}

	LCD_Window(x*TILE, y*TILE, TILE, TILE);
	LCD_Framebuffer(buffer,576);

	ResetWDT();
}



void LCD_DrawBoard(enum chess_pieces board[8][8])
{
	int i, j;
	unsigned short bg_colour;

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (i%2 == j%2) { bg_colour = LCD_LIGHTBROWN; }
			else { bg_colour = LCD_DARKBROWN; }

			switch (board[j][i])
			{
			case BLANK :
				LCD_DrawTile(i+1, j+1, graphics_blank, bg_colour, LCD_WHITE);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case PAW_W :
				LCD_DrawTile(i+1, j+1, graphics_pawn, bg_colour, LCD_WHITE);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case KNI_W :
				LCD_DrawTile(i+1, j+1, graphics_knight, bg_colour, LCD_WHITE);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case BIS_W :
				LCD_DrawTile(i+1, j+1, graphics_bishop, bg_colour, LCD_WHITE);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case ROO_W :
				LCD_DrawTile(i+1, j+1, graphics_rook, bg_colour, LCD_WHITE);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case QUE_W :
				LCD_DrawTile(i+1, j+1, graphics_queen, bg_colour, LCD_WHITE);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case KIN_W :
				LCD_DrawTile(i+1, j+1, graphics_king, bg_colour, LCD_WHITE);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case PAW_B :
				LCD_DrawTile(i+1, j+1, graphics_pawn, bg_colour, LCD_BLACK);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case KNI_B :
				LCD_DrawTile(i+1, j+1, graphics_knight, bg_colour, LCD_BLACK);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case BIS_B :
				LCD_DrawTile(i+1, j+1, graphics_bishop, bg_colour, LCD_BLACK);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case ROO_B :
				LCD_DrawTile(i+1, j+1, graphics_rook, bg_colour, LCD_BLACK);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case QUE_B :
				LCD_DrawTile(i+1, j+1, graphics_queen, bg_colour, LCD_BLACK);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			case KIN_B :
				LCD_DrawTile(i+1, j+1, graphics_king, bg_colour, LCD_BLACK);
				if (board_highlight[i] & 0x80 >> j) { LCD_DrawRect((i+1) * 24, (j+1) * 24, TILE, TILE, LCD_CYAN); }
				break;
			default :
				//LCD_DrawTile(i+1, j+1, graphics_blank, bg_colour, LCD_WHITE);
				break;
			}

			DrawCursor(cursor_xy[0],cursor_xy[1]);

		}
	}

	//DrawCursor(cursor_xy[0],cursor_xy[1]);

	ResetWDT();
}


void LCD_PutChar(int x, int y, unsigned char ch, unsigned short bg_colour, unsigned short fg_colour)
{
	int i, j;
	unsigned short buffer[64] = {0};

	if ( x < 0 ) { x = 0; }
	if ( y < 0 ) { y = 0; }

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if ( font8x8_basic[ch][i] & 1 << j)
			{
				buffer[(8*i) + j] = fg_colour;
				//LCD_WR_DATA(fg_colour);
			}
			else
			{
				buffer[(8*i) + j] = bg_colour;
				//LCD_WR_DATA(bg_colour);
			}
		}
	}

	LCD_Window(x, y, 8, 8);
	LCD_Framebuffer(buffer,64);

	ResetWDT();
}

void LCD_PutStr(int x, int y, unsigned char * string, unsigned short bg_colour, unsigned short fg_colour)
{
	int i, j, n;
	int len = strlen(string);
	unsigned short buffer[76800] = {0};

	if ( x < 0 ) { x = 0; }
	if ( y < 0 ) { y = 0; }


	for (n = 0; n < len; n++)
	{
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				if ( font8x8_basic[string[n]][i] & 1 << j)
				{
					buffer[(8*i*len)+((8*n)+j)] = fg_colour;
					ResetWDT();
				}
				else
				{
					buffer[(8*i*len)+((8*n)+j)] = bg_colour;
					ResetWDT();
				}
			}
		}
	}

	LCD_Window(x, y, 8 * len, 8);
	LCD_Framebuffer(buffer, 64*len);

	ResetWDT();
}

void LCD_DrawLine(int x, int y, int dx, int dy, unsigned short colour)
{
	int i, j;
	unsigned short buffer[76800] = {0};

	for (i = 0; i < dx; i++)
	{
		for (j = 0; j < dy; j++)
		{

		}
	}


	LCD_Window(x,y,dx,dy);
	LCD_Framebuffer(buffer,dx*dy);

	ResetWDT();

}

//Draw an orthogonal vector either Left->Right or Top->Bottom
void LCD_DrawOrthoVect(int x, int y, int dir, int len, unsigned short colour)
{
	int i;
	unsigned short buffer[LCD_HEIGHT];

	for (i = 0; i < 320; i++) { buffer[i] = colour; }

	if (dir) { LCD_Window(x,y,len,1); }
	else if (!dir) { LCD_Window(x,y,1,len); }

	LCD_Framebuffer(buffer,len);

	ResetWDT();

}

void LCD_DrawRect(int x, int y, int dx, int dy, unsigned short colour)
{
	LCD_DrawOrthoVect(x,y,0,dx,colour);
	LCD_DrawOrthoVect(x,y,1,dy,colour);
	LCD_DrawOrthoVect(x+dx-1,y,0,dy,colour);
	LCD_DrawOrthoVect(x,y+dy-1,1,dx,colour);

	ResetWDT();
}

void DrawCursor(int x, int y)
{
	LCD_DrawRect((x+1) * 24, (y+1) * 24, TILE, TILE, LCD_MAGENTA);
}


=======
#include "DE1-SoC_LT24.h"
#include "Delay.h"
#include "graphics_chess.h"
#include "address_map_arm.h"
#include "font8x8_basic.h"
#include "string.h"


#define TILE 24

enum chess_pieces {
	BLANK,
	PAW_W,
	KNI_W,
	BIS_W,
	ROO_W,
	QUE_W,
	KIN_W,
	PAW_B,
	KNI_B,
	BIS_B,
	ROO_B,
	QUE_B,
	KIN_B,
	ERROR
};

enum chess_pieces board[8][8] =
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

extern char board_highlight[8][8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

int cursor_xy[2] = {0, 0};

void LCD_DrawTile(int x, int y, unsigned char graphics_tile[ROWS_24_24][COLS_24_24], unsigned short bg_colour, unsigned short fg_colour);
void LCD_DrawBoard(enum chess_pieces board[8][8]);

void LCD_PutChar(int x, int y, unsigned char ch, unsigned short bg_colour, unsigned short fg_colour);
void LCD_PutStr(int x, int y, unsigned char * string, unsigned short bg_colour, unsigned short fg_colour);

void LCD_DrawLine(int x, int y, int dx, int dy, unsigned short colour);
void LCD_DrawRect(int x, int y, int dx, int dy, unsigned short colour);
void DrawCursor(int x, int y);

void configure_interrupts(void);

int main()
{

    // ARM A9 Private Timer load, value, control and interrupt addresses
    //volatile unsigned int *private_timer_load       = (unsigned int *) MPCORE_PRIV_TIMER;
    //volatile unsigned int *private_timer_value		= (unsigned int *) MPCORE_PRIV_TIMER + 0x4;
    //volatile unsigned int *private_timer_control    = (unsigned int *) MPCORE_PRIV_TIMER + 0x8;
    //volatile unsigned int *private_timer_interrupt  = (unsigned int *) MPCORE_PRIV_TIMER + 0xC;
    //volatile unsigned int *LED_ptr					= (unsigned int *) LEDR_BASE;

    //unsigned char * cal = "CALUM";

    configure_interrupts();

    //Clear watchdog
    ResetWDT(); //watchdog reset function. Cleaner than lots of wdt_csr writes

	// Initialise LCD
	LCD_Init();
	ResetWDT();



	while (1)
	{
		usleep(20000);

		LCD_DrawBoard(board);
		//LCD_DrawRect(24,24,23,23,LCD_CYAN);
		DrawCursor(cursor_xy[0],cursor_xy[1]);
		//LCD_PutStr(10,10,cal,LCD_WHITE,LCD_BLACK);
		//LCD_PutStr(0,280,"AaBbCcDdEeFfGgHhIiJjKkLlMm",LCD_BLUE,LCD_RED);
		//LCD_PutStr(0,289,"NnOoPpQqRrSsTtUuVvWwXxYyZz",LCD_GREEN,LCD_YELLOW);
		//LCD_PutStr(0,298,"0123456789.,/?!#@()+-*=   ",LCD_CYAN,LCD_MAGENTA);



		ResetWDT();

	}


}


void LCD_DrawTile(int x, int y, unsigned char graphics_tile[ROWS_24_24][COLS_24_24], unsigned short bg_colour, unsigned short fg_colour)
{
	int i, j, k;
	unsigned short buffer[576];



	for (j = 0; j < TILE; j++)
	{
		for (i = 0; i < 3; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if (graphics_tile[j][i] & (0x80 >> k))
				{
					buffer[(j*TILE)+((i*8)+k)] = fg_colour;
					//LCD_WR_DATA(fg_colour);
				}
				else
				{
					buffer[(j*TILE)+((i*8)+k)] = bg_colour;
					//LCD_WR_DATA(bg_colour);
				}
			}
		}
	}

	LCD_Window(x*TILE, y*TILE, TILE, TILE);
	LCD_Framebuffer(buffer,576);

	ResetWDT();
}



void LCD_DrawBoard(enum chess_pieces board[8][8])
{
	int i, j;
	unsigned short bg_colour;

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (i%2 == j%2) { bg_colour = LCD_LIGHTBROWN; }
			else { bg_colour = LCD_DARKBROWN; }

			switch (board[j][i])
			{
			case BLANK :
				LCD_DrawTile(i+1, j+1, graphics_blank, bg_colour, LCD_WHITE);
				break;
			case PAW_W :
				LCD_DrawTile(i+1, j+1, graphics_pawn, bg_colour, LCD_WHITE);
				break;
			case KNI_W :
				LCD_DrawTile(i+1, j+1, graphics_knight, bg_colour, LCD_WHITE);
				break;
			case BIS_W :
				LCD_DrawTile(i+1, j+1, graphics_bishop, bg_colour, LCD_WHITE);
				break;
			case ROO_W :
				LCD_DrawTile(i+1, j+1, graphics_rook, bg_colour, LCD_WHITE);
				break;
			case QUE_W :
				LCD_DrawTile(i+1, j+1, graphics_queen, bg_colour, LCD_WHITE);
				break;
			case KIN_W :
				LCD_DrawTile(i+1, j+1, graphics_king, bg_colour, LCD_WHITE);
				break;
			case PAW_B :
				LCD_DrawTile(i+1, j+1, graphics_pawn, bg_colour, LCD_BLACK);
				break;
			case KNI_B :
				LCD_DrawTile(i+1, j+1, graphics_knight, bg_colour, LCD_BLACK);
				break;
			case BIS_B :
				LCD_DrawTile(i+1, j+1, graphics_bishop, bg_colour, LCD_BLACK);
				break;
			case ROO_B :
				LCD_DrawTile(i+1, j+1, graphics_rook, bg_colour, LCD_BLACK);
				break;
			case QUE_B :
				LCD_DrawTile(i+1, j+1, graphics_queen, bg_colour, LCD_BLACK);
				break;
			case KIN_B :
				LCD_DrawTile(i+1, j+1, graphics_king, bg_colour, LCD_BLACK);
				break;
			default :
				LCD_DrawTile(i+1, j+1, graphics_blank, bg_colour, LCD_WHITE);
				break;
			}

			//DrawCursor(cursor_xy[0],cursor_xy[1]);

		}
	}

	//DrawCursor(cursor_xy[0],cursor_xy[1]);

	ResetWDT();
}


void LCD_PutChar(int x, int y, unsigned char ch, unsigned short bg_colour, unsigned short fg_colour)
{
	int i, j;
	unsigned short buffer[64] = {0};

	if ( x < 0 ) { x = 0; }
	if ( y < 0 ) { y = 0; }

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if ( font8x8_basic[ch][i] & 1 << j)
			{
				buffer[(8*i) + j] = fg_colour;
				//LCD_WR_DATA(fg_colour);
			}
			else
			{
				buffer[(8*i) + j] = bg_colour;
				//LCD_WR_DATA(bg_colour);
			}
		}
	}

	LCD_Window(x, y, 8, 8);
	LCD_Framebuffer(buffer,64);

	ResetWDT();
}

void LCD_PutStr(int x, int y, unsigned char * string, unsigned short bg_colour, unsigned short fg_colour)
{
	int i, j, n;
	int len = strlen(string);
	unsigned short buffer[76800] = {0};

	if ( x < 0 ) { x = 0; }
	if ( y < 0 ) { y = 0; }


	for (n = 0; n < len; n++)
	{
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				if ( font8x8_basic[string[n]][i] & 1 << j)
				{
					buffer[(8*i*len)+((8*n)+j)] = fg_colour;
					ResetWDT();
				}
				else
				{
					buffer[(8*i*len)+((8*n)+j)] = bg_colour;
					ResetWDT();
				}
			}
		}
	}

	LCD_Window(x, y, 8 * len, 8);
	LCD_Framebuffer(buffer, 64*len);

	ResetWDT();
}

void LCD_DrawLine(int x, int y, int dx, int dy, unsigned short colour)
{
	int i, j;
	unsigned short buffer[76800] = {0};

	for (i = 0; i < dx; i++)
	{
		for (j = 0; j < dy; j++)
		{

		}
	}


	LCD_Window(x,y,dx,dy);
	LCD_Framebuffer(buffer,dx*dy);

	ResetWDT();

}

//Draw an orthogonal vector either Left->Right or Top->Bottom
void LCD_DrawOrthoVect(int x, int y, int dir, int len, unsigned short colour)
{
	int i;
	unsigned short buffer[LCD_HEIGHT];

	for (i = 0; i < 320; i++) { buffer[i] = colour; }

	if (dir) { LCD_Window(x,y,len,1); }
	else if (!dir) { LCD_Window(x,y,1,len); }

	LCD_Framebuffer(buffer,len);

	ResetWDT();

}

void LCD_DrawRect(int x, int y, int dx, int dy, unsigned short colour)
{
	LCD_DrawOrthoVect(x,y,0,dx,colour);
	LCD_DrawOrthoVect(x,y,1,dy,colour);
	LCD_DrawOrthoVect(x+dx-1,y,0,dy,colour);
	LCD_DrawOrthoVect(x,y+dy-1,1,dx,colour);

	ResetWDT();
}

void DrawCursor(int x, int y)
{
	LCD_DrawRect((x+1) * 24, (y+1) * 24, TILE, TILE, LCD_MAGENTA);
}

>>>>>>> refs/remotes/origin/main-dev

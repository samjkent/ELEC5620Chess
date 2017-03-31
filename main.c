
#include "DE1-SoC_LT24.h"
#include "Delay.h"
#include "graphics_chess.h"
#include "address_map_arm.h"
#include "stdio.h"


struct time {
	unsigned int seconds;
	unsigned int minutes;
	unsigned int hours;
};

struct time time1 = {0,0,0};
//struct time time2 = {0,0,0);

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

void configure_interrupts(void);

struct time tick(struct time time);

int main()
{

	unsigned char time_str[8];

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



	while (1)
	{
		usleep(20000);

		LCD_DrawBoard(board);

		sprintf(time_str,"%02d:%02d:%02d",time1.seconds,time1.minutes,time1.hours);

		// LCD_PutStr(0,0,time_str,LCD_WHITE,LCD_BLACK);

		//time1 = tick(time1);

		ResetWDT();

	}
    
	return 0; //Unreachable
}

struct time tick(struct time time)
{
	time.seconds++;
	if (time.seconds > 59) { time.seconds = 0; time.minutes++; }
	if (time.minutes > 59) { time.minutes = 0; time.hours++; }
	if (time.hours > 24) { time.hours = 0; }

	return time;

	ResetWDT();
}

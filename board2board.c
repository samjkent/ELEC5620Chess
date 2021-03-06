#include "address_map_arm.h"
#include "graphics_chess.h"
#include "ChessLogic/ChessBoard.h"

#define JP1_BASE 				0xFF200060
#define INTERVAL_TIMER_BASE 	0xFF202000
#define TX_PIN 					0
#define RX_PIN					1
#define BIT_PERIOD 				10000

volatile unsigned int 	*JP1_DATA 		= (unsigned int *) 0xFF200060;
volatile unsigned int 	*JP1_DATA_DIR	= (unsigned int *) 0xFF200064;
volatile unsigned int 	*JP1_IRQ_MASK	= (unsigned int *) 0xFF200068;
volatile unsigned int 	*JP1_EDGE_CAP	= (unsigned int *) 0xFF20006C;




volatile unsigned int	*HPS_TIMER0_LOAD	= (unsigned int *) 0xFFC08000;
volatile unsigned int	*HPS_TIMER0_COUNTER	= (unsigned int *) 0xFFC08004;
volatile unsigned int	*HPS_TIMER0_CONTROL = (unsigned int *) 0xFFC08008;
volatile unsigned int	*HPS_TIMER0_EOI   	= (unsigned int *) 0xFFC0800C;
volatile unsigned int	*HPS_TIMER0_IRQ    	= (unsigned int *) 0xFFC08010;

volatile unsigned int	*HPS_TIMER1_LOAD	= (unsigned int *) 0xFFC09000;
volatile unsigned int	*HPS_TIMER1_COUNTER	= (unsigned int *) 0xFFC09004;
volatile unsigned int	*HPS_TIMER1_CONTROL = (unsigned int *) 0xFFC09008;
volatile unsigned int	*HPS_TIMER1_EOI   	= (unsigned int *) 0xFFC0900C;
volatile unsigned int	*HPS_TIMER1_IRQ    	= (unsigned int *) 0xFFC09010;



volatile unsigned int *IR_DATA			= (unsigned int *) 0xFF201020;
volatile unsigned int *IR_CONTROL		= (unsigned int *) 0xFF201024;

extern char board[8][8];
int bit_count = 0;
int tx_data;
int rx_data;

char buffer[2];

extern int input_mode;
extern struct BoardCoordinate start_coordinate;
extern struct BoardCoordinate end_coordinate;
extern promotion_no;


void JP1_init(void)
{
	*JP1_DATA_DIR |= 0x1;
	*JP1_IRQ_MASK &= ~0x2; //Initialise to masked - unmask in 2p serial mode
	*JP1_DATA |= 0x1;
}

void hps_timer_init(void)
{
	*HPS_TIMER0_CONTROL &= ~0x4; //enable irq
	*HPS_TIMER1_CONTROL &= ~0x4; //enable irq
}


void hps_start_timer0(int load_val)
{

	*HPS_TIMER0_CONTROL &= ~0x1; //stop timer
	*HPS_TIMER0_LOAD = load_val; //load count value
	*HPS_TIMER0_CONTROL |= 0x3; //start timer
}

void hps_start_timer1(int load_val)
{

	*HPS_TIMER1_CONTROL &= ~0x1; //stop timer
	*HPS_TIMER1_LOAD = load_val; //load count value
	*HPS_TIMER1_CONTROL |= 0x3; //start timer
}


void send_data(int data)
{
	bit_count = 0;
	tx_data = data;

	*JP1_DATA &= ~(0x1 << TX_PIN);
	hps_start_timer0(BIT_PERIOD);

}

void bit_timeout(void){

	int i;
	int clear = *HPS_TIMER0_EOI;
	*JP1_DATA ^= (-((tx_data >> bit_count) & 0x1)^ *JP1_DATA) & (1 << TX_PIN); // set nth bit to x
	++bit_count;

	if (bit_count < 21)
	{
		hps_start_timer0(BIT_PERIOD);
	}
	else  {
		*HPS_TIMER0_CONTROL &= ~0x1; //stop timer
		*JP1_DATA = 0x1 << TX_PIN;
		*JP1_IRQ_MASK |= 0x2;
		key_IRQ_set(0);
	}


	//if (bit_count == 16) {parity check}

}


void read_data(void)
{
	*JP1_EDGE_CAP |= 0xFF;
	*JP1_IRQ_MASK &= ~0x2;
	hps_start_timer1(BIT_PERIOD/2);
	bit_count = 0;

}

int check_valid_packet(int packet){
	int a0,a1,a2,a3;
	a0 = (rx_data & 0xF000) >> 12;
	a1 = (rx_data & 0x0F00) >> 8;
	a2 = (rx_data & 0x00F0) >> 4;
	a3 = (rx_data & 0x000F) >> 0;

	if(a0 > 7 || a1 > 7 || a2 > 7 || a3 >7){
		return 0; // Invalid
	} else {
		return 1; // Valid
	}


}

void read_timeout(){

	int clear = *HPS_TIMER1_EOI;
	rx_data ^= (-((*JP1_DATA >> RX_PIN) & 0x1)^ rx_data) & (1 << bit_count); // set nth bit to x
	++bit_count;

	if(bit_count < 21){
		hps_start_timer1(BIT_PERIOD); // Reload
	} else {
		rx_data = rx_data >> 1; // Remove start bit
		*HPS_TIMER1_CONTROL &= ~0x1; //stop timer

		if(!check_valid_packet(rx_data))
		{
			*JP1_IRQ_MASK |= 0x2;
			return;
		}

		// Set start and end coords
		start_coordinate.x 	= (rx_data & 0xF0000)  >> 16;
		start_coordinate.y 	= (rx_data & 0x0F000)  >> 12;
		end_coordinate.x 	= (rx_data & 0x00F00)  >> 8;
		end_coordinate.y 	= (rx_data & 0x000F0)  >> 4;
        promotion_no 		= (rx_data & 0x0000F)  >> 0;

        key_IRQ_set(1);

		input_mode = 4; // OPP_P_MODE
		//*JP1_IRQ_MASK |= 0x2;
	}

}


/*
int ir_buffer_state;

void ir_init(void)
{
	*IR_CONTROL |= 0x1;
}

//void gpio_init(void)
//{
//	*GPIO0_DATA_DIR |= 0x1;
//	*GPIO0_IRQ_EN |= 0x2;
//}

void send_ir_byte(char data)
{
	*IR_DATA |= data;
}

void read_ir_byte(void)
{

	buffer[ir_buffer_state] = *IR_DATA & 0xFF;

	ir_buffer_state = !ir_buffer_state;

	if (ir_buffer_state == 0)
	{
		board[buffer[0] & 0xFF][buffer[0] >> 4] = board[buffer[1] & 0xFF][buffer[1] >> 4];
		board[buffer[1] & 0xFF][buffer[1] >> 4] = BLANK;
	}

}
*/

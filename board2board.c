#include "address_map_arm.h"
#include "graphics_chess.h"

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

void JP1_init(void)
{
	*JP1_DATA_DIR |= 0x1;
	*JP1_IRQ_MASK |= 0x2;
	*JP1_DATA |= 0x1;
}

void hps_timer_init(void)
{
	*HPS_TIMER0_CONTROL &= ~0x4; //enable irq
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

	*JP1_DATA ^= (-((tx_data >> bit_count) & 0x1)^ *JP1_DATA) & (1 << TX_PIN); // set nth bit to x
	++bit_count;

	if (bit_count < 16)
	{
		hps_start_timer0(BIT_PERIOD);
	}
	else
	{
		// send_data(0xF0F0);

	}


	//if (bit_count == 16) {parity check}

}


void read_data(void)
{
	*JP1_EDGE_CAP |= 0xFF;
	*JP1_IRQ_MASK |= 0x2;
	hps_start_timer1(BIT_PERIOD/2);
	bit_count = 0;

}

void read_timeout(){
	rx_data ^= (-((*JP1_DATA >> bit_count) & 0x1)^ rx_data) & (1 << RX_PIN); // set nth bit to x
	++bit_count;

	if(bit_count < 16){
		hps_start_timer1(BIT_PERIOD); // Reload
	} else {
		rx_data = rx_data >> 1; // Remove start bit
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

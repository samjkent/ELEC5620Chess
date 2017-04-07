#include "address_map_arm.h"
#include "graphics_chess.h"

//#define GPIO0_BASE 	0xFF708000
//#define TX_PIN 		0
//#define RX_PIN		1
//
//volatile unsigned int *GPIO0_DATA 		= (unsigned int *) GPIO0_BASE;
//volatile unsigned int *GPIO0_DATA_DIR	= (unsigned int *) GPIO0_BASE + 0x4;
//volatile unsigned int *GPIO0_IRQ_EN		= (unsigned int *) GPIO0_BASE + 0x30;
//volatile unsigned int *GPIO0_IRQ_MASK	= (unsigned int *) GPIO0_BASE + 0x34;
//volatile unsigned int *GPIO0_IRQ_CLR	= (unsigned int *) GPIO0_BASE + 0x4C;
//volatile unsigned int *GPIO0_IRQ_POL	= (unsigned int *) GPIO0_BASE + 0x3C;

volatile unsigned int *IR_DATA			= (unsigned int *) 0xFF201020;
volatile unsigned int *IR_CONTROL		= (unsigned int *) 0xFF201024;

extern char board[8][8];

char buffer[2];

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

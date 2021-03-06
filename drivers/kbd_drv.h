#ifndef kbd_drv
#define kbd_drv

volatile int * PS2_Data = (int *) 0xFF200100;
volatile int * PS2_Control = (int *) 0xFF200104;

#define KEY_UP 0x75
#define KEY_DOWN 0x72
#define KEY_LEFT 0x6B
#define KEY_RIGHT 0x74

#define KEY_W 0x1D
#define KEY_A 0x1C
#define KEY_S 0x1B
#define KEY_D 0x23

#define KEY_1 0x16
#define KEY_2 0x1E
#define KEY_3 0x26
#define KEY_4 0x25

#define KEY_Q 0x15

#define KEY_SPACE 0x29
#define KEY_ENTER 0x5A
#define KEY_ESC 0x76

void key_IRQ_set(unsigned int state);

#endif

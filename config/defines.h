#define		EDGE_TRIGGERED			0x1
#define		LEVEL_SENSITIVE			0x0
#define		CPU0					0x01	// bit-mask; bit 0 represents cpu0
#define		ENABLE					0x1

#define		KEY0 					0
#define		KEY1 					1
#define		KEY2					2
#define		KEY3					3
#define		NONE					4

#define		RIGHT					1
#define		LEFT					2

#define		USER_MODE				0x10    //0b1 0000
#define		FIQ_MODE				0x11    //0b1 0001
#define		IRQ_MODE				0x12    //0b1 0010
#define		SVC_MODE				0x13    //0b1 0011
#define		ABORT_MODE				0x17    //0b1 0111
#define		UNDEF_MODE				0x1B    //0b1 1011
#define		SYS_MODE				0x1F    //0b1 1111

#define		INT_ENABLE				0x40 //0b0100 0000
#define		INT_DISABLE				0xC0 //0b1100 0000

void configure_interrupts(void);
int key_value;
volatile unsigned int *wdt_crr = (unsigned int *) 0xFFD0200C;

int cursor_xy[2] = {0, 0};

int main(){
	configure_interrupts();
	while(1){
		// Do nothing
		key_value = (*(int *) 0xFF200100) & 0xFF;

		// Reset watchdog
		*wdt_crr = 0x76;

	}
	return 0;
}

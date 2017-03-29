
#include "Delay.h"

//Microsecond sleep function based on ARM A9 Private Timer
void usleep(int x)
{
    volatile unsigned int *private_timer_load       = (unsigned int *) 0xFFFEC600; // ARM A9 Private Timer Load register
    volatile unsigned int *private_timer_control    = (unsigned int *) 0xFFFEC608; // ARM A9 Private Timer Control register
    volatile unsigned int *private_timer_interrupt  = (unsigned int *) 0xFFFEC60C; // ARM A9 Private Timer Interrupt status register

    *private_timer_control = 0x0;                // Disable the timer
    *private_timer_load = x;                     // Load new time value
    *private_timer_control = (200 << 8) + 0x5;   // Enable timer count down once with prescaler of 200 so each tick is 1us.

    while((*private_timer_interrupt) == 0);      // Wait until timer overflows
    *private_timer_interrupt = 0x1;              // Set timer interrupt flag high to clear flag
    
    ResetWDT();
}

//Millisecond Delay function
void Delay_Ms(int x)
{
    while(x--) {
        usleep(1000);
    }
}


void ResetWDT(void){
    volatile unsigned int *wdt_crr  				   = (unsigned int *) 0xFFD0200C; // WDT Counter Reload Register
    *wdt_crr = 0x76;								// Clear the watchdog timer
}

#include "address_map_arm.h"
#include "interrupt_ID.h"
#include "defines.h"

/*******************************************************************************
 * This file provides code that:
 * 1. provides the IRQ interrupt service routine (ISR).
 * 2. Compiles under ARMCC (DS-5) compiler or GNUC (Altera Monitor Program).
 *    This is achieved using preprocessor defines based on active compiler.
 *
 ******************************************************************************/

#ifdef __GNUC__
#define __IRQ void __attribute__ ((interrupt))
#else
#define __IRQ __irq void
#endif

// Declare external Interrupt Service Routines (ISR)
void kbd_interrupt (void);


__IRQ __cs3_isr_irq (void)
{
	// Read the ICCIAR from the processor interface 
	unsigned address = MPCORE_GIC_CPUIF + ICCIAR;
	int int_ID = *((int *) address); 
   
	// check if interrupt is from the PS2 port
<<<<<<< HEAD
	if (int_ID == 79) // : Check if 544 is correct? Datasheet = 79
=======
	if (int_ID == PS2_IRQ) // TODO: Check if 544 is correct? Datasheet = 79
>>>>>>> refs/remotes/origin/main-dev
		kbd_interrupt ();
	else
		// if unexpected, then stay here
		while (1);

	// Write to the End of Interrupt Register (ICCEOIR)
	address = MPCORE_GIC_CPUIF + ICCEOIR;
	*((int *) address) = int_ID;

	return;
} 

// Define the remaining exception handlers
//void __attribute__ ((interrupt)) __cs3_reset (void)
__IRQ __cs3_reset (void)
{
    while(1);
}

//void __attribute__ ((interrupt)) __cs3_isr_undef (void)
__IRQ __cs3_isr_undef (void)
{
    while(1);
}

//void __attribute__ ((interrupt)) __cs3_isr_swi (void)
__IRQ __cs3_isr_swi (void)
{
    while(1);
}

//void __attribute__ ((interrupt)) __cs3_isr_pabort (void)
__IRQ __cs3_isr_pabort (void)
{
    while(1);
}

//void __attribute__ ((interrupt)) __cs3_isr_dabort (void)
__IRQ __cs3_isr_dabort (void)
{
    while(1);
}

//void __attribute__ ((interrupt)) __cs3_isr_fiq (void)
__IRQ __cs3_isr_fiq (void)
{
    while(1);
}

#include "address_map_arm.h" // defines values for KEY1, KEY2, KEY3
#include "defines.h"         // defines values for KEY1, KEY2, KEY3
#include "interrupt_ID.h"         // defines values for KEY1, KEY2, KEY3

/*******************************************************************************
 * This file provides code that:
 * 1. Configures the IRQ interrupts.
 * 2. Configures periperals to send interrupt requests.
 * 3. Compiles under ARMCC (DS-5) compiler or GNUC (Altera Monitor Program).
 *    This is achieved using preprocessor defines based on active compiler.
 *
 ******************************************************************************/

void kbd_setup(void);

void enable_A9_interrupts(void);
void set_A9_IRQ_stack(void);
void config_GIC(void);

// Interrupt configuration sequence
void configure_interrupts()
{
    // initialise the stack pointer for IRQ mode
    set_A9_IRQ_stack();

    // configure the general interrupt controller
    config_GIC();

    // configure PS/2 interrupt
    kbd_setup();

    // enable interrupts
    enable_A9_interrupts();
}


/* Setup stack pointer (SP) in IRQ mode using assembler.
 *
 *    MSR CPSR_c, #0xD2        //
 *    MOV SP, #0xFFFFFFF8        //
 *    MSR CPSR_c, #0xD3        //
 *    BX LR                    //
 *
 *    This can only be code using assembly language.     */
#ifdef __CC_ARM
__asm void set_A9_IRQ_stack(void)
{
    MSR CPSR_c, #0xD2
    MOV SP, #0xFFFFFFF8
    MSR CPSR_c, #0xD3
    BX LR
}
#endif

void config_interrupt (int N, int CPU_target)
{
	int reg_offset, index, value, address;
	/* Configure the Interrupt Set-Enable Registers (ICDISERn).
	* reg_offset = (integer_div(N / 32) * 4
	* value = 1 << (N mod 32) */
	reg_offset = (N >> 3) & 0xFFFFFFFC;
	index = N & 0x1F;
	value = 0x1 << index;
	address = 0xFFFED100 + reg_offset;

	/* Now that we know the register address and value, set the appropriate bit */
	*(int *)address |= value;

	/* Configure the Interrupt Processor Targets Register (ICDIPTRn)
	* reg_offset = integer_div(N / 4) * 4
	* index = N mod 4 */
	reg_offset = (N & 0xFFFFFFFC);
	index = N & 0x3;
	address = 0xFFFED800 + reg_offset + index;

	/* Now that we know the register address and value, write to (only) the appropriate byte */
	*(char *)address = (char) CPU_target;
}

/* Configure Global Interrupt Controller (GIC) */
void config_GIC(void)
{

    unsigned int address;    // used to calculate register addresses

    config_interrupt(PS2_IRQ,1);

    // Set Interrupt Priority Mask Register (ICCPMR)
    // Enable interrupts of all priorities
    address = MPCORE_GIC_CPUIF + ICCPMR;
    *((int *) address) = 0xFFFF;

    // Set CPU Interface Control Register (ICCICR)
    // Enable signaling of interrupts
    address = MPCORE_GIC_CPUIF + ICCICR;
    *((int *) address) = ENABLE;

    // Configure the Distributor Control Register (ICDDCR)
    // Send pending interrupts to CPUs
    address = MPCORE_GIC_DIST + ICDDCR;
    *((int *) address) = ENABLE;
}


/* Enable interrupts using assembler.
 *
 *  MSR CPSR_c, #0x53       //
 *  NOP                     //
 *  NOP                     //
 *  BX LR                    //
 *
 *  This can only be code using assembly language.     */
 #ifdef __CC_ARM
 __asm void enable_A9_interrupts(void)
{
    MSR CPSR_c, #0x53
    NOP
    NOP
    BX LR
}
#endif



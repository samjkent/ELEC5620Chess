#define BOARD                "DE1-SoC"

/* Memory */
#define DDR_START            0x00000000
#define DDR_END              0x3FFFFFFF
#define A9_ONCHIP_START      0xFFFF0000
#define A9_ONCHIP_END        0xFFFFFFFF
#define SDRAM_START          0xC0000000
#define SDRAM_END            0xC3FFFFFF
#define FPGA_ONCHIP_START    0xC8000000
#define FPGA_ONCHIP_END      0xC8003FFF
#define FPGA_CHAR_START      0xC9000000
#define FPGA_CHAR_END        0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE            0xFF200000
#define HEX3_HEX0_BASE       0xFF200020
#define HEX5_HEX4_BASE       0xFF200030
#define SW_BASE              0xFF200040
#define KEY_BASE             0xFF200050
#define PS2_BASE             0xFF200100
#define JTAG_UART_BASE       0xFF201000
#define UART_BASE            0xFF201000
#define TIMER_BASE           0xFF202000
#define AUDIO_BASE           0xFF203040

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE       0xFF709000
#define HPS_TIMER0_BASE      0xFFC08000
#define HPS_TIMER1_BASE      0xFFC09000
#define HPS_TIMER2_BASE      0xFFD00000
#define HPS_TIMER3_BASE      0xFFD01000
#define FPGA_BRIDGE          0xFFD0501C

/* ARM A9 MPCORE devices */
#define PERIPH_BASE          0xFFFEC000   // base address of peripheral devices

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF     0xFFFEC100   // PERIPH_BASE + 0x100
#define ICCICR               0x00         // + to CPU interface control
#define ICCPMR               0x04         // + to interrupt priority mask
#define ICCIAR               0x0C         // + to interrupt acknowledge
#define ICCEOIR              0x10         // + to end of interrupt reg

/* Interrupt (INT) controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST      0xFFFED000   // PERIPH_BASE + 0x1000
#define ICDDCR               0x000        // + to distributor control reg
#define ICDISER              0x100        // + to INT set-enable regs
#define ICDICER              0x180        // + to INT clear-enable regs
#define ICDIPTR              0x800        // + to INT processor targets regs
#define ICDICFR              0xC00        // + to INT configuration regs

/* Private timer */
#define MPCORE_PRIV_TIMER    0xFFFEC600   // PERIPH_BASE + 0x0600

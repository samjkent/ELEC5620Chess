            AREA    VECTORS, CODE, READONLY
            EXPORT  __Vectors

			extern __rt_entry               ; Main program entry point
			extern main          ; Main program entry point
            extern __cs3_isr_irq            ; IRQ ISR
__Vectors
            LDR         PC,=main ; 0x00 Reset
            LDR         PC,=__rt_entry      ; 0x04 Undefined Instructions
            DCD         0                   ; 0x08 Software Interrupts (SWI)
            DCD         0                   ; 0x0C Prefetch Abort
            DCD         0                   ; 0x10 Data Abort
            DCD         0                   ; 0x14 RESERVED
            LDR         PC,=__cs3_isr_irq   ; 0x18 IRQ
            DCD         0                   ; 0x1C FIQ
            END

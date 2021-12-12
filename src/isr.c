#include <pic32mx.h>
#include "../include/isr.h"
#include "../include/timer.h"

/**
 * Written by: Alex Gunnarsson & Marcus Nilszén
 * 
 * @brief The Interrupt Service Routine. Gets called
 * from vector.S in assembly.
 * 
 */
void isr_handler() {
    // timer2
    if (IFS(0) & 0x100) {
        IFS(0) &= ~0x100; // reset T2IF
        timer2_handler();
    }

    // timer3
    if (IFS(0) & 0x1000) {
        IFS(0) &= ~0x1000;
        timer3_handler();
    }
}

/**
 * Written by: Alex Gunnarsson & Marcus Nilszén
 * 
 * @brief Enables interups for all events
 * used in the program.
 * 
 */
void isr_init() {
    // handle interrupt for timer2
    IEC(0) |= 1 << 8; // 0x100 T2IE
    IPC(2) = IPC(2) & ~0x1C | 16; // 4 T2IP

    // handle interrupt for timer3
    IEC(0) |= 1 << 12; // T3IE
    IPC(3) = IPC(3) & ~0x1C | 5 << 2; // T3IP

    enable_interrupt(); // has to be last
}
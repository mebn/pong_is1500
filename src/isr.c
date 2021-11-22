#include <pic32mx.h>
#include "../include/isr.h"
#include "../include/timer.h"

/**
 * @brief The Interrupt Service Routine. Gets called
 * from vector.S in assembly. Currently only handles timer2.
 * 
 */
void isr_handler() {
    // timer2
    if (IFS(0) & 0x100) {
        IFS(0) &= ~0x100; // reset T2IF
        timer2_handler();
    }
}

/**
 * @brief Enables interups for all events
 * used in the program. Currently only handles timer2.
 * 
 */
void isr_init() {
    // handle interrupt for timer2
    IEC(0) |= 1 << 8; // 0x100 T2IE
    IPC(2) = IPC(2) & ~0x1C | 16; // 4 T2IP

    enable_interrupt(); // has to be last
}
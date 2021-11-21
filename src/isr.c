#include <pic32mx.h>
#include "../include/isr.h"
#include "../include/timer.h"

// Interrupt Service Routine
void isr_handler() {
    // timer2
    if (IFS(0) & 0x100) {
        timer2_handler();

        IFS(0) &= ~0x100; // reset T2IF
    }
}

void isr_init() {
    // handle interrupt for timer2
    IEC(0) |= 1 << 8; // 0x100 T2IE
    IPC(2) = IPC(2) & ~0x1C | 16; // 4 T2IP

    enable_interrupt(); // has to be last
}
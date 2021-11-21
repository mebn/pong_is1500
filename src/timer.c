#include <pic32mx.h>
#include <stdbool.h>
#include "../include/timer.h"
#include "../include/buttons.h"

static int counter_ms = 0;

void delay(unsigned int ms) {
    counter_ms = 0;

    while (counter_ms <= ms);
}

void timer2_handler() {
    counter_ms++;
}

void timer2_reset() {
    TMR2 = 0; // reset timer
}

void timer2_init() {
    T2CON = 0x60; // prescale 1:64
    PR2 = (80000000 / 64) / 1000; // period, 1 ms
    timer2_reset();

    timer2_on();
}

void timer2_on() {
    timer2_reset();
    T2CON |= 0x8000; // turn on timer
}

void timer2_off() {
    timer2_reset();
    T2CON &= ~0x8000; // turn off timer
}
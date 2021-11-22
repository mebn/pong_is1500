#include <pic32mx.h>
#include <stdbool.h>
#include "../include/timer.h"
#include "../include/buttons.h"

static int counter_ms = 0;

/**
 * Written by Marcus Nilszén
 * 
 * @brief Delay for a specified amount of time in milliseconds.
 * 
 * @param ms How many milliseconds to
 * delay code execution for.
 */
void delay(unsigned int ms) {
    counter_ms = 0;
    while (counter_ms <= ms);
}

/**
 * Written by Marcus Nilszén
 * 
 * @brief This code gets run every period specified
 * in timer2_init().
 * Increments global varible once every ms
 * to keep track of time and handle timer2 events.
 * 
 */
void timer2_handler() {
    counter_ms++;
}

/**
 * @brief Initialization for timer2.
 * Sets prescale and period.
 * Resets and turns timer2 on.
 * 
 */
void timer2_init() {
    T2CON = 0x60; // prescale 1:64
    PR2 = (80000000 / 64) / 1000; // period, 1 ms
    TMR2 = 0; // reset timer
    T2CON |= 0x8000; // turn on timer
}
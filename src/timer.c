#include <pic32mx.h>
#include <stdbool.h>
#include "../include/timer.h"
#include "../include/buttons.h"
#include "../include/screens.h"

/**
 * @brief Keeps track of time for timer2.
 * 
 */
static int counter_ms = 0;

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Turns timer2 ON.
 * 
 */
void timer2_on() {
    TMR2 = 0;           // reset timer
    T2CON |= 0x8000;
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Turns timer2 OFF.
 * 
 */
void timer2_off() {
    T2CON &= ~0x8000;
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Initialization for timer2.
 * Sets prescale and period.
 * Resets and turns timer2 on.
 * 
 */
void timer2_init() {
    T2CON = 0x60; // prescale 1:64
    PR2 = (80000000 / 64) / 1000; // period, 1 ms
}

/**
 * Written by: Marcus Nilszén
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
 * Written by: Marcus Nilszén & Alex Gunnarsson
 * 
 * @brief Delay for a specified amount of time in milliseconds.
 * 
 * @param ms How many milliseconds to
 * delay code execution for.
 */
void delay(unsigned int ms) {
    timer2_on();
    counter_ms = 0;
    while (counter_ms <= ms);
    timer2_off();
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Turns timer3 ON.
 * 
 */
void timer3_on() {
    TMR3 = 0;           // reset timer
    T3CON |= 0x8000;
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Turns timer3 OFF.
 * 
 */
void timer3_off() {
    T3CON &= ~0x8000;
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Initialization for timer3.
 * Sets prescale and period.
 * Resets and turns timer3 on.
 * 
 */
void timer3_init() {
    T3CON |= 7 << 4;            // prescale 1:256
    PR3 = (80000000 / 256) / 5; // period
    timer3_on();
}

/**
 * Written by: Alex Gunnarsson.
 * 
 * @brief Stops timer3.
 * 
 */
void timer3_uninit() {
    timer3_off();
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief This code gets run every period specified
 * in timer3_init().
 * Decrements the in-game timer used in the game-routine
 * to keep track of in-game time and handle timer3 events.
 * 
 */
void timer3_handler() {
    decrement_timer();
}
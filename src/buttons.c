#include <pic32mx.h>
#include "../include/buttons.h"
#include <stdbool.h>

/**
 * @brief Get the button pressed status.
 * 
 * @return char 4-bit int (char most appropriate, using 8-bit)
 * where the 4 least significant bits are 1 if btn is pressed
 * (char = 0000 BTN4 BTN3 BTN2 BTN1, as they appear on the 
 * I/O Shield from left to right).
 * Ex: If BTN3 and BTN1 are pressed, return is 0x05.
 */
char get_btns() {
    // BTN1 RF1
    // BTN2 RD5
    // BTN3 RD6
    // BTN4 RD7

    char btn42 = ((PORTD & 0xE0) >> 4);
    char btn1 = (PORTF & (1 << 1)) >> 1;
    
    return btn42 | btn1;
}

/**
 * @brief Get the button pressed status for BTN1.
 * 
 * @return true if BTN1 is pressed.
 * @return false if BTN1 is not pressed.
 */
bool btn1_ispressed() {
    return ((get_btns() & 0x01) >> 0) == 1;
}

/**
 * @brief Get the button pressed status for BTN2.
 * 
 * @return true if BTN2 is pressed.
 * @return false if BTN2 is not pressed.
 */
bool btn2_ispressed() {
    return ((get_btns() & 0x02) >> 1) == 1;
}

/**
 * @brief Get the button pressed status for BTN3.
 * 
 * @return true if BTN3 is pressed.
 * @return false if BTN3 is not pressed.
 */
bool btn3_ispressed() {
    return ((get_btns() & 0x04) >> 2) == 1;
}

/**
 * @brief Get the button pressed status for BTN4.
 * 
 * @return true if BTN4 is pressed.
 * @return false if BTN4 is not pressed.
 */
bool btn4_ispressed() {
    return ((get_btns() & 0x08) >> 3) == 1;
}

/**
 * @brief Initialize the four buttons
 * found on the IO Shield as input.
 * 
 */
void buttons_init() {
    TRISF |= 1 << 1; // BTN1
    TRISD |= 1 << 5; // BTN2
    TRISD |= 1 << 6; // BTN3
    TRISD |= 1 << 7; // BTN4
}
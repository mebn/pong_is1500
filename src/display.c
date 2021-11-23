#include <pic32mx.h>
#include <stdint.h>
#include "../include/display.h"
#include "../include/graphics.h"
#include "../include/timer.h"
#include <stdbool.h>

// (low to turn on)
// RF4
#define DISPLAY_COMMAND_MODE (PORTFCLR = 1 << 4)
#define DISPLAY_DATA_MODE (PORTFSET = 1 << 4) 

// RG9
#define DISPLAY_RESET_ON (PORTGCLR = 1 << 9)
#define DISPLAY_RESET_OFF (PORTGSET = 1 << 9)

// PIN 38, RF6
#define DISPLAY_VDD_ON (PORTFCLR = 1 << 6)
#define DISPLAY_VDD_OFF (PORTFSET = 1 << 6)

// PIN 40, RF5
#define DISPLAY_VBAT_ON (PORTFCLR = 1 << 5)
#define DISPLAY_VBAT_OFF (PORTFSET = 1 << 5)

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

#define FONT_SIZE 5
#define FONT_SPACING 1
const unsigned int new_font[] = {
    0x00000000, // ' '
    0x00100421, // '!'
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x00208422, // '('
    0x00110841, // ')'
    0x0,
    0x0,
    0x0,
    0x00007C00, // '-'
    0x00100000, // '.'
    0x0,
    0x0064A526, // '0'
    0x00710862, // '1'
    0x00F09907, // '2'
    0x00741907, // '3'
    0x00843D29, // '4'
    0x00741C2F, // '5'
    0x00649C26, // '6'
    0x0021110F, // '7'
    0x00649926, // '8'
    0x00643926, // '9'
    0x00008020, // ':'
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0094BD26, // 'A'
    0x00749D27, // 'B'
    0x00E0842E, // 'C'
    0x0074A527, // 'D'
    0x00F09C2F, // 'E'
    0x00109C2F, // 'F'
    0x00E4B42E, // 'G'
    0x0094BD29, // 'H'
    0x00710847, // 'I'
    0x0064A10F, // 'J'
    0x00928CA9, // 'K'
    0x00F08421, // 'L'
    0x0118D771, // 'M'
    0x0094B569, // 'N'
    0x0064A526, // 'O'
    0x00109D27, // 'P'
    0x00F6A52F, // 'Q'
    0x00929D27, // 'R'
    0x0074182E, // 'S'
    0x00210847, // 'T'
    0x0064A529, // 'U'
    0x002294A5, // 'V'
    0x00AAD6B1, // 'W'
    0x00949929, // 'X'
    0x002108A5, // 'Y'
    0x00F0990F, // 'Z'
};

// Alla tecken har en unik bredd som lagras i den här vektorn.
const unsigned char fontWidth[] = {
    4, // ' '
    1, // '!'
    0,
    0,
    0,
    0,
    0,
    0,
    2, // '('
    2, // ')'
    0,
    0,
    0,
    5,
    1, // '.'
    0,
    4, // '0'
    3, // '1'
    4, // '2'
    4, // '3'
    4, // '4'
    4, // '5'
    4, // '6'
    4, // '7'
    4, // '8'
    4, // '9'
    1, // ':'
    0,
    0,
    0,
    0,
    0,
    0,
    4, // 'A'
    4, // 'B'
    4, // 'C'
    4, // 'D'
    4, // 'E'
    4, // 'F'
    4, // 'G'
    4, // 'H'
    3, // 'I'
    4, // 'J'
    4, // 'K'
    4, // 'L'
    5, // 'M'
    4, // 'N'
    4, // 'O'
    4, // 'P'
    4, // 'Q'
    4, // 'R'
    4, // 'S'
    3, // 'T'
    4, // 'U'
    3, // 'V'
    5, // 'W'
    4, // 'X'
    3, // 'Y'
    4, // 'Z'
};

/**
 * @brief A 128*4 array used to display
 * everything on the oled display.
 * Each element currently set to 0.
 * 
 * Valid x-range: 0-127 (both inclusive).
 * Valid y-range: 0-31 (both inclusive).
 * 
 */
static uint8_t canvas[128*4] = {0};

uint8_t spi_send_recv(uint8_t data) {
    while(!(SPI2STAT & 0x08)); // Wait for transmitter to be ready
    SPI2BUF = data; //  Write the next transmit byte.
    while(!(SPI2STAT & 0x01)); // Wait for receive byte.
    return SPI2BUF;
}

/*
Power on sequence:
1. Apply power to VDD.
2. Send Display Off command.
3. Initialize display to desired operating mode.
4. Clear screen.
5. Apply power to VBAT.
6. Delay 100ms.
7. Send Display On command.
*/

// https://digilent.com/reference/_media/chipkit_shield_basic_io_shield:chipkit_basic_io_shield_rm.pdf
void display_on() {
    DISPLAY_COMMAND_MODE;
    delay(1);
    DISPLAY_VDD_ON; // 1. Apply power to VDD.
    delay(10);
    spi_send_recv(0xAE); // 2. Display off comand
    
    // 3. Initialize display to desired operating mode.
    DISPLAY_RESET_ON;
    delay(1);
    DISPLAY_RESET_OFF;
    
    spi_send_recv(0x8D); // Charge Pump Setting
    spi_send_recv(0x14); // Enable Charge Pump
    spi_send_recv(0xD9); // Pre-charge Period
    spi_send_recv(0xF1); // ? invert screen on x-axis

    // Send the commands to invert the display.
    // This puts the display origin in the upper left corner.
    spi_send_recv(0xA1); // remap columns
    spi_send_recv(0xC8); // remap the rows

    spi_send_recv(0xDA); // set COM configuration command
    spi_send_recv(0x20); // sequential COM, left/right remap enabled

    // 4. Clear screen.
    
    DISPLAY_VBAT_ON; // 5. Apply power to VBAT.
    delay(100); // 6. Delay 100ms.
    
    spi_send_recv(0xAF); // 7. Send Display On command.
}

void display_init() {
    // I don't know if these are relevant to display of not
    /* Set up output pins */
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    PORTE = 0x0;
    
    /* Output pins for display signals */
    // PORTF = 0xFFFF;
    PORTFSET = (1 << 4); // OLED data/command select (RF4)
    PORTFSET = (1 << 5); // OLED VBAT enable (RF5)
    PORTFSET = (1 << 6); // OLED VDD enable (RF6)
    PORTGSET = (1 << 9); // tied to reset?
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;
    
    // I don't know if these are relevant to display of not
    /* Set up input pins */
    TRISDSET = (1 << 8);
    TRISFSET = (1 << 1);



    // Initialize SPI port 2.
    SPI2CON = 0;
    // SPI2BRG = 15; //8Mhz, with 80Mhz PB clock
    SPI2STATCLR &= ~0x40; // Clear SPIROV
    SPI2CON |= 0x60; // Set CKP = 1, MSTEN = 1;
    SPI2CONSET = 0x8000; // Turn on SPI
    
    display_on();
}

/**
 * Written by Marcus Nilszén
 * 
 * @brief Draw a string on a specific cordinate.
 * 
 * @param str The string to draw.
 * @param x X posistion. Limited from 0 to 122.
 * @param y Y posotion. Limited from 0 to 26.
 */
void draw_string(const char *str, unsigned int x, unsigned int y) {
    int x_pos, y_pos;
    
    while (*str) {
        char char_index = *str - 0x20; // 0x20 == space, first char
        char bit = 0;
        
        for (y_pos = y; y_pos < y + FONT_SIZE; y_pos++) {
            for (x_pos = x; x_pos < x + FONT_SIZE; x_pos++) {
                if (new_font[char_index] & (1 << bit)) {
                    draw_pixel(x_pos, y_pos);
                }

                bit++;
            }
        }

        x += fontWidth[char_index] + FONT_SPACING;
        str++;
    }
}

/**
 * Written by Marcus Nilszén
 * 
 * @brief Draw a string on a specified y cordinate
 * and align in to the LEFT, CENTER or RIGHT
 * relative to the screen.
 * 
 * @param str The string to draw.
 * @param y Y position. Limited from 0 to 26.
 * @param align Align text on screen. LEFT, CENTER or RIGHT.
 */
void draw_string_align(const char *str, unsigned int y, alignment align) {
    if (align == LEFT) {
        draw_string(str, 0, y);
        return;
    }

    unsigned int x;
    int len = 0;
    char *str2 = str;

    while (*str2) {
        len += fontWidth[*str-0x20] + FONT_SPACING;
        str2++;
    }

    len -= FONT_SPACING;

    if (align == CENTER) {
        x = DISPLAY_WIDTH/2 - len/2;
    } else {
        x = DISPLAY_WIDTH - len;
    }

    draw_string(str, x, y);
}

/**
 * Written by: Marcus Nilszén
 * Modified by: Alex Gunnarsson
 * 
 * @brief Fill the pixel (set to 1).
 * 
 * @param x X-coordinate for the pixel.
 * @param y Y-coordinate for the pixel.
 */
void draw_pixel(char x, char y) {
    canvas[(y / 8) * DISPLAY_WIDTH + x] |= 1 << (y % 8);
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Clear the pixel (set to 0).
 * 
 * @param x X-coordinate for the pixel.
 * @param y Y-coordinate for the pixel.
 */
void clear_pixel(char x, char y) {
    canvas[(y / 8) * DISPLAY_WIDTH + x] &= ~(1 << (y % 8));
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Get the pixel value of a given pixel.
 * 
 * @param x X-coordinate for the pixel.
 * @param y Y-coordinate for the pixel.
 * @return true If current pixel value is 1.
 * @return false If current pixel value is 0.
 */
bool pixel_ison(char x, char y) {
    return (canvas[(y / 8) * DISPLAY_WIDTH + x] & 1 << (y % 8)) == 1;
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Inverts all current pixel values stored in frame buffer
 * in a rectangle from (xStart, yStart) to (xEnd, yEnd).
 * 
 * @param xStart Starting x-coordinate (inclusive)
 * @param yStart Starting y-coordinate (inclusive)
 * @param xEnd Ending x-coordinate (inclusive)
 * @param yEnd Ending y-coordinate (inclusive)
 */
void display_invert(char xStart, char yStart, char xEnd, char yEnd) {
    char x;
    for (x = xStart; x <= xEnd; x++) {
        char y;
        for (y = yStart; y <= yEnd; y++) {
            if (pixel_ison(x, y)) {
                clear_pixel(x, y);
            } else {
                draw_pixel(x, y);
            }
        }
    }
}

void draw_clear() {
    int i;

    for (i = 0; i < 128*4; i++)
        canvas[i] = 0;
}

void draw_canvas() {
    int i, j;
    
    for(i = 0; i < 4; i++) {
        DISPLAY_COMMAND_MODE;
        
        spi_send_recv(0x22); // Set page command
        spi_send_recv(i); // page number (1-4)

        //  Start at the left column
        spi_send_recv(0x00); // set low nybble of column
        spi_send_recv(0x10); // set high nybble of column

        DISPLAY_DATA_MODE;

        for(j = 0; j < 128; j++)
            spi_send_recv(canvas[i*128 + j]);
    }
}


void display_fullscreen_image(unsigned int x, const uint8_t *data) {
    int i, j;
    
    for(i = 0; i < 4; i++) {
        DISPLAY_COMMAND_MODE;

        spi_send_recv(0x22);
        spi_send_recv(i);
        
        spi_send_recv(x & 0xF);
        spi_send_recv(0x10 | ((x >> 4) & 0xF));
        
        DISPLAY_DATA_MODE;
        
        int row = 128;
        for(j = 0; j < row; j++)
            spi_send_recv(~data[i*row + j]);
    }
}

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

/**
 * @brief Checks whether or not a pixel is valid.
 * 
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @return true If specified pixel is within bounds.
 * @return false If specified pixel is not within bounds.
 */
bool is_pixel(char x, char y) {
    return (x >= 0 && x <= 127) && (y >= 0 && y <= 31);
}

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
 * @brief Draw a string where the specified cordinate
 * is its upper left corner. Accepts both upper- and
 * lowercase letters but only prints uppercase.
 * 
 * @param str The string to draw.
 * @param x X posistion of upper left corner.
 * @param y Y posotion of upper left corner.
 */
void draw_string(char *str, unsigned int x, unsigned int y) {
    int x_pos, y_pos;

    while (*str) {
        char bit = 0;
        char char_index = *str - 0x20;  // 0x20 == space, first char.
        
        if (*str >= 97 && *str <= 122) {
            char_index -= 32;           // make uppercase
        }
        
        for (y_pos = y; y_pos < y + FONT_SIZE; y_pos++) {
            for (x_pos = x; x_pos < x + FONT_SIZE; x_pos++) {
                if (font[char_index] & (1 << bit)) {
                    draw_pixel(x_pos, y_pos);
                }

                bit++;
            }
        }

        x += font_width[char_index] + FONT_SPACING;
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
 * @param y Y position.
 * @param align Align text on screen. LEFT, CENTER or RIGHT.
 */
void draw_string_align(char *str, unsigned int y, alignment align) {
    if (align == LEFT) {
        draw_string(str, 0, y);
        return;
    }

    unsigned int x;
    int len = 0;
    char *str2 = str;

    while (*str2) {
        len += font_width[*str-0x20] + FONT_SPACING;
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
 * Written by: Marcus Nilszén & Alex Gunnarsson
 * 
 * @brief Fill the pixel (set to 1).
 * 
 * @param x X-coordinate for the pixel.
 * @param y Y-coordinate for the pixel.
 */
void draw_pixel(char x, char y) {
    if (!is_pixel(x, y)) return;
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
    if (!is_pixel(x, y)) return;
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
 * @return false If current pixel value is 0 or
 * pixel is invalid.
 */
bool pixel_ison(char x, char y) {
    if (!is_pixel(x, y)) return false;
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
    char x, y;
    for (x = xStart; x <= xEnd; x++) {
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

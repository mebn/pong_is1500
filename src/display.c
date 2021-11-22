#include <pic32mx.h>
#include <stdint.h>
#include "../include/display.h"
#include "../include/graphics.h"
#include "../include/timer.h"

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

/**
 * @brief A 128*4 array used to display
 * everything on the oled display.
 * Each element currently set to 0.
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

void draw_pixel(unsigned int x, unsigned int y) {
    short row = y / 8;
    canvas[row * 128 + x] |= 1 << (y % 8);
}

void draw_text(unsigned int x, unsigned int y, char *s) {
    if (x > 128 || x < 0 || y > 32 || y < 0) return;
    if (!s) return;
    
    short row = y / 8;
    int i;
    while (*s) {
        // each char (*s) is 8 bits. see graphics.h/font
        for (i = 0; i < 8; i++) {
            canvas[row * 128 + x] |= font[(*s) * 8 + i];
            x++;
        }

        s++;
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

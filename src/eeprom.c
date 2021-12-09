#include <pic32mx.h>
#include "../include/eeprom.h"

/**
 * Written by: Marcus Nilszén
 * 
 * @brief Initilize eeprom.
 * 
 */
void eeprom_init() {
    I2C1CON = 0; // turn off
    I2C1BRG = 0x0C2; // ?
    I2C1CONSET = PIC32_I2CCON_ON; // turn on
}

/**
 * @brief 
 * 
 */
void eeprom_wait() {
    while (I2C1CON & 0x1F);
    while (I2C1STAT & PIC32_I2CSTAT_TRSTAT);
}

/**
 * @brief 
 * 
 */
void eeprom_start() {
    eeprom_wait();
    I2C1CONSET = PIC32_I2CCON_SEN;
    while (I2C1CON & PIC32_I2CCON_SEN == 1);
}

/**
 * @brief 
 * 
 */
void eeprom_stop() {
    eeprom_wait();
    I2C1CONSET = PIC32_I2CCON_PEN;
}

/**
 * @brief 
 * 
 */
void eeprom_ack() {
    eeprom_wait();
    I2C1CONCLR = PIC32_I2CCON_ACKDT;
    I2C1CONSET = PIC32_I2CCON_ACKEN;
    while(I2C1CON & PIC32_I2CCON_ACKEN);
}

/**
 * @brief 
 * 
 */
void eeprom_nack() {
    eeprom_wait();
    I2C1CONSET = PIC32_I2CCON_ACKDT;
    I2C1CONSET = PIC32_I2CCON_ACKEN;
    while(I2C1CON & PIC32_I2CCON_ACKEN);
}

/**
 * @brief 
 * 
 * @param address 
 * @param byte 
 */
void eeprom_write(unsigned short address, unsigned char data) {
    do {
        eeprom_start();
        eeprom_wait();
        I2C1TRN = 0xA0; // CONTROL_BYTE_WRITE
        eeprom_wait();
    } while(I2C1STAT & PIC32_I2CSTAT_ACKSTAT);

    // send highbyte
    I2C1TRN = address >> 8;
    eeprom_wait();

    // send lowbyte
    I2C1TRN = address;
    eeprom_wait();

    // send data
    I2C1TRN = data;
    eeprom_wait();

    eeprom_stop();
}

/**
 * @brief 
 * 
 * @param address 
 * @return char 
 */
char eeprom_read(unsigned short address) {
    do {
        eeprom_start();
        eeprom_wait();
        I2C1TRN = 0xA0; // CONTROL_BYTE_WRITE
        eeprom_wait();
    } while(I2C1STAT & PIC32_I2CSTAT_ACKSTAT);

    // send highbyte
    I2C1TRN = address >> 8;
    eeprom_wait();

    // send lowbyte
    I2C1TRN = address;
    eeprom_wait();
    
    //Initialize Procedure Read
    do {
        eeprom_start();
        eeprom_wait();
        I2C1TRN = 0xA1; // CONTROL_BYTE_READ
        eeprom_wait();
    } while (I2C1STAT & PIC32_I2CSTAT_ACKSTAT);

    eeprom_wait();
    I2C1CONSET = PIC32_I2CCON_RCEN;
    eeprom_wait();
    I2C1STATCLR = PIC32_I2CSTAT_I2COV;
    char data = I2C1RCV;

    eeprom_nack();
    eeprom_stop();
    
    return data;
}

/**
 * @brief 
 * 
 * @param address 
 * @param s 
 */
void eeprom_write_str(unsigned short address, char *s) {
    int i = 0;
    while (1) {
        eeprom_write(address + i, *s);
        if (!*s) return;
        i++;
        s++;
    }
}

/**
 * @brief 
 * 
 * @param address 
 * @param buffer 
 */
void eeprom_read_str(unsigned short address, char *buffer) {
    int i = 0;
    while (1) {
        buffer[i] = eeprom_read(address + i);
        if (!buffer[i]) return;
        i++;
    }
}

/*
HOW TO USE:
When writing to eeprom:
    eeprom_write_str(0x100, "SOME TEXT");

When reading (and drawing): 
    char buffer[20];
    eeprom_read_str(0x100, buffer);
*/
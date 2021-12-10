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
 * Written by: Marcus Nilszén
 * 
 * @brief [PRIVATE] Waits for "Transmit Status bit" to be 0
 * before proceeding.
 * 
 */
void eeprom_wait() {
    while (I2C1CON & 0x1F);
    while (I2C1STAT & PIC32_I2CSTAT_TRSTAT);
}

/**
 * Written by: Marcus Nilszén
 * 
 * @brief [PRIVATE] Initiate Start condition on SDAx and SCLx pins.
 * Necessary for communication.
 * 
 */
void eeprom_start() {
    eeprom_wait();
    I2C1CONSET = PIC32_I2CCON_SEN;
    while (I2C1CON & PIC32_I2CCON_SEN == 1);
}

/**
 * Written by: Marcus Nilszén
 * 
 * @brief [PRIVATE] Initiate Stop condition on SDAx and SCLx pins.
 * 
 */
void eeprom_stop() {
    eeprom_wait();
    I2C1CONSET = PIC32_I2CCON_PEN;
    while (I2C1CON & PIC32_I2CCON_PEN == 1);
}

/**
 * Written by: Marcus Nilszén
 * 
 * @brief [PRIVATE] Sends an acknowledge condition.
 * 
 */
void eeprom_ack() {
    eeprom_wait();
    I2C1CONCLR = PIC32_I2CCON_ACKDT;
    I2C1CONSET = PIC32_I2CCON_ACKEN;
    while(I2C1CON & PIC32_I2CCON_ACKEN);
}

/**
 * Written by: Marcus Nilszén
 * 
 * @brief [PRIVATE] Sends an not acknowledge condition.
 * 
 */
void eeprom_nack() {
    eeprom_wait();
    I2C1CONSET = PIC32_I2CCON_ACKDT;
    I2C1CONSET = PIC32_I2CCON_ACKEN;
    while(I2C1CON & PIC32_I2CCON_ACKEN);
}

/**
 * Written by: Marcus Nilszén
 * 
 * @brief [PRIVATE] Writes a char to EEPROM memory.
 * 
 * @param address Address in EEPROM memory to write to.
 * @param byte The char/data to write.
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
 * Written by: Marcus Nilszén
 * 
 * @brief [PRIVATE] Reads a char from EEPROM memory.
 * 
 * @param address Address in EEPROM memory to read from.
 * @return char The char/data in given address location.
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


/***** EEPROM WRITE *****/

/**
 * Written by: Marcus Nilszén
 * 
 * @brief Writes a string to EEPROM memory.
 * Example usage: eeprom_write_str(0x100, "SOME TEXT");
 * 
 * @param address Address in EEPROM memory to write to.
 * @param s String to write.
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
 * Written by: Marcus Nilszén
 * 
 * @brief Writes a int to EEPROM memory.
 * Example usage: eeprom_write_str(0x100, 101);
 * 
 * @param address Address in EEPROM memory to write to.
 * @param data Int to write.
 */
void eeprom_write_int(unsigned short address, unsigned int data) {
    eeprom_write(address, data >> 24);
    eeprom_write(address + 1, data >> 16);
    eeprom_write(address + 2, data >> 8);
    eeprom_write(address + 3, data);
}


/***** EEPROM READ *****/

/**
 * Written by: Marcus Nilszén
 * 
 * @brief Reads a string from EEPROM memory.
 * Example usage:
 * char buffer[20];
 * eeprom_read_str(0x100, buffer);
 * 
 * @param address Address in EEPROM memory to read from.
 * @param buffer The string in given address location.
 */
void eeprom_read_str(unsigned short address, char *buffer) {
    int i = 0;
    while (1) {
        buffer[i] = eeprom_read(address + i);
        if (!buffer[i]) return;
        i++;
    }
}

/**
 * Written by: Marcus Nilszén
 * 
 * @brief Reads a int from EEPROM memory.
 * Example usage:
 * int res = eeprom_read_int(0x100);
 * 
 * @param address Address in EEPROM memory to read from.
 * @return The int in given address location.
 */
unsigned int eeprom_read_int(unsigned short address) {
    unsigned int data = 0;
    data |= eeprom_read(address) << 24;
    data |= eeprom_read(address + 1) << 16;
    data |= eeprom_read(address + 2) << 8;
    data |= eeprom_read(address + 3);
    return data;
}

/**
 * Written by: Marcus Nilszén
 * 
 * @brief Used to reset EEPROM memory. Reset meaning
 * setting name to "0" and score to 0.
 * 
 */
void reset_eeprom() {
    short name_addrs[DIFFICULTYLEVELS][TOPNPLAYERS] = {
        {ADDR_EASY1_NAME, ADDR_EASY2_NAME, ADDR_EASY3_NAME, ADDR_EASY4_NAME},
        {ADDR_NORMAL1_NAME, ADDR_NORMAL2_NAME, ADDR_NORMAL3_NAME, ADDR_NORMAL4_NAME},
        {ADDR_HARD1_NAME, ADDR_HARD2_NAME, ADDR_HARD3_NAME, ADDR_HARD4_NAME}
    };
    short score_addrs[DIFFICULTYLEVELS][TOPNPLAYERS] = {
        {ADDR_EASY1_SCORE, ADDR_EASY2_SCORE, ADDR_EASY3_SCORE, ADDR_EASY4_SCORE},
        {ADDR_NORMAL1_SCORE, ADDR_NORMAL2_SCORE, ADDR_NORMAL3_SCORE, ADDR_NORMAL4_SCORE},
        {ADDR_HARD1_SCORE, ADDR_HARD2_SCORE, ADDR_HARD3_SCORE, ADDR_HARD4_SCORE}
    };
    char i, j;
    for (i = 0; i < DIFFICULTYLEVELS; i++) {
        for (j = 0; j < TOPNPLAYERS; j++) {
            eeprom_write_str(name_addrs[i][j], "0");
            eeprom_write_int(score_addrs[i][j], 0);
        }
    }
}
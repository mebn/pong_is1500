#ifndef EEPROM_H
#define EEPROM_H

// #define ADDR_SEED 0x0

/***** EEPROM MEMORY ADDRESSES *****/

// normal
#define ADDR_NORMAL1_NAME 0x100
#define ADDR_NORMAL1_SCORE 0x110
#define ADDR_NORMAL2_NAME 0x120
#define ADDR_NORMAL2_SCORE 0x130
#define ADDR_NORMAL3_NAME 0x140
#define ADDR_NORMAL3_SCORE 0x150
#define ADDR_NORMAL4_NAME 0x160
#define ADDR_NORMAL4_SCORE 0x170

// hard
#define ADDR_HARD1_NAME 0x180
#define ADDR_HARD1_SCORE 0x190
#define ADDR_HARD2_NAME 0x1a0
#define ADDR_HARD2_SCORE 0x1b0
#define ADDR_HARD3_NAME 0x1c0
#define ADDR_HARD3_SCORE 0x1d0
#define ADDR_HARD4_NAME 0x1e0
#define ADDR_HARD4_SCORE 0x1f0

void eeprom_init();

/***** EEPROM WRITE *****/
void eeprom_write_str(unsigned short address, char *s);
void eeprom_write_int(unsigned short address, unsigned int data);

/***** EEPROM READ *****/
void eeprom_read_str(unsigned short address, char *buffer);
unsigned int eeprom_read_int(unsigned short address);

void reset_eeprom();

#endif
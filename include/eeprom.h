#ifndef EEPROM_H
#define EEPROM_H

#include "screens.h"

#define TOPNPLAYERS 4
#define DIFFICULTYLEVELS 4

/***** EEPROM MEMORY ADDRESSES *****/
#define ADDR_SEED              0x100

// easy
#define ADDR_EASY1_NAME        0x110
#define ADDR_EASY1_SCORE       0x120
#define ADDR_EASY2_NAME        0x130
#define ADDR_EASY2_SCORE       0x140
#define ADDR_EASY3_NAME        0x150
#define ADDR_EASY3_SCORE       0x160
#define ADDR_EASY4_NAME        0x170
#define ADDR_EASY4_SCORE       0x180

// normal
#define ADDR_NORMAL1_NAME      0x190
#define ADDR_NORMAL1_SCORE     0x1a0
#define ADDR_NORMAL2_NAME      0x1b0
#define ADDR_NORMAL2_SCORE     0x1c0
#define ADDR_NORMAL3_NAME      0x1d0
#define ADDR_NORMAL3_SCORE     0x1e0
#define ADDR_NORMAL4_NAME      0x1f0
#define ADDR_NORMAL4_SCORE     0x200

// hard
#define ADDR_HARD1_NAME        0x210
#define ADDR_HARD1_SCORE       0x220
#define ADDR_HARD2_NAME        0x230
#define ADDR_HARD2_SCORE       0x240
#define ADDR_HARD3_NAME        0x250
#define ADDR_HARD3_SCORE       0x260
#define ADDR_HARD4_NAME        0x270
#define ADDR_HARD4_SCORE       0x280

// impossible
#define ADDR_IMPOSSIBLE1_NAME  0x290
#define ADDR_IMPOSSIBLE1_SCORE 0x2a0
#define ADDR_IMPOSSIBLE2_NAME  0x2b0
#define ADDR_IMPOSSIBLE2_SCORE 0x2c0
#define ADDR_IMPOSSIBLE3_NAME  0x2d0
#define ADDR_IMPOSSIBLE3_SCORE 0x2e0
#define ADDR_IMPOSSIBLE4_NAME  0x2f0
#define ADDR_IMPOSSIBLE4_SCORE 0x300

static short name_addrs[DIFFICULTYLEVELS][TOPNPLAYERS] = {
    {ADDR_EASY1_NAME, ADDR_EASY2_NAME, ADDR_EASY3_NAME, ADDR_EASY4_NAME},
    {ADDR_NORMAL1_NAME, ADDR_NORMAL2_NAME, ADDR_NORMAL3_NAME, ADDR_NORMAL4_NAME},
    {ADDR_HARD1_NAME, ADDR_HARD2_NAME, ADDR_HARD3_NAME, ADDR_HARD4_NAME},
    {ADDR_IMPOSSIBLE1_NAME, ADDR_IMPOSSIBLE2_NAME, ADDR_IMPOSSIBLE3_NAME, ADDR_IMPOSSIBLE4_NAME}
};

static short score_addrs[DIFFICULTYLEVELS][TOPNPLAYERS] = {
    {ADDR_EASY1_SCORE, ADDR_EASY2_SCORE, ADDR_EASY3_SCORE, ADDR_EASY4_SCORE},
    {ADDR_NORMAL1_SCORE, ADDR_NORMAL2_SCORE, ADDR_NORMAL3_SCORE, ADDR_NORMAL4_SCORE},
    {ADDR_HARD1_SCORE, ADDR_HARD2_SCORE, ADDR_HARD3_SCORE, ADDR_HARD4_SCORE},
    {ADDR_IMPOSSIBLE1_SCORE, ADDR_IMPOSSIBLE2_SCORE, ADDR_IMPOSSIBLE3_SCORE, ADDR_IMPOSSIBLE4_SCORE}
};

void eeprom_init();


/***** EEPROM WRITE *****/

void eeprom_write(unsigned short address, unsigned char data);
void eeprom_write_str(unsigned short address, char *s);
void eeprom_write_seed(unsigned int value);


/***** EEPROM READ *****/

char eeprom_read(unsigned short address);
void eeprom_read_str(unsigned short address, char *buffer);
unsigned int eeprom_read_seed();

/***** EEPROM OTHER *****/

void eeprom_reset(char screen);

#endif
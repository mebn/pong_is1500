#ifndef EEPROM_H
#define EEPROM_H

// IMPROVE??
/***** addresses for highscore *****/
// easy
#define ADDR_EASY1_NAME 0x100
#define ADDR_EASY1_SCORE 0x110

#define ADDR_EASY2_NAME 0x120
#define ADDR_EASY2_SCORE 0x130

#define ADDR_EASY3_NAME 0x140
#define ADDR_EASY3_SCORE 0x150

#define ADDR_EASY4_NAME 0x160
#define ADDR_EASY4_SCORE 0x170



void eeprom_init();
void eeprom_write_str(unsigned short address, char *s);
void eeprom_read_str(unsigned short address, char *buffer);

#endif
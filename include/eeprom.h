#ifndef EEPROM_H
#define EEPROM_H

#define A_SEED          0
#define A_EASY1         4
#define A_EASY2         5
#define A_EASY3         6
#define A_EASY4         7
#define A_NORMAL1       8
#define A_NORMAL2       9
#define A_NORMAL3       10
#define A_NORMAL4       11
#define A_HARD1         12
#define A_HARD2         13
#define A_HARD3         14
#define A_HARD4         15
#define A_IMPOSSIBLE1   16
#define A_IMPOSSIBLE2   17
#define A_IMPOSSIBLE3   18
#define A_IMPOSSIBLE4   19

void eeprom_init();
void eeprom_write_str(unsigned short address, char *s);
void eeprom_read_str(unsigned short address, char *buffer);
void eeprom_write_seed(unsigned int value);
unsigned int eeprom_read_seed();

#endif
#ifndef EEPROM_H
#define EEPROM_H

#define ADDR_SEED       0x200

void eeprom_init();
void eeprom_write_str(unsigned short address, char *s);
void eeprom_read_str(unsigned short address, char *buffer);
void eeprom_write_seed(unsigned int value);
unsigned int eeprom_read_seed();

#endif
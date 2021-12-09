#ifndef EEPROM_H
#define EEPROM_H

void eeprom_init();
void eeprom_write_str(unsigned short address, char *s);
void eeprom_read_str(unsigned short address, char *buffer);

#endif
#ifndef _EEPROM_H_
#define	_EEPROM_H_

#include"i2c.h"

extern void write_eeprom(uchar addr,uchar databyte);
extern uchar read_eeprom(uchar addr);
extern void I2C_init_ADDR(uchar devaddr);

#endif
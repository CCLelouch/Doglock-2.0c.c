#ifndef _I2C_H_
#define	_I2C_H_

#include "head.h"
#include"i2c.h"

/*void Delay(uint i); */
void I2CStart();
void I2CStop();
void I2CSend(unsigned char byte);
uchar I2CRead(bit ACK_Y_N);


#endif
#ifndef I2C_H
#define I2C_H

#include <xc.h>

void toggleColon(uint8_t toggle);
void updateDisplay(char* data, uint8_t len);

#endif
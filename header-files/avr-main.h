#ifndef F_CPU
#define F_CPU 3333333
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <stdio.h>


#include "initialization.h"
#include "i2c.h"


// display I2C address
#define DISPLAY_ADDRESS 0x71

volatile uint8_t elapsed = 0;
#ifndef COOKING_H
#define COOKING_H

#define F_CPU 3333333

#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "initialization.h"
#include "i2c.h"


// atempt at trying to get the speaker/ADC logic to work
uint16_t prev_adc_val = 0;

uint8_t inc = 0;
uint8_t dec = 102;

uint8_t final_adc_val = 0;

#endif
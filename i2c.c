/*
 * File:   i2c.c
 * Author: janani
 *
 * Created on October 20, 2024, 3:06 PM
 */

#include "i2c.h"


void toggleColon(uint8_t toggle){
    
    TWI0.MADDR = (0x71 << 1);
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
    
    TWI0.MDATA = 0x77;
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
    
    // toggling operation
    if(toggle == 1){
        TWI0.MDATA = 0x10;
    } else {
        TWI0.MDATA = 0x00;
    }

    while (!(TWI0.MSTATUS & TWI_WIF_bm));
    
    
    TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
    
}

void updateDisplay(char* new_value, uint8_t len){
    
    TWI0.MADDR = (0x71 << 1);
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
    
    TWI0.MDATA = 0x76;
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
    
    // writing to the display 
    for(uint8_t i=0; i<len; i++){
        TWI0.MDATA = new_value[i];
        while (!(TWI0.MSTATUS & TWI_WIF_bm));
    }
    
    TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
}
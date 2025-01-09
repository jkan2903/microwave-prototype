/*
 * File:   initialization.c
 * Author: janani
 *
 * Created on October 20, 2024, 1:52 PM
 */

#include "initialization.h"
#include <xc.h>

// global variables
volatile enum Mode current_mode = CLOCK;
volatile uint8_t hour = 12;
volatile uint8_t minute = 0;

volatile uint8_t cook_minute = 0;
volatile uint8_t cook_second = 0;
volatile uint8_t cook_elapsed = 0;

volatile uint8_t cook_spkr = 0;

volatile uint8_t timer_running = 0;
volatile uint8_t timer_minute=0;
volatile uint8_t timer_second=0;

volatile uint8_t timer_show = 0;


void initRTC(){

    /* Initialize RTC: */
    while (RTC.STATUS > 0) {
       ; /* Wait for all register to be synchronized */
    }

    /* Set period */
    RTC.PER = 512; // overflow every 1 second
    RTC.CMP = 256; // toggle ':' every 0.5 seconds

    /* 32.768kHz External Crystal Oscillator (XOSC32K) */
    RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;

    /* Run in debug: enabled */
    RTC.DBGCTRL |= RTC_DBGRUN_bm;

    RTC.CTRLA = RTC_PRESCALER_DIV64_gc /* 64 */
            | RTC_RTCEN_bm; /* Enable: enabled */

    /* Enable Overflow Interrupt */
    RTC.INTCTRL |= RTC_OVF_bm | RTC_CMP_bm;
}

void initButtons(){
    PORTA.DIRCLR = PIN4_bm; // red
    PORTA.DIRCLR = PIN5_bm; // yellow
    PORTA.DIRCLR = PIN6_bm; // green
    PORTA.DIRCLR = PIN7_bm; // blue
    
    // enable pull-ups and interrupt handling
    PORTA.PIN4CTRL |= (PORT_PULLUPEN_bm | PORT_ISC_RISING_gc);
    PORTA.PIN5CTRL |= (PORT_PULLUPEN_bm | PORT_ISC_RISING_gc);
    PORTA.PIN6CTRL |= (PORT_PULLUPEN_bm | PORT_ISC_RISING_gc);
    PORTA.PIN7CTRL |= (PORT_PULLUPEN_bm | PORT_ISC_RISING_gc);
    
}

void initTWIpins(){
    //Set up SDA and SCL as outputs
    PORTA.DIR |= PIN2_bm; // SDA
    PORTA.DIR |= PIN3_bm; // SCL
    
    //Enable pull-ups
    PORTA.PIN2CTRL= PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL= PORT_PULLUPEN_bm;
}

void initTWI() {
    
    initTWIpins();
    
    //Enable Run in Debug
    TWI0.DBGCTRL = TWI_DBGRUN_bm;
    
    //Clear MSTATUS (write 1 to flags). BUSSTATE set to idle
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
    
    //Set for 100kHz from a 4MHz oscillator
    TWI0.MBAUD = 10;
    
    //[No ISRs] and Host Mode
    TWI0.MCTRLA = TWI_ENABLE_bm;
    
}


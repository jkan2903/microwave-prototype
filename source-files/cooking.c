/*
 * File:   cooking.c
 * Author: janani
 *
 * Created on October 20, 2024, 1:39 PM
 */

#include "cooking.h"
#include <xc.h>


/**
 * @brief Interrupt Service Routine for handling button presses connected to PORTA.
 *
 * This ISR processes actions based on the button pressed and the current mode of the system. 
 * Each button is associated with a specific pin on PORTA and triggers different actions:
 *
 * - **Red Button (PIN4):** Manages transitions between modes (e.g., CLOCK to COOK_SET_TIME).
 * - **Green Button (PIN6):** Handles single second increments when setting cook time and timer limits in COOK_SET_TIME or TIMER_SET modes.
 * - **Blue Button (PIN7):** Handles 5 second increments when setting cook time and timer limits in COOK_SET_TIME or TIMER_SET modes.
 * - **Yellow Button (PIN5):** Manages the timer, toggling between TIMER_SET, TIMER_PREVIEW, and other modes.
 *
 * @note
 * - Implements debouncing using `_delay_ms(50)`.
 * - Clears the corresponding interrupt flags for each pin after processing.
 */
ISR(PORTA_PORT_vect){
    
    _delay_ms(50);
    
    // red button released
    if(PORTA.INTFLAGS & PIN4_bm){
        char data[4];

        switch(current_mode){
            case CLOCK:
                current_mode = COOK_SET_TIME;
                
                updateDisplay("0000",4);
                
                break;
            case COOK_SET_TIME:
                updateDisplay("0000", 4);
                current_mode = COOK_SET_PWR;

                ADC0.INTCTRL |= ADC_RESRDY_bm;
                ADC0_start();
                break;
            case COOK_SET_PWR:
                current_mode = COOKING;

                sprintf(data, "%02d%02d", cook_minute, cook_second);
                updateDisplay(data, 4);

                ADC0.INTCTRL &= ~(ADC_RESRDY_bm);
                break;
            case COOKING:
                current_mode = CLOCK;

                sprintf(data, "%02d%02d", hour, minute);
                updateDisplay(data, 4);
                break;
            case COOK_COMPLETE:
                current_mode = CLOCK;

                cook_minute = 0;
                cook_second = 0;

                sprintf(data, "%02d%02d", hour, minute);
                updateDisplay(data, 4);
                break;
            case TIMER_SET:
                current_mode = CLOCK;

                timer_minute = 0;
                timer_second = 0;

                sprintf(data, "%02d%02d", hour, minute);
                updateDisplay(data, 4);
                break;
            case TIMER_COMPLETE:
                
                current_mode = CLOCK;

                sprintf(data, "%02d%02d", hour, minute);
                updateDisplay(data, 4);
                break;


        }

        PORTA.INTFLAGS = PIN4_bm;
    }
    
    // green button     
    if(PORTA.INTFLAGS & PIN6_bm){
        char data[4];
        
        switch(current_mode){
            case COOK_SET_TIME:
                if(cook_second == 59){
                    if(cook_minute == 99){
                        cook_minute = 99;
                    } else {
                        cook_minute = cook_minute + 1;
                        cook_second = 0;
                    }
                } else {
                    cook_second += 1;
                }

                
                sprintf(data, "%02d%02d", cook_minute, cook_second);
                updateDisplay(data, 4);
                break;     
            case TIMER_SET:
                if(timer_second == 59){
                    if(timer_minute == 99){
                        timer_minute = 99;
                    } else {
                        timer_minute = timer_minute + 1;
                        timer_second = 0;
                    }
                } else {
                    timer_second += 1;
                }

                sprintf(data, "%02d%02d", timer_minute, timer_second);
                updateDisplay(data, 4);

                break;
                


        }

        PORTA.INTFLAGS = PIN6_bm;
    }
    
    // blue button
    if(PORTA.INTFLAGS & PIN7_bm){
        
        char data[4];
        
        switch(current_mode){
            case COOK_SET_TIME:
                if(cook_second == 59){
                    if(cook_minute == 99){
                        cook_minute = 99;
                    }
                } else {

                    uint8_t temp = cook_second + 5;

                    if(temp >= 60){
                        if(cook_minute == 99){
                            cook_minute = 99;
                            cook_second = 59;
                        } else {
                            cook_minute = cook_minute + 1;
                            cook_second = temp - 60;
                        }
                    } else {
                        cook_second += 5;
                    }

                }

                sprintf(data, "%02d%02d", cook_minute, cook_second);
                updateDisplay(data, 4);
                break;
            case TIMER_SET:
                if(timer_second == 59){
                    if(timer_minute == 99){
                        timer_minute = 99;
                    }
                } else {

                    uint8_t temp = timer_second + 5;

                    if(temp >= 60){
                        if(timer_minute == 99){
                            timer_minute = 99;
                            timer_second = 59;
                        } else {
                            timer_minute = timer_minute + 1;
                            timer_second = temp - 60;
                        }
                    } else {
                        timer_second += 5;
                    }

                }

                sprintf(data, "%02d%02d", timer_minute, timer_second);
                updateDisplay(data, 4);
                break;
                
        }
        PORTA.INTFLAGS = PIN7_bm;
    }
    
    // yellow button
    if(PORTA.INTFLAGS & PIN5_bm){
        char data[4];
        switch(current_mode){
            case CLOCK:
                if(!(timer_running)){
                    toggleColon(1);
                    updateDisplay("0000", 4);

                    current_mode = TIMER_SET;

                }
                if(timer_running){
                    timer_show = 0;
                    current_mode = TIMER_PREVIEW;
                }
                break;
            case COOK_SET_TIME:
                current_mode = CLOCK;
                
                sprintf(data, "%02d%02d", hour, minute);
                updateDisplay(data, 4);
                break;
            case TIMER_SET:
                if((timer_minute == 0) && (timer_second == 0) ){
                    current_mode = CLOCK;
                    
                    sprintf(data, "%02d%02d", hour, minute);
                    updateDisplay(data, 4);
                    
                }
                
                if((timer_minute !=0) || (timer_second !=0)){
                    timer_running = 1;
                    current_mode = TIMER_PREVIEW;
                }
                break;
            case TIMER_PREVIEW:
                
                // yellow button pressed when timer is in preview 
                // return to timer setup mode
                timer_running = 0;
                timer_show = 0;

                current_mode = TIMER_SET;

                updateDisplay("0000", 4);
                
                
                
        }
        
        PORTA.INTFLAGS = PIN5_bm;
    }
    
    PORTA.INTFLAGS = PORTA.INTFLAGS;
}
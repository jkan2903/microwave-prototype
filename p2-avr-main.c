/*
 * File:   p2-avr-main.c
 * Author: janani
 *
 * Created on October 19, 2024, 1:38 PM
 */

#include "p2-avr-main.h"
#include <xc.h>


/**
 * @brief Interrupt Service Routine for handling RTC compare and overflow interrupts.
 *
 * This ISR manages various modes of a real-time clock showcasing the microwave's functionality:
 * - Toggling the colon character display in clock mode.
 * - Tracking wall clock time (hours, minutes, and elapsed seconds).
 * - Countdown functionality for cooking mode and timer mode.
 * - Managing transitions between different modes (e.g., CLOCK, COOKING, TIMER_COMPLETE).
 * - Displaying the appropriate data based on the current mode.
 *
 * @note
 * - The ISR is triggered by the `RTC_CNT_vect` vector.
 * - Handles RTC Compare Match (RTC_CMP_bm) and Overflow (RTC_OVF_bm) interrupt flags.
 */
ISR(RTC_CNT_vect){
    
    if(RTC_INTFLAGS & RTC_CMP_bm){
        
        // turn OFF ':' character
        if((current_mode == CLOCK)){
            toggleColon(0);
        }
        
        RTC.INTFLAGS = RTC_CMP_bm;
        
    }
    
    if(RTC_INTFLAGS & RTC_OVF_bm){
        
        // turn ON ':' character
        if((current_mode == CLOCK)){
            toggleColon(1);
        }
        
        if(current_mode == COOKING){
            
            // counting down
            if(cook_second == 0){
                if(cook_minute == 0){
                    
                    current_mode = COOK_COMPLETE;
                  
                } else {
                    cook_minute = cook_minute - 1;
                    cook_second = 59;
                }
                
            } else {
                cook_second = cook_second - 1;
            }
            
            char data[4];
            sprintf(data, "%02d%02d", cook_minute, cook_second);
            updateDisplay(data, 4);
            
            
            
        }
        
        // tracking wall clock time
        if(elapsed == 59){
        
            elapsed = 0;
            
            if(minute == 59){
                minute = 0;
                
                if(hour == 12){
                    hour = 1;
                } else {
                    hour = hour + 1;
                }
                
            } else {
                minute = minute + 1;
            }
            
            if(current_mode == CLOCK){
                char data[4];
            
                sprintf(data, "%02d%02d", hour, minute);
                updateDisplay(data, 4);
            }
            
        } else {

            elapsed += 1;
        }
        if(current_mode == TIMER_COMPLETE){
            current_mode = CLOCK;
            
            char data[4];
            sprintf(data, "%02d%02d", hour, minute);
            updateDisplay(data, 4);
        }
        
        // tracking timer countdown
        if(timer_running){

            if(timer_second == 0){
                if(timer_minute == 0){
                    
                    current_mode = TIMER_COMPLETE;
                    
                    timer_running = 0;
                    timer_show = 0;
                    updateDisplay("done",4);
                    _delay_ms(3000);
                    
                } else {
                    timer_minute = timer_minute - 1;
                    timer_second = 59;
                }
                
            } else {
                timer_second = timer_second - 1;
            }
        }
        
        
        // timer preview mode
        if(current_mode == TIMER_PREVIEW){
            
            char data[4];
            if(timer_show == 5){
                timer_show += 1;
                
                current_mode = CLOCK;
                
                sprintf(data, "%02d%02d", hour, minute);
                updateDisplay(data, 4);
                
            } else {
                sprintf(data, "%02d%02d", timer_minute, timer_second);
                updateDisplay(data, 4);
                
                timer_show += 1;
                
            }
            
        }
        
        if(current_mode == COOK_COMPLETE){
            
            updateDisplay("FOOD", 4);
            
            
            if(cook_spkr == 3){
                TCA0.SINGLE.CTRLA &= ~(TCA_SINGLE_ENABLE_bm);
                current_mode = CLOCK;
                
                char data[4];
                sprintf(data, "%02d%02d", hour, minute);
                updateDisplay(data, 4);
                
            } else {
                
                cook_spkr += 1;
            }
                
        }
        
        RTC.INTFLAGS = RTC_OVF_bm;
        
    }
    
}


int main(void) {
    /* Initializing the necessary peripherals */
    initTWI();
    
    initRTC();
    
    initButtons();
    
    _delay_ms(100);
    
    updateDisplay("1200", 4);
    
    // enable interrupts
    sei();
    
    
    while (1) {
        ;
    }
}



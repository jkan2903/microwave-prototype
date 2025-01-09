#include <stdint.h>

// enum to keep track of several possible modes
enum Mode {
    CLOCK,
    COOK_SET_TIME,
    COOK_SET_PWR,
    COOKING,
    COOK_COMPLETE,
    TIMER_SET,
    TIMER_START,
    TIMER_PREVIEW, 
    TIMER_COMPLETE
};

// declaring global variables
extern volatile enum Mode current_mode;
extern volatile uint8_t hour;
extern volatile uint8_t minute;

extern volatile uint8_t cook_minute;
extern volatile uint8_t cook_second;
extern volatile uint8_t cook_elapsed;

extern volatile uint8_t cook_spkr;

extern volatile uint8_t timer_running;
extern volatile uint8_t timer_minute;
extern volatile uint8_t timer_second;

extern volatile uint8_t timer_show;

void initRTC();
void initButtons();
void initTWIpins();
void initTWI();
void initTCA0();

void ADC0_init();
void ADC0_start();






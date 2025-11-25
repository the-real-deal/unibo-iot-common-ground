#ifndef __GLOBAL_CONFIGURATION__
#define __GLOBAL_CONFIGURATION__

#define BTN1 8
#define BTN2 7
#define BTN3 4
#define BTN4 2

#define LS 5 // PWM pin in order to allow fading
#define L1 13
#define L2 12
#define L3 11
#define L4 6

#define POT_PIN A0

LiquidCrystal_I2C lcd(0x27, 16, 2);

typedef enum {
    sleeping,
    started,
    preparing,
    playing,
    gameover_lost,
    gameover_victory 
} State;

#endif
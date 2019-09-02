/*
 * clockControl.c
 *
 *  Created on: Sep 25, 2015
 *      Author: rdmclean
 */

#include "clockControl.h"
#include "clockDisplay.h"
#include "SupportFiles/display.h"

int adTimer;
int autoTimer;
int rateTimer;
#define EXPIRED_VALUE_AD (1)   //EXPIRED RATE FOR ANALOG DIGITAL CONVERTER
#define EXPIRED_VALUE_AUTO (10) // FOR AUTO DECREMENT .5 SECONDS
#define EXPIRED_VALUE_RATE (2) // EXPIRED VALUE for the rate
int clock_sec = 0;      // Because of period, I need 20 ticks to make
int clock_sec_exp = 20; // one second

enum CLKCONT_States { // These are the declaration of all of my needed states
    CLKCONT_SMSTART, CLKCONT_init_st, CLKCONT_wait_for_touch_st, CLKCNT_ad_timer_running_st,
    CLKCONT_auto_timer_running_st, CLKCONT_rate_timer_running_st, CLKCONT_rate_timer_expired_st
} CLKCONT_State = CLKCONT_SMSTART;


void clockControl_tick(){
    ////////////////////////////////////////////////////////
    switch(CLKCONT_State){                                          // Switch statement for Actions
    case CLKCONT_SMSTART:                                           //No actions in Start
        break;
    case CLKCONT_init_st:                                           //No actions in init
        break;
    case CLKCONT_wait_for_touch_st:
        adTimer = 0;                                                //wait action
        autoTimer = 0;                                              // reset all timers
        rateTimer = 0;
        clock_sec++;                                                // when one second has expired
        if(clock_sec == clock_sec_exp){                             //call advance one second
            clockDisplay_advanceTimeOneSecond();                    //for normal increment
            clock_sec = 0;
        }
        break;
    case CLKCNT_ad_timer_running_st:                                //increment adTimer
        adTimer++;
        break;
    case CLKCONT_auto_timer_running_st:                             //increment autoTimer
        autoTimer++;
        break;
    case CLKCONT_rate_timer_running_st:                             //increment rateTimer
        rateTimer++;
        break;
    case CLKCONT_rate_timer_expired_st:                             //reset rateTimer
        rateTimer = 0;
        break;
    }
    ////////////////////////////////////////////////////////////////////////////////////////
    switch(CLKCONT_State){                                                  // Switch statement for transitions
    case CLKCONT_SMSTART:                                                   // A simple state to start things off
        //    printf("ENTERED SMSTART transition State\n\r");
        CLKCONT_State = CLKCONT_init_st;                                    // Move to init state
        break;
    case CLKCONT_init_st:
        //     printf("ENTERED init_st transition State\n\r");
        if(!display_isTouched())                                            // An arc waiting for an original touch to start
            CLKCONT_State = CLKCONT_init_st;
        else
            CLKCONT_State = CLKCONT_wait_for_touch_st;                      // once it is touched we move into functionality
        break;
    case CLKCONT_wait_for_touch_st:
        //    printf("ENTERED wait_for_touch_st transition State\n\r");
        if(display_isTouched()){                                            // once touched move next state
            CLKCONT_State = CLKCNT_ad_timer_running_st;
            display_clearOldTouchData();                                    // clears the touch data after touch
        }
        else{                                                               // arc to stay here until touched
            CLKCONT_State = CLKCONT_wait_for_touch_st;
        }
        break;
    case CLKCNT_ad_timer_running_st:
        //    printf("ENTERED ad_timer_running_st transition State\n\r");
        if(display_isTouched()&& adTimer == EXPIRED_VALUE_AD){
            CLKCONT_State = CLKCONT_auto_timer_running_st;                  // move on after being touched and waiting
        }
        else if(!display_isTouched()&& adTimer == EXPIRED_VALUE_AD){
            CLKCONT_State = CLKCONT_wait_for_touch_st;                      // when not touched go back and expired time is insufficient go back to waiting
            clockDisplay_performIncDec();                                   // update the clock based on previous touch
        }
        else{
            CLKCONT_State = CLKCNT_ad_timer_running_st;                     //arc to stay in current state
        }
        break;
    case CLKCONT_auto_timer_running_st:
        //    printf("ENTERED auto_timer_running_st transition State\n\r");
        if(!display_isTouched()){                                           // if released quickly
            CLKCONT_State = CLKCONT_wait_for_touch_st;                      // go back to wait for touch
            clockDisplay_performIncDec();                                   //perform inc/dec based on touch
        }
        else if(display_isTouched() && autoTimer == EXPIRED_VALUE_AUTO){    // if touched for extended period and value expires
            CLKCONT_State = CLKCONT_rate_timer_running_st;                  // move on to next state
            clockDisplay_performIncDec();                                   // update display based on touch
        }
        else{                                                               // arc to stay in current state
            CLKCONT_State = CLKCONT_auto_timer_running_st;
        }
        break;
    case CLKCONT_rate_timer_running_st:
        //    printf("ENTERED rate_timer_running_st transition State\n\r");
        if(!display_isTouched())                                            // if released quickly
            CLKCONT_State = CLKCONT_wait_for_touch_st;                      //go back to wait
        else if(display_isTouched() && rateTimer == EXPIRED_VALUE_RATE)     // if touched for extended period and value expires
            CLKCONT_State = CLKCONT_rate_timer_expired_st;                  //move on to next state
        else
            CLKCONT_State = CLKCONT_rate_timer_running_st;                  // arc to stay in current state
        break;
    case CLKCONT_rate_timer_expired_st:
        //   printf("ENTERED rate_timer_expired_st transition State\n\r");
        if(display_isTouched()){                                            //if button held,
            CLKCONT_State = CLKCONT_rate_timer_running_st;                  //go to next state
            clockDisplay_performIncDec();                                   //update display based on touch
        }
        else if(!display_isTouched())                                       //if released quickly
            CLKCONT_State = CLKCONT_wait_for_touch_st;                      //go back to wait state
        break;
    }
}




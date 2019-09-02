/*
 * simonMain.c
 *
 *  Created on: Oct 30, 2015
 *      Author: rdmclean
 */

#include <stdio.h>
#include "supportFiles/display.h"
#include "supportFiles/leds.h"
#include "supportFiles/globalTimer.h"
#include "supportFiles/interrupts.h"
#include "supportFiles/intervalTimer.h"
#include "supportFiles/utils.h"
#include "xparameters.h"

#include "simonDisplay.h"
#include "buttonHandler.h"
#include "globals.h"
#include "flashSequence.h"
#include "verifySequence.h"
#include "simonControl.h"

#define TOTAL_SECONDS 650
// load-value = (period * timer-clock) - 1
#define TIMER_PERIOD 80.0E-3
#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define TIMER_LOAD_VALUE ((TIMER_PERIOD * TIMER_CLOCK_FREQUENCY) - 1.0)

static uint32_t isr_functionCallCount = 0;

int main(){
    printf("The main has begun\n");
    //    static uint16_t touchCount = 5;
    //    uint32_t timerNumber_SIMON = 0;
    //    double seconds_SIMON;
    //   simonDisplay_runTest(touchCount);
    //   buttonHandler_runTest(touchCount);
    //    flashSequence_runTest();
    //    verifySequence_runTest();

    /*********************************************************************************************/
    // Initialize the GPIO LED driver and print out an error message if it fails (argument = true).
    // You need to init the LEDs so that LD4 can function as a heartbeat.
    leds_init(true);
    // Init all interrupts (but does not enable the interrupts at the devices).
    // Prints an error message if an internal failure occurs because the argument = true.
    interrupts_initAll(true);
    interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);
    printf("timer load value:%ld\n\r", (int32_t) TIMER_LOAD_VALUE);
    u32 privateTimerTicksPerSecond = interrupts_getPrivateTimerTicksPerSecond();
    printf("private timer ticks per second: %ld\n\r", privateTimerTicksPerSecond);
    interrupts_enableTimerGlobalInts();
    // Initialization of the display is not time-dependent, do it outside of the state machine.
    display_init();
    // Start the private ARM timer running.
    interrupts_startArmPrivateTimer();
    // Enable interrupts at the ARM.
    interrupts_enableArmInts();
    // The while-loop just waits until the total number of timer ticks have occurred before proceeding.
    while (interrupts_isrInvocationCount() < (TOTAL_SECONDS * privateTimerTicksPerSecond));
    // All done, now disable interrupts and print out the interrupt counts.
    interrupts_disableArmInts();
    printf("isr invocation count: %ld\n\r", interrupts_isrInvocationCount());
    printf("internal interrupt count: %ld\n\r", isr_functionCallCount);
    /**************************************************************************************/
    printf("\nThe main has terminated\n");

    return 0;
}

void isr_function() {
    simonControl_tick();
    verifySequence_tick();
    flashSequence_tick();
    buttonHandler_tick();
    isr_functionCallCount++;
}



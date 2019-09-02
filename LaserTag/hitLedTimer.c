/*
 * hitLedTimer.c
 *
 *  Created on: Feb 28, 2017
 *      Author: rpn314
 */

#include "hitLedTimer.h"                // Hit LED Timer header file (it's own header file)
#include <stdbool.h>                    // Standard boolean support
#include <stdint.h>                     // Standard int support
#include <stdio.h>                      // Standard I/O (for printf functions)
#include "filter.h"                     // Used to get filter sample frequency
#include "supportFiles/leds.h"          // LEDs (on board) support
#include "supportFiles/mio.h"           // I/O pin (on board) support. Used to illuminate gune LED
#include "supportFiles/buttons.h"       // Buttons (on board) support. Used only in test function
#include "supportFiles/utils.h"         // Utilities support. Used only in test function
#include "supportFiles/intervalTimer.h" // Interval timer support. Used only in test function

#define HIT_LED_ON 1            // Logic to turn Gun Hit LED on
#define HIT_LED_OFF 0           // Logic to turn Gun Hit LED off
#define HIT_LED_ON_MASK 0xF     // Logic to turn Board Hit LED on
#define HIT_LED_OFF_MASK 0x0    // Logic to turn Board Hit LED off
#define BUTTONS_BTN0_MASK 0x1   // Button mask to read only button 0
#define BUTTONS_BTN3_MASK 0x8   // Button mask to read only button 3
#define HIT_LED_TEST_DELAY 3000 // 300ms dealay for period in between tests
#define HIT_LED_INTERVAL_TIMER 1// Interval timer number. Used only in test function

static bool hitLedTimerRunning;     // Boolean to determine if the timer is running.
static uint16_t hitLedTimer_timer;  // Keep track of current place in count

// Enumeration for state machine states
enum HITLEDTIMER_st_t{
    HITLEDTIMER_init,   // Initialize the state machine
    HITLEDTIMER_wait,   // Wait for the timer to be started
    HITLEDTIMER_running // Timer is running: waiting for timer to expire
} HITLEDTIMER_CurrentState = HITLEDTIMER_init;  // Start with init state

void hitLedTimer_debugStatePrint(); // Function Prototype

// Standard init function.
void hitLedTimer_init()
{
    hitLedTimer_timer = 0;                              // Initialize the hit LED timer
    leds_init(false);                                   // Initialize the board LEDS with a false to not print the failed status flag
    mio_setPinAsOutput(HIT_LED_TIMER_OUTPUT_PIN);       // Set the gun hit LED pin to be an output
    mio_writePin(HIT_LED_TIMER_OUTPUT_PIN, HIT_LED_OFF);// Initialize the gun hit LED to off
}

// Calling this starts the timer.
void hitLedTimer_start(){hitLedTimerRunning = true;}

// Returns true if the timer is currently running.
bool hitLedTimer_running(){return hitLedTimerRunning;}

// Standard tick function
void hitLedTimer_tick()
{
//    hitLedTimer_debugStatePrint();      // Prints state debug statements
    switch(HITLEDTIMER_CurrentState)    // State (Moore) Actions
    {
    case HITLEDTIMER_init:      // If the current state is HITLEDTIMER_init
        break;
    case HITLEDTIMER_wait:      // If the current state is HITLEDTIMER_wait
        hitLedTimer_timer = 0;  // Reset the hit LED timer
        break;
    case HITLEDTIMER_running:   // If the current state is HITLEDTIMER_running
        hitLedTimer_timer++;    // Increment the hit LED timer
        break;
    }


    switch(HITLEDTIMER_CurrentState)                // Transition (Mealy) Actions
    {
    case HITLEDTIMER_init:                          // If the current state is HITLEDTIMER_init
        HITLEDTIMER_CurrentState = HITLEDTIMER_wait;// Move to the wait state
        break;
    case HITLEDTIMER_wait:      // If the current state is HITLEDTIMER_wait
        if (hitLedTimerRunning) // If the timer has been triggered
        {
            HITLEDTIMER_CurrentState = HITLEDTIMER_running; // Move to the running state
            hitLedTimer_turnLedOn();                        // Turn the hit LEDs on
        }
        break;
    case HITLEDTIMER_running:                               // If the current state is HITLEDTIMER_running
        if (hitLedTimer_timer >= HIT_LED_TIMER_EXPIRE_VALUE)// If the hit LED timer has expired
        {
            HITLEDTIMER_CurrentState = HITLEDTIMER_wait;// Move to the wait state
            hitLedTimerRunning = false;                 // Disable hit led timer
            hitLedTimer_turnLedOff();                   // Turn the hit LEDs off
        }
        break;
    }
}

// Turns the gun's hit-LED on.
void hitLedTimer_turnLedOn()
{
    mio_writePin(HIT_LED_TIMER_OUTPUT_PIN, HIT_LED_ON); // Turn the gun hit LED on
//    leds_write(HIT_LED_ON_MASK);                        // Turn on the LEDs (on board) for debugging
}

// Turns the gun's hit-LED off.
void hitLedTimer_turnLedOff()
{
    mio_writePin(HIT_LED_TIMER_OUTPUT_PIN, HIT_LED_OFF);// Turn the gun hit LED off
//    leds_write(HIT_LED_OFF_MASK);                       // Turn off the LEDs for debugging
}

// Tests the hit LED Timer state machine
void hitLedTimer_runTest()
{
    hitLedTimer_init(); // Initialize the hit LED timer
    intervalTimer_init(HIT_LED_INTERVAL_TIMER);     // Initialize the interval timer. Used only in testing
    printf("starting hitLedTimer_runTest(). Runs continuously. btn3: exit test\n\r"); // Print test instructions
    while(!(buttons_read() & BUTTONS_BTN3_MASK))    // Leaves the hitLedTimer_runTest() when Button 3 is pressed
    {
        printf("starting hitLEDTimer test loop\n\r");

        intervalTimer_start(HIT_LED_INTERVAL_TIMER);        // Start the interval timer.
        hitLedTimer_start();                // Start hit led timer
        while(hitLedTimer_running());       // Wait for the timer to stop running
        intervalTimer_stop(HIT_LED_INTERVAL_TIMER);         // Stop interval timer.

        double seconds = 0;
        intervalTimer_getTotalDurationInSeconds(HIT_LED_INTERVAL_TIMER, &seconds);      //Gets the total duration in seconds of that Timer
        printf("hit LED timer finished. Lockout Duration: %lf seconds.\n\r", seconds);  // Print the results
        intervalTimer_reset(HIT_LED_INTERVAL_TIMER);        // Reset Interval timer.
        utils_msDelay(HIT_LED_TEST_DELAY);                  // Delay 300ms in between tests
    }
    printf("completed hitLedTimer_runTest().\n\r");
    while ((buttons_read() & BUTTONS_BTN3_MASK));   // Wait until exit button3 is released before moving on DANGER HERE
}

// Standard debug state print function
void hitLedTimer_debugStatePrint()
{
    static HITLEDTIMER_st_t previousState;  // Create previous state variable
    static bool firstPass = true;           // Create the first pass variable

    // If the previous state is not the current state or if it is the first pass
    if (previousState != HITLEDTIMER_CurrentState || firstPass)
    {
        firstPass = false;                              // Update the firstPass, since we're on at least the first pass now
        previousState = HITLEDTIMER_CurrentState;       // Update the previous state to the current state

        switch(HITLEDTIMER_CurrentState)                // Switch based on the current state
        {
        case HITLEDTIMER_init:                          // If the current state is HITLEDTIMER_init
            printf("hitLedTimer State: init_st\n\r");   // Print the state debug statement
            break;
        case HITLEDTIMER_wait:                          // If the current state is HITLEDTIMER_wait
            printf("hitLedTimer State: wait_st\n\r");   // Print the state debug statement
            break;
        case HITLEDTIMER_running:                       // If the current state is HITLEDTIMER_running
            printf("hitLedTimer State: running_st\n\r");// Print the state debug print
            break;
        }
    }
}

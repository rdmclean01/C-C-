/*
 * lockoutTimer.c
 *
 *  Created on: Feb 28, 2017
 *      Author: rpn314
 */

#include "lockoutTimer.h"               // Lockout Timer header file (it's own header file)
#include <stdbool.h>                    // Standard boolean support
#include <stdint.h>                     // Standard int support
#include <stdio.h>                      // Standard I/O (for printf functions)
#include "filter.h"                     // Used to get filter sample frequency
#include "supportFiles/buttons.h"       // Buttons (on board) support. Used only in test function
#include "supportFiles/utils.h"         // Utilities support. Used only in test function
#include "supportFiles/intervalTimer.h" // Interval timer support. Used only in test function

#define BUTTONS_BTN0_MASK 0x1                   // Button mask to read only button 0
#define BUTTONS_BTN3_MASK 0x8                   // Button mask to read only button 3
#define LOCKOUT_TIMER_INTERVAL_TIMER_NUMBER 1   // Interval timer number. Used only in test function

static bool lockoutTimerRunning;    // Boolean to determine if the timer is running
static uint16_t lockoutTimer_timer; // Keep track of current place in count

// Enumeration for state machine states
enum LOCKOUTTIMER_st_t{
    LOCKOUTTIMER_init,  // Initialize the state machine
    LOCKOUTTIMER_wait,   // Wait for the timer to be started
    LOCKOUTTIMER_running // Timer is running: waiting for timer to expire
} LOCKOUTTIMER_CurrentState = LOCKOUTTIMER_init;

void lockoutTimer_debugStatePrint();// Function Prototype

// Standard init function.
void lockoutTimer_init()
{
    lockoutTimerRunning = false;// Initialize the timer to not be running
    lockoutTimer_timer = 0;     // Initialize the lockout timer
}

// Calling this starts the timer.
void lockoutTimer_start(){lockoutTimerRunning = true;}

// Returns true if the timer is running.
bool lockoutTimer_running(){return lockoutTimerRunning;}

// Standard tick function
void lockoutTimer_tick()
{
//    lockoutTimer_debugStatePrint();     // Print the debug statements
    switch(LOCKOUTTIMER_CurrentState)   // State (Moore) Actions
    {
    case LOCKOUTTIMER_init:     // If the current state is LOCKOUTTIMER_init
        break;
    case LOCKOUTTIMER_wait:     // If the current state is LOCKOUTTIMER_wait
        lockoutTimer_timer = 0; // Reset the lockout timer
        break;
    case LOCKOUTTIMER_running:  // If the current state is LOCKOUTTIMER_running
        lockoutTimer_timer++;   // Increment the timer
        break;
    }


    switch(LOCKOUTTIMER_CurrentState)   // Transition (Mealy) Actions
    {
    case LOCKOUTTIMER_init:     // If the current state is LOCKOUTTIMER_init
        LOCKOUTTIMER_CurrentState = LOCKOUTTIMER_wait;
        break;
    case LOCKOUTTIMER_wait:     // If the current state is LOCKOUTTIMER_wait
        if (lockoutTimerRunning)// If the timer has been triggered
        {
            LOCKOUTTIMER_CurrentState = LOCKOUTTIMER_running;   // Move to the running state
        }
        break;
    case LOCKOUTTIMER_running:  // If the current state is LOCKOUTTIMER_running
        if (lockoutTimer_timer >= LOCKOUT_TIMER_EXPIRE_VALUE)   // If the lockout timer has expired
        {
            LOCKOUTTIMER_CurrentState = LOCKOUTTIMER_wait;  // Move to the wait state
            lockoutTimerRunning = false;                    // Disable the lockout timer now that the time has expired
        }
        break;
    }
}

// Tests the lockout timer state machine
void lockoutTimer_runTest()
{
    lockoutTimer_init();    // Initialize the hit LED timer
    intervalTimer_init(LOCKOUT_TIMER_INTERVAL_TIMER_NUMBER);    // Initialize an interval timer (#1) to test the lockout timer
    printf("starting lockoutTimer_runTest(). Runs on button press. btn0: run test. btn3: exit test\n\r"); // Print test instructions
    while(!(buttons_read() & BUTTONS_BTN3_MASK)) // Leaves the trigger_runTest() when Button 3 is pressed
    {
        printf("starting lockoutTimer test loop\n\r");
        intervalTimer_start(LOCKOUT_TIMER_INTERVAL_TIMER_NUMBER);   // Start the interval timer
        lockoutTimer_start();                                       // Start the lockout timer state machine
        while(lockoutTimer_running());                              // Wait for the timer to stop running
        intervalTimer_stop(LOCKOUT_TIMER_INTERVAL_TIMER_NUMBER);    // Stop the interval timer
        double seconds;
        intervalTimer_getTotalDurationInSeconds(LOCKOUT_TIMER_INTERVAL_TIMER_NUMBER, &seconds); // Get the seconds duration from the interval timer
        printf("lockout timer test loop finished. Lockout Duration: %lf seconds.\n\r", seconds);// Print the results
        intervalTimer_reset(LOCKOUT_TIMER_INTERVAL_TIMER_NUMBER);                               // Reset the interval timer
        while(!(buttons_read() & BUTTONS_BTN0_MASK) && !(buttons_read() & BUTTONS_BTN3_MASK));  // Waits for a button to leave. btn0 to test again. btn2 to exit
    }
    printf("exiting lockoutTimer_runTest().\n\r");
    while ((buttons_read() & BUTTONS_BTN3_MASK));   // Wait until exit button is released before moving on
}

// Standard debug state print function
void lockoutTimer_debugStatePrint()
{
    static LOCKOUTTIMER_st_t previousState; // Create previous state variable
    static bool firstPass = true;           // Create the first pass variable

    // If the previous state is not the current state or if it is the first pass
    if (previousState != LOCKOUTTIMER_CurrentState || firstPass)
    {
        firstPass = false;                          // Update the firstPass, since we're on at least the first pass now
        previousState = LOCKOUTTIMER_CurrentState;  // Update the previous state to the current state

        switch(LOCKOUTTIMER_CurrentState)           // Switch based on the current state
        {
        case LOCKOUTTIMER_init:                         // If the current state is LOCKOUTTIMER_init
            printf("lockoutTimer State: init_st\n\r");  // Print the state debug statement
            break;
        case LOCKOUTTIMER_wait:                         // If the current state is LOCKOUTTIMER_wait
            printf("lockoutTimer State: wait_st\n\r");  // Print the state debug statement
            break;
        case LOCKOUTTIMER_running:                          // If the current state is LOCKOUTTIMER_running
            printf("lockoutTimer State: running_st\n\r");   // Print the state debug print
            break;
        }
    }
}

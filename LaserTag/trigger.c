/*
 * trigger.c
 *
 *  Created on: Feb 28, 2017
 *      Author: rpn314
 */

#include "trigger.h"                // Trigger header file (it's own header file)
#include <stdbool.h>                // Standard boolean support
#include <stdint.h>                 // Standard int support
#include <stdio.h>                  // Standard I/O (for printf functions)
#include "filter.h"                 // Used to get filter sample frequency
#include "supportFiles/mio.h"       // I/O pin (on board) support
#include "supportFiles/buttons.h"   // Buttons (on board) support. Used only in test function
#include "supportFiles/utils.h"     // Utilities support. Used only in test function
#include "transmitter.h"            // Transmitter header file. Starts transmitter state machine after debouncing trigger

static bool triggerTimerEnabled;        // Boolean to determine if the timer is enabled
static bool triggerPreviouslyPressed;   // Boolean to determine if the previous debounced state was "pressed"
static bool ignoreGunInput;             // If we need to ignore the gun input (gun wasn't plugged in and floating high on startup)
static bool trigger_testMode;           // If the state machine is in test mode.
static uint16_t trigger_timer, debounce_time;

#define DEBOUNCE_TIME FILTER_SAMPLE_FREQUENCY_IN_KHZ*50 // Debounce time. 50ms * 100kHz
#define TRIGGER_TEST_TICK_PERIOD_IN_MS  10              // Test tick period
#define TEST_DEBOUNCE_TIME 5                            // Test debounce time
#define TRIGGER_GUN_PIN 10                              // MIO Pin corresponding to the gun trigger
#define GUN_TRIGGER_PRESSED 1                           // MIO return value when trigger is pressed
#define GUN_TRIGGER_RELEASED 0                          // MIO return value when trigger is not pressed
#define BUTTONS_BTN0_MASK 0x1                           // Button mask to read only button 0
#define BUTTONS_BTN3_MASK 0x8                           // Button mask to read only button 2

// Enumeration for state machine states
enum TRIGGER_st_t{
    TRIGGER_init,   // Initialize the state machine
    TRIGGER_wait,   // Wait for the timer to be started
    TRIGGER_running // Timer is running: waiting for timer to expire
} TRIGGER_CurrentState = TRIGGER_init;

void trigger_debugStatePrint(); // Function Prototype

// Checks if the trigger is currently being pressed
// "Trigger" can be input from gun or BTN0
bool trigger_pressed()
{
    return ((!ignoreGunInput & (mio_readPin(TRIGGER_GUN_PIN) == GUN_TRIGGER_PRESSED)) ||
            (buttons_read() & BUTTONS_BTN0_MASK));
}


// Initialize the trigger
// Also checks if the input trigger pin should be ignored
void trigger_init()
{
    mio_init(false);                        // Initialize the IO
    mio_setPinAsInput(TRIGGER_GUN_PIN);     // Set the gun trigger pin to be an input
    transmitter_setContinuousMode(false);   // Set the transmitter state machine to non-continous mode
    ignoreGunInput = false;                 // Set ignore gun input to false initially
    debounce_time = DEBOUNCE_TIME;          // Set the debounce time to 50ms for 100kHz tick frequency
    trigger_timer = 0;                      // Initialize the trigger timer
    if (trigger_pressed())                  // If the trigger is currently being pressed
    {
        ignoreGunInput = true;              // Assume the gun is not connect and ignore it's input
    }
}

// Enables the trigger timer state machine
void trigger_enable(){triggerTimerEnabled = true;}

// Standard tick function
void trigger_tick()
{
    if (trigger_testMode)
        trigger_debugStatePrint();  // Print out debug messages
    switch(TRIGGER_CurrentState)    // State (Moore) Actions
    {
    case TRIGGER_init:      // If the current state is TRIGGER_init
        break;
    case TRIGGER_wait:      // If the current state is TRIGGER_wait
        trigger_timer = 0;  // Reset trigger timer
        break;
    case TRIGGER_running:   // If the current state is TRIGGER_running
        trigger_timer++;    // Increment the debounce timer
        break;
    }

    switch(TRIGGER_CurrentState)                    // Transition (Mealy) Actions
    {
    case TRIGGER_init:                              // If the current state is TRIGGER_init
        if (triggerTimerEnabled)                    // If the trigger state machine is enabled
            TRIGGER_CurrentState = TRIGGER_wait;    // Move to the TRIGGER_wait state
        break;
    case TRIGGER_wait:                              // If the current state is TRIGGER_wait
        if (trigger_pressed() != triggerPreviouslyPressed)  // If the current trigger pressed state does not match the previous state
            TRIGGER_CurrentState = TRIGGER_running; // Move to the TRIGGER_running state
        break;
    case TRIGGER_running:                           // If the current state is TRIGGER_running
        if (trigger_pressed() == triggerPreviouslyPressed)  // If the current trigger pressed state matches the previous pressed state
            TRIGGER_CurrentState = TRIGGER_wait;    // Move back to the TRIGGER_wait state
        else if (trigger_timer == debounce_time)    // If the trigger debounce timer has expired
        {
            TRIGGER_CurrentState = TRIGGER_wait;    // Move back to the TRIGGER_wait state
            triggerPreviouslyPressed = trigger_pressed();   // Update the previous pressed state to the new debounced state
            if (trigger_pressed())      // If the new debounce state is the trigger pressed
            {
                if (trigger_testMode)   // If the trigger state machine is in test mode
                    printf("Down\n\r"); // Print the down (presssed) debug statement
                else                    // If the trigger state machine is in regular mode
                    transmitter_run();  // Start the transmitter state machine
            }
            else if (trigger_testMode)  // If the new debounce state is trigger not pressed and we're in test mode
                printf("Up\n\r");       // Print the up (not pressed) debug statement
        }
        break;
    }
}

// Tests the trigger state machine
void trigger_runTest()
{
    trigger_testMode = true;    // Set the state machine to test mode (for debug print statements)
    trigger_init();             // Initialize the trigger state machine
    debounce_time = TEST_DEBOUNCE_TIME; // Set the debounce time to 50ms for 10ms tick period
    printf("starting trigger_runTest(). Runs continuously. btn0: trigger, btn3: exit test\n\r");
    trigger_enable();                   // Enable the trigger state machine
    while(!(buttons_read() & BUTTONS_BTN3_MASK)) // Leaves the trigger_runTest() when Button 2 is pressed
    {
        utils_msDelay(TRIGGER_TEST_TICK_PERIOD_IN_MS);  // short delay between ticks.
    }
    printf("exiting trigger_runTest()\n\r");
    while ((buttons_read() & BUTTONS_BTN3_MASK));       // Wait until exit button3 is released before moving on
}

// Standard debug state print
void trigger_debugStatePrint()
{
    static TRIGGER_st_t previousState;  // Create previous state variable
    static bool firstPass = true;       // Create the first pass variable

    // If the previous state is not the current state or if it is the first pass
    if (previousState != TRIGGER_CurrentState || firstPass)
    {
        firstPass = false;                      // Update the firstPass, since we're on at least the first pass now
        previousState = TRIGGER_CurrentState;   // Update the previous state to the current state

        switch(TRIGGER_CurrentState)            // Switch based on the current state
        {
        case TRIGGER_init:                          // If the current state is TRIGGER_init
            printf("TRIGGER State: init_st\n\r");   // Print the state debug statement
            break;
        case TRIGGER_wait:                          // If the current state is TRIGGER_wait
            printf("TRIGGER State: wait_st\n\r");   // Print the state debug statement
            break;
        case TRIGGER_running:                       // If the current state is TRIGGER_running
            printf("TRIGGER State: running_st\n\r");// Print the state debug print
            break;
        }
    }
}

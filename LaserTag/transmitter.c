/*
 * transmitter.c
 *
 *  Created on: Feb 28, 2017
 *      Author: rpn314
 */

#include "transmitter.h"            // Transmitter header file (it's own header)
#include "filter.h"                 // Included to get the player frequencies
#include <stdbool.h>                // Standard boolean support
#include <stdio.h>                  // Standard I/O (for printf functions)
#include "supportFiles/mio.h"       // Zybo board I/O - Used to send out pulses
#include "supportFiles/buttons.h"   // Buttons (on board) support. Used only in test function
#include "supportFiles/switches.h"  // Switches (on board) support. Used only in test function
#include "supportFiles/utils.h"     // Utilities support. Used only in test function

#define TRANSMITTER_TEST_TICK_PERIOD_IN_MS 10   // Tick period for testing
#define TEST_PULSE_WIDTH 200    // Pulse width for testing
#define MIN_PLAYER_NUMBER 0     // Minimum player number
#define MAX_PLAYER_NUMBER 9     // Maximum player number
#define TRANSMIT_HIGH 1         // Constant for Transmitter High
#define TRANSMIT_LOW 0          // Constant for Transmitter Low
#define BUTTONS_BTN0_MASK 0x01  // Button mask to only read Button 0
#define BUTTONS_BTN3_MASK 0x08  // Button mask to only read Button 3
#define HALF_PERIOD_SCALAR 2    // Used to divide the table values by two

static bool transmitterRunning;         // Determines if the state machine is currently running
static bool transmitterContinuousMode;  // Set the state machine to continuous mode (principally for testing)
static bool transmitterTestMode;        // Sets the state machine into test mode
static uint16_t halfPeriodTime;         // Time for half a period (50% duty cycle, the time to transmit a high and a low)
static uint16_t nextHalfPeriodTime;     // The period time for the next 200ms pulse
static uint16_t transmitTimer;          // The timer for how the 200ms puls
static uint16_t highLowTimer;           // The timer for a single high or low time
static uint16_t pulseWidth;             // The expire time for a pulse width so it can be changed for testing

// Enumeration for state machine states
enum TRANSMITTER_st_t{
    TRANSMITTER_init,   // Initialize the state machine
    TRANSMITTER_wait,   // Wait for the timer to be started
    TRANSMITTER_high,    // Transmit a high. Waiting for half period time to expire
    TRANSMITTER_low     // Transmit a low. Waiting for half period time or total time to expire
} TRANSMITTER_CurrentState = TRANSMITTER_init;  // Initialize the state to the init state

void transmitter_debugStatePrint(); // Function prototype

// Initialize the transmitter
void transmitter_init()
{
    transmitterTestMode = false;                // Initialize test mode to disabled
    pulseWidth = TRANSMITTER_PULSE_WIDTH;       // Initialize the transmitter pulse witdh
    mio_init(false);                            // Initialize the IO
    mio_setPinAsOutput(TRANSMITTER_OUTPUT_PIN); // Set the transmitter pin to be an output
}

// Enables the transmitter timer state machine
void transmitter_run(){transmitterRunning = true;}

// Returns true if the transmitter is still running.
bool transmitter_running(){return transmitterRunning;}

// Set the frequency that is transmitted. Corresponds to player frequencies in filter.h Min = 0, Max = 9.
void transmitter_setFrequencyNumber(uint16_t frequencyNumber)
{
    //    if (frequencyNumber < MIN_PLAYER_NUMBER)        // If the input frequencyNumber is below the number of players
    //        nextHalfPeriodTime = filter_frequencyTickTable[MIN_PLAYER_NUMBER];  // Set the next half period time to the lowest player number time
    if (frequencyNumber > MAX_PLAYER_NUMBER)   // If the input frequencyNumber is above the number of players
        nextHalfPeriodTime = filter_frequencyTickTable[MAX_PLAYER_NUMBER]/HALF_PERIOD_SCALAR;  // Set the next half period time to the highest player number time MAGIC
    else                                            // The input frequency number is in bounds
        nextHalfPeriodTime = filter_frequencyTickTable[frequencyNumber]/HALF_PERIOD_SCALAR;    // Set the next half period time to the corresponding time MAGIC
}

// Standard tick function
void transmitter_tick()
{
    if (transmitterTestMode)            // If test mode is enabled
    {
        transmitter_debugStatePrint();  // Print the debug statements
    }
    switch(TRANSMITTER_CurrentState)    // State (Moore) Actions
    {
    case TRANSMITTER_init:          // If the current state is TRANSMITTER_init
        transmitTimer = 0;          // Initialize the transmit timer (counting to 200ms)
        highLowTimer = 0;           // Initialize the high/low period timer
        transmitterRunning = false; // Initialize the transmitter to not running
        break;
    case TRANSMITTER_wait:          // If the current state is TRANSMITTER_wait
        transmitTimer = 0;          // Reset the transmit timer
        highLowTimer = 0;           // Reset the period timer
        break;
    case TRANSMITTER_high:      // If the current state is TRANSMITTER_high
        transmitTimer++;        // Increment the transmit timer
        highLowTimer++;         // Increment the high/low timer
        break;
    case TRANSMITTER_low:       // If the current state is TRANSMITTER_low
        transmitTimer++;        // Increment the timer
        highLowTimer++;         // Increment the high/low timer
        break;
    }


    switch(TRANSMITTER_CurrentState)   // Transition (Mealy) Actions
    {
    case TRANSMITTER_init:                                      // If the current state is TRANSMITTER_init
        TRANSMITTER_CurrentState = TRANSMITTER_wait;            // Move to the TRANSMITTER_wait state
        break;
    case TRANSMITTER_wait:                                      // If the current state is TRANSMITTER_wait
        if (transmitterRunning)                                 // If the timer has been triggered.
        {
            mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMIT_HIGH);// Write high to the transmit pin
            TRANSMITTER_CurrentState = TRANSMITTER_high;        // Move to the high state
            halfPeriodTime = nextHalfPeriodTime;                // Update the half period time to the next period time
        }
        break;
    case TRANSMITTER_high:                                      // If the current state is TRANSMITTER_high
        if (transmitTimer == pulseWidth)                        // If the full transmission pulse timer has expired
        {
            mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMIT_LOW); // Write low to the transmit pin
            TRANSMITTER_CurrentState = TRANSMITTER_wait;        // Move back to the wait state
            if (!transmitterContinuousMode)                     // If the transmitter is not in continuous mode
                transmitterRunning = false;                     // Reset the transmitter running boolean if we're not in continuous mode
        }
        else if (highLowTimer == halfPeriodTime)                // If the high/low timer has expired but the full transmission time has not
        {
            mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMIT_LOW); // Write low to the transmit pin
            TRANSMITTER_CurrentState = TRANSMITTER_low;         // Move to the low state
            highLowTimer = 0;                                   // Reset the high/low period timer
        }
        break;
    case TRANSMITTER_low:                                       // If the current state is TRANSMITTER_low
        if (transmitTimer == pulseWidth)                        // If the full transmission pulse timer has expired
        {
            TRANSMITTER_CurrentState = TRANSMITTER_wait;        // Move back to the wait state
            if (!transmitterContinuousMode)                     // If the state machine is not in continuous mode
                transmitterRunning = false;                     // Reset the transmitter running boolean if we're not in continuous mode
        }
        else if (highLowTimer == halfPeriodTime)                // If the high/low timer has expired but the full transmission time has not
        {
            mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMIT_HIGH);// Write high to the transmit pin
            TRANSMITTER_CurrentState = TRANSMITTER_high;        // Move to the high state
            highLowTimer = 0;                                   // Reset the high/low period timer
        }
        break;
    }
}

// Function to test transmitter.
// Running this will test the whole transmitter and print debug statements
void transmitter_runTest()
{
    printf("starting transmitter_runTest()\n\r");               // Starting test debug statement
    buttons_init();                                             // Using buttons
    switches_init();                                            // and switches.
    transmitter_init();                                         // init the transmitter.
    //transmitter_enableTestMode();                               // Prints diagnostics to stdio.
    printf("starting transmitter test. Continuous mode. btn0: exit test (after 200ms pulse completes)\n\r");     // Print debug statement
    while (!(buttons_read() & BUTTONS_BTN3_MASK)) {             // Btn3 to exit continuous mode
        uint16_t switchValue = switches_read() % FILTER_FREQUENCY_COUNT;  // Compute a safe number from the switches.
        transmitter_setFrequencyNumber(switchValue);            // set the frequency number based upon switch value.
        transmitter_run();                                      // Start the transmitter.
        // This while loop is for testing, and should not be used when tick is in the ISR
        //        while (transmitter_running()) {                         // Keep ticking until it is done.
        //            //transmitter_tick();                                 // run the tick.
        //            //utils_msDelay(TRANSMITTER_TEST_TICK_PERIOD_IN_MS);  // short delay between ticks.
        //        }

        printf("completed one test period.\n\r");               // Completion of one test period debug statement
    }
    printf("finished transmitter continuous mode test.\n\rstarting transmitter non-continuous mode. Runs on button press btn0: trigger, btn3: exit test\n\r");
    while ((buttons_read() & BUTTONS_BTN3_MASK));               // Wait until exit button is released before moving on
    while(!(buttons_read() & BUTTONS_BTN3_MASK))                // Btn3 to exit non-continuous mode
    {
        while (!(buttons_read() & BUTTONS_BTN0_MASK));          // Wait until btn0 is pressed.
        uint16_t switchValue = switches_read() % FILTER_FREQUENCY_COUNT;    // Compute a safe number from the switches.
        transmitter_setFrequencyNumber(switchValue);            // set the frequency number based upon switch value.
        transmitter_run();                                      // Start the transmitter.
        // This while loop is for testing, and should not be used when tick is in the ISR
        //        while (transmitter_running()) {                         // Keep ticking until it is done.
        //            //transmitter_tick();                                 // tick.
        //           //utils_msDelay(TRANSMITTER_TEST_TICK_PERIOD_IN_MS);  // short delay between ticks.
        //        }
        printf("completed one test period.\n\r");               // Completion of one test period debug statement
        while (!(buttons_read() & BUTTONS_BTN0_MASK) && !(buttons_read() & BUTTONS_BTN3_MASK)); // Wait until btn0 (to run again) or btn2 (to exit) is pressed.
    }
    printf("exiting transmitter_runTest()\n\r");    // Completion of test debug statement
    while ((buttons_read() & BUTTONS_BTN3_MASK));   // Wait until exit button is released before moving on
}

// Set the transmitter to continuous (true) or single (false) mode
void transmitter_setContinuousMode(bool continuousModeFlag){transmitterContinuousMode = continuousModeFlag;}

// Enables the test mode.
// Prints diagnostics to stdio.
void transmitter_enableTestMode()
{
    transmitterTestMode = true;     // Change the test mode to true (for debug printing)
    pulseWidth = TEST_PULSE_WIDTH;  // Change the pulse width for testing
}

// Disables the test mode
void transmitter_disableTestMode()
{
    transmitterTestMode = false;            // Change the test mode back to false
    pulseWidth = TRANSMITTER_PULSE_WIDTH;   // Change the pulse width back to the default
}

// Standard debug state print
void transmitter_debugStatePrint()
{
    static TRANSMITTER_st_t previousState; // Create previous state variable
    static bool firstPass = true;          // Create the first pass variable

    // If the previous state is not the current state or if it is the first pass
    if (previousState != TRANSMITTER_CurrentState || firstPass)
    {
        firstPass = false;                          // Update the firstPass, since we're on at least the first pass now
        previousState = TRANSMITTER_CurrentState;   // Update the previous state to the current state

        switch(TRANSMITTER_CurrentState)            // Switch based on the current state
        {
        case TRANSMITTER_init:                          // If the current state is TRANSMITTER_init
            printf("TRANSMITTER State: init_st\n\r");   // Print the state debug statement
            break;
        case TRANSMITTER_wait:                          // If the current state is TRANSMITTER_wait
            printf("TRANSMITTER State: wait_st\n\r");   // Print the state debug statement
            break;
        case TRANSMITTER_high:                          // If the current state is TRANSMITTER_high
            printf("TRANSMITTER State: high_st\n\r");   // Print the state debug print
            break;
        case TRANSMITTER_low:                           // If the current state is TRANSMITTER_low
            printf("TRANSMITTER State: low_st\n\r");    // Print the state debug print
            break;
        }
    }
}


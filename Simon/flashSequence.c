/*
 * flashSequence.c
 *
 *  Created on: Nov 2, 2015
 *      Author: rdmclean
 */

#include "flashSequence.h"

bool flashSequenceEnable;
bool flashSequenceComplete;
static uint16_t iteration_counter;
static uint16_t flashSequence_iterationLength;
static uint8_t flashSequence_regionNo;
static uint16_t flashSequence_flashTimer;
#define EXPIRED_VALUE_FLASH 10

enum FLASHSEQUENCE_CurState{
    FLASHSEQUENCE_init, FLASHSEQUENCE_flash,
    FLASHSEQUENCE_eraseSquare, FLASHSEQUENCE_endState
} FLASHSEQUENCE_CurState = FLASHSEQUENCE_init;

void flashSequence_enable(){flashSequenceEnable = true;}//Turns on the state machine. Part of the interlock.
void flashSequence_disable(){flashSequenceEnable = false;}// Turns off the state machine. Part of the interlock.
bool flashSequence_completed(){return flashSequenceComplete;}// Other state machines can call this to determine if this state machine is finished.

// Standard tick function.
void flashSequence_tick(){
    switch(FLASHSEQUENCE_CurState){ //State Actions
    case FLASHSEQUENCE_init:
        //printf("\tFLASHSEQUENCE: init\n");
        iteration_counter = 0;                                               //Keeps track of place in sequence
        flashSequence_flashTimer= 0;                                         //A timer to make the flash see-able
        flashSequence_iterationLength = globals_getSequenceIterationLength();//Get new length each time
        flashSequence_regionNo = globals_getSequenceValue(iteration_counter);//Get region to flash
        flashSequenceComplete = false;                                       //Sequence is not yet complete
        break;
    case FLASHSEQUENCE_flash:
       //printf("\tFLASHSEQUENCE: flash\n");
        simonDisplay_drawSquare(flashSequence_regionNo, false); // Draws a colored square at the correct region
        flashSequence_flashTimer++;                             //Increments readability timer
        break;
    case FLASHSEQUENCE_eraseSquare:
        //printf("\tFLASHSEQUENCE: erase\n");
        flashSequence_flashTimer = 0;                           //Reset the readability timer for next use
        simonDisplay_drawSquare(flashSequence_regionNo, true);  // Erases a colored square at the correct region
        iteration_counter++;                                    //Iterates to next place in sequence
        //printf("Iteration Counter %d\n", iteration_counter);
        //printf("Looking for %d\n", flashSequence_iterationLength);
        if(iteration_counter < flashSequence_iterationLength){                   //If sequence is not over
            flashSequence_regionNo = globals_getSequenceValue(iteration_counter);// get the new region number
        }
        //else if(iteration_counter >= globals_getSequenceLength()){            THIS LINE PASSED TESTS
        else if(iteration_counter >= globals_getSequenceIterationLength()){     //THIS LINE IS BETTER I THINK
            flashSequenceComplete = true;
        }
        break;
    case FLASHSEQUENCE_endState:
        //printf("\tFLASHSEQUENCE: End State\n");
        break;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    switch(FLASHSEQUENCE_CurState){ //Transition and Mealy Actions
    case FLASHSEQUENCE_init:
        if(flashSequenceEnable){
            FLASHSEQUENCE_CurState = FLASHSEQUENCE_flash;       // Enter the state machine when enabled
        }
        else
            FLASHSEQUENCE_CurState = FLASHSEQUENCE_init;        // Stay in init until enabled
        break;
    case FLASHSEQUENCE_flash:
        if(flashSequence_flashTimer == EXPIRED_VALUE_FLASH)    //Square has been on screen long
            FLASHSEQUENCE_CurState = FLASHSEQUENCE_eraseSquare;// enough, move to erase state
        else
            FLASHSEQUENCE_CurState = FLASHSEQUENCE_flash;       //Stay until length expires
        break;
    case FLASHSEQUENCE_eraseSquare:
        if(flashSequence_completed()){                          //When sequence is done
            FLASHSEQUENCE_CurState = FLASHSEQUENCE_endState;    // go to the end
        }
        else if(iteration_counter < flashSequence_iterationLength){//If not done,
            FLASHSEQUENCE_CurState = FLASHSEQUENCE_flash;          //go back to flash
        }
        else
            FLASHSEQUENCE_CurState = FLASHSEQUENCE_eraseSquare; //Default: Stay here
        break;                                                  // until something happens
    case FLASHSEQUENCE_endState:
        if(flashSequenceEnable == false){                       //When state machine is disabled
            FLASHSEQUENCE_CurState = FLASHSEQUENCE_init;        // safe to go back to init
        }
        else                                                    //Stay here until disabled
            FLASHSEQUENCE_CurState = FLASHSEQUENCE_endState;
        break;
    }
}

/*****************************************************
 * Everything below this line corresponds to test code
 *****************************************************/
// This will set the sequence to a simple sequential pattern.
// It starts by flashing the first color, and then increments the index and flashes the first
// two colors and so forth. Along the way it prints info messages to the LCD screen.
#define TEST_SEQUENCE_LENGTH 8  // Just use a short test sequence.
uint8_t flashSequence_testSequence[TEST_SEQUENCE_LENGTH] = {SIMON_DISPLAY_REGION_0,
        SIMON_DISPLAY_REGION_1,
        SIMON_DISPLAY_REGION_2,
        SIMON_DISPLAY_REGION_3,
        SIMON_DISPLAY_REGION_3,
        SIMON_DISPLAY_REGION_2,
        SIMON_DISPLAY_REGION_1,
        SIMON_DISPLAY_REGION_0};
#define INCREMENTING_SEQUENCE_MESSAGE1 "Incrementing Sequence"  // Info message.
#define RUN_TEST_COMPLETE_MESSAGE "Runtest() Complete"      // Info message.
#define MESSAGE_TEXT_SIZE 2                                 // Make the text easy to see.

// Print the incrementing sequence message.
void flashSequence_printIncrementingMessage() {
    display_fillScreen(DISPLAY_BLACK);// Otherwise, tell the user that you are incrementing the sequence.
    display_setCursor(0, display_height()/2);     // Roughly centered.
    display_println(INCREMENTING_SEQUENCE_MESSAGE1);  // Print the message.
    utils_msDelay(2000);                              // Hold on for 2 seconds.
    display_fillScreen(DISPLAY_BLACK);            // Clear the screen.
}

void flashSequence_runTest() {
    display_init();   // We are using the display.
    display_fillScreen(DISPLAY_BLACK);    // Clear the display.
    globals_setSequence(flashSequence_testSequence, TEST_SEQUENCE_LENGTH);    // Set the sequence.
    flashSequence_enable();                   // Enable the flashSequence state machine.
    int16_t sequenceLength = 1;                           // Start out with a sequence of length 1.
    globals_setSequenceIterationLength(sequenceLength);   // Set the iteration length.
    display_setTextSize(MESSAGE_TEXT_SIZE);           // Use a standard text size.
    while (1) {                   // Run forever unless you break.
        flashSequence_tick();   // tick the state machine.
        utils_msDelay(1);   // Provide a 1 ms delay.
        if (flashSequence_completed()) {  // When you are done flashing the sequence.
            flashSequence_disable();  // Interlock by first disabling the state machine.
            flashSequence_tick(); // tick is necessary to advance the state.
            utils_msDelay(1);     // don't really need this here, just for completeness.
            flashSequence_enable();   // Finish the interlock by enabling the state machine.
            utils_msDelay(1); // Wait 1 ms for no good reason.
            sequenceLength++; // Increment the length of the sequence.
            if (sequenceLength > TEST_SEQUENCE_LENGTH) // Stop if you have done the full sequence.
                break;
            flashSequence_printIncrementingMessage();  // Tell the user that you are going to the next step in the pattern.
            globals_setSequenceIterationLength(sequenceLength);   // Set the length of the pattern.
        }
    }
    // Let the user know that you are finished.
    display_fillScreen(DISPLAY_BLACK);
    display_setCursor(0, display_height()/2);
    display_println(RUN_TEST_COMPLETE_MESSAGE);
}



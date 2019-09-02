/*
 * verifySequence.c
 *
 *  Created on: Nov 4, 2015
 *      Author: rdmclean
 */
// State machine will run when enabled.

#include "verifySequence.h"
//#include "src/SwitchesAndButtons/buttons.h"

bool verifySequenceEnable;
bool verifySequence_TimeOut;
bool verifySequence_UserInput_Error;
bool verifySequence_Complete;
static uint8_t regionNo;
static uint16_t verifySeq_TimeOut_Timer;
static uint16_t iteration_Counter;
#define EXPIREDVALUE_TIMEOUT 50

// This is part of the interlock. You disable the state-machine and then enable it again.
void verifySequence_enable(){verifySequenceEnable = true;}              //Function enables the state machine to start
void verifySequence_disable(){verifySequenceEnable = false;}            //Function disables the state machine to end
bool verifySequence_isTimeOutError(){return verifySequence_TimeOut;}    //Used to detect if there has been a time-out error.
bool verifySequence_isUserInputError(){return verifySequence_UserInput_Error;}//Used to detect if the user tapped the incorrect sequence.
bool verifySequence_isComplete(){return verifySequence_Complete;}       // Used to detect if the verifySequence state machine has finished verifying.

enum VERIFYSEQUENCE_CurState{
    VERIFYSEQUENCE_init, VERIFYSEQUENCE_enableButtonHandler,
    VERIFYSEQUENCE_verify, VERIFYSEQUENCE_endState
} VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_init;

/**** Standard tick function for the verifySequence State Machine***********/
void verifySequence_tick(){
    switch(VERIFYSEQUENCE_CurState){        //State ACTIONS
    case VERIFYSEQUENCE_init:               //Init state to begin
        //printf("\tVERIFY SEQUENCE: init\n");
        verifySeq_TimeOut_Timer = 0;     //Enable the timer that counts for Time Out Error
        iteration_Counter = 0;           //Counter to keep track of place in sequence
        verifySequence_TimeOut = false;  //Initialize so user has time to react
        verifySequence_UserInput_Error = false;//Initialize user input, he has not messed up yet
        verifySequence_Complete = false; //The sequence is not complete
        break;
    case VERIFYSEQUENCE_enableButtonHandler: //Waits for Button Handler to perform its whole machine
        //printf("\tVERIFY SEQUENCE: enableButtons\n");
        verifySeq_TimeOut_Timer++;          //Increment the timer, checking for timeout error
        buttonHandler_enable();             //Enable the button Handler state Machine
        break;                              // to start it up
    case VERIFYSEQUENCE_verify:             //Verifies if touch was correct
        //printf("\tVERIFY SEQUENCE: verify\n");//No actions needed
        break;
    case VERIFYSEQUENCE_endState:           //State to finalize the state machine
        //printf("\tVERIFY SEQUENCE: endState\n");
        break;
    }
    switch(VERIFYSEQUENCE_CurState){ //TRANSITIONS and Mealy Actions
    case VERIFYSEQUENCE_init:
        if(verifySequenceEnable){                           //When enabled, move into the state machine
            VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_enableButtonHandler;
        }
        else
            VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_init;  //Wait in init until enabled
        break;
    case VERIFYSEQUENCE_enableButtonHandler:
        if(verifySeq_TimeOut_Timer >= EXPIREDVALUE_TIMEOUT){    //User took too long
            verifySequence_TimeOut = true;                          //Time out error
            verifySequence_Complete = true;                         // situation
            VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_endState;      //Send to the endState
        }
        else if(buttonHandler_releaseDetected()){               //User has touched board
            regionNo = buttonHandler_getRegionNumber();             //Receives region number
            buttonHandler_disable();                                //Restart the button Handler
            VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_verify;        //Go to the verify state
        }
        else
            VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_enableButtonHandler;//Wait until user does something
        break;
    case VERIFYSEQUENCE_verify:
        if(regionNo == globals_getSequenceValue(iteration_Counter)){        //User pressed correct square
            if(iteration_Counter >= globals_getSequenceIterationLength()- 1){   //If sequence is completed,
                verifySequence_Complete = true;                                 //(-1 is for offset of length vs array access)
                VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_endState;              // send to the endState
            }else{                                                           //If not completed,
                iteration_Counter++;                                            //Increment the counter
                VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_enableButtonHandler;   //Wait for next touch
            }
        }else if(regionNo != globals_getSequenceValue(iteration_Counter)){   //User pressed incorrect square
            verifySequence_UserInput_Error = true;                              //User has lost that round
            verifySequence_Complete = true;                                     //Send to the endState
            VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_endState;
        }
        else{                                                                //User has not done anything yet
            VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_verify;                    //Stay in verify state
        }
        break;
    case VERIFYSEQUENCE_endState:
        if(verifySequenceEnable == false){                                  //When verifySequence is disabled
            VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_init;                  // go to the init state
        }
        else
            VERIFYSEQUENCE_CurState = VERIFYSEQUENCE_endState;              //Wait here until disabled
        break;
    }
}

/************************************************************************
 * Everything below this message is associated with the Run Test function
 ************************************************************************/
#define MESSAGE_X 0
#define MESSAGE_Y (display_width()/4)
#define MESSAGE_TEXT_SIZE 2
#define MESSAGE_STARTING_OVER
// Prints the instructions that the user should follow when
// testing the verifySequence state machine.
// Takes an argument that specifies the length of the sequence so that
// the instructions are tailored for the length of the sequence.
// This assumes a simple incrementing pattern so that it is simple to
// instruct the user.
void verifySequence_printInstructions(uint8_t length, bool startingOver) {
    display_fillScreen(DISPLAY_BLACK);        // Clear the screen.
    display_setTextSize(MESSAGE_TEXT_SIZE);   // Make it readable.
    display_setCursor(MESSAGE_X, MESSAGE_Y);  // Rough center.
    if (startingOver) {                       // Print a message if you start over.
        display_fillScreen(DISPLAY_BLACK);      // Clear the screen if starting over.
        display_setTextColor(DISPLAY_WHITE);    // Print whit text.
        display_println("Starting Over. ");
    }
    display_println("Tap: ");
    display_println();
    switch (length) {
    case 1:
        display_println("red");
        break;
    case 2:
        display_println("red, yellow ");
        break;
    case 3:
        display_println("red, yellow, blue ");
        break;
    case 4:
        display_println("red, yellow, blue, green ");
        break;
    default:
        break;
    }
    display_println("in that order.");
    display_println();
    display_println("hold BTN0 to quit.");
}

// Just clears the screen and draws the four buttons used in Simon.
void verifySequence_drawButtons() {
    display_fillScreen(DISPLAY_BLACK);
    simonDisplay_drawAllButtons();  // Draw the four buttons.
}

// This will set the sequence to a simple sequential pattern.
#define MAX_TEST_SEQUENCE_LENGTH 4  // the maximum length of the pattern
uint8_t verifySequence_testSequence[MAX_TEST_SEQUENCE_LENGTH] = {0, 1, 2, 3};  // A simple pattern.
#define MESSAGE_WAIT_MS 4000  // Display messages for this long.

// Increment the sequence length making sure to skip over 0.
// Used to change the sequence length during the test.
int16_t incrementSequenceLength(int16_t sequenceLength) {
    int16_t value = (sequenceLength + 1) % (MAX_TEST_SEQUENCE_LENGTH+1);
    if (value == 0) value++;
    return value;
}

// Used to select from a variety of informational messages.
enum verifySequence_infoMessage_t {
    user_time_out_e,            // means that the user waited too long to tap a color.
    user_wrong_sequence_e,      // means that the user tapped the wrong color.
    user_correct_sequence_e,    // means that the user tapped the correct sequence.
    user_quit_e                 // means that the user wants to quit.
};

// Prints out informational messages based upon a message type (see above).
void verifySequence_printInfoMessage(verifySequence_infoMessage_t messageType) {
    // Setup text color, position and clear the screen.
    display_setTextColor(DISPLAY_WHITE);
    display_setCursor(MESSAGE_X, MESSAGE_Y);
    display_fillScreen(DISPLAY_BLACK);
    switch(messageType) {
    case user_time_out_e:  // Tell the user that they typed too slowly.
        display_println("Error:");
        display_println();
        display_println("  User tapped sequence");
        display_println("  too slowly.");
        break;
    case user_wrong_sequence_e:  // Tell the user that they tapped the wrong color.
        display_println("Error: ");
        display_println();
        display_println("  User tapped the");
        display_println("  wrong sequence.");
        break;
    case user_correct_sequence_e:  // Tell the user that they were correct.
        display_println("User tapped");
        display_println("the correct sequence.");
        break;
    case user_quit_e:             // Acknowledge that you are quitting the test.
        display_println("quitting runTest().");
        break;
    default:
        break;
    }
}

#define BTN0 1
// Tests the verifySequence state machine.
// It prints instructions to the touch-screen. The user responds by tapping the
// correct colors to match the sequence.
// Users can test the error conditions by waiting too long to tap a color or
// by tapping an incorrect color.
void verifySequence_runTest() {
    display_init();  // Always must do this.
    buttons_init();  // Need to use the push-button package so user can quit.
    int16_t sequenceLength = 1;  // Start out with a sequence length of 1.
    verifySequence_printInstructions(sequenceLength, false);  // Tell the user what to do.
    utils_msDelay(MESSAGE_WAIT_MS);  // Give them a few seconds to read the instructions.
    verifySequence_drawButtons();    // Now, draw the buttons.
    // Set the test sequence and it's length.
    globals_setSequence(verifySequence_testSequence, MAX_TEST_SEQUENCE_LENGTH);
    globals_setSequenceIterationLength(sequenceLength);
    // Enable the verifySequence state machine.
    verifySequence_enable();           // Everything is interlocked, so first enable the machine.
    while (!(buttons_read() & BTN0)) { // Need to hold button until it quits as you might be stuck in a delay.
        // verifySequence uses the buttonHandler state machine so you need to "tick" both of them.
        verifySequence_tick();  // Advance the verifySequence state machine.
        buttonHandler_tick();   // Advance the buttonHandler state machine.
        utils_msDelay(1);       // Wait 1 ms.
        // If the verifySequence state machine has finished, check the result, otherwise just keep ticking both machines.
        if (verifySequence_isComplete()) {
            if (verifySequence_isTimeOutError()) {                      // Was the user too slow?
                verifySequence_printInfoMessage(user_time_out_e);         // Yes, tell the user that they were too slow.
                printf("\t\tUSER TIME OUT ERROR\n\r");
            } else if (verifySequence_isUserInputError()) {             // Did the user tap the wrong color?
                verifySequence_printInfoMessage(user_wrong_sequence_e);   // Yes, tell them so.
                printf("\t\tUSER TOUCHED THE WRONG SQUARE\n\r");
            } else {
                verifySequence_printInfoMessage(user_correct_sequence_e); // User was correct if you get here.
                printf("\t\tUSER TOUCHED CORRECT SEQUENCE\n\r");
            }
            utils_msDelay(MESSAGE_WAIT_MS);                            // Allow the user to read the message.
            sequenceLength = incrementSequenceLength(sequenceLength);  // Increment the sequence.
            globals_setSequenceIterationLength(sequenceLength);  // Set the length for the verifySequence state machine.
            verifySequence_printInstructions(sequenceLength, true);    // Print the instructions.
            utils_msDelay(MESSAGE_WAIT_MS);                            // Let the user read the instructions.
            verifySequence_drawButtons();                              // Draw the buttons.
            verifySequence_disable();                                  // Interlock: first step of handshake.
            verifySequence_tick();                                     // Advance the verifySequence machine.
            utils_msDelay(1);                                          // Wait for 1 ms.
            verifySequence_enable();                                   // Interlock: second step of handshake.
            utils_msDelay(1);                                          // Wait 1 ms.
        }
    }
    verifySequence_printInfoMessage(user_quit_e);  // Quitting, print out an informational message.
}




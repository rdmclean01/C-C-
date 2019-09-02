/*
 * buttonHandler.c
 *
 *  Created on: Oct 30, 2015
 *      Author: rdmclean
 */

#include "buttonHandler.h"

int16_t x_coord,y_coord;
uint8_t z_coord;
bool buttonHandlerEnable, releaseDetected;
static uint16_t buttonHandler_adTimer;
#define EXPIRED_VALUE_AD 1 // Period of 100ms, Analog Digital Converter requires 50ms
uint8_t buttonHandler_regionNo;

enum BUTTONHANDLE_CurStates{
    BUTTONHANDLE_init, BUTTONHANDLE_wait_touch,
    BUTTONHANDLE_wait_settle, BUTTONHANDLE_touch,
    BUTTONHANDLE_final
} BUTTONHANDLE_CurState = BUTTONHANDLE_init;

// Get the simon region numbers. See the source code for the region numbering scheme.
uint8_t buttonHandler_getRegionNumber(){
    display_clearOldTouchData();                              //Clear the old data before new comes in
    display_getTouchedPoint(&x_coord, &y_coord, &z_coord);    //Get the point that was touched
    return simonDisplay_computeRegionNumber(x_coord, y_coord);//Return the Region number based on touch
}

void buttonHandler_enable(){buttonHandlerEnable = true;}//Turn on the state machine. Part of the interlock.
void buttonHandler_disable(){buttonHandlerEnable = false;}//Turn off the state machine. Part of the interlock.
// Other state machines can call this function to see if the user has stopped touching the pad.
bool buttonHandler_releaseDetected(){return releaseDetected;}

// Standard tick function.
void buttonHandler_tick(){
    switch(BUTTONHANDLE_CurState){              //State Actions
    case BUTTONHANDLE_init:                             //Initialize the state machine
        //printf("\t\tBUTTON HANDLER STATE: init\n");
        display_clearOldTouchData();                    //Make sure data is cleared out
        buttonHandler_adTimer = 0;                      //Analog to digital Converter timer
        releaseDetected = false;                        //Release has not happened yet
        break;
    case BUTTONHANDLE_wait_touch:                          //Wait for user to touch the screen
        //printf("\t\tBUTTON HANDLER STATE: wait touch\n");//No action required
        break;
    case BUTTONHANDLE_wait_settle:                          //Wait for the analog/digital converter to settle
        //printf("\t\tBUTTON HANDLER STATE: wait settle\n");//Needs to count up until expired value
        buttonHandler_adTimer++;                            // Timer to stay in state at least 50ms
        break;
    case BUTTONHANDLE_touch:                                      //Compute the touched region
        //printf("\t\tBUTTON HANDLER STATE: touch\n");
        if(!display_isTouched()){                                 //When display is no longer touched
            releaseDetected = true;                               //Release has been detected
            simonDisplay_drawSquare(buttonHandler_regionNo, true);//Erase the square at that region
            simonDisplay_drawButton(buttonHandler_regionNo);      //Redraw the button to continue playing
        }
        break;
    case BUTTONHANDLE_final:                                      //A final state
        //printf("\t\tBUTTON HANDLER STATE: final\n");            //No state actions required
        break;
    }
    /*******************************************************************/
    switch(BUTTONHANDLE_CurState){                      //TRANSITIONS and Mealy Actions
    case BUTTONHANDLE_init:
        if(buttonHandlerEnable){                                //Move into machine once enabled
            BUTTONHANDLE_CurState = BUTTONHANDLE_wait_touch;
            simonDisplay_drawAllButtons();                      // Draw the four buttons.
        }
        else
            BUTTONHANDLE_CurState = BUTTONHANDLE_init;          //Wait here until enabled
        break;
    case BUTTONHANDLE_wait_touch:
        if(display_isTouched()){                                //If display has been touched
            BUTTONHANDLE_CurState = BUTTONHANDLE_wait_settle;   // wait for AD converter
        }
        else
            BUTTONHANDLE_CurState = BUTTONHANDLE_wait_touch;    //Stay here until touched
        break;
    case BUTTONHANDLE_wait_settle:
        if(buttonHandler_adTimer >= EXPIRED_VALUE_AD){               //When 50ms has expired,
            BUTTONHANDLE_CurState = BUTTONHANDLE_touch;              // move on to next state
            buttonHandler_regionNo = buttonHandler_getRegionNumber();//Get the region number
            simonDisplay_drawSquare(buttonHandler_regionNo, false);  //Draw a square at that region
        }
        else
            BUTTONHANDLE_CurState = BUTTONHANDLE_wait_settle;   //Wait for expired value
        break;
    case BUTTONHANDLE_touch:
        if(buttonHandler_releaseDetected())                     //If release was detected
            BUTTONHANDLE_CurState = BUTTONHANDLE_final;         // ready to move on to final
        else
            BUTTONHANDLE_CurState = BUTTONHANDLE_touch;         //Wait for release and state actions
        break;
    case BUTTONHANDLE_final:
        if(buttonHandlerEnable == false){                       //Once disabled, it is safe to move
            BUTTONHANDLE_CurState = BUTTONHANDLE_init;          // on to init restart state
        }
        else
            BUTTONHANDLE_CurState = BUTTONHANDLE_final;         //Wait to be disabled
        break;
    }
}
/*******************************************************
 * Everything below corresponds to the runtest function
 *******************************************************/
#define RUN_TEST_TERMINATION_MESSAGE1 "buttonHandler_runTest()"
#define RUN_TEST_TERMINATION_MESSAGE2 "terminated."
#define RUN_TEST_TEXT_SIZE 2
// buttonHandler_runTest(int16_t touchCount) runs the test until
// the user has touched the screen touchCount times. It indicates
// that a button was pushed by drawing a large square while
// the button is pressed and then erasing the large square and
// redrawing the button when the user releases their touch.

void buttonHandler_runTest(int16_t touchCountArg) {
    int16_t touchCount = 0;             // Keep track of the number of touches.
    display_init();                     // Always have to init the display.
    display_fillScreen(DISPLAY_BLACK);  // Clear the display.
    simonDisplay_drawAllButtons();      // Draw the four buttons.
    buttonHandler_enable();
    while (touchCount < touchCountArg) {  // Loop here while touchCount is less than the touchCountArg
        buttonHandler_tick();               // Advance the state machine.
        utils_msDelay(1);           // Wait here for 1 ms.
        if (buttonHandler_releaseDetected()) {  // If a release is detected, then the screen was touched.
            touchCount++;                         // Keep track of the number of touches.
            printf("button released: %d\n\r", buttonHandler_getRegionNumber());  // Get the region number that was touched.
            buttonHandler_disable();             // Interlocked behavior: handshake with the button handler (now disabled).
            utils_msDelay(1);                     // wait 1 ms.
            buttonHandler_tick();                 // Advance the state machine.
            buttonHandler_enable();               // Interlocked behavior: enable the buttonHandler.
            utils_msDelay(1);                     // wait 1 ms.
            buttonHandler_tick();                 // Advance the state machine.
        }
    }
    display_fillScreen(DISPLAY_BLACK);            // clear the screen.
    display_setTextSize(RUN_TEST_TEXT_SIZE);      // Set the text size.
    display_setCursor(0, display_height()/2);     // Move the cursor to a rough center point.
    display_println(RUN_TEST_TERMINATION_MESSAGE1);   // Print the termination message on two lines.
    display_println(RUN_TEST_TERMINATION_MESSAGE2);
}



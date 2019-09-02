/*
 * simonDisplay.c
 *
 *  Created on: Oct 30, 2015
 *      Author: rdmclean
 */

#include "simonDisplay.h"

#define SIMONDISPLAY_HALF_WIDTH (DISPLAY_WIDTH / 2)
#define SIMONDISPLAY_HALF_HEIGHT (DISPLAY_HEIGHT / 2)
#define SIMONDISPLAY_ERROR_VALUE -1
#define LEFTBUTTON_X_COORD ((DISPLAY_WIDTH/4)-30)
#define RIGHTBUTTON_X_COORD ((3*DISPLAY_WIDTH)/4 - 30)
#define TOPBUTTON_Y_COORD ((DISPLAY_HEIGHT/4)-30)
#define BOTBUTTON_Y_COORD ((3*DISPLAY_HEIGHT/4)-30)
#define LEFTSQUARE_X_COORD 0
#define RIGHTSQUARE_X_COORD (DISPLAY_WIDTH / 2)
#define TOPSQUARE_Y_COORD 0
#define BOTSQUARE_Y_COORD (DISPLAY_HEIGHT / 2)
/*
|----------|----------|
|          |          |
|    0     |     1    |
|  (RED)   | (YELLOW) |
-----------------------
|          |          |
|     2    |    3     |
|  (BLUE)  |  (GREEN) |
-----------------------
 */
/* Computes the region, to include all pixels, convention used is
 * less than or greater than and equal to.
 */
int8_t simonDisplay_computeRegionNumber(int16_t x, int16_t y){
    if(x < SIMONDISPLAY_HALF_WIDTH && y < SIMONDISPLAY_HALF_HEIGHT )      //Puts in upper right corner
        return SIMON_DISPLAY_REGION_0;
    else if(x >= SIMONDISPLAY_HALF_WIDTH && y < SIMONDISPLAY_HALF_HEIGHT) //Puts in upper left corner
        return SIMON_DISPLAY_REGION_1;
    else if(x < SIMONDISPLAY_HALF_WIDTH && y >= SIMONDISPLAY_HALF_HEIGHT) //Puts in lower right corner
        return SIMON_DISPLAY_REGION_2;
    else if(x >= SIMONDISPLAY_HALF_WIDTH && y >= SIMONDISPLAY_HALF_HEIGHT) //Puts in lower left corner
        return SIMON_DISPLAY_REGION_3;
    else                                                                   //An error case
        return SIMONDISPLAY_ERROR_VALUE;
}

// Draws a colored "button" that the user can touch.
// The colored button is centered in the region but does not fill the region.
void simonDisplay_drawButton(uint8_t regionNumber){
    if(regionNumber == SIMON_DISPLAY_REGION_0)      //Puts a RED filled in square in Region 0
        display_fillRect(LEFTBUTTON_X_COORD, TOPBUTTON_Y_COORD, SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_RED);
    else if(regionNumber == SIMON_DISPLAY_REGION_1) //Puts a YELLOW filled in square in Region 1
        display_fillRect(RIGHTBUTTON_X_COORD, TOPBUTTON_Y_COORD, SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_YELLOW);
    else if(regionNumber == SIMON_DISPLAY_REGION_2) //Puts a BLUE filled in square in Region 2
        display_fillRect(LEFTBUTTON_X_COORD, BOTBUTTON_Y_COORD, SIMON_DISPLAY_BUTTON_WIDTH,SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_BLUE);
    else if(regionNumber == SIMON_DISPLAY_REGION_3) //Puts a GREEN filled in square in Region 3
        display_fillRect(RIGHTBUTTON_X_COORD, BOTBUTTON_Y_COORD, SIMON_DISPLAY_BUTTON_WIDTH,SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_GREEN);
    else
        printf("ERROR, INVALID REGION NUMBER\n");   //Region number is outside range
}

// Convenience function that draws all of the buttons.
void simonDisplay_drawAllButtons(){
    simonDisplay_drawButton(SIMON_DISPLAY_REGION_0);//Draws button at
    simonDisplay_drawButton(SIMON_DISPLAY_REGION_1);// the specific region
    simonDisplay_drawButton(SIMON_DISPLAY_REGION_2);//Colors differ for
    simonDisplay_drawButton(SIMON_DISPLAY_REGION_3);// clarity
}

// Draws a bigger square that completely fills the region.
// If the erase argument is true, it draws the square as black background to "erase" it.
void simonDisplay_drawSquare(uint8_t regionNo, bool erase){
    if(erase){
        if(regionNo == SIMON_DISPLAY_REGION_0)      //Erases the square upper Left
            display_fillRect(LEFTSQUARE_X_COORD, TOPSQUARE_Y_COORD, SIMONDISPLAY_HALF_WIDTH, SIMONDISPLAY_HALF_HEIGHT, DISPLAY_BLACK);
        else if(regionNo == SIMON_DISPLAY_REGION_1) //Erases square upper right
            display_fillRect(RIGHTSQUARE_X_COORD, TOPSQUARE_Y_COORD, SIMONDISPLAY_HALF_WIDTH, SIMONDISPLAY_HALF_HEIGHT, DISPLAY_BLACK);
        else if(regionNo == SIMON_DISPLAY_REGION_2) //Erases square bottom left
            display_fillRect(LEFTSQUARE_X_COORD, BOTSQUARE_Y_COORD, SIMONDISPLAY_HALF_WIDTH, SIMONDISPLAY_HALF_HEIGHT, DISPLAY_BLACK);
        else if(regionNo == SIMON_DISPLAY_REGION_3) //Erases square bottom right
            display_fillRect(RIGHTSQUARE_X_COORD, BOTSQUARE_Y_COORD, SIMONDISPLAY_HALF_WIDTH, SIMONDISPLAY_HALF_HEIGHT, DISPLAY_BLACK);
    }else{
        if(regionNo == SIMON_DISPLAY_REGION_0)      //Draws Red square upper left
            display_fillRect(LEFTSQUARE_X_COORD, TOPSQUARE_Y_COORD, SIMONDISPLAY_HALF_WIDTH, SIMONDISPLAY_HALF_HEIGHT, DISPLAY_RED);
        else if(regionNo == SIMON_DISPLAY_REGION_1) //Draws Yellow square upper right
            display_fillRect(RIGHTSQUARE_X_COORD, TOPSQUARE_Y_COORD, SIMONDISPLAY_HALF_WIDTH, SIMONDISPLAY_HALF_HEIGHT, DISPLAY_YELLOW);
        else if(regionNo == SIMON_DISPLAY_REGION_2) //Draws Blue square bottom left
            display_fillRect(LEFTSQUARE_X_COORD, BOTSQUARE_Y_COORD, SIMONDISPLAY_HALF_WIDTH, SIMONDISPLAY_HALF_HEIGHT, DISPLAY_BLUE);
        else if(regionNo == SIMON_DISPLAY_REGION_3) //Draws Green square bottom right
            display_fillRect(RIGHTSQUARE_X_COORD, BOTSQUARE_Y_COORD, SIMONDISPLAY_HALF_WIDTH, SIMONDISPLAY_HALF_HEIGHT, DISPLAY_GREEN);
    }
}

/**********************************************************
 * Everything below is associated with the runTest function
 **********************************************************/
// Runs a brief demonstration of how buttons can be pressed and squares lit up to implement the user
// interface of the Simon game. The routine will continue to run until the touchCount has been reached, e.g.,
// the user has touched the pad touchCount times.
#define TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS 60 // in ms
#define MAX_STR 255
#define TEXT_SIZE 2
// Runs a brief demonstration of how buttons can be pressed and squares lit up to implement the user
// interface of the Simon game. The routine will continue to run until the touchCount has been reached, e.g.,
// the user has touched the pad touchCount times.

// I used a busy-wait delay (utils_msDelay) that uses a for-loop and just blocks until the time has passed.
// When you implement the game, you CANNOT use this function as we discussed in class. Implement the delay
// using the non-blocking state-machine approach discussed in class.
void simonDisplay_runTest(uint16_t touchCount) {
    display_init();  // Always initialize the display.
    char str[MAX_STR];   // Enough for some simple printing.
    uint8_t regionNumber;
    uint16_t touches = 0;
    // Write an informational message and wait for the user to touch the LCD.
    display_fillScreen(DISPLAY_BLACK);        // clear the screen.
    display_setCursor(0, display_height()/2); //
    display_setTextSize(TEXT_SIZE);
    display_setTextColor(DISPLAY_RED, DISPLAY_BLACK);
    sprintf(str, "Touch and release to start the Simon demo.");
    display_println(str);
    display_println();
    sprintf(str, "Demo will terminate after %d touches.", touchCount);
    display_println(str);
    while (!display_isTouched());       // Wait here until the screen is touched.
    while (display_isTouched());        // Now wait until the touch is released.
    display_fillScreen(DISPLAY_BLACK);  // Clear the screen.
    simonDisplay_drawAllButtons();      // Draw all of the buttons.
    bool touched = false;           // Keep track of when the pad is touched.
    int16_t x, y;               // Use these to keep track of coordinates.
    uint8_t z;                  // This is the relative touch pressure.
    while (touches < touchCount) {  // Run the loop according to the number of touches passed in.
        if (!display_isTouched() && touched) {         // user has stopped touching the pad.
            simonDisplay_drawSquare(regionNumber, true); // Erase the square.
            simonDisplay_drawButton(regionNumber);       // DISPLAY_REDraw the button.
            touched = false;                                                          // Released the touch, set touched to false.
        } else if (display_isTouched() && !touched) {   // User started touching the pad.
            touched = true;                               // Just touched the pad, set touched = true.
            touches++;                                                                    // Keep track of the number of touches.
            display_clearOldTouchData();  // Get rid of data from previous touches.
            // Must wait this many milliseconds for the chip to do analog processing.
            utils_msDelay(TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS);
            display_getTouchedPoint(&x, &y, &z);                  // After the wait, get the touched point.
            regionNumber = simonDisplay_computeRegionNumber(x, y);// Compute the region number.
            simonDisplay_drawSquare(regionNumber, false);     // Draw the square (erase = false).
        }
    }
    // Done with the demo, write an informational message to the user.
    display_fillScreen(DISPLAY_BLACK);        // clear the screen.
    display_setCursor(0, display_height()/2); // Place the cursor in the middle of the screen.
    display_setTextSize(2);                   // Make it readable.
    display_setTextColor(DISPLAY_RED, DISPLAY_BLACK);  // red is foreground color, black is background color.
    sprintf(str, "Simon demo terminated");    // Format a string using sprintf.
    display_println(str);                     // Print it to the LCD.
    sprintf(str, "after %d touches.", touchCount);  // Format the rest of the string.
    display_println(str);  // Print it to the LCD.
}

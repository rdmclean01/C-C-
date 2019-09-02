/*
 * clockDisplay.c
 *
 *  Created on: Sep 25, 2015
 *      Author: rdmclean
 */
#include "clockDisplay.h"
#include <stdio.h>
#include "supportFiles/utils.h"
#include "SupportFiles/display.h"
#include "xil_io.h"
#include "stdint.h"
#include "xparameters.h"


#define CLOCK_TEXT_SIZE 5 // CAN BE ANYTHING 3-6
#define CLOCK_SIZE (10*CLOCK_TEXT_SIZE)   // Scales the text size to appear on the screen
#define CLOCK_XCENTER (DISPLAY_WIDTH / 2) // Denotes the X center
#define CLOCK_YCENTER (DISPLAY_HEIGHT / 2)// Denotes the Y Center
#define CLOCKDISPLAY_L_X0 (CLOCK_XCENTER - ((5*CLOCK_SIZE)/2))//X coordinates for Left triangles
#define CLOCKDISPLAY_L_X1 (CLOCK_XCENTER - ((4*CLOCK_SIZE)/2))
#define CLOCKDISPLAY_L_X2 (CLOCK_XCENTER - ((3*CLOCK_SIZE)/2))
#define CLOCKDISPLAY_L_COLON  (CLOCK_XCENTER - ((5*CLOCK_SIZE)/4))
#define CLOCKDISPLAY_M_X0 (CLOCK_XCENTER - ((1*CLOCK_SIZE)/2))//X coordinates for Middle Triangles
#define CLOCKDISPLAY_M_X1 (CLOCK_XCENTER)
#define CLOCKDISPLAY_M_X2 (CLOCK_XCENTER + ((1*CLOCK_SIZE)/2))
#define CLOCKDISPLAY_R_COLON (CLOCK_XCENTER + ((3*CLOCK_SIZE)/4))
#define CLOCKDISPLAY_R_X0 (CLOCK_XCENTER + ((3*CLOCK_SIZE)/2))//X coordinates for Right Triangles
#define CLOCKDISPLAY_R_X1 (CLOCK_XCENTER + ((4*CLOCK_SIZE)/2))
#define CLOCKDISPLAY_R_X2 (CLOCK_XCENTER + ((5*CLOCK_SIZE)/2))
#define CLOCKDISPLAY_T_Y0 (CLOCK_YCENTER - CLOCK_SIZE)        //Y coordinates for all Upper Triangles
#define CLOCKDISPLAY_T_Y1 (CLOCK_YCENTER - (2*CLOCK_SIZE))
#define CLOCKDISPLAY_T_Y2 (CLOCK_YCENTER - CLOCK_SIZE)
#define CLOCKDISPLAY_B_Y0 (CLOCK_YCENTER + CLOCK_SIZE)        //Y coordinates for all Lower Triangles
#define CLOCKDISPLAY_B_Y1 (CLOCK_YCENTER + (2*CLOCK_SIZE))
#define CLOCKDISPLAY_B_Y2 (CLOCK_YCENTER + CLOCK_SIZE)
#define CLOCKDISPLAY_YCURSOR (CLOCK_YCENTER - (CLOCK_SIZE/2))
#define CLOCKDISPLAY_RUNTEST_LENGTH 10000                 // 10000ms is 10 sec
#define CLOCKDISPLAY_TIME_SIZE 9
#define HRS_MAX 12 //Typical min and max values for a clock
#define HRS_MIN 1
#define MIN_MAX 59
#define MIN_MIN 0
#define SEC_MAX 59
#define SEC_MIN 0
#define CLKDISPLAY_LFT_REGION (DISPLAY_WIDTH / 3) // To be in Left region, must be less than this value
#define CLKDISPLAY_MID_REGION ((2*DISPLAY_WIDTH)/3)// To be in Middle region, must be less than this value
#define CLKDISPLAY_RGT_REGION (DISPLAY_WIDTH)// To be in Right region, must be less than this value
#define RGN_INC_HRS 0//|-----------------------|
#define RGN_INC_MIN 1//|   0   |   1   |   2   |
#define RGN_INC_SEC 2//|-----------------------|
#define RGN_DEC_HRS 3//|   3   |   4   |   5   |
#define RGN_DEC_MIN 4//|-----------------------|
#define RGN_DEC_SEC 5 // This will be the configuration of the regions
#define RGN_ERROR -1
static int16_t Hrs_Cur; // Sets global variables to hold the current
static int16_t Min_Cur; // and new values needed for the
static int16_t Sec_Cur; // hours, minutes and
static int16_t Hrs_New; // seconds
static int16_t Min_New;
static int16_t Sec_New;
static char Current_Time[CLOCKDISPLAY_TIME_SIZE];//Arrays to hold the current
static char New_Time[CLOCKDISPLAY_TIME_SIZE];// and new values in an easy string


/* Called only once - performs any necessary inits.*/
void clockDisplay_init(){
    display_init();                      // Initializes the display
    display_setTextSize(CLOCK_TEXT_SIZE);// Sets text size to anywhere, 3-6, defined above
    display_fillScreen(DISPLAY_BLACK);   // Makes the display initially black
    // Makes the upper left triangle
    display_fillTriangle(CLOCKDISPLAY_L_X0, CLOCKDISPLAY_T_Y0, CLOCKDISPLAY_L_X1, CLOCKDISPLAY_T_Y1, CLOCKDISPLAY_L_X2, CLOCKDISPLAY_T_Y2, DISPLAY_GREEN);
    // Makes the upper middle triangle
    display_fillTriangle(CLOCKDISPLAY_M_X0, CLOCKDISPLAY_T_Y0, CLOCKDISPLAY_M_X1, CLOCKDISPLAY_T_Y1, CLOCKDISPLAY_M_X2, CLOCKDISPLAY_T_Y2, DISPLAY_GREEN);
    // Makes the upper right triangle
    display_fillTriangle(CLOCKDISPLAY_R_X0, CLOCKDISPLAY_T_Y0, CLOCKDISPLAY_R_X1, CLOCKDISPLAY_T_Y1, CLOCKDISPLAY_R_X2, CLOCKDISPLAY_T_Y2, DISPLAY_GREEN);
    // Makes the bottom left triangle
    display_fillTriangle(CLOCKDISPLAY_L_X0, CLOCKDISPLAY_B_Y0, CLOCKDISPLAY_L_X1, CLOCKDISPLAY_B_Y1, CLOCKDISPLAY_L_X2, CLOCKDISPLAY_B_Y2,DISPLAY_GREEN);
    // Makes the bottom middle triangle
    display_fillTriangle(CLOCKDISPLAY_M_X0, CLOCKDISPLAY_B_Y0, CLOCKDISPLAY_M_X1, CLOCKDISPLAY_B_Y1, CLOCKDISPLAY_M_X2, CLOCKDISPLAY_B_Y2, DISPLAY_GREEN);
    // Makes the bottom right triangle
    display_fillTriangle(CLOCKDISPLAY_R_X0, CLOCKDISPLAY_B_Y0, CLOCKDISPLAY_R_X1, CLOCKDISPLAY_B_Y1, CLOCKDISPLAY_R_X2, CLOCKDISPLAY_B_Y2, DISPLAY_GREEN);
    //NOTE that triangles are only drawn in init() for efficiency of code
    Hrs_Cur = HRS_MAX; // Sets to Current values to their maximum
    Min_Cur = MIN_MAX; // default of personal choice
    Sec_Cur = SEC_MAX;
    Hrs_New = HRS_MAX; // Sets to New values to their maximum
    Min_New = MIN_MAX; // default of personal choice
    Sec_New = SEC_MAX;
    sprintf(Current_Time, "%2hd:%02hd:%02hd",Hrs_Cur,Min_Cur,Sec_Cur); //Initializes the current array
    sprintf(New_Time,     "%2hd:%02hd:%02hd",Hrs_New,Min_New,Sec_New); //Initializes the new array
    int current; // Is an incremental place holder to walk through array
    for(current = 0; current < CLOCKDISPLAY_TIME_SIZE; current++ )
    {
        switch(current){
        case 0: // Prints off the Tens place of the Hours
            display_drawChar(CLOCKDISPLAY_L_X0, CLOCKDISPLAY_YCURSOR , Current_Time[current], DISPLAY_GREEN, DISPLAY_BLACK, CLOCK_TEXT_SIZE);
            break;
        case 1: // Prints off the Ones place of the Hours
            display_drawChar(CLOCKDISPLAY_L_X0 + (CLOCK_SIZE/2), CLOCKDISPLAY_YCURSOR , Current_Time[current], DISPLAY_GREEN, DISPLAY_BLACK, CLOCK_TEXT_SIZE);
            break;
        case 2: // Prints off the colon between Hours and Minutes
            display_drawChar(CLOCKDISPLAY_L_COLON, CLOCKDISPLAY_YCURSOR , Current_Time[current], DISPLAY_GREEN, DISPLAY_BLACK, CLOCK_TEXT_SIZE);
        case 3: // Prints off the Tens place of Minutes
            display_drawChar(CLOCKDISPLAY_M_X0, CLOCKDISPLAY_YCURSOR , Current_Time[current], DISPLAY_GREEN, DISPLAY_BLACK, CLOCK_TEXT_SIZE);
            break;
        case 4: // Prints off the Ones place of Minutes
            display_drawChar(CLOCKDISPLAY_M_X0 + (CLOCK_SIZE/2), CLOCKDISPLAY_YCURSOR , Current_Time[current], DISPLAY_GREEN, DISPLAY_BLACK, CLOCK_TEXT_SIZE);
            break;
        case 5: // Prints off the Colon
            display_drawChar(CLOCKDISPLAY_R_COLON, CLOCKDISPLAY_YCURSOR , Current_Time[current], DISPLAY_GREEN, DISPLAY_BLACK, CLOCK_TEXT_SIZE);
        case 6: // Prints off the Tens place of Seconds
            display_drawChar(CLOCKDISPLAY_R_X0, CLOCKDISPLAY_YCURSOR , Current_Time[current], DISPLAY_GREEN, DISPLAY_BLACK, CLOCK_TEXT_SIZE);
            break;
        case 7: // Prints off the Ones place of Seconds
            display_drawChar(CLOCKDISPLAY_R_X0 + (CLOCK_SIZE/2), CLOCKDISPLAY_YCURSOR , Current_Time[current], DISPLAY_GREEN, DISPLAY_BLACK, CLOCK_TEXT_SIZE);
            break;
        }
    }
}

/* Updates the time display with latest time.*/
void clockDisplay_updateTimeDisplay(bool forceUpdateAll)
{
    if (forceUpdateAll == false)
    {
        int current; //incremental place holder to walk through the array
        for(current = 0; current <CLOCKDISPLAY_TIME_SIZE; current++ )
        {
            if(Current_Time[current] != New_Time[current])
            {
                switch(current){
                case 0: // Prints off the Tens place of the Hours
                    display_drawChar(CLOCKDISPLAY_L_X0, CLOCKDISPLAY_YCURSOR , New_Time[current], DISPLAY_GREEN, DISPLAY_BLACK,CLOCK_TEXT_SIZE);
                    break;
                case 1: // Prints off the Ones place of the Hours
                    display_drawChar(CLOCKDISPLAY_L_X0 + (CLOCK_SIZE/2), CLOCKDISPLAY_YCURSOR , New_Time[current], DISPLAY_GREEN, DISPLAY_BLACK,CLOCK_TEXT_SIZE);
                    break;
                case 3:// Prints off the Tens place of the Minutes
                    display_drawChar(CLOCKDISPLAY_M_X0, CLOCKDISPLAY_YCURSOR , New_Time[current], DISPLAY_GREEN, DISPLAY_BLACK,CLOCK_TEXT_SIZE);
                    break;
                case 4: // Prints off the Ones place of the Minutes
                    display_drawChar(CLOCKDISPLAY_M_X0+ (CLOCK_SIZE/2), CLOCKDISPLAY_YCURSOR , New_Time[current], DISPLAY_GREEN, DISPLAY_BLACK,CLOCK_TEXT_SIZE);
                    break;
                case 6:// Prints off the Tens place of the Seconds
                    display_drawChar(CLOCKDISPLAY_R_X0, CLOCKDISPLAY_YCURSOR , New_Time[current], DISPLAY_GREEN, DISPLAY_BLACK,CLOCK_TEXT_SIZE);
                    break;
                case 7: // Prints off the Ones place of the Minutes
                    display_drawChar(CLOCKDISPLAY_R_X0+ (CLOCK_SIZE/2), CLOCKDISPLAY_YCURSOR , New_Time[current], DISPLAY_GREEN, DISPLAY_BLACK,CLOCK_TEXT_SIZE);
                    break;
                    //Note that colons are not redrawn for efficiency of code
                }

            }
        }
    }
    Hrs_Cur = Hrs_New; // Now updates the values before continuing on in the code
    Min_Cur = Min_New; // Value will only change if New is different than current
    Sec_Cur = Sec_New; // was before
    sprintf(Current_Time, "%2hd:%02hd:%02hd",Hrs_Cur,Min_Cur,Sec_Cur); //Update current string with new current values
}

/* Gets a Region on the screen based on touch*/
int getRegion()
{
    int region; // an integer reflecting the place on screen
    int16_t x_coord; //touched x coordinate
    int16_t y_coord; //touched y coordinate
    uint8_t z_coord; //touched z coordinate, reflects pressure

    display_getTouchedPoint(&x_coord, &y_coord, &z_coord); // a function to receive coordinates of touched point
    if(x_coord < CLKDISPLAY_LFT_REGION){ // If in the left third
        if(y_coord < CLOCK_YCENTER) // Left Third Upper Half
            region = RGN_INC_HRS; // Increment Hours region
        else
            region = RGN_DEC_HRS; // Left Third Bottom Half, Decrement Hours
    }
    else if(x_coord < CLKDISPLAY_MID_REGION) {//IF in middle third
        if(y_coord < CLOCK_YCENTER) // Middle upper region
            region = RGN_INC_MIN; // increment minutes
        else
            region = RGN_DEC_MIN; //middle lower region, decrement minutes
    }
    else if(x_coord < CLKDISPLAY_RGT_REGION){//IF in Right third
        if(y_coord < CLOCK_YCENTER)//Right upper region
            region = RGN_INC_SEC; // increment seconds
        else
            region = RGN_DEC_SEC; //right lower region, decrement seconds
    }
    else
        region = RGN_ERROR; // Somehow a region wasn't correctly found
    return region;
}

/* Performs the increment or decrement, depending upon the touched region.*/
void clockDisplay_performIncDec()
{
    int region;
    display_clearOldTouchData(); // Have to clear old data before getting new data
    region = getRegion();
    if(region == RGN_INC_HRS) { // Increment the hours when touched
        if(Hrs_Cur == HRS_MAX)
            Hrs_New = HRS_MIN; // If it needs to rollover, put it at minimum
        else
            Hrs_New = Hrs_Cur + 1; //Regular increment
        sprintf(New_Time, "%2hd:%02hd:%02hd",Hrs_New,Min_Cur,Sec_Cur); //Update new string array
        clockDisplay_updateTimeDisplay(false); // send to update the display
    }
    if(region == RGN_DEC_HRS) { // Decrement the hours when touched
        if(Hrs_Cur == HRS_MIN) // if rollover
            Hrs_New = HRS_MAX; // put at maximum
        else
            Hrs_New = Hrs_Cur - 1; // normal decrement
        sprintf(New_Time, "%2hd:%02hd:%02hd",Hrs_New,Min_Cur,Sec_Cur);//update new string array
        clockDisplay_updateTimeDisplay(false); // send to update display
    }
    if(region == RGN_INC_MIN) { // Increment the Minutes when touched
        if(Min_Cur == MIN_MAX) //Rollover case
            Min_New = MIN_MIN; //Make value minimum
        else
            Min_New = Min_Cur + 1;//normal increment
        sprintf(New_Time, "%2hd:%02hd:%02hd",Hrs_Cur,Min_New,Sec_Cur); //update new string array
        clockDisplay_updateTimeDisplay(false);// send to update display
    }
    if(region == RGN_DEC_MIN){ // Decrement the Minutes when touched
        if(Min_Cur == MIN_MIN)//Rollover case
            Min_New = MIN_MAX;//Make value maximum
        else
            Min_New = Min_Cur - 1;// normal decrement
        sprintf(New_Time, "%2hd:%02hd:%02hd",Hrs_Cur,Min_New,Sec_Cur);//update new string array
        clockDisplay_updateTimeDisplay(false);// send to update display
    }
    if(region == RGN_INC_SEC){ // Increment the Seconds when touched
        if(Sec_Cur == SEC_MAX)//Rollover case
            Sec_New = SEC_MIN;//make value minimum
        else
            Sec_New = Sec_Cur + 1;//normal increment
        sprintf(New_Time, "%2hd:%02hd:%02hd",Hrs_Cur,Min_Cur,Sec_New);//update new string array
        clockDisplay_updateTimeDisplay(false);// send to update display
    }
    if(region == RGN_DEC_SEC){ // Decrement the Seconds when touched
        if(Sec_Cur == SEC_MIN)//Rollover case
            Sec_New = SEC_MAX;//make value maximum
        else
            Sec_New = Sec_Cur - 1;// normal decrement
        sprintf(New_Time, "%2hd:%02hd:%02hd",Hrs_Cur,Min_Cur,Sec_New);//update new string array
        clockDisplay_updateTimeDisplay(false);// send to update display
    }
}

/* Advances the time forward by 1 second.*/
void clockDisplay_advanceTimeOneSecond()
{
    sprintf(Current_Time, "%2hd:%02hd:%02hd",Hrs_Cur,Min_Cur,Sec_Cur); // Make sure current array is correct
    if(Hrs_Cur == HRS_MAX && Min_Cur == MIN_MAX && Sec_Cur == SEC_MAX) // If at total rollover point
    {
        Hrs_New = HRS_MIN;// Rollover all of the values
        Min_New = MIN_MIN;// by setting to their minimums
        Sec_New = SEC_MIN;
    }
    else if(Min_Cur == MIN_MAX && Sec_Cur == SEC_MAX) // If minutes and seconds are at rollover point
    {
        Hrs_New = Hrs_Cur + 1;//Increment the hours
        Min_New = MIN_MIN;    // Rollover minutes and seconds
        Sec_New = SEC_MIN;    // by setting to their minimums
    }
    else if(Sec_Cur == SEC_MAX)// If only seconds are at rollover point
    {
        Min_New = Min_Cur +1;// increment minutes
        Sec_New = SEC_MIN;   //Rollover seconds
    }
    else // if nothing is at rollover point
    {
        Sec_New = Sec_Cur + 1; //simply increment the seconds

    }
    sprintf(New_Time, "%2hd:%02hd:%02hd",Hrs_New,Min_New,Sec_New);//put new values into new array
    clockDisplay_updateTimeDisplay(false);//Update the display for new values
}

/* Run a test of clock-display functions.
 * Runs through full functionality of the clock */
void clockDisplay_runTest(){
    clockDisplay_init(); // Initializes the clock display

    for(int i = 0; i < 100; i++){       //increment hours
        Hrs_New = Hrs_Cur + 1;
        clockDisplay_updateTimeDisplay(false);
    }
    for(int i = 0; i < 100; i++){       //decrement hours
        Hrs_New = Hrs_Cur - 1;
        clockDisplay_updateTimeDisplay(false);
    }
    for(int i = 0; i < 100; i++){       //increment minutes
        Min_New = Min_Cur + 1;
        clockDisplay_updateTimeDisplay(false);
    }
    for(int i = 0; i < 100; i++){       //decrement minutes
        Min_New = Min_Cur - 1;
        clockDisplay_updateTimeDisplay(false);
    }
    for(int i = 0; i < 100; i++){       //increment seconds
        Sec_New = Sec_Cur + 1;
        clockDisplay_updateTimeDisplay(false);
    }
    for(int i = 0; i < 100; i++){       //decrement seconds
        Sec_New = Sec_Cur - 1;
        clockDisplay_updateTimeDisplay(false);
    }

}









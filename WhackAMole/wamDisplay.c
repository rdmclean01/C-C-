/*
 * wamDisplay.c
 *
 *  Created on: Dec 7, 2015
 *      Author: rdmclean
 */

#include "supportFiles/display.h"
#include "wamDisplay.h"
#include "wamControl.h"
#include "wamControl.h"
#include "supportFiles/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WAMGAMETEXTSIZE 3
#define WAMGAMENAME ("Whack a Mole!")
#define WAMSTARTMESSAGE ("Touch Screen to Start")
#define WAMGAMEOVER ("Game Over")
#define WAMTEXTSIZE (WAMGAMETEXTSIZE-1)
#define CENTER_X (DISPLAY_WIDTH/4 - (6*WAMGAMETEXTSIZE))
#define CENTER_Y (DISPLAY_HEIGHT/4)
#define WAMBOARD_WIDTH DISPLAY_WIDTH
#define WAMBOARD_HEIGHT (DISPLAY_HEIGHT-20)
#define WAMBOARD_COUNT_Y (DISPLAY_HEIGHT-16)
#define WAMBOARD_HIT_X 0
#define WAMBOARD_HIT_X_VAL (WAMBOARD_HIT_X + 2*6*4)
#define WAMBOARD_MISS_X (DISPLAY_WIDTH/3)
#define WAMBOARD_MISS_X_VAL (WAMBOARD_MISS_X + 2*6*5)
#define WAMBOARD_LEVEL_X (2*DISPLAY_WIDTH/3)
#define WAMBOARD_LEVEL_X_VAL (WAMBOARD_LEVEL_X + 2*6*6)
#define LEFT_MOLE_X (DISPLAY_WIDTH/8)
#define CENTER_MOLE_X (DISPLAY_WIDTH/2)
#define RIGHT_MOLE_X (7*DISPLAY_WIDTH/8)
#define TOP_MOLE_Y (WAMBOARD_HEIGHT/8)
#define CENTER_MOLE_Y (WAMBOARD_HEIGHT/2)
#define LOW_MOLE_Y (7*WAMBOARD_HEIGHT/8)
#define MOLE_RADIUS 25
#define MOLE_SQUARE 30
#define SCORE_STR_LENGTH 50
#define MAX_NUM_MOLE_9 9
#define MAX_NUM_MOLE_6 6
#define MAX_NUM_MOLE_4 4
#define WAM_UPDATE_LEVEL 5
#define HIT_STR ("Hit:")
#define MISS_STR ("Miss:")
#define LEVEL_STR ("Level:")
#define SCORESCREEN_STR ("Hits:%d\nMisses:%d\nFinal Level:%d\n")
static uint16_t MyMoleCount = 0;
static uint16_t Hit_Count = 0;
static uint16_t Level_Count = 0;
static uint16_t Miss_Count = 0;
static uint16_t oldHit_Count = 0;

//| 0 | 4 | 1 |
//| 6 | 7 | 8 | Array needs ten locations to include null character
//| 2 | 5 | 3 | Position is given so you can print 9, 6 or 4 in proper positions
static wamDisplay_coord_t Board_X_Loc[MAX_NUM_MOLE_9]={LEFT_MOLE_X,RIGHT_MOLE_X,LEFT_MOLE_X,RIGHT_MOLE_X,CENTER_MOLE_X,CENTER_MOLE_X,LEFT_MOLE_X,  CENTER_MOLE_X,RIGHT_MOLE_X};
static wamDisplay_coord_t Board_Y_Loc[MAX_NUM_MOLE_9]={TOP_MOLE_Y, TOP_MOLE_Y,  LOW_MOLE_Y, LOW_MOLE_Y,  TOP_MOLE_Y,   LOW_MOLE_Y,   CENTER_MOLE_Y,CENTER_MOLE_Y,CENTER_MOLE_Y};

/********************** typedefs **********************/
// This keeps track of all mole information.
typedef struct {
    wamDisplay_point_t origin;  // This is the origin of the hole for this mole.
    // A mole is active if either of the tick counts are non-zero. The mole is dormant otherwise.
    // During operation, non-zero tick counts are decremented at a regular rate by the control state machine.
    // The mole remains in his hole until ticksUntilAwake decrements to zero and then he pops out.
    // The mole remains popped out of his hole until ticksUntilDormant decrements to zero.
    // Once ticksUntilDormant goes to zero, the mole hides in his hole and remains dormant until activated again.
    wamDisplay_moleTickCount_t ticksUntilAwake;  // Mole will wake up (pop out of hole) when this goes from 1 -> 0.
    wamDisplay_moleTickCount_t ticksUntilDormant; // Mole will go dormant (back in hole) this goes 1 -> 0.
} wamDisplay_moleInfo_t;

// This will contain pointers to all of the mole info records.
// This will ultimately be treated as an array of pointers.
static wamDisplay_moleInfo_t** wamDisplay_moleInfo;

// Allocates the memory for wamDisplay_moleInfo_t records.
// Computes the origin for each mole assuming a simple row-column layout:
// 9 moles: 3 rows, 3 columns, 6 moles: 2 rows, 3 columns, 4 moles: 2 rows, 2 columns
// Also inits the tick counts for awake and dormant.
void wamDisplay_computeMoleInfo(){
    // Setup all of the moles, creates and inits mole info records.
    // Create the container array. It contains pointers to each of the mole-hole info records.
    wamDisplay_moleInfo = (wamDisplay_moleInfo_t**) malloc(MyMoleCount * sizeof(wamDisplay_moleInfo_t*)); //Create a container
    for(uint16_t i=0;i<MyMoleCount;i++){
        wamDisplay_moleInfo[i]= (wamDisplay_moleInfo_t*) malloc(sizeof(wamDisplay_moleInfo_t)); //Point that container to a box
    }
    //Step 4: assign values to these structs
    for(uint16_t i=0;i<MyMoleCount;i++){
        wamDisplay_moleInfo[i]->origin.x = Board_X_Loc[i];  //Assign that box an x value
        wamDisplay_moleInfo[i]->origin.y = Board_Y_Loc[i];  //Assign that box a y value
        wamDisplay_moleInfo[i]->ticksUntilAwake = 0;        //Initialize so that the mole
        wamDisplay_moleInfo[i]->ticksUntilDormant=0;        // is not active
    }
}

// Provide support to set games with varying numbers of moles. This function
// would be called prior to calling wamDisplay_init();
void wamDisplay_selectMoleCount(wamDisplay_moleCount_e moleCount){
    switch(moleCount){                  //Assigns number of Moles based on enumeration
    case wamDisplay_moleCount_9:        // value passed in because of switch being read
        MyMoleCount = MAX_NUM_MOLE_9;   //This game handles either a 9 mole game
        break;
    case wamDisplay_moleCount_6:
        MyMoleCount = MAX_NUM_MOLE_6;   //A 6 mole game
        break;
    case wamDisplay_moleCount_4:
        MyMoleCount = MAX_NUM_MOLE_4;   //Or a 4 mole game
        break;
    }
}

// Call this before using any wamDisplay_ functions.
void wamDisplay_init(){
    display_setTextColor(DISPLAY_WHITE);//Print off text as white
    wamDisplay_computeMoleInfo();       //Allocates memory for the moles
}

// Draw the game display with a background and mole holes.
void wamDisplay_drawMoleBoard(){
    display_fillRect(0,0, WAMBOARD_WIDTH, WAMBOARD_HEIGHT, DISPLAY_GREEN);//Fill the screen with green for ground
    for(uint16_t i=0;i<MyMoleCount;i++)
        display_fillCircle(wamDisplay_moleInfo[i]->origin.x,wamDisplay_moleInfo[i]->origin.y, MOLE_RADIUS, DISPLAY_BLACK);//Draws all of the mole holes
    display_setTextSize(WAMGAMETEXTSIZE - 1);               //Sets the text size for the score screen along the bottom
    display_setCursor(WAMBOARD_HIT_X, WAMBOARD_COUNT_Y);    //Sets the cursor for the hit value
    display_println(HIT_STR);                               // Prints off the Hit sting
    wamDisplay_setHitScore(Hit_Count);                      // Prints off the Hit value
    display_setCursor(WAMBOARD_MISS_X, WAMBOARD_COUNT_Y);   //Sets the cursor for the miss value
    display_println(MISS_STR);                              // Prints off the miss string
    wamDisplay_setMissScore(Miss_Count);                    //Prints off the miss value
    display_setCursor(WAMBOARD_LEVEL_X, WAMBOARD_COUNT_Y);  //Sets the cursor for the level value
    display_println(LEVEL_STR);                             // Prints off the level string
    wamDisplay_incrementLevel();                            // Prints off the level value
}

// Draw the initial splash (instruction) screen.
void wamDisplay_drawSplashScreen(){
    display_fillScreen(DISPLAY_BLACK);      //Initially fill the screen with black
    display_setCursor(CENTER_X, CENTER_Y);  //Set the cursor for the message
    display_setTextSize(WAMGAMETEXTSIZE);   //A default text size, big enough to read and fill screen
    display_println(WAMGAMENAME);           //Outputs the name of the game
    display_setCursor(CENTER_X,CENTER_Y+MOLE_RADIUS);//Places the cursor down a little bit for more information
    display_setTextSize(WAMTEXTSIZE);       //Creates a subheading, needing a smaller next size
    display_println(WAMSTARTMESSAGE);       //Instructs on how to start game
}

// Draw the game-over screen.
void wamDisplay_drawGameOverScreen(){
    display_fillScreen(DISPLAY_BLACK);      //Initially fill screen with black to clear out screen
    display_setCursor(CENTER_X, CENTER_Y);  //Start the messages in the center of the screen
    display_setTextSize(WAMGAMETEXTSIZE);   //Create a good legible text size
    display_println(WAMGAMEOVER);           //Prints off a header for ending message
    wamDisplay_drawScoreScreen();           //Calls a function to print scores and such
    display_setTextSize(WAMTEXTSIZE);       //Set text size a little smaller for
    display_println(WAMSTARTMESSAGE);       // the instruction message
}

bool wamDisplay_isMoleActive(uint16_t position){
    if((wamDisplay_moleInfo[position]->ticksUntilAwake != 0)            //If he is preparing to jump out of his hole
            || (wamDisplay_moleInfo[position]->ticksUntilDormant != 0)){// or he is preparing to jump back in
        return true;                                                    // Mole is active
    }
    else
        return false;                                                   //Otherwise mole is not active
}

// Selects a random mole and activates it.
// Activating a mole means that the ticksUntilAwake and ticksUntilDormant counts are initialized.
// See the comments for wamDisplay_moleInfo_t for details.
// Returns true if a mole was successfully activated. False otherwise. You can
// use the return value for error checking as this function should always be successful
// unless you have a bug somewhere.
bool wamDisplay_activateRandomMole(){
    uint16_t numMole = MyMoleCount + 1;             //Just a value outside of range for error checking
    for(uint16_t i=0;i<MyMoleCount;i++){            //For all moles
        uint16_t newMole = rand() % MyMoleCount;    //Gives a random value of a mole on the screen
        if(!wamDisplay_isMoleActive(i)){            //Checks is mole is already active
            wamDisplay_moleInfo[newMole]->ticksUntilAwake =(rand()%(MyMoleCount*abs(MyMoleCount-Level_Count)))+MyMoleCount;//Sets ticks Until Awake to a random value mixed with level
            wamDisplay_moleInfo[newMole]->ticksUntilDormant =(rand()%(abs(MyMoleCount-Level_Count)))+MyMoleCount;          //Sets ticks Until Dormant to a random value mixed with level
            numMole = newMole;                      //Keeps track of that mole
        }//If that mole is already active, move on to try a different number
    }
    if(numMole == (MyMoleCount + 1))    //This checks for error and
        return false;                   // and prevents segmentation faults
    else if(wamDisplay_moleInfo[numMole]!= NULL)
        return true;                    //This is a valid mole, return true
    else
        return false;                   //Non valid mole, there was a problem
}

// This takes the provided coordinates and attempts to whack a mole. If a
// mole is successfully whacked, all internal data structures are updated and
// the display and score is updated. You can only whack a mole if the mole is awake (visible).
wamDisplay_moleIndex_t wamDisplay_whackMole(wamDisplay_point_t* whackOrigin){
    for(uint16_t i=0;i<MyMoleCount;i++){    //Check if he hit an active mole
        //This checks if he hit a real mole. Must hit inside a certain square around mole hole
        if((abs(whackOrigin->x-wamDisplay_moleInfo[i]->origin.x)<MOLE_SQUARE)&&(abs(whackOrigin->y-wamDisplay_moleInfo[i]->origin.y)<MOLE_SQUARE)){
            //He hit a real mole hole
            if((wamDisplay_moleInfo[i]->ticksUntilDormant != 0)){   //Verifies mole was actually awake
                display_fillCircle(wamDisplay_moleInfo[i]->origin.x,wamDisplay_moleInfo[i]->origin.y, MOLE_RADIUS, DISPLAY_BLACK); //Erases the mole that was whacked
                wamDisplay_setHitScore(Hit_Count + 1);          //Need to send in Hit_Count + 1 to increment the hits
                wamDisplay_moleInfo[i]->ticksUntilAwake = 0;    //Resets the values
                wamDisplay_moleInfo[i]->ticksUntilDormant = 0;  // after successfully whacked the mole
            }//If he touched an incorrect hole, don't do anything
        }//If he touched a place that was not a hole, don't do anything
    }
    if(((Hit_Count % WAM_UPDATE_LEVEL) == 0)&&(oldHit_Count != Hit_Count)){//Every time he gets proper number of hits, update level
        oldHit_Count = Hit_Count;                   //Have an old hit count to make sure level doesn't change if he fails to hit
        wamDisplay_incrementLevel();                //Update level on the screen
        wamControl_setMaxActiveMoles(Level_Count);  //Change max active moles for difficulty
    }
    return 0;
}

// This updates the ticksUntilAwake/ticksUntilDormant clocks for all of the moles.
void wamDisplay_updateAllMoleTickCounts(){
    printf("Update All Mole Tick Counts\n\r");
    for(uint16_t i=0;i<MyMoleCount;i++){                            //Go through the moles,
        if(wamDisplay_isMoleActive(i)){                             // Check if they are active
            if(wamDisplay_moleInfo[i]->ticksUntilAwake==0){       //If mole is awake
                if(wamDisplay_moleInfo[i]->ticksUntilDormant!=0)  // But not dormant(means he is on screen)
                    wamDisplay_moleInfo[i]->ticksUntilDormant--;    // bring him closer to his rest
            }
            else if(wamDisplay_moleInfo[i]->ticksUntilAwake==1){  //When he is almost awake, he needs to be drawn

                display_fillCircle(wamDisplay_moleInfo[i]->origin.x,wamDisplay_moleInfo[i]->origin.y, MOLE_RADIUS-1, DISPLAY_RED);//When ready, tell him to wake up
                wamDisplay_moleInfo[i]->ticksUntilAwake=0;//Taken care of, so decrement to zero once he is on the board
            }
            else if(wamDisplay_moleInfo[i]->ticksUntilAwake!=0)   //If active but not yet awake
                wamDisplay_moleInfo[i]->ticksUntilAwake--;          // Bring him closer to being awake
            if(wamDisplay_moleInfo[i]->ticksUntilDormant==1){     //He was not touched in time,
                display_fillCircle(wamDisplay_moleInfo[i]->origin.x,wamDisplay_moleInfo[i]->origin.y, MOLE_RADIUS, DISPLAY_BLACK);//Send him back into his hole
                wamDisplay_setMissScore(Miss_Count + 1);   //Send in Miss_Count + 1 so miss count can erase the previous score and incremented score
                wamDisplay_moleInfo[i]->ticksUntilDormant = 0;//Taken care of so make him rest
            }
        }
    }
}

// Returns the count of currently active moles.
uint16_t wamDisplay_getActiveMoleCount(){
    uint16_t activeMoleCount = 0;
    for(uint16_t i=0;i<MyMoleCount;i++){ //For all the moles
        if(wamDisplay_isMoleActive(i))   //Check if they are active
            activeMoleCount++;//Increments only when Mole either has ticks till awake or ticks until dormant
    }
    return activeMoleCount;   //Return number of moles that are in their holes, waiting or popped
}

// Sets the hit value in the score window.
void wamDisplay_setHitScore(uint16_t hits){
    printf("Set Hit Score\n\r");
    display_setTextSize(WAMTEXTSIZE);                       //Set text size to correct size
    display_setCursor(WAMBOARD_HIT_X_VAL, WAMBOARD_COUNT_Y);//Correct Place for the value
    display_setTextColor(DISPLAY_BLACK);                    //Will erase character when drawn
    display_println(Hit_Count);                             //Erases the previous value
    display_setCursor(WAMBOARD_HIT_X_VAL, WAMBOARD_COUNT_Y);//Correct Place for the value
    display_setTextColor(DISPLAY_WHITE);                    //Able to draw a new value
    display_println(hits);                                  //Draws the new value
    Hit_Count = hits;
}

// Gets the current hit value.
uint16_t wamDisplay_getHitScore(){return Hit_Count;}

// Sets the miss value in the score window.
void wamDisplay_setMissScore(uint16_t misses){
    display_setTextSize(WAMTEXTSIZE);                       //Set text size to correct size
    display_setCursor(WAMBOARD_MISS_X_VAL, WAMBOARD_COUNT_Y);//Correct Place for the value
    display_setTextColor(DISPLAY_BLACK);                    //Will erase character when drawn
    display_println(Miss_Count);                            //Erases the previous value
    display_setCursor(WAMBOARD_MISS_X_VAL, WAMBOARD_COUNT_Y);//Replace to redraw new value
    display_setTextColor(DISPLAY_WHITE);                    //Able to draw a new value
    display_println(misses);                                //Draws the new value
    Miss_Count = misses;                                    //Updates my value of miss count
}

// Gets the miss value.
// Can be used for testing and other functions.
uint16_t wamDisplay_getMissScore(){return Miss_Count;}

// Sets the level value on the score board.
void wamDisplay_incrementLevel(){
    display_setTextSize(WAMTEXTSIZE);                       //Set text size to correct size
    display_setCursor(WAMBOARD_LEVEL_X_VAL, WAMBOARD_COUNT_Y);//Correct Place for the value
    display_setTextColor(DISPLAY_BLACK);                    //Will erase character when drawn
    display_println(Level_Count);                           //Erases the previous value
    Level_Count++;                                          //Increments the level count
    display_setCursor(WAMBOARD_LEVEL_X_VAL, WAMBOARD_COUNT_Y);//Correct Place for the value
    display_setTextColor(DISPLAY_WHITE);                    //Able to draw a new value
    display_println(Level_Count);                           //Draws the new value
}

// Retrieves the current level value.
// Can be used for testing and other functions.
uint16_t wamDisplay_getLevel(){return Level_Count;}

// Completely draws the score screen.
// This function renders all fields, including the text fields for "Hits" and "Misses".
// Usually only called once when you are initializing the game.
void wamDisplay_drawScoreScreen(){
    char str[SCORE_STR_LENGTH]={};
    sprintf(str,SCORESCREEN_STR,wamDisplay_getHitScore(),wamDisplay_getMissScore(),wamDisplay_getLevel());
    display_println(str);
}

// Make this function available for testing purposes.
void wamDisplay_incrementMissScore(){
    Miss_Count++;   //Increments miss score
}

// Reset the scores and level to restart the game.
void wamDisplay_resetAllScoresAndLevel(){
    if(wamDisplay_moleInfo != NULL){        //Check to make sure that wamDisplay is pointing somewhere before you free it
        for(uint16_t i=0;i<MyMoleCount;i++){//Do the reverse process to allocation
            free(wamDisplay_moleInfo[i]);   //Stop pointing into the box
            wamDisplay_moleInfo[i]=NULL;    //Set that stage to NULL pointers
        }
        free(wamDisplay_moleInfo);          //Free the container
        wamDisplay_moleInfo=NULL;           //Set container to NULL
    }
    Hit_Count = 0;      //All these values should be
    Level_Count = 0;    // reset to begin game again
    Miss_Count = 0;
    oldHit_Count = 0;
    wamControl_setMaxActiveMoles(1);//Make sure level restarts as well
}

// Test function that can be called from main() to demonstrate milestone 1.
// Invoking this function should provide the same behavior as shown in the Milestone 1 video.
void wamDisplay_runMilestone1_test(){
    MyMoleCount = 9;
    wamDisplay_init();
    wamDisplay_drawSplashScreen();      //THIS IS TEST CODE
    utils_msDelay(2000);
    wamDisplay_drawMoleBoard();
    utils_msDelay(2000);
    wamDisplay_drawGameOverScreen();
    wamDisplay_resetAllScoresAndLevel();
}



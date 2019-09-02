/*
 * wamControl.c
 *
 *  Created on: Dec 7, 2015
 *      Author: rdmclean
 */

#include "wamControl.h"

static bool WAM_Game_Over;
static uint16_t WAM_msPerTick;
static uint16_t WAM_MaxActiveMoles;
static uint16_t WAM_MAXMissCount;
static uint8_t WAM_z_coord;
wamDisplay_point_t WAM_touched;

// Call this before using any wamControl_ functions.
void wamControl_init() {
    WAM_Game_Over = false;
}

// Call this to set how much time is consumed by each tick of the controlling state machine.
// This information makes it possible to set the awake and sleep time of moles in ms, not ticks.
void wamControl_setMsPerTick(uint16_t msPerTick) {
    WAM_msPerTick = msPerTick;
}

// This returns the time consumed by each tick of the controlling state machine.
uint16_t wamControl_getMsPerTick() {
    return WAM_msPerTick;
}

//These are the states for my control state machine
enum WAMCONTROL_CurState {
    WAMCONTROL_init, WAMCONTROL_wait_touch,
    WAMCONTROL_wait_settle, WAM_CONTROL_Send_Coordinates
} WAMCONTROL_CurState = WAMCONTROL_init;

// Standard tick function.
void wamControl_tick() {
    switch (WAMCONTROL_CurState) {//State Actions
    case WAMCONTROL_init:
        break; //No State Actions required, taken care of in control_init
    case WAMCONTROL_wait_touch:
        if (wamDisplay_getActiveMoleCount() < wamControl_getMaxActiveMoles())//If amount of active moles is not yet saturated for the level
            wamDisplay_activateRandomMole();                                // Activate a new mole
        wamDisplay_updateAllMoleTickCounts();                               //Always update the mole tick counts
        if (wamDisplay_getMissScore() >= WAM_MAXMissCount)                  //Checks to see if user has lost the game
            WAM_Game_Over = true;                                           // If he has, end the game
        break;
    case WAMCONTROL_wait_settle:        //No state actions required in this state
        break;
    case WAM_CONTROL_Send_Coordinates:
        display_getTouchedPoint(&WAM_touched.x, &WAM_touched.y, &WAM_z_coord);//Get the touched point from the LCD display
        display_clearOldTouchData();                                          //Clear the touch data
        wamDisplay_whackMole(&WAM_touched);                                   //Send coordinates into the whack a mole function
        break;
    default:
        printf("ERROR: Tick in False State\n\r");   //No state actions, print off for error checking
        break;
    }
    ///////////////////////////////////////////////////////////
    switch (WAMCONTROL_CurState) {      //Mealy Actions and Transitions
    case WAMCONTROL_init:
        wamDisplay_activateRandomMole();        //Activate a mole on transition to begin game more quickly
        WAMCONTROL_CurState = WAMCONTROL_wait_touch;//Send straight into the wait touch game state
        break;
    case WAMCONTROL_wait_touch:
        if (display_isTouched()) {              //A mole has been whacked
            display_clearOldTouchData();        //Clear the touch data
            WAMCONTROL_CurState = WAMCONTROL_wait_settle;//Move on to ADC state
        }
        else
            WAMCONTROL_CurState = WAMCONTROL_wait_touch; //Nothing has happened, so do not leave
        break;
    case WAMCONTROL_wait_settle:
        WAMCONTROL_CurState = WAM_CONTROL_Send_Coordinates;//Move on to send coordinates
        break;
    case WAM_CONTROL_Send_Coordinates:
        if (WAM_Game_Over == true)
            WAMCONTROL_CurState = WAMCONTROL_init;  //If the game should be over, end it
        else
            WAMCONTROL_CurState = WAMCONTROL_wait_touch;//If not, just keep playing
        break;
    default:
        printf("ERROR: Tick in False State\n\r");//Error checking
        break;
    }
}

// Returns a random value that indicates how long the mole should sleep before awaking.
wamDisplay_moleTickCount_t wamControl_getRandomMoleAsleepInterval() {
    return (rand() % wamDisplay_getLevel()); //Level is linked to mole count, returns value on board
}

// Returns a random value that indicates how long the mole should stay awake before going dormant.
wamDisplay_moleTickCount_t wamControl_getRandomMoleAwakeInterval() {
    return (rand() % wamDisplay_getLevel()); //Level is linked to mole count, returns value on board
}

// Set the maximum number of active moles.
void wamControl_setMaxActiveMoles(uint16_t count) {
    WAM_MaxActiveMoles = count; //Set a max number to be used in display
}

// Get the current allowable count of active moles.
uint16_t wamControl_getMaxActiveMoles() {
    return WAM_MaxActiveMoles;  //Returns max number on the board
}

// Set the seed for the random-number generator.
void wamControl_setRandomSeed(uint32_t seed) {
    srand(seed);    //Sets the random seed based on the timer passed in for pseudo randomness
}

// Set the maximum number of misses until the game is over.
void wamControl_setMaxMissCount(uint16_t missCount) {
    WAM_MAXMissCount = missCount; //Sets the max miss count before starting over
}

// Use this predicate to see if the game is finished.
bool wamControl_isGameOver() {
    return WAM_Game_Over;   //Tells if game is over or not
} //True is over, False is not over



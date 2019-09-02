/*
 * simonControl.h
 *
 *      Author: RYAN MCLEAN
 *      My period finished at about 80ms which worked well
 *      for functionality, playable and accuracy
 */

#ifndef SIMONCONTROL_H_
#define SIMONCONTROL_H_

#include "verifySequence.h"
#include "flashSequence.h"
#include "globals.h"
#include <time.h>
#include <stdlib.h>


/* *************************************************************
 * A standard tick function that controls and operates the Simon
 * game, playable by a user on an LCD display
 ***************************************************************/
void simonControl_tick();

/**************************************************
 * Helper function to create random array sequence
 **************************************************/
void simonControl_setSequence(uint32_t timer_duration);



#endif /* SIMONCONTROL_H_ */

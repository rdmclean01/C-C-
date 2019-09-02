/*
 * clockDisplay.h
 *
 *  Created on: Sep 25, 2015
 *      Author: rdmclean
 */

#ifndef CLOCKDISPLAY_H_
#define CLOCKDISPLAY_H_
#include <stdbool.h>
#include <stdio.h>
#include "clockControl.h"

/* Called only once - performs any necessary inits.*/
void clockDisplay_init();

/* Updates the time display with latest time.*/
void clockDisplay_updateTimeDisplay(bool forceUpdateAll);

/* Performs the increment or decrement, depending upon the touched region.*/
void clockDisplay_performIncDec();

/* Advances the time forward by 1 second.*/
void clockDisplay_advanceTimeOneSecond();

/* Run a test of clock-display functions.*/
void clockDisplay_runTest();



#endif /* CLOCKDISPLAY_H_ */

/*
 * flashSequence.h
 *
 *  Created on: Nov 2, 2015
 *      Author: rdmclean
 */

#ifndef FLASHSEQUENCE_H_
#define FLASHSEQUENCE_H_

#include "simonDisplay.h"
#include "globals.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Turns on the state machine. Part of the interlock.
void flashSequence_enable();

// Turns off the state machine. Part of the interlock.
void flashSequence_disable();

// Other state machines can call this to determine if this state machine is finished.
bool flashSequence_completed();

// Standard tick function.
void flashSequence_tick();

// Tests the flashSequence state machine.
void flashSequence_runTest();



#endif /* FLASHSEQUENCE_H_ */

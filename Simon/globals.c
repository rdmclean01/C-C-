/*
 * globals.c
 *
 *  Created on: Oct 30, 2015
 *      Author: rdmclean
 */

#include "globals.h"

static uint16_t size;
static uint16_t globals_iterationLength;
static uint8_t sequenceArray[GLOBALS_MAX_FLASH_SEQUENCE];

// This is the length of the complete sequence at maximum length.
// You must copy the contents of the sequence[] array into the global variable that you maintain.
// Do not just grab the pointer as this will fail.
void globals_setSequence(const uint8_t sequence[], uint16_t length){
    size = length;                      //Set my size to desired length
    for(int i = 0; i < size; i++){      //Walk through array and copy
        sequenceArray[i]=sequence[i];   // the information to personal
    }                                   // array
}

// This returns the value of the sequence at the index.
uint8_t globals_getSequenceValue(uint16_t index){return sequenceArray[index];}

// Retrieve the sequence length.
uint16_t globals_getSequenceLength(){return size;}

// This is the length of the sequence that you are currently working on.
void globals_setSequenceIterationLength(uint16_t length){globals_iterationLength = length;}

// This is the length of the sequence that you are currently working on (not the maximum length but the interim length as
// the use works through the pattern one color at a time.
uint16_t globals_getSequenceIterationLength(){return globals_iterationLength;}

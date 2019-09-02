/*
 * switchAndButtonLabMain.c
 *
 *  Created on: Sep 25, 2015
 *      Author: rdmclean
 */
/* RYAN MCLEAN, 330 LAB 2
 * Main runs the switches_runTest and buttons_runTest functions */

#include "buttons.h"
#include "switches.h"
#include "supportFiles/leds.h"
#include <stdio.h>

int main()
{
    printf("The main has begun\n");
//    leds_runTest();
    switches_runTest(); // Runs the test connecting the switches with the LEDs
    buttons_runTest(); // Runs the test that connects buttons to the display board, a colored, labeled, rectangle
    printf("The main has terminated\n");
}

// This is called in the timer interrupt service routine (see interrupts.c in supportFiles).
void isr_function() {
    // Empty for now.
}




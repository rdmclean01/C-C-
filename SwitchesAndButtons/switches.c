/*
 * switches.c
 *
 *  Created on: Sep 25, 2015
 *      Author: rdmclean
 */
#include "switches.h"
#include "xil_io.h"
#include "supportFiles/leds.h"
#include <stdio.h>

#define SWITCHES_GPIO_DIRECTION 0xF
#define SWITCHES_TRI_OFFSET 0x04
#define SWITCHES_MASKSWITCH0 0x1
#define SWITCHES_MASKSWITCH1 0x2
#define SWITCHES_MASKSWITCH2 0x4
#define SWITCHES_MASKSWITCH3 0x8
#define DATA_OFFSET 0x0
#define READ_SWITCH_MASK 0xF

/*Initializes the SWITCHES driver software and hardware.*/
int switches_init()
{
    Xil_Out32( XPAR_SLIDE_SWITCHES_BASEADDR + SWITCHES_TRI_OFFSET, SWITCHES_GPIO_DIRECTION); // Will write the TRI_STATE Driver to 1
    return 0;
}

/*Returns the current value of all 4 SWITCHES as the lower bits of the returned value.
 * bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.*/
int32_t switches_read()
{
    uint32_t read = Xil_In32(XPAR_SLIDE_SWITCHES_BASEADDR + DATA_OFFSET); // calls function to read the switches value
    return read & READ_SWITCH_MASK;
}

/* This function will light up the LEDs when switches are pressed */
void Light_LED()
{
    uint32_t sRead = switches_read(); // Reads the switches before starting
    while (sRead != READ_SWITCH_MASK)
    {
        sRead = switches_read();              // Reads switches on each iteration
        if(sRead & SWITCHES_MASKSWITCH0)      // When right switch is pressed,
            leds_write(SWITCHES_MASKSWITCH0); // light up the right LED
        if(sRead & SWITCHES_MASKSWITCH1)      // When right-middle switch is pressed,
            leds_write(SWITCHES_MASKSWITCH1); // light up the right-middle LED
        if(sRead & SWITCHES_MASKSWITCH2)      // When left-middle switch is pressed,
            leds_write(SWITCHES_MASKSWITCH2); // light up the left-middle LED
        if(sRead & SWITCHES_MASKSWITCH3)      // When left switch is pressed,
            leds_write(SWITCHES_MASKSWITCH3); // light up the left LED
        leds_write(0x0);
    }
}

/* Runs a test of the switches. As you slide the switches, LEDs directly above the switches will illuminate.
 * The test will run until all switches are slid upwards. When all 4 slide switches are slid upward,
 * this function will return.*/
void switches_runTest()
{
    printf("In switches_runTest()\n");
    leds_init(1);   // Initializes the display
    switches_init();// Calls the init function
    switches_read();// Reads the switches
    Light_LED();    // Lights the LEDs based on switch input
}










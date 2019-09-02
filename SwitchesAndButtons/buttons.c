/*
 * buttons.c
 *
 *  Created on: Sep 25, 2015
 *      Author: rdmclean
 */

#include "buttons.h"
#include "SupportFiles/display.h"
#include <stdio.h>

#define BUTTONS_GPIO_DEVICE_BASE_ADDRESS XPAR_PUSH_BUTTONS_BASEADDR
#define BUTTONS_GPIO_DIRECTION 0xF
#define BUTTONS_TRI_OFFSET 0x04
#define BUTTONS_READ_BUTTONS_MASK 0xF
#define BUTTONS_DATA_OFFSET 0x00
#define BUTTONS_MASKBUTTON1 0x1
#define BUTTONS_MASKBUTTON2 0x2
#define BUTTONS_MASKBUTTON3 0x4
#define BUTTONS_MASKBUTTON4 0x8
#define BUTTONS_WHILE_FINISH 0xF
#define BUTTONS_RECTANGLE_HEIGHT (DISPLAY_HEIGHT / 2) // Rectangle is half the height
#define BUTTONS_RECTANGLE_WIDTH (DISPLAY_WIDTH / 4) // Width is a quarter of total
#define BUTTONS_RECT1 ((3*DISPLAY_WIDTH) / 4) // Puts a rect 3/4 way over
#define BUTTONS_RECT2 (DISPLAY_WIDTH / 2) // Puts a rect at half
#define BUTTONS_RECT3 (DISPLAY_WIDTH / 4) // Puts a rect at one quarter
#define BUTTONS_RECT1_STRING "BTN0"
#define BUTTONS_RECT2_STRING "BTN1"
#define BUTTONS_RECT3_STRING "BTN2"
#define BUTTONS_RECT4_STRING "BTN3"
#define ORIGIN 0
#define XCURSOR_OFFSET 5
#define YCURSOR_OFFSET 40
#define TEXT_SIZE 3

/* Helper function to read GPIO registers.*/
uint32_t buttons_readGpioRegister(uint32_t offset)
{
    uint32_t *ptr = (uint32_t *) BUTTONS_GPIO_DEVICE_BASE_ADDRESS + offset;
    return *ptr;
}

/* Helper function to write the GPIO registers*/
void buttons_writeGpioRegister(uint32_t offset, uint32_t data)
{
    uint32_t *ptr = (uint32_t *) BUTTONS_GPIO_DEVICE_BASE_ADDRESS + offset;
    *ptr = data;
}

/* Initializes the button driver software and hardware. Returns one of the defined status values (above)*/
int buttons_init()
{
    buttons_writeGpioRegister(BUTTONS_TRI_OFFSET, BUTTONS_GPIO_DIRECTION);  // Writes the tristate driver to ZERO
    return 0;
}

/* Returns the current value of all 4 buttons as the lower 4 bits of the returned value.
 *  bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.  */
int32_t buttons_read()
{
    uint32_t read = buttons_readGpioRegister(BUTTONS_DATA_OFFSET); // This receives the values from the buttons
    return read & BUTTONS_READ_BUTTONS_MASK; // Returns a 32 bit value with 0's for all except for low 4 bits which are the values of the buttons
}


/* Will draw the rectangles when each button is pressed */
void drawBoard() // This function will draw rectangles on the LCD when buttons are pressed
{
    uint32_t bRead = buttons_read();
    while (bRead != BUTTONS_WHILE_FINISH)
    {
        bRead = buttons_read();
        if(bRead & BUTTONS_MASKBUTTON1)               // For right Button lighting up the board
        {
            display_fillRect(BUTTONS_RECT1, ORIGIN, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_YELLOW); // Displays a Yellow rectangle for BTN0 on right when pressed
            display_setCursor(BUTTONS_RECT1 + XCURSOR_OFFSET, YCURSOR_OFFSET);   // PLACES INSIDE THE RECTANGLE.
            display_setTextColor(DISPLAY_BLACK);       // Make the text white.
            display_setTextSize(TEXT_SIZE);            // Make the text larger.
            display_println(BUTTONS_RECT1_STRING);     // This prints the string to the LCD.
        }
        else
            display_fillRect(BUTTONS_RECT1, ORIGIN, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_BLACK); // ELSE Displays a BLACK rectangle for BTN0 on right when not pressed

        if(bRead & BUTTONS_MASKBUTTON2)               // For left-middle Button lighting up the board
        {
            display_fillRect(BUTTONS_RECT2, ORIGIN, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_GREEN); // Displays a Green rectangle for BTN1 on middle right when pressed
            display_setCursor(BUTTONS_RECT2 + XCURSOR_OFFSET, YCURSOR_OFFSET);    // PLACES INSIDE THE RECTANGLE.
            display_setTextColor(DISPLAY_BLACK);      // Make the text white.
            display_setTextSize(TEXT_SIZE);           // Make the text a little larger.
            display_println(BUTTONS_RECT2_STRING);                  // This actually prints the string to the LCD.
        }
        else
            display_fillRect(BUTTONS_RECT2, ORIGIN, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_BLACK); // ELSE Displays a BLACK rectangle for BTN1 on middle right when not pressed
        if(bRead & BUTTONS_MASKBUTTON3)               // For right-middle button lighting up the board
        {
            display_fillRect(BUTTONS_RECT3, ORIGIN, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_RED); // Displays a Red rectangle for BTN2 on middle left when pressed
            display_setCursor(BUTTONS_RECT3 + XCURSOR_OFFSET, YCURSOR_OFFSET);    // PLACES INSIDE THE RECTANGLE.
            display_setTextColor(DISPLAY_WHITE);      // Make the text white.
            display_setTextSize(TEXT_SIZE);           // Make the text a little larger.
            display_println(BUTTONS_RECT3_STRING);                  // This actually prints the string to the LCD.
        }
        else
            display_fillRect(BUTTONS_RECT3, ORIGIN, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_BLACK); // Displays a Red rectangle for BTN2 on middle left when not pressed
        if(bRead & BUTTONS_MASKBUTTON4)
        {
            display_fillRect(ORIGIN, ORIGIN, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_BLUE); // Displays a blue rectangle for BTN3 on left when pressed
            display_setCursor(ORIGIN + XCURSOR_OFFSET, YCURSOR_OFFSET);           // PLACES INSIDE THE RECTANGLE.
            display_setTextColor(DISPLAY_WHITE);      // Make the text white.
            display_setTextSize(TEXT_SIZE);           // Make the text a little larger.
            display_println(BUTTONS_RECT4_STRING);                  // This actually prints the string to the LCD.
        }
        else
            display_fillRect(ORIGIN, ORIGIN, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_BLACK); // Displays a blue rectangle for BTN3 on left when not pressed

    }

    display_fillScreen(DISPLAY_BLACK);                // Makes the display black when done



}

// Runs a test of the buttons. As you push the buttons, graphics and messages will be written to the LCD
// panel. The test will continue until all 4 pushbuttons are simultaneously pressed.
void buttons_runTest()
{
    printf("In buttons_runTest()\n");
    display_init();  // Initializes the display
    display_fillScreen(DISPLAY_BLACK); // Makes the display initially black
    buttons_init();
    buttons_read();
    drawBoard();
}





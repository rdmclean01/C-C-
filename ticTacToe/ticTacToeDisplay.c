/*
 * ticTacToeDisplay.c
 *
 *  Created on: Oct 9, 2015
 *      Author: rdmclean
 */
#include "ticTacToeDisplay.h"
//#include "src/SwitchesAndButtons/switches.h"
//#include "src/SwitchesAndButtons/buttons.h"
#include "supportFiles/switches.h"
#include "supportFiles/buttons.h"
#include "supportFiles/utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define TICTACTOE_COL_1_LINE (display_width() / 3)
#define TICTACTOE_COL_2_LINE (2*display_width() / 3)
#define TICTACTOE_ROW_1_LINE (display_height() / 3)
#define TICTACTOE_ROW_2_LINE (2*display_height() / 3)
#define TICTACTOE_COLUMN_0 0
#define TICTACTOE_COLUMN_1 1
#define TICTACTOE_COLUMN_2 2
#define TICTACTOE_ROW_0 0
#define TICTACTOE_ROW_1 1
#define TICTACTOE_ROW_2 2
#define CIRCLE_COLX_0 (display_width() / 6)
#define CIRCLE_COLX_1 (display_width() / 2)
#define CIRCLE_COLX_2 (5*(display_width()) / 6)
#define CIRCLE_ROWY_0 (display_height() / 6)
#define CIRCLE_ROWY_1 (display_height() / 2)
#define CIRCLE_ROWY_2 (5*(display_height()) / 6)
#define CIRCLE_RADIUS 30
#define X_COL0_POINT_0 (display_width() / 12)
#define X_COL0_POINT_1 (3 * display_width() / 12)
#define X_COL1_POINT_0 (5 * display_width() / 12)
#define X_COL1_POINT_1 (7 * display_width() / 12)
#define X_COL2_POINT_0 (9 * display_width() / 12)
#define X_COL2_POINT_1 (11* display_width() / 12)
//////////////////////////////////////////////////
#define X_ROW0_POINT_0 (display_height() / 12)
#define X_ROW0_POINT_1 (3 * display_height() / 12)
#define X_ROW1_POINT_0 (5 * display_height() / 12)
#define X_ROW1_POINT_1 (7 * display_height() / 12)
#define X_ROW2_POINT_0 (9 * display_height() / 12)
#define X_ROW2_POINT_1 (11* display_height() / 12)
#define BUTTON_RESTART_VALUE 0x1
#define BUTTON_ENDTEST_VALUE 0x2
#define TEST_DELAY 50


/**** Inits the tic-tac-toe display, draws the lines that form the board.****/
void ticTacToeDisplay_init()
{
    display_init();                     // Initializes the display
    display_fillScreen(DISPLAY_BLACK);  // Makes the display initially black
    ticTacToeDisplay_drawBoardLines();  // Calls the function to draw the lines
}

/****  Draws an X at the specified row and column. ****/
void ticTacToeDisplay_drawX(uint8_t row, uint8_t column){
    //An X in drawn by drawing two lines in a particular square
    if(row == TICTACTOE_ROW_0){         //For the 0th row
        switch(column){
        case TICTACTOE_COLUMN_0:            //Draws an X for 0th column
            display_drawLine(X_COL0_POINT_0, X_ROW0_POINT_0, X_COL0_POINT_1 , X_ROW0_POINT_1, DISPLAY_GREEN);
            display_drawLine(X_COL0_POINT_1, X_ROW0_POINT_0, X_COL0_POINT_0 , X_ROW0_POINT_1, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_1:            //Draws an X for 1st column
            display_drawLine(X_COL1_POINT_0, X_ROW0_POINT_0, X_COL1_POINT_1 , X_ROW0_POINT_1, DISPLAY_GREEN);
            display_drawLine(X_COL1_POINT_1, X_ROW0_POINT_0, X_COL1_POINT_0 , X_ROW0_POINT_1, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_2:            //Draws an X for 2nd column
            display_drawLine(X_COL2_POINT_0, X_ROW0_POINT_0, X_COL2_POINT_1 , X_ROW0_POINT_1, DISPLAY_GREEN);
            display_drawLine(X_COL2_POINT_1, X_ROW0_POINT_0, X_COL2_POINT_0 , X_ROW0_POINT_1, DISPLAY_GREEN);
            break;
        }
    }
    else if(row == TICTACTOE_ROW_1)     //For the 1st row
        switch(column){
        case TICTACTOE_COLUMN_0:            //Draws an X for 0th column
            display_drawLine(X_COL0_POINT_0, X_ROW1_POINT_0, X_COL0_POINT_1 , X_ROW1_POINT_1, DISPLAY_GREEN);
            display_drawLine(X_COL0_POINT_1, X_ROW1_POINT_0, X_COL0_POINT_0 , X_ROW1_POINT_1, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_1:            //Draws an X for 1st column
            display_drawLine(X_COL1_POINT_0, X_ROW1_POINT_0, X_COL1_POINT_1 , X_ROW1_POINT_1, DISPLAY_GREEN);
            display_drawLine(X_COL1_POINT_1, X_ROW1_POINT_0, X_COL1_POINT_0 , X_ROW1_POINT_1, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_2:            //Draws an X for 2nd column
            display_drawLine(X_COL2_POINT_0, X_ROW1_POINT_0, X_COL2_POINT_1 , X_ROW1_POINT_1, DISPLAY_GREEN);
            display_drawLine(X_COL2_POINT_1, X_ROW1_POINT_0, X_COL2_POINT_0 , X_ROW1_POINT_1, DISPLAY_GREEN);
            break;
        }
    else{                               //For the 2nd row
        switch(column){
        case TICTACTOE_COLUMN_0:            //Draws an X for 0th column
            display_drawLine(X_COL0_POINT_0, X_ROW2_POINT_0, X_COL0_POINT_1 , X_ROW2_POINT_1, DISPLAY_GREEN);
            display_drawLine(X_COL0_POINT_1, X_ROW2_POINT_0, X_COL0_POINT_0 , X_ROW2_POINT_1, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_1:            //Draws an X for 1st column
            display_drawLine(X_COL1_POINT_0, X_ROW2_POINT_0, X_COL1_POINT_1 , X_ROW2_POINT_1, DISPLAY_GREEN);
            display_drawLine(X_COL1_POINT_1, X_ROW2_POINT_0, X_COL1_POINT_0 , X_ROW2_POINT_1, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_2:            //Draws an X for 2nd column
            display_drawLine(X_COL2_POINT_0, X_ROW2_POINT_0, X_COL2_POINT_1 , X_ROW2_POINT_1, DISPLAY_GREEN);
            display_drawLine(X_COL2_POINT_1, X_ROW2_POINT_0, X_COL2_POINT_0 , X_ROW2_POINT_1, DISPLAY_GREEN);
            break;
        }
    }
}

/****  Draws an O at the specified row and column. ****/
void ticTacToeDisplay_drawO(uint8_t row, uint8_t column){
    //Drawing an O just calls the function to draw an O
    if(row == TICTACTOE_ROW_0){         //For the 0th row
        switch(column){
        case TICTACTOE_COLUMN_0:            //Draws an O for the 0th column
            display_drawCircle(CIRCLE_COLX_0, CIRCLE_ROWY_0, CIRCLE_RADIUS, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_1:            //Draws an O for the 1st column
            display_drawCircle(CIRCLE_COLX_1, CIRCLE_ROWY_0, CIRCLE_RADIUS, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_2:            //Draws an O for the 2nd column
            display_drawCircle(CIRCLE_COLX_2, CIRCLE_ROWY_0, CIRCLE_RADIUS, DISPLAY_GREEN);
            break;
        }
    }
    else if(row == TICTACTOE_ROW_1)     //For the 1st row
        switch(column){
        case TICTACTOE_COLUMN_0:            //Draws an O for the 0th column
            display_drawCircle(CIRCLE_COLX_0, CIRCLE_ROWY_1, CIRCLE_RADIUS, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_1:            //Draws an O for the 1st column
            display_drawCircle(CIRCLE_COLX_1, CIRCLE_ROWY_1, CIRCLE_RADIUS, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_2:            //Draws an O for the 2nd column
            display_drawCircle(CIRCLE_COLX_2, CIRCLE_ROWY_1, CIRCLE_RADIUS, DISPLAY_GREEN);
            break;
        }
    else{                               //For the 2nd row
        switch(column){
        case TICTACTOE_COLUMN_0:            //Draws an O for the 0th column
            display_drawCircle(CIRCLE_COLX_0, CIRCLE_ROWY_2, CIRCLE_RADIUS, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_1:            //Draws an O for the 1st column
            display_drawCircle(CIRCLE_COLX_1, CIRCLE_ROWY_2, CIRCLE_RADIUS, DISPLAY_GREEN);
            break;
        case TICTACTOE_COLUMN_2:            //Draws an O for the 2nd column
            display_drawCircle(CIRCLE_COLX_2, CIRCLE_ROWY_2, CIRCLE_RADIUS, DISPLAY_GREEN);
            break;
        }
    }
}

/**** After a touch has been detected and after the proper delay, this sets the row and column arguments
 * according to where the user touched the board.
 * |------------------
 * |  0  |  1  |  2  |
 * |------------------
 * |  1  |     |     |
 * |------------------
 * |  2  |     |     |
 * |------------------
 * ***********************************************************************************/
void ticTacToeDisplay_touchScreenComputeBoardRowColumn(uint8_t* row, uint8_t* column){
    int16_t coord_x;
    int16_t coord_y;
    uint8_t coord_z;
    display_clearOldTouchData();                           // Clear the old touch data
    display_getTouchedPoint(&coord_x, &coord_y, &coord_z); // Get data from the board

    if(coord_x < TICTACTOE_COL_1_LINE)          //When in left third
        *column = (uint8_t) TICTACTOE_COLUMN_0; // assign column to 0th column
    else if(coord_x < TICTACTOE_COL_2_LINE)     //When in middle third
        *column = (uint8_t) TICTACTOE_COLUMN_1; // assign column to 1st column
    else                                        //When in right third
        *column = (uint8_t) TICTACTOE_COLUMN_2; //assign column to 2nd column

    if(coord_y < TICTACTOE_ROW_1_LINE)          //When in top third
        *row = (uint8_t) TICTACTOE_ROW_0;       // assign row to 0th row
    else if(coord_y < TICTACTOE_ROW_2_LINE)     //When in middle third
        *row = (uint8_t) TICTACTOE_ROW_1;       // assign row to 1st row
    else                                        //When in bottom third
        *row = (uint8_t) TICTACTOE_ROW_2;       // assign row to 2nd row
}

/**** Runs a test of the display. Does the following.
 * Draws the board. Each time you touch one of the screen areas, the screen will paint
 * an X or an O, depending on whether switch 0 (SW0) is slid up (O) or down (X).
 * When BTN0 is pushed, the screen is cleared. The test terminates when BTN1 is pushed.****/
void ticTacToeDisplay_runTest(){
    uint8_t row;
    uint8_t column;
    ticTacToeDisplay_init();                    // Calls init to start the function and draw the board
    while(1){
        if(display_isTouched()){                // Wait for a touch
            utils_msDelay(TEST_DELAY);                  // wait for the analog digital converter to work
            ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column); // Define row and column based on touched
            if(switches_read() != 0){                   // Up switch is "O"
                ticTacToeDisplay_drawO(row, column);    //Draw a 0 in square
            }
            else{                                       // Down switch is "X"
                ticTacToeDisplay_drawX(row, column);    //Draw an X in square
            }
        }
        if(buttons_read() == BUTTON_RESTART_VALUE)       //Restarts game when button 0 pressed
            ticTacToeDisplay_init();
        if(buttons_read() == BUTTON_ENDTEST_VALUE){      //Ends test when button 1 pressed
            break;                                       //Leaves while and returns back to terminate main
        }
    }
}

/**** This will draw the four board lines.****/
void ticTacToeDisplay_drawBoardLines(){
    display_drawLine(TICTACTOE_COL_1_LINE, 0, TICTACTOE_COL_1_LINE, display_height(),DISPLAY_GREEN); // Makes Right vertical line
    display_drawLine(TICTACTOE_COL_2_LINE, 0, TICTACTOE_COL_2_LINE, display_height(),DISPLAY_GREEN); // Makes Left Vertical line
    display_drawLine(0, TICTACTOE_ROW_1_LINE, display_width(), TICTACTOE_ROW_1_LINE, DISPLAY_GREEN); // Draws Top Horizontal line
    display_drawLine(0, TICTACTOE_ROW_2_LINE, display_width(), TICTACTOE_ROW_2_LINE, DISPLAY_GREEN); // Draws Top Horizontal line
}





/*
 * ticTacToeMain.c

 *
 *  Created on: Oct 9, 2015
 *      Author: Ryan McLean
 */

#include "ticTacToeDisplay.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "xparameters.h"
#include "supportFiles/utils.h"
#include "SupportFiles/display.h"
#include "supportFiles/globalTimer.h"
#include "supportFiles/interrupts.h"
#include "supportFiles/intervalTimer.h"
#include "supportFiles/leds.h"
#include "minimax.h"
#include "ticTacToeControl.h"


#define TOTAL_SECONDS 100
// The formula for computing the load value is based upon the formula from 4.1.1 (calculating timer intervals)
// in the Cortex-A9 MPCore Technical Reference Manual 4-2.
// Assuming that the prescaler = 0, the formula for computing the load value based upon the desired period is:
// load-value = (period * timer-clock) - 1
#define TIMER_PERIOD 150E-3                                            // NEED TO CHECK OUT THIS WITH MY TIMER
#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define TIMER_LOAD_VALUE ((TIMER_PERIOD * TIMER_CLOCK_FREQUENCY) - 1.0)
static uint32_t isr_functionCallCount = 0;
uint32_t timerNumber = 0;
double seconds;
int main()
{
    printf("The main has begun\n");
   //  ticTacToeDisplay_runTest();
                                    // COMMENT TESTS MINIMAX
    /* minimax_board_t board1;  // Board 1 is the main example in the web-tutorial that I use on the web-site.
    board1.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
    board1.squares[0][1] = MINIMAX_EMPTY_SQUARE;
    board1.squares[0][2] = MINIMAX_PLAYER_SQUARE;
    board1.squares[1][0] = MINIMAX_PLAYER_SQUARE;
    board1.squares[1][1] = MINIMAX_EMPTY_SQUARE;
    board1.squares[1][2] = MINIMAX_EMPTY_SQUARE;
    board1.squares[2][0] = MINIMAX_PLAYER_SQUARE;
    board1.squares[2][1] = MINIMAX_OPPONENT_SQUARE;
    board1.squares[2][2] = MINIMAX_OPPONENT_SQUARE;

    minimax_board_t board2;
    board2.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
    board2.squares[0][1] = MINIMAX_EMPTY_SQUARE;
    board2.squares[0][2] = MINIMAX_PLAYER_SQUARE;
    board2.squares[1][0] = MINIMAX_EMPTY_SQUARE;
    board2.squares[1][1] = MINIMAX_EMPTY_SQUARE;
    board2.squares[1][2] = MINIMAX_EMPTY_SQUARE;
    board2.squares[2][0] = MINIMAX_PLAYER_SQUARE;
    board2.squares[2][1] = MINIMAX_EMPTY_SQUARE;
    board2.squares[2][2] = MINIMAX_OPPONENT_SQUARE;

    minimax_board_t board3;
    board3.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
    board3.squares[0][1] = MINIMAX_EMPTY_SQUARE;
    board3.squares[0][2] = MINIMAX_EMPTY_SQUARE;
    board3.squares[1][0] = MINIMAX_OPPONENT_SQUARE;
    board3.squares[1][1] = MINIMAX_EMPTY_SQUARE;
    board3.squares[1][2] = MINIMAX_EMPTY_SQUARE;
    board3.squares[2][0] = MINIMAX_PLAYER_SQUARE;
    board3.squares[2][1] = MINIMAX_EMPTY_SQUARE;
    board3.squares[2][2] = MINIMAX_PLAYER_SQUARE;

    minimax_board_t board4;
    board4.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
    board4.squares[0][1] = MINIMAX_EMPTY_SQUARE;
    board4.squares[0][2] = MINIMAX_EMPTY_SQUARE;
    board4.squares[1][0] = MINIMAX_EMPTY_SQUARE;
    board4.squares[1][1] = MINIMAX_EMPTY_SQUARE;
    board4.squares[1][2] = MINIMAX_EMPTY_SQUARE;
    board4.squares[2][0] = MINIMAX_PLAYER_SQUARE;
    board4.squares[2][1] = MINIMAX_EMPTY_SQUARE;
    board4.squares[2][2] = MINIMAX_PLAYER_SQUARE;

    minimax_board_t board5;
    board5.squares[0][0] = MINIMAX_PLAYER_SQUARE;
    board5.squares[0][1] = MINIMAX_PLAYER_SQUARE;
    board5.squares[0][2] = MINIMAX_EMPTY_SQUARE;
    board5.squares[1][0] = MINIMAX_EMPTY_SQUARE;
    board5.squares[1][1] = MINIMAX_OPPONENT_SQUARE;
    board5.squares[1][2] = MINIMAX_EMPTY_SQUARE;
    board5.squares[2][0] = MINIMAX_EMPTY_SQUARE;
    board5.squares[2][1] = MINIMAX_EMPTY_SQUARE;
    board5.squares[2][2] = MINIMAX_EMPTY_SQUARE;

    uint8_t row, column;

    minimax_computeNextMove(&board1, true, &row, &column);
    printf("next move for board1: (%d, %d)\n\r", row, column);
    printBoard(&board1);
    minimax_computeNextMove(&board2, true, &row, &column);
    printf("next move for board2: (%d, %d)\n\r", row, column);
    printBoard(&board2);
    minimax_computeNextMove(&board3, true, &row, &column);
    printf("next move for board3: (%d, %d)\n\r", row, column);
    printBoard(&board3);
    minimax_computeNextMove(&board4, false, &row, &column);
    printf("next move for board4: (%d, %d)\n\r", row, column);
    printBoard(&board4);
    minimax_computeNextMove(&board5, false, &row, &column);
    printf("next move for board5: (%d, %d)\n\r", row, column);
    printBoard(&board5);
    minimax_board_t boardt;

    bool player = true;
    while (1) {
        test_Board(&boardt, player);
        int16_t testScore = minimax_computeBoardScore(&boardt, player);
        if(minimax_isGameOver(testScore)){
            printf("Final score is %d",testScore);
            break;
        }
    }*/

    /*******************************************************************************************
        // Initialize the GPIO LED driver and print out an error message if it fails (argument = true).
        // You need to init the LEDs so that LD4 can function as a heartbeat.
        leds_init(true);
        // Init all interrupts (but does not enable the interrupts at the devices).
        // Prints an error message if an internal failure occurs because the argument = true.
        interrupts_initAll(true);
        interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);
        u32 privateTimerTicksPerSecond = interrupts_getPrivateTimerTicksPerSecond();
        printf("private timer ticks per second: %ld\n\r", privateTimerTicksPerSecond);
        // Allow the timer to generate interrupts.
        interrupts_enableTimerGlobalInts();
      //  ticTacToeControl_tick();
        // Keep track of your personal interrupt count. Want to make sure that you don't miss any interrupts.
        int32_t personalInterruptCount = 0;
        // Start the private ARM timer running.
        interrupts_startArmPrivateTimer();
        // Enable interrupts at the ARM.
        interrupts_enableArmInts();
        // interrupts_isrInvocationCount() returns the number of times that the timer ISR was invoked.
        // This value is maintained by the timer ISR. Compare this number with your own local
        // interrupt count to determine if you have missed any interrupts.

        while (interrupts_isrInvocationCount() < (TOTAL_SECONDS * privateTimerTicksPerSecond)) {
           // printf("HERE in while\n\n");
            if (interrupts_isrFlagGlobal) {  // This is a global flag that is set by the timer interrupt handler.
                // Count ticks.
                personalInterruptCount++;
                intervalTimer_start(timerNumber); // Start a Timer for testing
                ticTacToeControl_tick();
                intervalTimer_stop(timerNumber); // Stop a Timer for testing
                intervalTimer_getTotalDurationInSeconds(timerNumber, &seconds);      //Gets the total duration in seconds of that Timer
                printf("Time Duration %lu: %6.2e seconds.\n\r",timerNumber, seconds);//Prints off the total duration
                interrupts_isrFlagGlobal = 0;
            }
        }
        interrupts_disableArmInts();
        printf("isr invocation count: %ld\n\r", interrupts_isrInvocationCount());
        printf("internal interrupt count: %ld\n\r", personalInterruptCount);
         ***************************************************************************************************/

    /********************************************************************************************/
    // Initialize the GPIO LED driver and print out an error message if it fails (argument = true).
    // You need to init the LEDs so that LD4 can function as a heartbeat.
    leds_init(true);
    // Init all interrupts (but does not enable the interrupts at the devices).
    // Prints an error message if an internal failure occurs because the argument = true.
    interrupts_initAll(true);
    interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);
    printf("timer load value:%ld\n\r", (int32_t) TIMER_LOAD_VALUE);
    u32 privateTimerTicksPerSecond = interrupts_getPrivateTimerTicksPerSecond();
    printf("private timer ticks per second: %ld\n\r", privateTimerTicksPerSecond);
    interrupts_enableTimerGlobalInts();
    // Start the private ARM timer running.
    interrupts_startArmPrivateTimer();
    // Enable interrupts at the ARM.
    interrupts_enableArmInts();
    // The while-loop just waits until the total number of timer ticks have occurred before proceeding.
    while (interrupts_isrInvocationCount() < (TOTAL_SECONDS * privateTimerTicksPerSecond)){
     //   printf("%d ",interrupts_isrInvocationCount());
        ;
    }
    // All done, now disable interrupts and print out the interrupt counts.
    interrupts_disableArmInts();
    printf("isr invocation count: %ld\n\r", interrupts_isrInvocationCount());
    printf("internal interrupt count: %ld\n\r", isr_functionCallCount);
    /*************************************************************************************/
    printf("\nThe main has terminated\n");
    return 0;
}

void isr_function() {
 //   printf("CALLED ISR\n");
    ticTacToeControl_tick();
    isr_functionCallCount++;
}







/*
 * ticTacToeControl.h
 *
 *  Created on: Oct 21, 2015
 *      Author: rdmclean
 */

#ifndef TICTACTOECONTROL_H_
#define TICTACTOECONTROL_H_
/*
 * RYAN McLEAN
 *
 *
 * After sufficient timing with my interval timers, I determined that
 * I needed a period of 100 ms. Running with this period, my code did not lag
 * and I minimized the interrupts that I missed.
 * Worst case periods showed values between 60 and 100
 */
/* This is a state machine function which
 * controls the full game of Tic Tac Toe
 */
void ticTacToeControl_tick();


#endif /* TICTACTOECONTROL_H_ */

/*
 * ticTacToeControl.c
 *
 *  Created on: Oct 21, 2015
 *      Author: rdmclean
 */
#include "ticTacToeControl.h"
#include "ticTacToeDisplay.h"
#include "supportFiles/buttons.h"
#include "miniMax.h"
/*
 * After sufficient timing with my interval timers, I determined that
 * I needed a period of 100 ms. Running with this period, my code did not lag
 * and I minimized the interrupts that I missed.
 * Worst case periods showed values between 60 and 100
 */

#define EXPIRED_VALUE_WAITSETTLE 1    // settle for analog to digital converter > 50ms
#define EXPIRED_VALUE_READINSTRUCT 15 // Time for user to read instructions
#define EXPIRED_VALUE_FIRSTTOUCH 15   // Time for user to play first if desired
#define TICTACTOE_TEXT_SIZE 2         // CHANGE TO BE SPEC VALUE
#define INSTRUCTIONS_1 ("Touch board to play X")
#define INSTRUCTIONS_2 ("       -or-          ")
#define INSTRUCTIONS_3 ("wait for the computer")
#define INSTRUCTIONS_4 ("     and play O.     ")
#define DEFAULT_ROW_VAL 2             // Placed for maximum win without
#define DEFAULT_COL_VAL 2             // time for minimax computation
#define CURSOR_HEIGHT (DISPLAY_HEIGHT / 4) //Set cursor for
#define CURSOR_WIDTH (DISPLAY_WIDTH / 8)   //instructions
#define BUTTON_RESET_VALUE 0x1
uint32_t wait_settle_timer;
uint32_t read_instruction_timer;
uint32_t first_touch_timer;
bool user;      // Will hold X or O based on who touches first
bool opponent;  //Will hold other value based on wait_first_touch state
minimax_board_t board;
uint8_t row, column; // for the row and column on the board

enum TICTACTOECONT_CurStates {      //All the states of my state machine
    TICTACTOECONT_init_st, TICTACTOECONT_print_instruct, TICTACTOECONT_read_instruct,
    TICTACTOECONT_wait_first_touch, TICTACTOECONT_wait_touch, TICTACTOECONT_computer_play,
    TICTACTOECONT_user_release, TICTACTOECONT_wait_settle, TICTACTOECONT_user_play,
    TICTACTOECONT_end_game
}TICTACTOECONT_CurState = TICTACTOECONT_init_st; //Initialize the state to init

/* Tick function controls the Tic Tac Toe Control state machine */
void ticTacToeControl_tick(){
    /*****************************************/
    switch(TICTACTOECONT_CurState){ // For actions and print for debugging
    case TICTACTOECONT_init_st:          // State for initializing
        printf("STATE: init_st\n");
        display_init();                         //Initialize the LCD display
        display_fillScreen(DISPLAY_BLACK);      //Initially fill screen to black
        display_setTextSize(TICTACTOE_TEXT_SIZE);//Set text size for instructions
        display_setTextColor(DISPLAY_WHITE);    //Instructions will print in white
        display_setCursor(CURSOR_HEIGHT, CURSOR_WIDTH);//Set cursor for instructions
        wait_settle_timer = 0;                  //Initialize all of the
        read_instruction_timer = 0;             // state machine's
        first_touch_timer = 0;                  // timers and
        display_clearOldTouchData();            // other values
        break;
    case TICTACTOECONT_print_instruct:  //State to print instructions
        printf("STATE: print_instructions\n");
        display_println(INSTRUCTIONS_1);//Prints first line of instructions
        display_println(INSTRUCTIONS_2);//Second line
        display_println(INSTRUCTIONS_3);//Third line
        display_println(INSTRUCTIONS_4);//Fourth line of instructions
        break;
    case TICTACTOECONT_read_instruct:   //State to wait for user to read
        printf("STATE: read_instruct\n");
        read_instruction_timer++;       //Increment the timer waiting for user to read
        if(read_instruction_timer == EXPIRED_VALUE_READINSTRUCT)
            ticTacToeDisplay_init();    //Call the init when instruction time is finished
        break;
    case TICTACTOECONT_wait_first_touch:    //State to wait for first touch, determines user
        printf("STATE: wait_first_touch\n");// and computer
        first_touch_timer++;                //Increment first touch timer when called
        break;
    case TICTACTOECONT_wait_touch:      //State for waiting for the user to touch the display
        printf("STATE: wait_touch\n");  //No actions needed
        break;
    case TICTACTOECONT_computer_play:    //State for computer to play with minimax
        printf("STATE: computer_play\n");
        minimax_computeNextMove(&board, opponent, &row, &column);//Compute next move using minimax
        if(opponent){
            board.squares[row][column] = MINIMAX_PLAYER_SQUARE;  // Will make square an X in array if user is true
            ticTacToeDisplay_drawX(row, column);                 //Will draw an X on the board
        }
        else{
            board.squares[row][column]= MINIMAX_OPPONENT_SQUARE; //Will make square an O in array if user is false
            ticTacToeDisplay_drawO(row, column);                 //Will draw an O on the LCD display
        }
        printBoard(&board);             //Prints board to console
        break;
    case TICTACTOECONT_user_release:     //State to not do anything until user releases
        printf("STATE: user_release\n"); //No functionality, essentially a wait state
        break;
    case TICTACTOECONT_wait_settle:      //State to wait for analog-digital converter
        printf("STATE: wait_settle\n");  // to settle
        wait_settle_timer++;             //Increments timer, but only needs 1
        break;                           // cycle because of period
    case TICTACTOECONT_user_play:        //State for the user to play
        printf("STATE: user_play\n");    // performs error check of already
        wait_settle_timer = 0;           // full square
        ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column); //Computes row and column of touch
        if(board.squares[row][column] == MINIMAX_EMPTY_SQUARE){           // if not empty square, square doesn't change
            if(user){
                board.squares[row][column] = MINIMAX_PLAYER_SQUARE;//Adds x to array when user
                ticTacToeDisplay_drawX(row, column);               //Draw an X to the board
            }
            else{
                board.squares[row][column]= MINIMAX_OPPONENT_SQUARE;//Adds O to array when !user
                ticTacToeDisplay_drawO(row, column);                //Draw an O to the board
            }
            printBoard(&board);                                     //Prints board to console
            break;
        }
    case TICTACTOECONT_end_game:                                    //Sent here when game has ended
        printf("STATE: end_game\n");
        if(buttons_read() == BUTTON_RESET_VALUE){                   //Button 0 resets game
            wait_settle_timer = 0;                                  //Make sure timers are
            read_instruction_timer = 0;                             // all reset before starting
            first_touch_timer = 0;
            minimax_initBoard(&board);                              //Initialize the board array
            ticTacToeDisplay_init();                                //Initializes the board for restart
            //Transitions back to restart game
            TICTACTOECONT_CurState = TICTACTOECONT_wait_first_touch;
        }
        break;
    }
    /*****************************************/
    switch(TICTACTOECONT_CurState){                                 //Transition switch statement
    case TICTACTOECONT_init_st:                                     //Move on directly to print
        TICTACTOECONT_CurState = TICTACTOECONT_print_instruct;      // instructions after initialization
        break;
    case TICTACTOECONT_print_instruct:                              //After printing move straight to read
        TICTACTOECONT_CurState = TICTACTOECONT_read_instruct;       // instruction state
        break;
    case TICTACTOECONT_read_instruct:                               //After instructions have been on screen for
        if(read_instruction_timer == EXPIRED_VALUE_READINSTRUCT)    //sufficient time, move on
            TICTACTOECONT_CurState = TICTACTOECONT_wait_first_touch;//to wait for first touch state
        break;
    case TICTACTOECONT_wait_first_touch:                            //Waits for an amount of time for user
        if(first_touch_timer == EXPIRED_VALUE_FIRSTTOUCH){          // to choose X if desired
            opponent = true;                                        //Computer plays X first
            user = false;                                           // leaves O for the user
            if(opponent){
                board.squares[DEFAULT_ROW_VAL][DEFAULT_COL_VAL] = MINIMAX_PLAYER_SQUARE;//Puts X in a square
                ticTacToeDisplay_drawX(DEFAULT_ROW_VAL, DEFAULT_COL_VAL);// that he knows he can win to save and
            }                                                            // minimize time spent executing
            else{
                board.squares[DEFAULT_ROW_VAL][DEFAULT_COL_VAL]= MINIMAX_OPPONENT_SQUARE;//Puts O in a square
                ticTacToeDisplay_drawO(DEFAULT_ROW_VAL, DEFAULT_COL_VAL);                // to minimize time
            }
            printBoard(&board);                                 //Prints board to console to show array
            TICTACTOECONT_CurState = TICTACTOECONT_wait_touch;  //Sends to wait touch to begin user's turn
        }
        else if(display_isTouched()){                           //When user decides to be X
            user = true;                                        //User plays first as X
            opponent = false;                                   // leaves O for computer
            TICTACTOECONT_CurState = TICTACTOECONT_user_release;//Send to user release state
        }                                                       // waiting for user to let go before reacting
        break;
    case TICTACTOECONT_wait_touch:
        if(display_isTouched())                                 //When board is touched
            TICTACTOECONT_CurState = TICTACTOECONT_user_release;// send to user release state
        break;                                                  // to wait for user to let go
    case TICTACTOECONT_computer_play:
        if(minimax_isGameOver(minimax_computeBoardScore(&board, opponent))){          //First compute if game is over from score
            printf("Board score is %d\n",minimax_computeBoardScore(&board, opponent));// if yes output the winning score
            TICTACTOECONT_CurState = TICTACTOECONT_end_game;                          // and send to end game state
        }
        else                                                  //Because action was done, computer turn is over.
            TICTACTOECONT_CurState = TICTACTOECONT_wait_touch;//send to wait touch for user turn
        break;
    case TICTACTOECONT_user_release:
        if(!display_isTouched())                               //When the user has released
            TICTACTOECONT_CurState = TICTACTOECONT_wait_settle;//send to wait settle for the Analog
        break;                                                 //to Digital converter
    case TICTACTOECONT_wait_settle:                            //Analog to digital converter state
        if(wait_settle_timer == EXPIRED_VALUE_WAITSETTLE)      //wait for expired value, > 50ms
            TICTACTOECONT_CurState = TICTACTOECONT_user_play;  //send to user play state
        break;
    case TICTACTOECONT_user_play:
        if(minimax_isGameOver(minimax_computeBoardScore(&board, user))){          //First compute if game is over from score
            printf("Board score is %d\n",minimax_computeBoardScore(&board, user));//if yes output the winning score
            TICTACTOECONT_CurState = TICTACTOECONT_end_game;                      // and send to end game state
        }
        else
            TICTACTOECONT_CurState = TICTACTOECONT_computer_play;//user is now done,
        break;                                                   //time for the computer to play
    case TICTACTOECONT_end_game:
        //Transition is done in action switch statement because of timing
        break;
    }
}





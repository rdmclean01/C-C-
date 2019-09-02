#include "miniMax.h"
#include "supportFiles/intervalTimer.h"

//minimax_score_t score = MINIMAX_NOT_ENDGAME;
minimax_move_t choice;
#define NUM_ROWS 2
#define NUM_COLS 2
#define ROW_0 0
#define ROW_1 1
#define ROW_2 2
#define COL_0 0
#define COL_1 1
#define COL_2 2
#define DEFAULT_VALUE 5

/*******Helper function to print off the board for debugging and visual help**********************/
void printBoard(minimax_board_t* board) {
    printf("\n");
    for (int m = 0; m <=NUM_ROWS; m++){      // Goes through all rows
        for (int n = 0; n <=NUM_COLS; n++) { // Goes through all columns
            if (board->squares[m][n] == MINIMAX_OPPONENT_SQUARE)    // Opponent (!player) is an O
                printf("O ");
            else if (board->squares[m][n] == MINIMAX_PLAYER_SQUARE) //Player is an X
                printf("X ");
            else
                printf("_ ");                                       // An empty square has _ placeholder
            if (n == NUM_ROWS)
                printf("\n");                                       // Put a new line at the end of each row
        }
    }
    printf("\n");
}

/* Is a recursive function which computes the best possible move creating a unbeatable game*/
minimax_score_t minimax(minimax_board_t* board, bool player) {
    minimax_score_t score = minimax_computeBoardScore(board, !player); //Gets the score from compute board score
    if (minimax_isGameOver(score)) {                                   //Base case, if game is over
        return score;                                                  // return the score
    }
    minimax_moveScore_pair moveScoreTable[MINIMAX_BOARD_SIZE]={};      //An array of zeros for the table of possible scores
    uint32_t moveScoreTable_Index = 0;                                 //Keeps track of level of recursion
    for (int m = 0; m <= NUM_ROWS; m++) {                            //For all rows
        for (int n = 0; n <= NUM_COLS; n++) {                        //For all columns
            if (board->squares[m][n] == MINIMAX_EMPTY_SQUARE) {      //When square is empty:
                if (player)                                          // Assign the square to player or opponent
                    board->squares[m][n] = MINIMAX_PLAYER_SQUARE;    // Assigns to X for testing
                else
                    board->squares[m][n] = MINIMAX_OPPONENT_SQUARE;  // Or assigns to O for testing
                score = minimax(board, !player);                     // Recursive function to fill up the board
                moveScoreTable[moveScoreTable_Index].score = score;  // adds the score to the table
                moveScoreTable[moveScoreTable_Index].move.row = m;   // adds the coordinates of the move
                moveScoreTable[moveScoreTable_Index].move.column = n;//  to the table
                board->squares[m][n] = MINIMAX_EMPTY_SQUARE;         // Resets the board to an empty square, undoing the test
                moveScoreTable_Index++;                              // Increment the index for the level of recursion
            }
        }
    }
    minimax_score_t tempScore = MINIMAX_DRAW_SCORE;                 //Initializes to a draw score
    tempScore = moveScoreTable[0].score;                            //Initialize tempScore to first position
    choice.column = moveScoreTable[0].move.column;                  //sets choice column to be best column
    choice.row = moveScoreTable[0].move.row;                        //sets choice row to be best row
    if (player) {                                                   // X wants the highest score
        for (uint32_t i = 0; i < moveScoreTable_Index; i++) {       //  Iterates through table
            if (moveScoreTable[i].score > tempScore) {              //  if score is higher
                tempScore = moveScoreTable[i].score;                //  resets tempScore to be highest
                score = moveScoreTable[i].score;                    //  sets score to be highest
                choice.column = moveScoreTable[i].move.column;      //  sets choice column to be best column
                choice.row = moveScoreTable[i].move.row;            //  sets choice row to be best row
            }
        }
    }
    else{                                                           // O wants lowest score
        for (uint32_t i = 0; i < moveScoreTable_Index; i++) {            //  Iterates through table
            if (moveScoreTable[i].score < tempScore) {              //  if score is lower
                tempScore = moveScoreTable[i].score;                //  resets tempScore to be lowest
                score = moveScoreTable[i].score;                    //  sets score to be lowest
                choice.column = moveScoreTable[i].move.column;      //  sets choice column to be best column
                choice.row = moveScoreTable[i].move.row;            //  sets choice row to be best row
            }
        }
    }
    return score;
}
/* This routine is not recursive but will invoke the recursive minimax function.
 *  It computes the row and column of the next move based upon:
 *  the current board,
 *  the player. true means the computer is X. false means the computer is O.*/
void minimax_computeNextMove(minimax_board_t* board, bool player, uint8_t* row, uint8_t* column) {
    double duration0;
    uint32_t timerNumber = 0;
    intervalTimer_start(timerNumber);                                       //Starts the Timer being tested
    minimax(board, player);                                   // Calls minimax to start algorithm, in the end global choice is modified
    intervalTimer_stop(timerNumber);                                        //Stops the timer being Tested
    intervalTimer_getTotalDurationInSeconds(timerNumber, &duration0);       //Gets the total duration in seconds of that Timer
    printf("Time Duration %lu: %6.2e seconds.\n\r",timerNumber, duration0); //Prints off the total duration
    *row = choice.row;                                                      //Changes row move to choice's parameter row
    *column = choice.column;                                                //Puts column move to choice's parameter column
}

/*************** Determine that the game is over by looking at the score.**************/
bool minimax_isGameOver(minimax_score_t score) {
    if (score == MINIMAX_NOT_ENDGAME)
        return false; // Game is not over if score is still NOT_ENDGAME)
    else{
        return true; // If score has changed, GAME IS OVER
    }
}

/******Computes if the board is full *********/
bool board_isFull(minimax_board_t* board) {
    for (int m = 0; m <= NUM_ROWS; m++) {                              // Goes through the entire board
        for (int n = 0; n <= NUM_COLS; n++) {                          // To check if all squares are full
            if (board->squares[m][n] == MINIMAX_EMPTY_SQUARE)   // used to know if game is a draw
                return false;                                   // returns false if any square is empty
        }
    }
    return true;                                                // if none were empty the board is full
}

/* Returns the score of the board, based upon the player and the board.
 *  This returns one of 4 values: MINIMAX_PLAYER_WINNING_SCORE,
 *  MINIMAX_OPPONENT_WINNING_SCORE, MINIMAX_DRAW_SCORE, MINIMAX_NOT_ENDGAME*/
int16_t minimax_computeBoardScore(minimax_board_t* board, bool player) {
    int16_t target;
    minimax_score_t winningScore = MINIMAX_NOT_ENDGAME;
    if (player) {                                   //If player is X, set function variables to his values
        target = MINIMAX_PLAYER_SQUARE;
        winningScore = MINIMAX_PLAYER_WINNING_SCORE;

    }
    else {                                          //If player is O, set function variables to his values
        target = MINIMAX_OPPONENT_SQUARE;
        winningScore = MINIMAX_OPPONENT_WINNING_SCORE;
    }                               //checks top left to bottom right diagonal for a win
    if ((board->squares[ROW_0][COL_0] == target && board->squares[ROW_1][COL_1] == target && board->squares[ROW_2][COL_2] == target) ||
            //checks top right to bottom left diagonal for a win
            (board->squares[ROW_0][COL_2] == target && board->squares[ROW_1][COL_1] == target && board->squares[ROW_2][COL_0] == target)){
        return winningScore;        //Breaks out of the function when found
    }                               //Computes if Row 0 has a win
    else if ((board->squares[ROW_0][COL_0] == target && board->squares[ROW_0][COL_1] == target && board->squares[ROW_0][COL_2] == target) ||
            //Computes if Row 1 has a win
            (board->squares[ROW_1][COL_0] == target && board->squares[ROW_1][COL_1] == target && board->squares[ROW_1][COL_2] == target) ||
            //Computes if Row 2 has a win
            (board->squares[ROW_2][COL_0] == target && board->squares[ROW_2][COL_1] == target && board->squares[ROW_2][COL_2] == target)) {
        return winningScore;        //Breaks out of the function when found
    }                               //Computes if Column 0 has a win
    else if ((board->squares[ROW_0][COL_0] == target && board->squares[ROW_1][COL_0] == target && board->squares[ROW_2][COL_0]==target) ||
            //Computes if Column 1 has a win
            (board->squares[ROW_0][COL_1] == target && board->squares[ROW_1][COL_1] == target && board->squares[ROW_2][COL_1]==target) ||
            //Computes if Column 2 has a win
            (board->squares[ROW_0][COL_2] == target && board->squares[ROW_1][COL_2] == target && board->squares[ROW_2][COL_2]==target)) {
        return winningScore;        //Breaks out of the function when found
    }
    else if (board_isFull(board)) {
        winningScore = MINIMAX_DRAW_SCORE;
    }
    else{
        winningScore = MINIMAX_NOT_ENDGAME;
    }
    return winningScore; //Finally return winning if still in function
}

/**************** Init the board to all empty squares.**********************/
void minimax_initBoard(minimax_board_t* board) {
    for (int m = 0; m <= NUM_ROWS; m++) {
        for (int n = 0; n <= NUM_COLS; n++) {
            board->squares[m][n] = MINIMAX_EMPTY_SQUARE;
        }
    }
    printBoard(board);
}

/**********************A helper function to test the minimax algorithm*************/
void test_Board(minimax_board_t* board, bool player) {
    uint8_t test_Row = DEFAULT_VALUE;
    uint8_t test_Col = DEFAULT_VALUE;
    uint8_t test_Row2 = DEFAULT_VALUE;
    uint8_t test_Col2 = DEFAULT_VALUE;
    printf("Entered test Board\n");
    printf("Enter: row column: ");
    scanf("%u %u", &test_Row, &test_Col);                               //Reads in a row and column from the user
    if (player)
        board->squares[test_Row][test_Col] = MINIMAX_PLAYER_SQUARE;     //Puts an X into board for player
    else
        board->squares[test_Row][test_Col] = MINIMAX_OPPONENT_SQUARE;   //Puts an O into board for !player
    printBoard(board);                                                  //Prints new board to console
    player = !player;
    minimax_computeNextMove(board, player, &test_Row2, &test_Col2);     //Calls Minimax for computer turn
    printf("New move should be %u %u\n", test_Row2, test_Col2);         //Prints out new move
    if (player)
        board->squares[test_Row2][test_Col2] = MINIMAX_PLAYER_SQUARE;   //Adds X based on computed move
    else
        board->squares[test_Row2][test_Col2] = MINIMAX_OPPONENT_SQUARE;// Or adds O based on computed move
    printBoard(board);                                                  //Prints board after computer's move
}

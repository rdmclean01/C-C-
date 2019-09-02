/*
 * simonControl.c

 *
 *  Created on: Nov 9, 2015
 *  author:rdmclean

 */

#include "simonControl.h"


#define CONTROL_SEQUENCELENGTH 50
#define MESSAGE_X (display_height()/4)
#define MESSAGE_Y (display_width()/4)
#define MESSAGE_BIG_TEXT_SIZE 4
#define MESSAGE_TEXT_SIZE 2
#define MESSAGE_GAMENAME ("SIMON")
#define MESSAGE_START ("Touch to Start")
#define CONTROL_TIMEOUT_STR ("You took too long.")
#define CONTROL_USERERROR_STR ("You touched the wrong square.")
#define CONTROL_LEVELCOMPLETE_STR ("Level Complete")
static uint32_t init_duration;
static int16_t ControlSequenceLength;
static int16_t levelCount;
static uint16_t Control_Restart_Timer;
static uint16_t Control_Message_Timer;
#define EXPIREDVALUE_RESTART 150
#define EXPIREDVALUE_LONGEST 75
#define EXPIREDVALUE_CORRECT 25
#define EXPIREDVALUE_ERRORMESSAGE 25
#define CONT_INIT_SEQLENGTH 1
#define CONT_INIT_LEVELCNT 4
#define CONT_SPRINTF_LENGTH 50
#define NUM_SQUARES 4

/**************************************************
 * Helper function to create random array sequence
 **************************************************/
void simonControl_setSequence(uint32_t timer_duration){
    uint8_t sequence[levelCount];     //Allocates a level based sequence array size
    srand(timer_duration);            //Seed will be different each time for pseudo random behavior
    for(int i = 0; i < levelCount;i++)
        sequence[i] = (rand() % NUM_SQUARES); //Fill up sequence with random values
    globals_setSequence(sequence, levelCount);//Modify the global array that everyone can read
}

// Used to select from a variety of informational messages.
enum verifySequence_infoMessage_t {
    user_begin_e,               //A simple message at the beginning
    user_time_out_e,            //Means that the user waited too long to tap a color.
    user_wrong_sequence_e,      //Means that the user tapped the wrong color.
    user_correct_sequence_e,    //Means that the user tapped the correct sequence.
    user_restart_e,             //A message prompting to restart
};

/**Helper function to print off messages as needed during gameplay**/
void simonControl_printInfo(verifySequence_infoMessage_t messageType){
    display_setTextColor(DISPLAY_WHITE);    //Text color should be white
    display_setCursor(MESSAGE_X, MESSAGE_Y);//Put cursor centered in board
    display_fillScreen(DISPLAY_BLACK);      //Fill screen with black before message
    switch(messageType){                    //Based on messageType parameter
    case user_begin_e:
        display_setTextSize(MESSAGE_BIG_TEXT_SIZE);//Game name should be a  bigger
        display_println(MESSAGE_GAMENAME);         // size than other messages
        display_setTextSize(MESSAGE_TEXT_SIZE);    //Reset text size back to normal
        display_println(MESSAGE_START);            // tell user to touch when ready
        break;
    case user_time_out_e:
        display_println(CONTROL_TIMEOUT_STR);      //Tell the user he took too long
        break;
    case user_wrong_sequence_e:
        display_println(CONTROL_USERERROR_STR);    //Tell user he pressed wrong square
        break;
    case user_correct_sequence_e:
        display_println(CONTROL_LEVELCOMPLETE_STR);//Congratulate user for finishing level
        break;
    case user_restart_e:
        display_println(MESSAGE_START);            //Tell user to begin when ready
        break;
    }
}

enum SIMONCONTROL_CurStates{
    SIMONCONTROL_init, SIMONCONTROL_printInstruct,
    SIMONCONTROL_enableFlash, SIMONCONTROL_enableVerify,
    SIMONCONTROL_userRestart, SIMONCONTROL_userDecision,
    SIMONCONTROL_printMessage
} SIMONCONTROL_CurState = SIMONCONTROL_init;

/* ******************************************************************
 * A standard tick function that controls and operates the Simon game
 ********************************************************************/
void simonControl_tick(){
    switch(SIMONCONTROL_CurState){ //State Actions
    case SIMONCONTROL_init:
        printf("SIMONCONTROL: init\n\r");
        init_duration = 0;                              //Inits random seed counter
        Control_Restart_Timer = 0;                      //Inits the restart timer
        ControlSequenceLength = CONT_INIT_SEQLENGTH;    //Sets initial Sequence Length
        levelCount = CONT_INIT_LEVELCNT;                //Initialize the level cnt
        globals_setSequenceIterationLength(CONT_INIT_SEQLENGTH);//Sets a starting sequence length
        simonControl_printInfo(user_begin_e);           //Prints off the starting message
        break;
    case SIMONCONTROL_printInstruct:
        printf("SIMONCONTROL: printInstruct\n");
        init_duration++;                                //Increments a counter used for the random seed
        break;
    case SIMONCONTROL_enableFlash:
        printf("SIMONCONTROL: enableFlash\n");          //No actions required while Flash runs
        break;
    case SIMONCONTROL_enableVerify:
        printf("SIMONCONTROL: enableVerify\n");         //No actions required while Verify runs
        break;
    case SIMONCONTROL_userDecision:
        printf("SIMONCONTROL: userDecision\n");
        Control_Restart_Timer++;                        //Increments the restart timer
        break;
    case SIMONCONTROL_userRestart:
        printf("SIMONCONTROL: userRestart\n");
        init_duration = 0;                              //Inits random seed counter
        Control_Restart_Timer = 0;                      //Inits the restart timer
        ControlSequenceLength = CONT_INIT_SEQLENGTH;    //Sets initial Sequence Length
        globals_setSequenceIterationLength(CONT_INIT_SEQLENGTH);//Sets a starting sequence length
        break;
    case SIMONCONTROL_printMessage:
        Control_Message_Timer++;                        //Makes message readable with a timer
        break;
    default:
        printf("SIMONCONTROL: ERROR: IN DEFAULT STATE\n"); //Tells the user something is wrong
        break;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    switch(SIMONCONTROL_CurState){ //State Transitions
    case SIMONCONTROL_init:
        SIMONCONTROL_CurState = SIMONCONTROL_printInstruct;//Can move straight into state machine
        break;
    case SIMONCONTROL_printInstruct:
        if(display_isTouched()){                                //When display is touched
            simonControl_setSequence(init_duration);            //Sets an array of Random Numbers for sequence
            flashSequence_enable();                             //Enables the flashSequence to begin flashing
            display_fillScreen(DISPLAY_BLACK);                  //Fills screen with black to get rid of message
            SIMONCONTROL_CurState = SIMONCONTROL_enableFlash;
        }
        break;
    case SIMONCONTROL_enableFlash:
        if(flashSequence_completed()){                          //Once flash Sequence has been completed
            flashSequence_disable();                            //Flash Sequence must be disabled
            verifySequence_enable();                            //Enable verify sequence State Machine to move on
            simonDisplay_drawAllButtons();                      //Draw all buttons for user to see
            SIMONCONTROL_CurState = SIMONCONTROL_enableVerify;  //Go to state to wait for user to play his turn
        }
        break;
    case SIMONCONTROL_enableVerify:
        if(verifySequence_isTimeOutError()){                    //If user took too long
            verifySequence_disable();                           //Disable verify sequence for interlock
            simonControl_printInfo(user_time_out_e);            //Tell user he waited too long
            char str[CONT_SPRINTF_LENGTH];                                 //Creates a string to display
            sprintf(str,"Longest Sequence: %d", ControlSequenceLength - 1);// on board telling user how
            display_println(str);                                          // his turn went
            Control_Message_Timer = 0;                           //Timer to make messages readable
            SIMONCONTROL_CurState = SIMONCONTROL_printMessage;   //See if he wants to try again
        }
        else if(verifySequence_isUserInputError()){             //If user pressed the wrong sequence
            verifySequence_disable();                           //Disable for interlock
            simonControl_printInfo(user_wrong_sequence_e);      //Tell user he pressed incorrect square
            char str[CONT_SPRINTF_LENGTH];                                 //Creates a string to display
            sprintf(str,"Longest Sequence: %d", ControlSequenceLength - 1);// on board telling user how
            display_println(str);                                          // his turn went
            Control_Message_Timer = 0;                           //Timer to make messages readable
            SIMONCONTROL_CurState = SIMONCONTROL_printMessage;   //See if he wants to try again
        }
        else if(verifySequence_isComplete()){                       //User completed his turn
            verifySequence_disable();                               //Disable verify for interlock
            if(ControlSequenceLength < globals_getSequenceLength()){        //If level is not finished,
                ControlSequenceLength++;                                    //Increment user turn
                globals_setSequenceIterationLength(ControlSequenceLength);  //Updates the length for turn
                flashSequence_enable();                                     //Enables the flashSequence to flash next turn
                display_fillScreen(DISPLAY_BLACK);                          //Set screen to black for the flash
                SIMONCONTROL_CurState = SIMONCONTROL_enableFlash;           //Send back to start turn
            }
            else if(ControlSequenceLength >= globals_getSequenceLength()){  //Finished sequence and level
                simonControl_printInfo(user_correct_sequence_e);
                SIMONCONTROL_CurState = SIMONCONTROL_userDecision;          //See if he wants to try again
            }
        }
        break;
    case SIMONCONTROL_userDecision:
        if(Control_Restart_Timer == EXPIREDVALUE_LONGEST){  //If user decides not to play again
            char str[CONT_SPRINTF_LENGTH];                  //Create a string telling the user
            sprintf(str,"Longest Sequence: %d", levelCount);//how well he did
            display_println(str);                           //And print it to the board
            Control_Message_Timer = 0;                      //A timer to make message readabble
            SIMONCONTROL_CurState = SIMONCONTROL_printMessage;
        }else if(display_isTouched()){                        //If user decides to play again
            SIMONCONTROL_CurState = SIMONCONTROL_userRestart; //Send him to state to start next level
            levelCount++;                                     //Increment the level, length of sequence
        }
        else if(Control_Restart_Timer == EXPIREDVALUE_CORRECT){//Print off a message telling user
            simonControl_printInfo(user_restart_e);            //to touch if he wants to restart
        }
        break;
    case SIMONCONTROL_userRestart:                             //Once actions are done,
        SIMONCONTROL_CurState = SIMONCONTROL_printInstruct;    //move on to print instructions
        break;
    case SIMONCONTROL_printMessage:
        if (Control_Message_Timer >= EXPIREDVALUE_ERRORMESSAGE) //When message has been printed for sufficient time
            SIMONCONTROL_CurState = SIMONCONTROL_init;          // move back to init to restart the game
    default:
        SIMONCONTROL_CurState = SIMONCONTROL_init; //Send back to init if there is a problem
        break;
    }
}






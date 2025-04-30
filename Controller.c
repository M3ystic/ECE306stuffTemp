/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains the implementation of the autonomous control logic for a robot
 * utilizing the MSP430 microcontroller. The functions in this file manage the movement
 * and control of the robot, including following a black line, making turns, and responding
 * to sensor input. The main tasks are managed through a state machine that transitions
 * between different movement and detection states, such as going forward, turning, and pausing.
 * The file also interfaces with the ADC to detect obstacles and uses timer-based logic
 * for movement control and task sequencing.
 */
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <msp430.h>             // MSP430-specific definitions
#include <string.h>             // Standard string library
#include "include/functions.h"  // Header for utility functions
#include "include/LCD.h"        // Header for LCD interface functions
#include "include/ports.h"      // Header for port and pin definitions
#include "include/macros.h"     // Header for global macros
#include <stdint.h>             // Header for fixed-width integer types
//------------------------------------------------------------------------------
// ADC Variables
//------------------------------------------------------------------------------
extern unsigned int ADC_LEFT_DETECT;  // ADC value for left obstacle detection
extern unsigned int ADC_RIGHT_DETECT; // ADC value for right obstacle detection
//------------------------------------------------------------------------------
// Display Variables
//------------------------------------------------------------------------------
extern volatile char slow_input_down;         // Flag to slow down input handling
extern char display_line[4][11];              // Buffer for display lines
extern char *display[4];                      // Pointers to display lines
extern unsigned char display_mode;            // Display mode selector
extern volatile unsigned char display_changed; // Flag for display changes
extern volatile unsigned char update_display;  // Flag for display updates
extern volatile unsigned int update_display_count; // Counter for display updates
//------------------------------------------------------------------------------
// Movement and Control Variables
//------------------------------------------------------------------------------
int side;                                     // Tracks the side for pivoting
int maxCounter;                               // Maximum counter value for timing movements
unsigned int FinishedFlag = FALSE;            // Indicates when a task is finished
//------------------------------------------------------------------------------
// Task and Timing Variables
//------------------------------------------------------------------------------
extern unsigned int blacklinefollowing;       // Flag for blackline following mode
unsigned int carfollowingLine;                // Flag for car following mode
extern unsigned int leavingCircle;            // Flag for exiting a circular path
extern int counter2;                          // Counter for timing and events
extern unsigned int timechanged;              // Flag for time changes
extern volatile unsigned int updatetimerflag; // Flag for timer updates
//------------------------------------------------------------------------------
// Task State Variables
//------------------------------------------------------------------------------
extern int task2;                             // Task state for autonomous movement
extern int task3;                             // Task state for exiting autonomous mode
//------------------------------------------------------------------------------
// Communication Variables
//------------------------------------------------------------------------------
extern int IOT_RECIEVEDFLAG;                  // Flag for IoT communication status
extern char stored_command[COMMANDSIZE];      // Command buffer for received commands
extern char IOTmodule_rx_buf[BUFFERSIZE];     // IoT module receive buffer
//------------------------------------------------------------------------------
// Turn Control Variables
//------------------------------------------------------------------------------
extern unsigned int turnFlag;                 // Flag for direction of turns
//------------------------------------------------------------------------------
// Pre-defined Display Messages
//------------------------------------------------------------------------------
char BL_Start[LINE_LEN] = " BL Start ";       // Indicates the start of blackline following
char Intercept[LINE_LEN] = "Intercept!";      // Indicates obstacle detection
char BL_Turn[LINE_LEN] = " BL Turn! ";        // Indicates a turning action
char BL_Travel[LINE_LEN] = "BL Travel!";      // Indicates traveling along the blackline
char BL_Circle[LINE_LEN] = "BL Circle!";      // Indicates following a circular path
char BL_Exit[LINE_LEN] = " BL Exit! ";        // Indicates exiting the blackline


//------------------------------------------------------------------------------
// Autonomous Control Functions
//------------------------------------------------------------------------------

/**
 * Function: go_autonomous
 */
void go_autonomous(void) {
    switch (task2) {
        case 0: // Turn based on the turnFlag
            maxCounter = (turnFlag == 1) ? 30 : 25; // Set counter limits for right or left turns , 6 or 5 seconds
            if (counter2 < maxCounter) {
                if (turnFlag == 1) {
                    rightboard_turn(); // Right turn
                } else if (turnFlag == 2) {
                    leftboard_turn(); // Left turn
                }
            } else {
                task2++; // Proceed to the next task
                counter2 = RESET_STATE; // Reset counter
            }
            break;

        case 1: // Continue turning with finer adjustments
            maxCounter = (turnFlag == 1) ? 10 : 20; // Adjust counter limits , 1 or 2 seconds
            if (counter2 < maxCounter) {
                if (turnFlag == 1) {
                    rightboard_turn2(); // Fine right turn
                } else if (turnFlag == 2) {
                    leftboard_turn(); // Fine left turn
                }
            } else {
                task2++;
                counter2 = RESET_STATE;
            }
            break;

        case 2: // Move forward until detecting an obstacle
            strcpy(display_line[0], " Straight ");
            straight(); // Move forward
            if (ADC_LEFT_DETECT > (MIDDLELINE - 100) || ADC_RIGHT_DETECT > (MIDDLELINE - 100)) {
                strncpy(display_line[0], Intercept, LINE_WIDTH);
                stop(); // Stop on black line detection
                task2++;
                counter2 = RESET_STATE;
            }
            break;

        case 3: // Pause for 2 seconds
            if (counter2 < 10) {
                stop();
            } else {
                task2++;
                counter2 = RESET_STATE;
            }
            break;

        case 4: // Turn based on detected conditions
            strncpy(display_line[0], BL_Turn, LINE_WIDTH);
            if (turnFlag == 1) {
                turn_left(); // Left turn
            } else if (turnFlag == 2) {
                turn_right(); // Right turn
            }
            if (ADC_LEFT_DETECT > MIDDLELINE || ADC_RIGHT_DETECT > MIDDLELINE) {
                stop();
                task2++;
                counter2 = RESET_STATE;
            }
            break;

        case 5: // Idle state before continuing
            if (counter2 < 50)  // 10 seconds
            {
                stop();
            } else {
                strncpy(display_line[0], BL_Travel, LINE_WIDTH);
                task2++;
                counter2 = RESET_STATE;
            }
            break;

        case 6: // Follow a circular path
            while (ALWAYS) {
                Display_Process(); // Update the display
                if (counter2 > 30) {
                    counter2 = RESET_STATE;
                    strncpy(display_line[0], BL_Circle, LINE_WIDTH);
                    while (counter2 < 50) {
                        stop();
                    }
                    counter2 = RESET_STATE;
                    TB1CCTL2 &= ~CCIE;
                }

                if (IOT_RECIEVEDFLAG == ENABLED) {
                    parse_and_execute_commands(IOTmodule_rx_buf);
                    strncpy(display_line[3], stored_command, 5);
                }

                timer_update(timechanged);
                followblackline();

                if (leavingCircle) {
                    P1OUT ^= RED_LED;
                    blacklinefollowing = FALSE;
                    stop();
                    break;
                }
            }
            break;

        default: // Reset state machine
            task2 = 0;
            counter2 = 0;
            stop();
            strcpy(display_line[3], "   ERROR  ");
            break;
    }
}

/**
 * Function: end_autonomous
 * -------------------------
 * Ends the autonomous mode by turning and moving straight before stopping.
 */
void end_autonomous(void) {
    switch (task3) {
        case 0: // Perform a turn to exit the circle
            strncpy(display_line[0], BL_Exit, LINE_WIDTH);
            if (counter2 < 4) {
                if (turnFlag == 1) {
                    turn_right();
                } else if (turnFlag == 2) {
                    turn_left();
                }
            } else {
                stop();
                counter2 = RESET_STATE;
                task3 = 1;
                display_changed = TRUE;
            }
            break;

        case 1: // Move straight before stopping
            if (counter2 < 15) {
                straight();
            } else {
                stop();
                task3 = RESET_STATE;
                FinishedFlag = TRUE;
                display_changed = TRUE;
                TB1CCTL2 &= ~CCIE;
                leavingCircle = FALSE;
            }
            break;

        default: // Reset to initial state
            task3 = RESET_STATE;
            break;
    }
}

/**
 * Function: followblackline
 * --------------------------
 * Follows a black line using ADC values to adjust movement.
 * Adjusts left, right, or pivots based on sensor inputs.
 */
void followblackline(void) {
    if (fabs(ADC_LEFT_DETECT - ADC_RIGHT_DETECT) > HYSTERESIS) {
        side = (ADC_LEFT_DETECT > ADC_RIGHT_DETECT) ? 0 : 1;
    }
    RIGHT_FORWARD_SPEED = RIGHTSLOW;
    LEFT_FORWARD_SPEED = LEFTSLOW;
    if ((ADC_RIGHT_DETECT > MIDDLELINE) && (ADC_LEFT_DETECT > MIDDLELINE)) {
        P6OUT |= GRN_LED;
        P1OUT &= ~RED_LED;
        if (ADC_RIGHT_DETECT > ADC_LEFT_DETECT) {
            left_forward();
        } else if (ADC_LEFT_DETECT > ADC_RIGHT_DETECT) {
            right_forward();
        } else {
            forwards();
        }
    } else if ((ADC_RIGHT_DETECT > MIDDLELINE) && (ADC_LEFT_DETECT < MIDDLELINE)) {
        P6OUT &= ~GRN_LED;
        P1OUT |= RED_LED;
        left_forward();
        side = TRUE;
    } else if ((ADC_RIGHT_DETECT < MIDDLELINE) && (ADC_LEFT_DETECT > MIDDLELINE)) {
        P6OUT &= ~GRN_LED;
        P1OUT |= RED_LED;
        right_forward();
        side = FALSE;
    } else {
        if (side == TRUE) {
            clockwise();
        } else if (side == FALSE) {
            counterclockwise();
        }
        while (ADC_LEFT_DETECT < MIDDLELINE || ADC_RIGHT_DETECT < MIDDLELINE);
        stop();
    }
}

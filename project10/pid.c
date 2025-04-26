/*
 * pid.c
 *
 *  Created on: Mar 3, 2025
 *      Author: Dtwen
 */

#include <msp430.h>
#include <string.h>
#include "include/functions.h"
#include "include/LCD.h"
#include "include/ports.h"
#include "include/macros.h"
//#include "include\pid.h"
#include <stdint.h>

extern unsigned int ADC_LEFT_DETECT;
extern unsigned int ADC_RIGHT_DETECT;
extern volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
extern unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;


int side;
int maxCounter;

extern unsigned int blacklinefollowing;
unsigned int carfollowingLine;
extern unsigned int leavingCircle;
extern int counter2;
extern unsigned int timechanged;
extern volatile unsigned int updatetimerflag; // Flag for timer update


extern int task2;
extern int task3;

unsigned int FinishedFlag = FALSE;
extern int IOT_RECIEVEDFLAG;
extern unsigned int turnFlag;



char BL_Start[11] = " BL Start ";   // "BL Start" fits within 10 characters, null terminator added.
char Intercept[11] = "Intercept!"; // "Intercept" fits within 10 characters, null terminator added.
char BL_Turn[11] = " BL Turn! ";     // "BL Turn" fits within 10 characters, null terminator added.
char BL_Travel[11] = "BL Travel!"; // "BL Travel" fits within 10 characters, null terminator added.
char BL_Circle[11] = "BL Circle!"; // "BL Circle" fits within 10 characters, null terminator added.
char BL_Exit[11] = " BL Exit! ";     // "BL Exit" fits within 10 characters, null terminator added.
extern char stored_command[6];
extern char iot_rx_buf [128];





 void go_autonomous(void){

    switch (task2) {
    case 0: // Go forward for a specified duration
        // Determine the maximum counter value based on the turnFlag
         maxCounter = (turnFlag == 1) ? 30 : 25; // 35 for right, 25 for left

        if (counter2 < maxCounter) {
            // Check the value of the turnFlag
            if (turnFlag == 1) {
                // Turn right
                rightboard_turn();
            } else if (turnFlag == 2) {
                // Turn left
                leftboard_turn();
            }
        } else {
            task2++; // Move to the next task
            counter2 = 0; // Reset the counter
        }
        break;


    case 1: // Go forward for a specified duration
        // Determine the maximum counter value based on the turnFlag
        maxCounter = (turnFlag == 1) ? 10 : 20; // 10 for right, 20 for left

        if (counter2 < maxCounter) {
            // Check the turnFlag to decide whether to turn right or left
            if (turnFlag == 1) {
                rightboard_turn2(); // Turn right if the flag is 1
            } else if (turnFlag == 2) {
                leftboard_turn();  // Turn left if the flag is 2
            }
        } else {
            task2++; // Move to the next task
            counter2 = 0; // Reset the counter
        }
        break;

        case 2: // Forward till detection
            strcpy(display_line[0], "Straight");
               straight();
               if (ADC_LEFT_DETECT > (MIDDLELINE-100) || ADC_RIGHT_DETECT > (MIDDLELINE-100)) { // Update thresholds if necessary
                   strncpy(display_line[0], Intercept ,10);
                   stop();
                   task2++;
                   counter2 = 0;
               }
               break;

        case 3: // Pause for 3 seconds
            if (counter2 < 10) { // Adjust the counter threshold for timing
                       stop();
            } else {
                task2++;
                counter2 = 0;
            }
            break;

        case 4: // Turn action with counter
            strncpy(display_line[0], BL_Turn ,10);

            // Check the turnFlag to decide whether to turn left or right
            if (turnFlag == 1) {
                turn_left(); // Turn right if the flag is 1
            } else if (turnFlag == 2) {
                turn_right();  // Turn left if the flag is 2
            }

            // Check ADC values to stop when an obstacle is detected or other criteria are met
            if (ADC_LEFT_DETECT > MIDDLELINE || ADC_RIGHT_DETECT > MIDDLELINE) {
                stop(); // Stop when the condition is met
                task2++; // Move to the next task
                counter2 = 0; // Reset the counter
            }
            break;


        case 5: // Initial idle state
            if (counter2 < 50) { // Adjust the counter threshold for timing
                stop();
            } else {
                strncpy(display_line[0], BL_Travel ,10);
                task2++;
                counter2 = 0;
            }
            break;

        case 6: // Follow the circle

            while (ALWAYS) {
                Display_Process();                 // Update Display
                if (counter2 > 30) { // Adjust the counter threshold for timing
                    counter2 = 0;
                    strncpy(display_line[0], BL_Circle ,10);

                    while (counter2 < 50) {
                        stop();

                    }
                    counter2 = 0;
                    TB1CCTL2 &= ~CCIE;
                }

                if (IOT_RECIEVEDFLAG == ENABLED) {
                    parse_and_execute_commands(iot_rx_buf);
                    strncpy(display_line[3], stored_command,5);
                }

                // timer_update(timechanged);


                followblackline();

                if (leavingCircle) {
                    P1OUT ^= RED_LED; // Initial Value = Low / Off
                    blacklinefollowing = FALSE;
                    stop();
                    break;
                }

            }

            break;

        default: // Reset state machine if needed
            task2 = 0;
            counter2 = 0;
               stop();
            strcpy(display_line[3], "   ERROR  ");
            break;
    }


 }



void end_autonomous(void){

                switch (task3) {
                case 0: // Turning left
                    strncpy(display_line[0], BL_Exit ,10);
                    if (counter2 < 4) { // Assuming 200ms intervals; 1 second = 5 cycles
                        // Use the turnFlag to determine the turn direction
                        if (turnFlag == 1) {
                            turn_right(); // Turn right if the flag is 1
                        } else if (turnFlag == 2) {
                            turn_left(); // Turn left if the flag is 2
                        }
                    } else {
                        // After 1 second, stop turning
                        stop();
                        counter2 = 0; // Reset the timer
                        task3 = 1; // Move to the straight state
                        display_changed = TRUE;
                    }
                    break;

                case 1: // Going straight
                    if (counter2 < 15) { // Assuming 200ms intervals; 3 seconds = 15 cycles
                        straight();
                    } else {
                        // After 3 seconds, stop and reset
                        stop();
                        task3 = 0; // Reset task3 to allow repeating if needed
                        FinishedFlag = TRUE;
                        display_changed = TRUE;
                        TB1CCTL2 &= ~CCIE;
                        leavingCircle = FALSE;
                    }
                    break;

                default: // Default case to handle unexpected values
                    task3 = 0; // Reset to the initial state
                    break;
                }


            }




void followblackline(void) {

    if (fabs(ADC_LEFT_DETECT - ADC_RIGHT_DETECT) > HYSTERESIS) {
        side = (ADC_LEFT_DETECT > ADC_RIGHT_DETECT) ? 0 : 1;
    }
    RIGHT_FORWARD_SPEED = RIGHTSLOW; // P6.1 Right Forward PWM duty cycle  //130000
    LEFT_FORWARD_SPEED = LEFTSLOW;   // P6.2 Left Forward PWM duty cycle
    if ((ADC_RIGHT_DETECT > MIDDLELINE) && (ADC_LEFT_DETECT > MIDDLELINE)) {
        P6OUT |= GRN_LED;
        P1OUT &= ~RED_LED;
        if (ADC_RIGHT_DETECT > ADC_LEFT_DETECT) {
            left_forward();
        } else if (ADC_LEFT_DETECT > ADC_RIGHT_DETECT) {
            // Move right forward
            right_forward();
        } else {
            forwards();

        }
    } else if ((ADC_RIGHT_DETECT > MIDDLELINE) && (ADC_LEFT_DETECT < MIDDLELINE )) {
        P6OUT &= ~GRN_LED;
        P1OUT |= RED_LED;
        // Right sensor detects white, turn left
        left_forward();
        side = 1;
    } else if ((ADC_RIGHT_DETECT < MIDDLELINE ) && (ADC_LEFT_DETECT > MIDDLELINE)) {
        P6OUT &= ~GRN_LED;
        P1OUT |= RED_LED;
        // Left sensor detects white, turn right
        right_forward();
        side = 0;
    } else {
        // Both sensors detect white, pivot based on the last direction
        if (side == 0) {
            // Turn clockwise
            clockwise();
        } else if (side == 1) {
            // Turn counterclockwise
            counterclockwise();
        }

        // Wait until both sensors detect black
        while (ADC_LEFT_DETECT < MIDDLELINE || ADC_RIGHT_DETECT < MIDDLELINE);
        // Stop movement after the correction
        stop();
    }


}



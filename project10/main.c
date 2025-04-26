/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains the main routine for the ECE306 Project 10 program. It serves
 * as the entry point for the application and handles the core logic for initializing
 * hardware, processing inputs and outputs, and managing the operating system loop.
 * The main function initializes necessary components, displays messages, handles
 * various tasks, and responds to sensor inputs and commands. It also manages
 * autonomous behaviors, such as line-following and autonomous task completion.
 * The program ensures continuous operation through an operating system loop,
 * performing periodic updates and checks on system states.
 */

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <msp430.h>
#include <string.h>
#include "include/functions.h"
#include "include/LCD.h"
#include "include/ports.h"
#include "include/macros.h"
//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Display Variables
//------------------------------------------------------------------------------
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern unsigned char display_mode;
//------------------------------------------------------------------------------
// Timing and Sequence Variables
//------------------------------------------------------------------------------
extern unsigned int timechanged;
extern unsigned int updatetimerflag;
//------------------------------------------------------------------------------
// Task and Movement Variables
//------------------------------------------------------------------------------
unsigned int moving;
//------------------------------------------------------------------------------
// Sensor and Control Variables
//------------------------------------------------------------------------------
extern char adc_char3[4];
//------------------------------------------------------------------------------
// Stored IP
//------------------------------------------------------------------------------
extern char stored_ip[16];
// Autonomous Behavior Variables
//------------------------------------------------------------------------------
extern unsigned int blacklinefollowing;
extern unsigned int leavingCircle;
extern unsigned int FinishedFlag;
//------------------------------------------------------------------------------
// Miscellaneous Variables
//------------------------------------------------------------------------------
extern char BL_Stop[LINE_LEN] ;
//------------------------------------------------------------------------------
// Main Program - see system.c for function definitions
//------------------------------------------------------------------------------
void main(void) {
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO high-impedance mode

    initialize_hardware();

    // Display initial message
    strcpy(display_line[0], " Waiting  ");
    strcpy(display_line[1], "for input ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;

    //------------------------------------------------------------------------------
    // Operating System Loop
    //------------------------------------------------------------------------------
    while (ALWAYS) {
        Display_Process(); // Update display
        handle_switches();
        handle_received_data();
        handle_commands();
        handle_tasks();
        handle_sending_commands();

        // Update timer and handle movement
        if (updatetimerflag == TRUE) {
            timer_update(timechanged);
            if (moving) {
                P6OUT |= LCD_BACKLITE; // Turn on LCD backlight
                strcpy(display_line[0], "Arrived 0X");
                changedIPlines(stored_ip);
                moving = FALSE;
            }
        }

        // Autonomous behavior
        if (blacklinefollowing == TRUE) {
            go_autonomous();
        }
        if (leavingCircle) {
            end_autonomous();
        }

        // Handle finished flag
        if (FinishedFlag == TRUE) {
            strncpy(display_line[0], BL_Stop, LINE_WIDTH);
            strcpy(display_line[1], "Light Work");
            strcpy(display_line[2], "NoReaction");

            display_line[3][0] = 'T';
            display_line[3][1] = 'i';
            display_line[3][2] = 'm';
            display_line[3][3] = 'e';
            display_line[3][4] = ':';

            display_line[3][6] = adc_char3[0];
            display_line[3][7] = adc_char3[1];
            display_line[3][8] = adc_char3[2];
            display_line[3][9] = 's';
        }
    }
}

/*
 * movement.c
 *
 *  Created on: Feb 13, 2025
 *      Author: Dtwen
 */


#include "include/ports.h"
#include "include/functions.h"
#include "include/macros.h"
#include "msp430.h"
#include "include\LCD.h"
#include <string.h>

extern unsigned int circleturned;
extern char event;
 char state = 'W';
extern char event;
unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
unsigned int cycle_time; // is a new time base used to control making shapes
unsigned int time_change; // is an identifier that a change has occurred


extern volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
extern unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
unsigned int firstTime = 1;

extern unsigned int  goFlag;
unsigned int turn_phase;
unsigned int tracker;
unsigned int tCount;
unsigned int fCount;
unsigned int ttime;
unsigned int Time_Sequence;
unsigned int cCount;




// Define missing variables
int delay_start = 0;
int right_motor_count = 0;
int left_motor_count = 0;
int segment_count = 0;

// FSM dispatcher



void straight(void){

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle


    RIGHT_FORWARD_SPEED = RIGHTFAST; // P6.1 Right Forward PWM duty cycle  //130000
    LEFT_FORWARD_SPEED = LEFTFAST; // P6.2 Left Forward PWM duty cycle


}

void turn(void){
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle


        LEFT_FORWARD_SPEED = MAX_SPEED; // P6.1 Right Forward PWM duty cycle
        RIGHT_REVERSE_SPEED = MAX_SPEED; // P6.4 Left Reverse PWM duty cycle //4500



}















void moveCircle(void) {
    switch (state) {
        case WAIT: // Begin
            wait_case();
            break;
        case START: // Begin
            start_case();
            break;
        case RUN: // Run
            circlerun_case();
            break;
        case END: // End
            end_case();
            break;
        default:
            break;
    }
}

void moveTriangle(void) {
    switch (state) {
        case WAIT: // Begin
            wait_case();
            break;
        case START: // Begin
            start_case();
            break;
        case RUN: // Run
            trianglerun_case();
            break;
        case END: // End
            end_case();
            break;
        default:
            break;
    }
}

void moveFigure8(void) {
    switch (state) {
        case WAIT: // Begin
            wait_case();
            break;
        case START: // Begin
            start_case();
            break;
        case RUN: // Run
            f8run_case();
            break;
        case END: // End
            end_case();
            break;
        default:
            break;
    }
}

// Case functions
void wait_case(void) {
    if (time_change) {
        time_change = 0;
        if (delay_start++ >= WAITING2START) {
            delay_start = 0;
            state = START;
        }
    }



}

void start_case(void) {
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    motors_forward();
    segment_count = 0;
    state = RUN;
}


void end_case(void) {
    motors_off();
    state = WAIT;
    event = NONE;
}

void circlerun_case(void) {
//    if (time_change) {
//        time_change = 0;
//        if (segment_count <= circleTRAVEL_DISTANCE) {
//            if (right_motor_count++ >= circleRIGHT_COUNT_TIME) {
//                P6OUT &= ~R_FORWARD;
//            }
//            if (left_motor_count++ >= circleLEFT_COUNT_TIME) {
//                P6OUT &= ~L_FORWARD;
//            }
//            if (cycle_time >= circleWHEEL_COUNT_TIME) {
//                cycle_time = 0;
//                right_motor_count = 0;
//                left_motor_count = 0;
//                segment_count++;
//                motors_forward();
//            }
//        } else {
//            state = END;
//        }
//    }
}

      inline void pj5forward(void){




            strcpy(display_line[0], "           ");
            strcpy(display_line[1], "           ");
            strcpy(display_line[2], "  Forward  ");
            strcpy(display_line[3], "           ");



        }
      inline void pj5reverse(void){


            motors_reverse();

            strcpy(display_line[0], "           ");
            strcpy(display_line[1], "  Reverse  ");
            strcpy(display_line[2], "           ");
            strcpy(display_line[3], "           ");


        }
      inline void pj6pause(void){


            motors_off();

            strcpy(display_line[0], "           ");
            strcpy(display_line[1], "           ");
            strcpy(display_line[2], "   Pause   ");
            strcpy(display_line[3], "           ");


        }
       inline void clockwise(void)
        {


               P1OUT &= ~RED_LED; // Set Red LED off
               P6OUT |= GRN_LED; // set green led on

               P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
               P6OUT |= R_REVERSE; // Set Port pin High [Wheel On]

               strcpy(display_line[0], "           ");
               strcpy(display_line[1], " Clockwise ");
               strcpy(display_line[2], "           ");
               strcpy(display_line[3], "           ");

        }
       inline void counterclockwise(void)
        {



            P6DIR &= ~GRN_LED; // Set Green LED off
            P1OUT |= RED_LED; // Set Red LED On

            P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]
            P6OUT |= L_REVERSE; // Set Port pin High [Wheel On]

            strcpy(display_line[0], "           ");
            strcpy(display_line[1], "           ");
            strcpy(display_line[2], " CClockwise");
            strcpy(display_line[3], "           ");


        }

       void stop_motors(void){
           LEFT_FORWARD_SPEED = WHEEL_OFF;
           RIGHT_FORWARD_SPEED = WHEEL_OFF;
           RIGHT_REVERSE_SPEED = WHEEL_OFF;
           LEFT_REVERSE_SPEED = WHEEL_OFF;



       }




void trianglerun_case(void) {
    if(time_change){
           motors_forward();

           } else {
               state = END;
           }

}
void f8run_case(void) {
    if (time_change) {

        motors_forward();

        } else {
            state = END;
        }

}











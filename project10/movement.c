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
    RIGHT_FORWARD_SPEED = RIGHTFAST - 3000 ; // P6.1 Right Forward PWM duty cycle  //130000
    LEFT_FORWARD_SPEED = LEFTFAST; // P6.2 Left Forward PWM duty cycle


}

void ControlForward(void)
{

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
    RIGHT_FORWARD_SPEED = RIGHTFAST - 1000 ; // P6.1 Right Forward PWM duty cycle  //130000
    LEFT_FORWARD_SPEED = LEFTFAST; // P6.2 Left Forward PWM duty cycle
}

void forwards(void){

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
    RIGHT_FORWARD_SPEED = RIGHTFAST; // P6.1 Right Forward PWM duty cycle  //130000
    LEFT_FORWARD_SPEED = LEFTFAST; // P6.2 Left Forward PWM duty cycle

}

void backwards(void){
     LEFT_FORWARD_SPEED = WHEEL_OFF;
     RIGHT_FORWARD_SPEED = WHEEL_OFF;
     RIGHT_REVERSE_SPEED = RIGHTFAST - 4500;
     LEFT_REVERSE_SPEED = LEFTFAST;
}

void stop() {
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void turn_right() {
    LEFT_FORWARD_SPEED = 32000;
    RIGHT_REVERSE_SPEED = 20000;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void turn_left() {
    LEFT_REVERSE_SPEED = 32000;
    RIGHT_FORWARD_SPEED = 20000;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;

}

void Cturn_right() {
    LEFT_FORWARD_SPEED = LEFTSLOW;
    RIGHT_REVERSE_SPEED = RIGHTSLOW;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void Cturn_left() {
    LEFT_REVERSE_SPEED = LEFTSLOW;
    RIGHT_FORWARD_SPEED = RIGHTSLOW;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;

}
void left_forward() {
    LEFT_FORWARD_SPEED = LEFTFAST;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;

}
void right_forward() {
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = RIGHTFAST;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}

void clockwise() {
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = RIGHTFAST;
    LEFT_REVERSE_SPEED = LEFTFAST;    // Reverse left to spin clockwise
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // No reverse on the right
}

void counterclockwise() {
    LEFT_FORWARD_SPEED = LEFTFAST;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;  // No reverse on the left
    RIGHT_REVERSE_SPEED = RIGHTFAST; // Reverse right to spin counterclockwise
}


void leftboard_turn(){
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = RIGHTFAST;
    LEFT_FORWARD_SPEED = 20000;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;

}
void rightboard_turn(void) {
    static int pulse_counter = 0;  // Tracks the current pulse state
    int on_time_ticks = 9;         // On for 1 second (5 × 200ms)
    int off_time_ticks = 10;        // Off for 1 second (5 × 200ms)
    int total_pulse_ticks = on_time_ticks + off_time_ticks;

    if (pulse_counter < on_time_ticks) {
        // Wheels active during the on-time
        LEFT_REVERSE_SPEED = WHEEL_OFF;
        RIGHT_FORWARD_SPEED = 35000;  // Minimum allowed speed
        LEFT_FORWARD_SPEED = LEFTFAST;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
    } else {
        // Wheels inactive during the off-time
        LEFT_REVERSE_SPEED = WHEEL_OFF;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;  // Disable the right wheel
        LEFT_FORWARD_SPEED = LEFTFAST;   // Keep the left wheel turning
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
    }

    // Increment and reset the pulse counter as needed
    pulse_counter++;
    if (pulse_counter >= total_pulse_ticks) {
        pulse_counter = 0;  // Reset pulse counter after a full cycle
    }
}
void rightboard_turn2(void) {
    static int pulse_counter = 0;  // Tracks the current pulse state
    int on_time_ticks = 6;         // On for 1 second (5 × 200ms)
    int off_time_ticks = 12;        // Off for 1 second (5 × 200ms)
    int total_pulse_ticks = on_time_ticks + off_time_ticks;

    if (pulse_counter < on_time_ticks) {
        // Wheels active during the on-time
        LEFT_REVERSE_SPEED = WHEEL_OFF;
        RIGHT_FORWARD_SPEED = 30000;  // Minimum allowed speed
        LEFT_FORWARD_SPEED = LEFTFAST;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
    } else {
        // Wheels inactive during the off-time
        LEFT_REVERSE_SPEED = WHEEL_OFF;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;  // Disable the right wheel
        LEFT_FORWARD_SPEED = LEFTFAST;   // Keep the left wheel turning
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
    }

    // Increment and reset the pulse counter as needed
    pulse_counter++;
    if (pulse_counter >= total_pulse_ticks) {
        pulse_counter = 0;  // Reset pulse counter after a full cycle
    }
}


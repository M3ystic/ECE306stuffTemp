/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains functions to control motor movements, including forward,
 * backward, turning, and rotational movements.
 */

 #include "include/ports.h"
 #include "include/functions.h"
 #include "include/macros.h"
 #include "msp430.h"
 #include "include/LCD.h"
 
 // External variables
 extern volatile char slow_input_down;
 extern char display_line[4][11];
 extern char *display[4];
 extern unsigned char display_mode;
 extern volatile unsigned char display_changed;
 extern volatile unsigned char update_display;
 extern volatile unsigned int update_display_count;
 extern unsigned int goFlag;
 
 // Internal variables
 static unsigned int turn_phase;
 static unsigned int tracker;
 
 /*
  * Function Definitions
  */
 
 // Move forward in a straight line
 void straight(void) {
     RIGHT_REVERSE_SPEED = WHEEL_OFF;
     LEFT_REVERSE_SPEED = WHEEL_OFF;
     RIGHT_FORWARD_SPEED = RIGHTFAST - 3000; // Adjusted speed for balance
     LEFT_FORWARD_SPEED = LEFTFAST;
 }
 
 // Controlled forward movement for precise adjustments
 void ControlForward(void) {
     RIGHT_REVERSE_SPEED = WHEEL_OFF;
     LEFT_REVERSE_SPEED = WHEEL_OFF;
     RIGHT_FORWARD_SPEED = RIGHTFAST - 1000;
     LEFT_FORWARD_SPEED = LEFTFAST;
 }
 
 // Full-speed forward movement
 void forwards(void) {
     RIGHT_REVERSE_SPEED = WHEEL_OFF;
     LEFT_REVERSE_SPEED = WHEEL_OFF;
     RIGHT_FORWARD_SPEED = RIGHTFAST;
     LEFT_FORWARD_SPEED = LEFTFAST;
 }
 
 // Move backward
 void backwards(void) {
     LEFT_FORWARD_SPEED = WHEEL_OFF;
     RIGHT_FORWARD_SPEED = WHEEL_OFF;
     RIGHT_REVERSE_SPEED = RIGHTFAST - 4500;
     LEFT_REVERSE_SPEED = LEFTFAST;
 }
 
 // Stop all movements
 void stop(void) {
     RIGHT_FORWARD_SPEED = WHEEL_OFF;
     LEFT_FORWARD_SPEED = WHEEL_OFF;
     RIGHT_REVERSE_SPEED = WHEEL_OFF;
     LEFT_REVERSE_SPEED = WHEEL_OFF;
 }
 
 // Perform a right turn
 void turn_right(void) {
     LEFT_FORWARD_SPEED = 32000;
     RIGHT_REVERSE_SPEED = 20000;
     RIGHT_FORWARD_SPEED = WHEEL_OFF;
     LEFT_REVERSE_SPEED = WHEEL_OFF;
 }
 
 // Perform a left turn
 void turn_left(void) {
     LEFT_REVERSE_SPEED = 32000;
     RIGHT_FORWARD_SPEED = 20000;
     LEFT_FORWARD_SPEED = WHEEL_OFF;
     RIGHT_REVERSE_SPEED = WHEEL_OFF;
 }
 
 // Perform a controlled right turn
 void Cturn_right(void) {
     LEFT_FORWARD_SPEED = LEFTSLOW;
     RIGHT_REVERSE_SPEED = RIGHTSLOW;
     RIGHT_FORWARD_SPEED = WHEEL_OFF;
     LEFT_REVERSE_SPEED = WHEEL_OFF;
 }
 
 // Perform a controlled left turn
 void Cturn_left(void) {
     LEFT_REVERSE_SPEED = LEFTSLOW;
     RIGHT_FORWARD_SPEED = RIGHTSLOW;
     LEFT_FORWARD_SPEED = WHEEL_OFF;
     RIGHT_REVERSE_SPEED = WHEEL_OFF;
 }
 
 // Forward movement with the left motor
 void left_forward(void) {
     LEFT_FORWARD_SPEED = LEFTFAST;
     RIGHT_REVERSE_SPEED = WHEEL_OFF;
     RIGHT_FORWARD_SPEED = WHEEL_OFF;
     LEFT_REVERSE_SPEED = WHEEL_OFF;
 }
 
 // Forward movement with the right motor
 void right_forward(void) {
     LEFT_REVERSE_SPEED = WHEEL_OFF;
     RIGHT_FORWARD_SPEED = RIGHTFAST;
     LEFT_FORWARD_SPEED = WHEEL_OFF;
     RIGHT_REVERSE_SPEED = WHEEL_OFF;
 }
 
 // Rotate clockwise
 void clockwise(void) {
     LEFT_FORWARD_SPEED = WHEEL_OFF;
     RIGHT_FORWARD_SPEED = RIGHTFAST;
     LEFT_REVERSE_SPEED = LEFTFAST;  // Reverse left to spin clockwise
     RIGHT_REVERSE_SPEED = WHEEL_OFF;
 }
 
 // Rotate counterclockwise
 void counterclockwise(void) {
     LEFT_FORWARD_SPEED = LEFTFAST;
     RIGHT_FORWARD_SPEED = WHEEL_OFF;
     LEFT_REVERSE_SPEED = WHEEL_OFF;
     RIGHT_REVERSE_SPEED = RIGHTFAST; // Reverse right to spin counterclockwise
 }
 
 // Perform a left board turn
 void leftboard_turn(void) {
     LEFT_REVERSE_SPEED = WHEEL_OFF;
     RIGHT_FORWARD_SPEED = RIGHTFAST;
     LEFT_FORWARD_SPEED = 20000;
     RIGHT_REVERSE_SPEED = WHEEL_OFF;
 }
 
 // Perform a right board turn with pulsing
 void rightboard_turn(void) {
     static int pulse_counter = 0;
     int on_time_ticks = 9;  // Active for 9 ticks
     int off_time_ticks = 10; // Inactive for 10 ticks
     int total_pulse_ticks = on_time_ticks + off_time_ticks;
 
     if (pulse_counter < on_time_ticks) {
         // Wheels active during on-time
         LEFT_REVERSE_SPEED = WHEEL_OFF;
         RIGHT_FORWARD_SPEED = 35000; // Minimum allowed speed
         LEFT_FORWARD_SPEED = LEFTFAST;
         RIGHT_REVERSE_SPEED = WHEEL_OFF;
     } else {
         // Wheels inactive during off-time
         LEFT_REVERSE_SPEED = WHEEL_OFF;
         RIGHT_FORWARD_SPEED = WHEEL_OFF;
         LEFT_FORWARD_SPEED = LEFTFAST;
         RIGHT_REVERSE_SPEED = WHEEL_OFF;
     }
 
     pulse_counter++;
     if (pulse_counter >= total_pulse_ticks) {
         pulse_counter = 0; // Reset pulse counter after a full cycle
     }
 }
 
 // Perform an alternate right board turn with pulsing
 void rightboard_turn2(void) {
     static int pulse_counter = 0;
     int on_time_ticks = 6;  // Active for 6 ticks
     int off_time_ticks = 12; // Inactive for 12 ticks
     int total_pulse_ticks = on_time_ticks + off_time_ticks;
 
     if (pulse_counter < on_time_ticks) {
         // Wheels active during on-time
         LEFT_REVERSE_SPEED = WHEEL_OFF;
         RIGHT_FORWARD_SPEED = 30000;
         LEFT_FORWARD_SPEED = LEFTFAST;
         RIGHT_REVERSE_SPEED = WHEEL_OFF;
     } else {
         // Wheels inactive during off-time
         LEFT_REVERSE_SPEED = WHEEL_OFF;
         RIGHT_FORWARD_SPEED = WHEEL_OFF;
         LEFT_FORWARD_SPEED = LEFTFAST;
         RIGHT_REVERSE_SPEED = WHEEL_OFF;
     }
 
     pulse_counter++;
     if (pulse_counter >= total_pulse_ticks) {
         pulse_counter = 0; // Reset pulse counter after a full cycle
     }
 }

/*
 * pid.c
 *
 *  Created on: Mar 3, 2025
 *      Author: Dtwen
 */


#include "include\pid.h"
#include <stdint.h>

extern unsigned int ADC_LEFT_DETECT;
extern unsigned int ADC_RIGHT_DETECT;
int32_t right_speed, left_speed;
int error;
int32_t integralterm;
int32_t proportional;
int32_t right_speed;
int32_t left_speed;

int limit_car_speed(int pid_value, unsigned int minimum_adc, unsigned int maximum_adc){

    if (pid_value > maximum_adc)  return maximum_adc;
    if (pid_value < minimum_adc)  return minimum_adc;
    return pid_value;

}

void update_car_speed(unsigned int left_adc, unsigned int right_adc){

//     error = ADC_LEFT_DETECT - ADC_RIGHT_DETECT;
//
//      // Update integral (accumulated error)
//      integral += error;
//
//       integralterm = (int32_t)(Ki * error) ;
//      proportional = (int32_t)(Kp * error);
//      right_speed = BASE_SPEED - proportional - integralterm;
//      left_speed = (int32_t)(BASE_SPEED + proportional + integralterm);
//
//      // Clamp motor speeds to the allowed range
//      RIGHT_FORWARD_SPEED = limit_car_speed(right_speed, MIN_SPEED, MAX_SPEED);
//      LEFT_FORWARD_SPEED = limit_car_speed(left_speed, MIN_SPEED, MAX_SPEED);


//    int tolerance = 98;  // Tolerance for sensor values (black line detection)
//
//
//
//    if (left_adc > tolerance && right_adc > tolerance) {
//        // Both sensors see black, go straight
//        LEFT_FORWARD_SPEED = FAST;
//        RIGHT_FORWARD_SPEED = FAST;
//
//        // Ensure opposite directions are off
//        LEFT_REVERSE_SPEED = WHEEL_OFF;
//        RIGHT_REVERSE_SPEED = WHEEL_OFF;
//
//    } else if (right_adc < tolerance && left_adc > tolerance) {
//        // Right sensor sees white, turn left
//        LEFT_FORWARD_SPEED = FAST;
//        RIGHT_REVERSE_SPEED = FAST;  // Turning left
//
//        // Ensure opposite directions are off
//        LEFT_REVERSE_SPEED = WHEEL_OFF;
//        RIGHT_FORWARD_SPEED = WHEEL_OFF;
//
//        // Continuously turn left until the right sensor detects black
//        if (right_adc > tolerance) {
//            // Once the right sensor detects black, stop turning and go straight
//            LEFT_FORWARD_SPEED = FAST;
//            RIGHT_FORWARD_SPEED = FAST;
//            LEFT_REVERSE_SPEED = WHEEL_OFF;
//            RIGHT_REVERSE_SPEED = WHEEL_OFF;
//        }
//
//    } else if (left_adc < tolerance && right_adc > tolerance) {
//        // Left sensor sees white, turn right
//        RIGHT_FORWARD_SPEED = FAST;
//        LEFT_REVERSE_SPEED = FAST;  // Turning right
//
//        // Ensure opposite directions are off
//        LEFT_FORWARD_SPEED = WHEEL_OFF;
//        RIGHT_REVERSE_SPEED = WHEEL_OFF;
//
//        // Continuously turn right until the left sensor detects black
//        if (left_adc > tolerance) {
//            // Once the left sensor detects black, stop turning and go straight
//            LEFT_FORWARD_SPEED = FAST;
//            RIGHT_FORWARD_SPEED = FAST;
//            LEFT_REVERSE_SPEED = WHEEL_OFF;
//            RIGHT_REVERSE_SPEED = WHEEL_OFF;
//        }
//
//    } else if (left_adc <= tolerance && right_adc <= tolerance) {
//        // Both sensors see white, fallback behavior (e.g., stop or reverse)
//        // Compare sensor values to see which one is off the track more
//        if (left_adc < right_adc) {
//            // Left sensor is more off track, turn right
//            RIGHT_FORWARD_SPEED = SLOW;
//            LEFT_REVERSE_SPEED = SLOW;
//
//            // Ensure opposite directions are off
//            LEFT_FORWARD_SPEED = WHEEL_OFF;
//            RIGHT_REVERSE_SPEED = WHEEL_OFF;
//        } else {
//            // Right sensor is more off track, turn left
//            LEFT_FORWARD_SPEED = SLOW;
//            RIGHT_REVERSE_SPEED = SLOW;
//
//            // Ensure opposite directions are off
//            LEFT_REVERSE_SPEED = WHEEL_OFF;
//            RIGHT_FORWARD_SPEED = WHEEL_OFF;
//        }
//    }

}



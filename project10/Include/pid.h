/*
 * pid.h
 *
 *  Created on: Mar 3, 2025
 *      Author: Dtwen
 */

#ifndef INCLUDE_PID_H_
#define INCLUDE_PID_H_

#include <msp430.h>
#include  <string.h>
#include  "include\functions.h"
#include  "include\LCD.h"
#include  "include\ports.h"
#include  "include\macros.h"



int Kp = 500;
int Ki = 100;
int integral; // Accumulated error
int error;
//
//int cap_speed(int, unsigned int, unsigned int);
//void update_car_speed(unsigned int, unsigned int);



#endif /* INCLUDE_PID_H_ */

/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains functions to control motor movements, including forward,
 * backward, turning, and rotational movements. Earlier Projects, Currently holds no use.
 */

#include "include\macros.h"
#include "include\ports.h"
#include "include\functions.h"
#include "msp430.h"



void Init_motors(void)
{

    P6OUT &= ~L_FORWARD; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_FORWARD; // Set Port pin Low [Wheel Off]
    P6OUT &= ~L_REVERSE; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_REVERSE; // Set Port pin Low [Wheel Off]

    P6OUT &= ~GRN_LED; // Set Green LED off
    P1OUT &= ~RED_LED; // Set Red LED off


}

void motors_forward(void)
{
    P1OUT &= ~RED_LED; // Set Red LED off
    P6OUT |= GRN_LED; // set green led on

    P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
    P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]


}


void motors_reverse(void)
{
    P6DIR &= ~GRN_LED; // Set Green LED off
    P1OUT |= RED_LED; // Set Red LED On

    P6OUT |= L_REVERSE; // Set Port pin High [Wheel On]
    P6OUT |= R_REVERSE; // Set Port pin High [Wheel On]


}


inline void motors_off(void)
{


    P6OUT &= ~L_FORWARD; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_FORWARD; // Set Port pin Low [Wheel Off]
    P6OUT &= ~L_REVERSE; // Set Port pin Low [Wheel Off]
    P6OUT &= ~R_REVERSE; // Set Port pin Low [Wheel Off]

    P6OUT &= ~GRN_LED; // Set Green LED off
    P1OUT &= ~RED_LED; // Set Red LED off
}

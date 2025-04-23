/*
 * interrupts.h
 *
 *  Created on: Feb 16, 2025
 *      Author: Dtwen
 */

#ifndef INCLUDE_TIMERS_H_
#define INCLUDE_TIMERS_H_


    #include "include\macros.h"
    #include  "include\LCD.h"
    #include "include\ports.h"
    #include "include\functions.h"
    #include <msp430.h>


    unsigned int check;
    unsigned int updateDisplayFlag;

     int task =-1;
     int task2 =-1;
     int task3 =-1;
     int counter = -1;
     int counter2 = -1;





#endif /* INCLUDE_TIMERS_H_ */

/*
 * timerb1.c
 *
 * Created on: Feb 26, 2025
 * Author: Derrick Tweneboah
 *
 * Synopsis:
 * This file initializes and handles Timer B1 on the MSP430. Timer B1 is configured to
 * generate periodic interrupts for debounce handling and other time-based tasks.
 * The file defines two ISRs: one for CCR0 and another for CCR1 and other CCRx values.
 * The timer operates using SMCLK as the clock source, with clock division for slower timing.
 */

#include "include/timers.h"       // Header file for Timer-related macros and declarations
#include "include/macros.h"       // Header file for global macros
#include "include/functions.h"    // Header file for utility functions
#include "include/LCD.h"          // Header file for LCD interface functions
#include "msp430.h"               // MSP430-specific definitions
#include "include/ports.h"



// Global variables
extern unsigned int debounce_count1;  // Counter for debounce logic
extern unsigned int debounce1_in_progress;     // Flag indicating if debounce is active
extern int counter;                            // General-purpose counter
extern int counter2;
extern int task;

// Function to initialize Timer B1
void Init_timer_B1(void)
{
    TB1CTL = TBSSEL__SMCLK;        // Select SMCLK as the clock source
    TB1CTL |= TBCLR;               // Clear Timer B1: resets the timer counter and settings
    TB1CTL |= MC__UP;              // Set the timer to up mode: counts up to TB1CCR0
    TB1CTL |= ID__8;               // Divide the clock source by 8 for slower timing
    TB1EX0 = TBIDEX__8;            // Further divide the clock source by 8
    TB1CCR0 = TB1CCR0_200MS;       // Set CCR0 for 200ms interval
   // TB1CCTL0 |= CCIE;              // Enable interrupt for CCR0

    TB1CTL &= ~TBIE;               // Disable the timer overflow interrupt
    TB1CTL &= ~TBIFG;              // Clear the timer overflow interrupt flag

    TB1CCR1 = TB1CCR1_200MS;       // Set CCR1 for another 200ms interval
    TB1CCR2 = TB1CCR2_200MS;        // Uncomment if CCR2 is needed
}

// ISR for Timer B1 CCR0
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    debounce_count1++;             // Increment debounce counter
    if (debounce_count1 == 4)
    {    // Check if debounce period is complete

        TB1CCTL0 &= ~CCIE; // CCR1 enable interrupt
        debounce1_in_progress = DISABLED; // Reset debounce flag
        P4IE |= SW1; // Enable the Switch Interrupt.

    }
}

#pragma vector = TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void) {
    switch (__even_in_range(TB1IV, 14)) {
        case 0: break; // No interrupt
        case 2: counter++; break; // CCR1 interrupt
        case 4: counter2++; break; // CCR2 interrupt
        case 14: break; // Timer overflow
        default: break;
    }
}

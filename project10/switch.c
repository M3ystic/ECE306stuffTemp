/*
 * switches.c
 *
 * Created on: Feb 13, 2025
 * Author: Derrick Tweneboah
 *
 * Synopsis:
 * This file implements interrupt service routines (ISRs) for switches connected to PORT4 (Switch 1)
 * and PORT2 (Switch 2) of the MSP430. These ISRs handle debounce logic using timers and
 * toggle associated flags for further processing. Debounce is managed using Timer B modules.
 */

#include "include/switch.h"  // Header for switch-related macros and declarations
#include "include/macros.h"  // Header for global macros
#include "include/ports.h"   // Header for port and pin definitions
#include "msp430.h"          // MSP430-specific definitions
extern unsigned int position;
extern char message[];
// Global variables
extern unsigned  int debounce_count1; // Debounce counter for Switch 1
extern unsigned volatile int debounce_count2; // Debounce counter for Switch 2

extern unsigned int switch1_pressed;          // Flag for Switch 1 press event
unsigned int debounce1_in_progress;           // Debounce in-progress flag for Switch 1

extern unsigned int switch2_pressed;          // Flag for Switch 2 press event
unsigned int debounce2_in_progress;           // Debounce in-progress flag for Switch 2
extern int task;
extern int counter;                            // General-purpose counter



// Interrupt Service Routine for PORT4 (Switch 1)
#pragma vector = PORT4_VECTOR
__interrupt void switch_interrupt(void)
{
    if (P4IFG & SW1)
    {              // Check if Switch 1 caused the interrupt
        P4IFG &= ~SW1;              // Clear interrupt flag for Switch 1
        switch1_pressed = ENABLED;  // Set flag to indicate Switch 1 is pressed
        debounce1_in_progress = ENABLED; // Set debounce flag for Switch 1

        TB1CCTL0 |= CCIE;           // Enable interrupt for CCR0 of Timer B1
        TB1CCTL1 |= CCIE;           // Enable interrupt for CCR1 of Timer B1

        debounce_count1 = RESET;    // Reset debounce counter for Switch 1
        P4IE &= ~SW1;               // Disable Switch 1 interrupts during debounce

        // Clear Timer B1 interrupt flags
        TB1CCTL0 &= ~CCIFG;         // Clear interrupt flag for CCR0
        TB1CCTL1 &= ~CCIFG;         // Clear interrupt flag for CCR1
        TB1CCTL2 &= ~CCIFG;         // Clear interrupt flag for CCR2

        task = 0;
        counter = 0;
//
//        UCA1IE |= UCTXCPTIE;
        UCA1IFG &= ~UCTXIFG; // Clear TX flag

    }
}

// Interrupt Service Routine for PORT2 (Switch 2)
#pragma vector = PORT2_VECTOR
__interrupt void switch_interrupt2(void)
{
    if (P2IFG & SW2)
    {              // Check if Switch 2 caused the interrupt
        P2IFG &= ~SW2;              // Clear interrupt flag for Switch 2
        switch2_pressed = ENABLED;  // Set flag to indicate Switch 2 is pressed
        debounce2_in_progress = ENABLED; // Set debounce flag for Switch 2

        TB0CCTL2 |= CCIE;           // Enable interrupt for CCR2 of Timer B0

        debounce_count2 = RESET;    // Reset debounce counter for Switch 2
        P2IE &= ~SW2;               // Disable Switch 2 interrupts during debounce

        // Clear Timer B0 interrupt flags
        TB1CCTL1 |= CCIE;           // Enable interrupt for CCR1 of Timer B1

        TB0CCTL0 &= ~CCIFG;         // Clear interrupt flag for CCR0
        TB0CCTL1 &= ~CCIFG;         // Clear interrupt flag for CCR1
        TB0CCTL2 &= ~CCIFG;         // Clear interrupt flag for CCR2

     //  UCA1IFG &= ~UCTXIFG; // Clear TX flag

        task = -1;
        counter = 0;

    }
}

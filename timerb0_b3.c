/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains the initialization and interrupt service routines (ISRs)
 * for Timer_B0 and Timer_B3 on the MSP430 microcontroller. Timer_B0 is configured
 * for periodic interrupts and ADC sampling, as well as debounce logic. Timer_B3
 * is configured for generating PWM signals for motor control and LCD backlight dimming.
 */

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "include/macros.h"
#include "include/functions.h"
#include "include/LCD.h"
#include "msp430.h"
#include "include/ports.h"
//------------------------------------------------------------------------------
// External Variables for Display Update and Debounce Logic
//------------------------------------------------------------------------------
extern volatile unsigned char display_changed;  // Flag for display change
extern volatile unsigned char update_display;  // Flag to update the display
extern volatile unsigned int debounce_count2;  // Counter for debounce logic
extern unsigned int debounce2_in_progress;     // Debounce in progress flag
//------------------------------------------------------------------------------
// Timer Variables
//------------------------------------------------------------------------------
unsigned int timechanged = RESET_STATE;         // Time tracking variable
extern volatile unsigned int updatetimerflag;  // Flag for timer update
extern unsigned int DAC_data;                  // DAC data value
unsigned int timer = RESET_STATE;              // Timer for command delays
unsigned int command_sent = FALSE;             // Static flag to track sent command state
unsigned int timerflag;
//------------------------------------------------------------------------------
// Flags for Sending Commands
//------------------------------------------------------------------------------
volatile unsigned int send_commands_flag = FALSE;            // General command flag
volatile unsigned int send_ssid_command_flag = FALSE;        // Flag for sending SSID command
volatile unsigned int send_ip_command_flag = FALSE;          // Flag for sending IP command
volatile unsigned int send_cipmux_command_flag = FALSE;      // Flag for sending MUX command
volatile unsigned int send_cipserver_command_flag = FALSE;   // Flag for sending server command
//------------------------------------------------------------------------------
// Calibration Variables
//------------------------------------------------------------------------------
int calibratingTime;
extern int maxBlackValue;               // Track highest black value
extern int minWhiteValue;               // Track lowest black value
extern int calibrating;
extern unsigned int calibratingFlag;
//------------------------------------------------------------------------------
// Miscellaneous
//------------------------------------------------------------------------------
int counterIOT;

 
 // Function to initialize Timer_B0
 void Init_timer_B0(void)
 {
     TB0CTL = TBSSEL__SMCLK; // Select SMCLK as the clock source (8 MHz)
     TB0CTL |= TBCLR;        // Reset Timer_B0
     TB0CTL |= MC__CONTINUOUS;       // Configure Timer_B0 in up mode
     TB0CTL |= ID__8;        // Divide clock by 8
     TB0EX0 = TBIDEX__8;     // Further divide clock by 8
     TB0CCR0 = TB0CCR0_200MS; // Set CCR0 for 200ms interrupts
     TB0CCTL0 |= CCIE;       // Enable CCR0 interrupt
 
     TB0CTL &= ~TBIE;        // Disable overflow interrupt
     TB0CTL &= ~TBIFG;       // Clear overflow interrupt flag
 
     TB0CCR1 = TB0CCR1_10MS;  // Set CCR1 for 10ms (not used)
     TB0CCR2 = TB0CCR2_200MS; // Set CCR2 for 200ms debounce logic
 }
 
 // Timer_B0 CCR0 interrupt service routine
#pragma vector = TIMER0_B0_VECTOR
 __interrupt void Timer0_B0_ISR(void)
 {
     if (updatetimerflag == TRUE)
     {
         timechanged += TIMEINCREMENT; // Increment timechanged by 2
         if (timechanged > MAXTIMER)    // 999.8 seconds
         {
             timechanged = FALSE; // Reset timechanged if it exceeds 9998
         }
     }

     update_display = TRUE;   // Set flag to update display
     display_changed = TRUE; // Notify display has changed

     //calibration for 5 seconds
     if (calibratingFlag == TRUE){
         calibratingTime++;
         if (calibratingTime == FIVESECONDS)
         {
             MIDDLELINE = (int)(maxBlackValue * MAXMULTIPLIER + minWhiteValue * MINMULTIPLIER);
             P1OUT ^= RED_LED;
             calibratingFlag = FALSE;
         }
     }
     P3OUT ^= IOT_LINK_CPU;
     P6OUT ^= GRN_LED;
     TB0CCR0 += TB0CCR0_200MS;

 }

 // Timer_B0 CCR1-CCR2 and overflow ISR
#pragma vector = TIMER0_B1_VECTOR
 __interrupt void TIMER0_B1_ISR(void)
 {
     switch (__even_in_range(TB0IV, 14))
     {
     case 0: break; // No interrupt
     case 2: // CCR1 interrupt (not used)
         ADCCTL0 |= ADCSC;  // Start ADC conversion

         if (DAC_data >= DAC_LIMIT) {
             P2OUT |= DAC_ENB;    // Enable DAC
             DAC_data -= DACDECREMENTER;     // Decrease DAC data for voltage to slowly increase
             SAC3DAT = DAC_data;  // Set DAC output data
         }

         if (counterIOT < IOTREADY) {
             counterIOT++;

             if (counterIOT == IOTREADY) {
                 P3OUT |= IOT_EN_CPU;  // Enable IOT
                 send_commands_flag = TRUE;  // Set the flag to trigger command sending
             }
         }
         timerflag = TRUE;
         TB0CCR1 +=TB0CCR1_10MS;
         break;
     case 4: // CCR2 interrupt for debounce logic
         debounce_count2++;

         if (debounce_count2 == DEBOUNCEDONE)
         {
             debounce2_in_progress = DISABLED; // Disable debounce in progress
             TB0CCTL2 &= ~CCIE; // CCR1 enable interrupt
             P2IE |= SW2; // Enable the Switch Interrupt.
         }
         TB0CCR2 += TB0CCR2_200MS;
         break;
     case 14: break; // Overflow interrupt (not used)
     default: break;
     }
 }

 // Function to initialize Timer_B3 for PWM generation
 void Init_Timer_B3(void) {
     TB3CTL = TBSSEL__SMCLK; // Select SMCLK as clock source
     TB3CTL |= MC__UP;       // Configure Timer_B3 in up mode
     TB3CTL |= TBCLR;        // Clear Timer_B3
 
     PWM_PERIOD = WHEEL_PERIOD; // Set PWM period
 
     TB3CCTL1 = OUTMOD_7;        // CCR1 reset/set mode
     LCD_BACKLITE_DIMING = PERCENT_80; // Set duty cycle for LCD backlight (P6.0)
 
     TB3CCTL2 = OUTMOD_7;        // CCR2 reset/set mode
     RIGHT_FORWARD_SPEED = WHEEL_OFF; // Set duty cycle for right forward motor (P6.1)
 
     TB3CCTL3 = OUTMOD_7;        // CCR3 reset/set mode
     RIGHT_REVERSE_SPEED = WHEEL_OFF; // Set duty cycle for right reverse motor (P6.2)
 
     TB3CCTL4 = OUTMOD_7;        // CCR4 reset/set mode
     LEFT_FORWARD_SPEED = WHEEL_OFF; // Set duty cycle for left forward motor (P6.3)
 
     TB3CCTL5 = OUTMOD_7;        // CCR5 reset/set mode
     LEFT_REVERSE_SPEED = WHEEL_OFF; // Set duty cycle for left reverse motor (P6.4)
 }
 

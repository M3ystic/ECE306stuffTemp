/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains the implementation of the ADC interrupt service routine (ISR).
 * It handles the ADC conversions for detecting left and right sensor values and calibrating them.
 */

 #include "include\\macros.h"
 #include "include\\ports.h"
 #include "include\\functions.h"
 #include "msp430.h"
 
 // ADC channel variables
 unsigned int ADC_CHANNEL;
 unsigned int ADC_THUMB;
 unsigned int ADC_LEFT_DETECT;
 unsigned int ADC_RIGHT_DETECT;
 
 // Calibration variables
 int maxBlackValue = 0;      // Highest black detection for right sensor
 int minWhiteValue = 1023;   // Lowest white detection for right sensor
 unsigned int calibrating = TRUE; // Calibration mode flag
 
 #pragma vector=ADC_VECTOR
 __interrupt void ADC_ISR(void) {
     switch (__even_in_range(ADCIV, ADCIV_ADCIFG)) {
         case ADCIV_NONE:
             break;
 
         case ADCIV_ADCOVIFG: // ADC conversion overflow
             break;
 
         case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
             break;
 
         case ADCIV_ADCHIIFG: // Window comparator interrupt flags
             break;
 
         case ADCIV_ADCLOIFG: // Window comparator interrupt flag
             break;
 
         case ADCIV_ADCINIFG: // Window comparator interrupt flag
             break;
 
         case ADCIV_ADCIFG: // ADC conversion result ready
             ADCCTL0 &= ~ADCENC; // Disable conversion
 
             switch (ADC_CHANNEL++) {
                 case 0x00: // ADC_Right_Detect
                     ADC_RIGHT_DETECT = ADCMEM0 >> 2; // Process ADC data
                     if (calibrating) {
                         // Update calibration values
                         if (ADC_RIGHT_DETECT <= 800 && ADC_RIGHT_DETECT > maxBlackValue) {
                             maxBlackValue = ADC_RIGHT_DETECT;
                             P6OUT |= GRN_LED; // Indicate calibration
                         }
                         if (ADC_RIGHT_DETECT < minWhiteValue) {
                             minWhiteValue = ADC_RIGHT_DETECT;
                             P6OUT |= GRN_LED; // Indicate calibration
                         }
                     }
                     ADCMCTL0 &= ~ADCINCH_2; // Reset channel
                     ADCMCTL0 |= ADCINCH_3; // Set next channel
                     ADCCTL0 |= ADCSC;      // Start next sample
                     break;
 
                 case 0x01: // ADC_Left_Detect
                     ADC_LEFT_DETECT = ADCMEM0 >> 2; // Process ADC data
                     if (calibrating) {
                         // Update calibration values
                         if (ADC_LEFT_DETECT <= 800 && ADC_LEFT_DETECT > maxBlackValue) {
                             maxBlackValue = ADC_LEFT_DETECT;
                         }
                         if (ADC_LEFT_DETECT < minWhiteValue) {
                             minWhiteValue = ADC_LEFT_DETECT;
                             P6OUT |= GRN_LED; // Indicate calibration
                         }
                     }
                     ADCMCTL0 &= ~ADCINCH_3; // Reset channel
                     ADCMCTL0 = ADCINCH_2;  // Set next channel
                     ADC_CHANNEL = 0;      // Reset ADC channel
                     break;
 
                 default:
                     break;
             }
             ADCCTL0 |= ADCENC; // Re-enable conversions
             break;
 
         default:
             break;
     }
 }

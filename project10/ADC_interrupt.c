/*
 * ADC_interrupt.c
 *
 *  Created on: Feb 25, 2025
 *      Author: Dtwen
 */
#include "include\macros.h"
#include "include\ports.h"
#include "include\functions.h"
#include "msp430.h"

unsigned int ADC_CHANNEL;
unsigned int ADC_THUMB;
unsigned int ADC_LEFT_DETECT;
unsigned int ADC_RIGHT_DETECT;

int maxBlackValue = 0; // Track highest black detection for right sensor
int minWhiteValue = 1023; // Track highest black detection for right sensor
unsigned int calibrating = TRUE;    // Flag for calibration mode


#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
 switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
 case ADCIV_NONE:
 break;
 case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
 // before its previous conversion result was read.
 break;
 case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
 break;
 case ADCIV_ADCHIIFG: // Window comparator interrupt flags
 break;
 case ADCIV_ADCLOIFG: // Window comparator interrupt flag
 break;
 case ADCIV_ADCINIFG: // Window comparator interrupt flag
 break;


     case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result

         ADCCTL0 &= ~ADCENC; // Disable Conversion.

         switch (ADC_CHANNEL++){

         case 0x00: // ADC_left_Det
         ADC_RIGHT_DETECT = ADCMEM0; // Channel A2
        ADC_RIGHT_DETECT = ADC_RIGHT_DETECT >> 2;
        if (calibrating) {
            // Track highest black value
            if (ADC_RIGHT_DETECT <= 800 && ADC_RIGHT_DETECT > maxBlackValue) {
                maxBlackValue = ADC_RIGHT_DETECT;
                P6OUT |= GRN_LED;
            }

            // Track lowest white value
            if (ADC_RIGHT_DETECT < minWhiteValue) {
                minWhiteValue = ADC_RIGHT_DETECT;
                P6OUT |= GRN_LED;

            }
        }
         ADCMCTL0 &= ~ADCINCH_2; // Last channel A5
         ADCMCTL0 |= ADCINCH_3; // Next channel A3
         ADCCTL0 |= ADCSC; // Start next sample
         break;

         case 0x01: // ADC_Right_Det
         ADC_LEFT_DETECT = ADCMEM0; // Channel A3
         ADC_LEFT_DETECT = ADC_LEFT_DETECT >> 2;
         if (calibrating) {
             if (ADC_LEFT_DETECT <= 800 && ADC_LEFT_DETECT > maxBlackValue) {
                 maxBlackValue = ADC_LEFT_DETECT;
             }

             if (ADC_LEFT_DETECT < minWhiteValue) {
                 minWhiteValue = ADC_LEFT_DETECT;
                 P6OUT |= GRN_LED;

             }
         }
         ADCMCTL0 &= ~ADCINCH_3; // Last channel A3
         ADCMCTL0 = ADCINCH_2; // Next channel A5
//         ADCCTL0 |= ADCSC; // Start next sample
         ADC_CHANNEL = 0;// Do not start the next sample
         break;

//         case 0x02: // ADC_Thumb Wheel
//         ADC_THUMB = ADCMEM0; // Channel A2
//         ADC_THUMB = ADC_THUMB >> 2;
//         ADCMCTL0 &= ~ADCINCH_5; // Last channel A
//         ADCMCTL0 |= ADCINCH_2; // Next channel A2
//         ADC_CHANNEL = 0;// Do not start the next sample
//         break;
         default:break;
         }
         ADCCTL0 |= ADCENC; // Enable Conversions

         break;
         // ADCCTL0 |= ADCSC; // Start next sample


 default:
 break;
 }
}

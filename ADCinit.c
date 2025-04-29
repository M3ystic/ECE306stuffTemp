/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file initializes the Analog-to-Digital Converter (ADC) and configures the necessary ports for its operation.
 */

#include "include\macros.h"
#include "include\ports.h"
#include "include\functions.h"
#include "msp430.h"



void Init_port1ADC(void){
     P1DIR = 0x00; // Set P1 direction to input
     P1OUT = 0x00; // P1 set Low
     P1SEL0 &= ~RED_LED; // Set RED_LED as GP I/O
     P1SEL1 &= ~RED_LED; // Set RED_LED as GP I/O
     P1OUT |= RED_LED; // Set Red LED On
     P1DIR |= RED_LED; // Set Red LED direction to output
     P1SELC |= V_A1_SEEED; // ADC input for A1_SEEED
     P1SELC |= V_DETECT_L; // ADC input for V_DETECT_L
     P1SELC |= V_DETECT_R; // ADC input for V_DETECT_R
     P1SELC |= V_A4_SEEED; // ADC input for V_A4_SEEED
    // P1SELC |= V_THUMB; // ADC input for V_THUMB
     P1SEL0 |= UCA0TXD; // UCA0TXD pin
     P1SEL1 &= ~UCA0TXD; // UCA0TXD pin
     P1SEL0 |= UCA0RXD; // UCA0RXD pin
     P1SEL1 &= ~UCA0RXD; // UCA0RXD pin
}

void Init_port5ADC(void){
     P5DIR = 0x00; // Set P1 direction to input
     P5OUT = 0x00; // P1 set Low
     P5SELC |= V_BAT; // ADC input for V_BAT
     P5SELC |= V_5; // ADC input for V_BAT
     P5SELC |= V_DAC; // ADC input for V_DAC
     P5SELC |= V_3_3; // ADC input for V_3_3
     P5SEL0 &= ~IOT_BOOT_CPU; // IOT_BOOT GPIO operation
     P5SEL1 &= ~IOT_BOOT_CPU; // IOT_BOOT GPIO operation
     P5OUT |= IOT_BOOT_CPU; // Set Output value inactive
     P5DIR |= IOT_BOOT_CPU;
}
void Init_ADC(void){
    //------------------------------------------------------------------------------
    // V_DETECT_L (0x04) // Pin 2 A2
    // V_DETECT_R (0x08) // Pin 3 A3
    // V_THUMB (0x20) // Pin 5 A5
    //------------------------------------------------------------------------------
    // ADCCTL0 Register
     ADCCTL0 = 0; // Reset
     ADCCTL0 |= ADCSHT_2; // 16 ADC clocks
     ADCCTL0 |= ADCMSC; // MSC
     ADCCTL0 |= ADCON; // ADC ON
    // ADCCTL1 Register
     ADCCTL1 = 0; // Reset
     ADCCTL1 |= ADCSHS_0; // 00b = ADCSC bit
     ADCCTL1 |= ADCSHP; // ADC sample-and-hold SAMPCON signal from sampling timer.
     ADCCTL1 &= ~ADCISSH; // ADC invert signal sample-and-hold.
     ADCCTL1 |= ADCDIV_0; // ADC clock divider - 000b = Divide by 1
     ADCCTL1 |= ADCSSEL_0; // ADC clock MODCLK
     ADCCTL1 |= ADCCONSEQ_0; // ADC co a anversion sequence 00b = Single-channel single-conversion
    // ADCCTL1 & ADCBUSY identifies a conversion is in process
    // ADCCTL2 Register
     ADCCTL2 = 0; // Reset
     ADCCTL2 |= ADCPDIV0; // ADC pre-divider 00b = Pre-divide by 1
     ADCCTL2 |= ADCRES_2; // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
     ADCCTL2 &= ~ADCDF; // ADC data read-back format 0b = Binary unsigned.
     ADCCTL2 &= ~ADCSR; // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
    // ADCMCTL0 Register
     ADCMCTL0 |= ADCSREF_0; // VREF - 000b = {VR+ = AVCC and VR� = AVSS }
     ADCMCTL0 |= ADCINCH_2; // LEFT_DETECT (0x20) Pin 2 A2
     ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
     ADCCTL0 |= ADCENC; // ADC enable conversion.
     ADCCTL0 |= ADCSC; // ADC start conversion.

     Init_port1ADC();
     Init_port5ADC();

}



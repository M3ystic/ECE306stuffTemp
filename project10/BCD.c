/*
 * Apr 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains functions for converting hexadecimal values
 * to Binary-Coded Decimal (BCD) and updating display lines with the converted values.
 */

#include "include\\macros.h"
#include "include\\ports.h"
#include "include\\functions.h"
#include "msp430.h"
#include <string.h>
#include "include\\LCD.h"

// Global variables for ADC readings and display updates
char adc_char[4];
char adc_char2[4];
char adc_char3[4];
unsigned int location = 1;

// External variables
extern unsigned int ADC_LEFT_DETECT;
extern unsigned int ADC_RIGHT_DETECT;
extern char display_line[4][11];
extern unsigned int timechanged;

/**
 * Converts an integer to BCD and stores the result in adc_char.
 * The integer value to convert.
 */
void HEXtoBCD(int ADC_LEFT_DETECT) {
    int value = 0;
    int i;

    // Clear adc_char arrays
    for (i = 0; i < 4; i++) {
        adc_char3[i] = '0';
        adc_char2[i] = '0';
        adc_char[i] = '0';
    }

    // Process thousands, hundreds, tens, and units
    while (ADC_LEFT_DETECT > 999) {
        ADC_LEFT_DETECT -= LIMIT;
        value++;
        adc_char[0] = 0x30 + value;
    }
    value = 0;
    while (ADC_LEFT_DETECT > 99) {
        ADC_LEFT_DETECT -= 100;
        value++;
        adc_char[1] = 0x30 + value;
    }
    value = 0;
    while (ADC_LEFT_DETECT > 9) {
        ADC_LEFT_DETECT -= 10;
        value++;
        adc_char[2] = 0x30 + value;
    }
    adc_char[3] = 0x30 + ADC_LEFT_DETECT;
}

/**
 *  Updates line 1 of the display with ADC values.
 *  The starting location for the update.
 */
void adc_line_1(int location) {
    int i;
    for (i = 0; i < 4; i++) {
        display_line[1][i + location] = adc_char[i];
    }
}

/**
 *  Converts an integer to BCD and stores the result in adc_char2.
 *  The integer value to convert.
 */
void HEXtoBCD_2(int ADC_RIGHT_DETECT) {
    int value = 0;
    int i;

    // Clear adc_char arrays
    for (i = 0; i < 4; i++) {
        adc_char3[i] = '0';
        adc_char2[i] = '0';
        adc_char[i] = '0';
    }

    // Process thousands, hundreds, tens, and units
    while (ADC_RIGHT_DETECT > 999) {
        ADC_RIGHT_DETECT -= LIMIT;
        value++;
        adc_char2[0] = 0x30 + value;
    }
    value = 0;
    while (ADC_RIGHT_DETECT > 99) {
        ADC_RIGHT_DETECT -= 100;
        value++;
        adc_char2[1] = 0x30 + value;
    }
    value = 0;
    while (ADC_RIGHT_DETECT > 9) {
        ADC_RIGHT_DETECT -= 10;
        value++;
        adc_char2[2] = 0x30 + value;
    }
    adc_char2[3] = 0x30 + ADC_RIGHT_DETECT;
}

/**
 * Updates line 2 of the display with ADC values.
 * The starting location for the update.
 */
void adc_line_2(int location) {
    int i;
    for (i = 0; i < 4; i++) {
        display_line[2][i + location] = adc_char2[i];
    }
}

/**
 *  Converts a time value to BCD and stores the result in adc_char3.
 *  The time value to convert.
 */
void HEXtoBCD_3(int timechanged) {
    int value = 0;
    int i;

    // Clear adc_char3 array
    for (i = 0; i < 4; i++) {
        adc_char3[i] = '0';
    }

    // Safeguard for unexpected values
    if (timechanged < 0 || timechanged > 9999) {
        timechanged = 0; // Reset to prevent errors
    }

    // Process thousands, hundreds, tens, and units
    while (timechanged > 999) {
        timechanged -= LIMIT;
        value++;
        adc_char3[0] = 0x30 + value;
    }
    value = 0;
    while (timechanged > 99) {
        timechanged -= 100;
        value++;
        adc_char3[1] = 0x30 + value;
    }
    value = 0;
    while (timechanged > 9) {
        timechanged -= 10;
        value++;
        adc_char3[2] = 0x30 + value;
    }
    adc_char3[3] = 0x30 + timechanged;
}

/**
 *  Updates line 3 of the display with the timer value.
 *  The time value to display.
 */
void timer_update(unsigned int timechanged) {
    HEXtoBCD_3(timechanged);

    // Update display with timer value
    display_line[3][5] = ' ';
    display_line[3][6] = adc_char3[0];
    display_line[3][7] = adc_char3[1];
    display_line[3][8] = adc_char3[2];
    display_line[3][9] = 's';
}

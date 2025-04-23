/*
 * BCD.c
 *
 *  Created on: Apr 13, 2025
 *      Author: datweneb
 */
#include "include\macros.h"
#include "include\ports.h"
#include "include\functions.h"
#include "msp430.h"
#include <string.h>
#include  "include\LCD.h"

char adc_char[4];
char adc_char2[4];
char adc_char3[4];

unsigned int location = 1;

extern unsigned int ADC_LEFT_DETECT;
extern unsigned int ADC_RIGHT_DETECT;
extern char display_line[4][11];


extern unsigned int timechanged;



      void HEXtoBCD(int ADC_LEFT_DETECT){
              int value;
              int i;
              for(i=0; i < 4; i++) {
                  adc_char3[i] = '0';
                            adc_char2[i] = '0';
                            adc_char[i] = '0';
             }
              while (ADC_LEFT_DETECT > 999){
              ADC_LEFT_DETECT = ADC_LEFT_DETECT - LIMIT;
              value = value + 1;
              adc_char[0] = 0x30 + value;
              }
              value = 0;
              while (ADC_LEFT_DETECT > 99){
              ADC_LEFT_DETECT = ADC_LEFT_DETECT - 100;
              value = value + 1;
              adc_char[1] = 0x30 + value;
              }
              value = 0;
              while (ADC_LEFT_DETECT > 9){
              ADC_LEFT_DETECT = ADC_LEFT_DETECT - 10;
              value = value + 1;
              adc_char[2] = 0x30 + value;
              }
              adc_char[3] = 0x30 + ADC_LEFT_DETECT;
             }


             void adc_line_1(int location) {
                 //-------------------------------------------------------------
                 int i;
                 for (i = 0; i < 4; i++) {
                     display_line[1][i + location] = adc_char[i];
                 }
             }


             void HEXtoBCD_2(int ADC_RIGHT_DETECT){
              int value;
              int i;
              for(i=0; i < 4; i++) {
                  adc_char3[i] = '0';
                            adc_char2[i] = '0';
                            adc_char[i] = '0';
             }
              while (ADC_RIGHT_DETECT > 999){
              ADC_RIGHT_DETECT = ADC_RIGHT_DETECT - LIMIT;
              value = value + 1;
              adc_char2[0] = 0x30 + value;
              }
              value = 0;
              while (ADC_RIGHT_DETECT > 99){
              ADC_RIGHT_DETECT = ADC_RIGHT_DETECT - 100;
              value = value + 1;
              adc_char2[1] = 0x30 + value;
              }
              value = 0;
              while (ADC_RIGHT_DETECT > 9){
              ADC_RIGHT_DETECT = ADC_RIGHT_DETECT - 10;
              value = value + 1;
              adc_char2[2] = 0x30 + value;
              }
              adc_char2[3] = 0x30 + ADC_RIGHT_DETECT;
             }

             void adc_line_2(int location) {
                 //-------------------------------------------------------------
                 int i;
                 for (i = 0; i < 4; i++) {
                     display_line[2][i + location] = adc_char2[i];
                 }
             }


             void HEXtoBCD_3(int timechanged) {
                 int value;
                 int i;

                 // Clear only the adc_char3 array
                 for (i = 0; i < 4; i++) {
                     adc_char3[i] = '0';
                 }

                 // Add a safeguard for unexpected values
                 if (timechanged < 0 || timechanged > 9999) {
                     timechanged = 0;  // Reset to prevent errors
                 }

                 // Convert timechanged to BCD
                 value = 0;
                 while (timechanged > 999) {
                     timechanged = timechanged - LIMIT;
                     value = value + 1;
                     adc_char3[0] = 0x30 + value;
                 }

                 value = 0;
                 while (timechanged > 99) {
                     timechanged = timechanged - 100;
                     value = value + 1;
                     adc_char3[1] = 0x30 + value;
                 }

                 value = 0;
                 while (timechanged > 9) {
                     timechanged = timechanged - 10;
                     value = value + 1;
                     adc_char3[2] = 0x30 + value;
                 }

                 // Handle the last digit
                 adc_char3[3] = 0x30 + timechanged;
             }




             void timer_update(unsigned int timechanged){



                 HEXtoBCD_3(timechanged);

                 // Add the tenths digit
                 display_line[3][5] = ' ';  //adc_char3[0]
                 display_line[3][6] = adc_char3[0];
                 display_line[3][7] = adc_char3[1];
                 display_line[3][8] = adc_char3[2];              // Add decimal point
                 display_line[3][9] = 's';      // Add tenths digit

             }

/*
 * led.c
 *
 *  Created on: Jan 29, 2025
 *      Author: Dtwen
 */

#include "msp430.h"
#include "include/ports.h"

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

void Display_Process(void);

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}



//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include <msp430.h>
#include  <string.h>
#include  "include\functions.h"
#include  "include\LCD.h"
#include  "include\ports.h"
#include  "include\macros.h"


volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;


extern unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
extern unsigned int cycle_time; // is a new time base used to control making shapes

int tolerance = 98;


//extern unsigned int updateDisplayFlag;
//
//unsigned int check;
//
//
//char event;
//
unsigned volatile int debounce_count1;
unsigned volatile int debounce_count2;

unsigned int switch1_pressed;
unsigned int switch2_pressed;

extern unsigned int debounce1_in_progress;
extern unsigned int debounce2_in_progress;

extern unsigned int task;
extern int counter;
extern int updateDisplayFlag;

//

extern unsigned int ADC_LEFT_DETECT;
extern unsigned int ADC_RIGHT_DETECT;

extern float Kp;
extern float Ki;
extern int integral;
extern int error;

unsigned int ircheck;
char adc_char[4];
char adc_char2[4];
char adc_char3[4];

int location = 1;
int side;

extern volatile unsigned int timechanged;
unsigned int updatetimerflag = 0;

unsigned int wheel_move;
char forward;
//void main(void){
void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_DAC();

  Init_timer_B0();
  Init_timer_B1();
  Init_Timer_B3();
  Init_ADC();
  Init_DAC();
  Init_LCD();                          // Initialize LCD
  Init_motors();

  LCD_BACKLITE_DIMING = PERCENT_60; // P6.0 Right Forward PWM duty cycle
  RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
  RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
  LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
  LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle


  // Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "           ");
  strcpy(display_line[1], "L          ");
  strcpy(display_line[2], "R          ");
  strcpy(display_line[3], "           ");
  display_line[0][0] = 'T';
  display_line[0][1] = 'i';
  display_line[0][2] = 'm';
  display_line[0][3] = 'e';
  display_line[0][4] = ':';
  display_changed = TRUE;


//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------
    while(ALWAYS)
    {                      // Can the Operating system run
    Display_Process();                 // Update Display


if (!debounce1_in_progress){
        P2IFG &= ~SW2; // IFG SW1 cleared
        P2IE |= SW2; // Enable the Switch Interrupt.
        TB1CCTL0 &= ~CCIE; // CCR1 enable interrupt

 }


 if (!debounce2_in_progress){
          P2IFG &= ~SW2; // IFG SW1 cleared
          P2IE |= SW2; // Enable the Switch Interrupt.
          TB0CCTL2 &= ~CCIE; // CCR1 enable interrupt

}
 TB0CCTL1 |= CCIE; // CCR1 enable interrupt


 // Handle tasks in the switch statement
          if(switch1_pressed == ENABLED){
           switch (task)
           {
               case 0: // Initial idle state for 1 seconds
                   if (counter < 5) { // 1 seconds (0 to 9, 200ms intervals)
                       RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
                       RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
                       LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
                       LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
                       strcpy(display_line[3], " Stopped  ");

                   } else {
                       task++;        // Move to the next task
                       counter = 0;   // Reset counter
                   }
                   break;

               case 1: // Forward till detection
                   straight();
                   strcpy(display_line[3]," Intercept");
                   if (ADC_LEFT_DETECT > 70 || ADC_RIGHT_DETECT > 70) {//these numbers dont work because i fixed my diodes)
                       task++;
                       counter = 0;
                   }
                   break;

               case 2: // Pause 3 second
                   if (counter < 2) { // 3 seconds (15 to 19, 200ms intervals)

                       RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
                       RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
                       LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
                       LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle

                       strcpy(display_line[3], " Waiting  ");
                   } else {
                       task++;
                       counter = 0;
                   }
                   break;

               case 3: // Turn action with counter
                   turn();
                   if (counter < 2) { // Perform the turn for 8 cycles
                       strcpy(display_line[3], "   Turning   ");
                   } else {
                       task = 4; // Move to the next task
                       counter = 0;

                       RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
                       RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
                       LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
                       LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
                   }
                   break;

               case 4: // Initial idle state for 1+ seconds
                   if (counter < 2) { // 1 seconds (0 to 9, 200ms intervals)
                       RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
                       RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
                       LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
                       LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle

                   } else {
                       task++;        // Move to the next task
                       counter = 0;   // Reset counter
                   }
                   break;
               case 5: // Follow the circle
                   updatetimerflag = 1;

                   strcpy(display_line[3], "navigating  ");
                   display_changed = TRUE;

                   while (1)
                   {
                       HEXtoBCD(ADC_LEFT_DETECT);
                       adc_line_1(location);
                       HEXtoBCD_2(ADC_RIGHT_DETECT);
                       adc_line_2(location);
                       timer_update(timechanged);



                       if (timechanged >= 1100) {
                           break;  // Exit the loop after the required time
                       }

s
                       RIGHT_FORWARD_SPEED = RIGHTSLOW; // P6.1 Right Forward PWM duty cycle  //130000
                       LEFT_FORWARD_SPEED = LEFTSLOW;   // P6.2 Left Forward PWM duty cycle
                       if ((ADC_RIGHT_DETECT > MIDDLELINE) && (ADC_LEFT_DETECT > MIDDLELINE)) {
                           if (ADC_RIGHT_DETECT > ADC_LEFT_DETECT) {
                               // Move left forward
                               LEFT_FORWARD_SPEED = LEFTFAST;
                               RIGHT_FORWARD_SPEED = WHEEL_OFF;
                               LEFT_REVERSE_SPEED = WHEEL_OFF;
                               RIGHT_REVERSE_SPEED = WHEEL_OFF;
                           } else if (ADC_LEFT_DETECT > ADC_RIGHT_DETECT) {
                               // Move right forward
                               LEFT_FORWARD_SPEED = WHEEL_OFF;
                               RIGHT_FORWARD_SPEED = RIGHTFAST;
                               LEFT_REVERSE_SPEED = WHEEL_OFF;
                               RIGHT_REVERSE_SPEED = WHEEL_OFF;
                           } else {
                               // Move forward straight
                               LEFT_FORWARD_SPEED = LEFTFAST;
                               RIGHT_FORWARD_SPEED = RIGHTFAST;
                               LEFT_REVERSE_SPEED = WHEEL_OFF;
                               RIGHT_REVERSE_SPEED = WHEEL_OFF;
                           }
                       } else if ((ADC_RIGHT_DETECT > MIDDLELINE) && (ADC_LEFT_DETECT < MIDDLELINE)) {
                           // Right sensor detects white, turn left
                           LEFT_FORWARD_SPEED = LEFTFAST;
                           RIGHT_FORWARD_SPEED = WHEEL_OFF;
                           LEFT_REVERSE_SPEED = WHEEL_OFF;
                           RIGHT_REVERSE_SPEED = WHEEL_OFF;
                           side = 1;
                       } else if ((ADC_RIGHT_DETECT < MIDDLELINE) && (ADC_LEFT_DETECT > MIDDLELINE)) {
                           // Left sensor detects white, turn right
                           LEFT_FORWARD_SPEED = WHEEL_OFF;
                           RIGHT_FORWARD_SPEED = RIGHTFAST;
                           LEFT_REVERSE_SPEED = WHEEL_OFF;
                           RIGHT_REVERSE_SPEED = WHEEL_OFF;
                           side = 0;
                       } else {
                           // Both sensors detect white, pivot based on the last direction
                           if (side == 1) {
                               // Turn clockwise
                               LEFT_FORWARD_SPEED = WHEEL_OFF;
                               RIGHT_FORWARD_SPEED = RIGHTFAST;
                               LEFT_REVERSE_SPEED = LEFTFAST;    // Reverse left to spin clockwise
                               RIGHT_REVERSE_SPEED = WHEEL_OFF; // No reverse on the right
                           } else if (side == 0) {
                               // Turn counterclockwise
                               LEFT_FORWARD_SPEED = LEFTFAST;
                               RIGHT_FORWARD_SPEED = WHEEL_OFF;
                               LEFT_REVERSE_SPEED = WHEEL_OFF;  // No reverse on the left
                               RIGHT_REVERSE_SPEED = RIGHTFAST; // Reverse right to spin counterclockwise
                           }

                           // Wait until both sensors detect black
                           while (ADC_LEFT_DETECT < MIDDLELINE || ADC_RIGHT_DETECT < MIDDLELINE);

                           // Stop movement after the correction
                           LEFT_FORWARD_SPEED = WHEEL_OFF;
                           RIGHT_FORWARD_SPEED = WHEEL_OFF;
                           LEFT_REVERSE_SPEED = WHEEL_OFF;
                           RIGHT_REVERSE_SPEED = WHEEL_OFF;
                       }


                   }

                 //  update_car_speed(ADC_LEFT_DETECT, ADC_RIGHT_DETECT);


                   // Stop condition: Transition to the next case when timechanged reaches 10.0
                   if (timechanged >= 1100) {
                       RIGHT_FORWARD_SPEED = WHEEL_OFF;
                       LEFT_FORWARD_SPEED = WHEEL_OFF;
                       task++;  // Move to case 6
                       counter = 0;  // Reset counter for case 6
                   }
                   break;

               case 6: // Pause for 3 cycles
                   if (counter < 3) {
                      turn();
                       strcpy(display_line[3], " turning    ");
                   } else {
                       task++;  // Move to case 7
                       counter = 0;  // Reset counter for case 7
                   }
                   break;

               case 7: // Final case: Car moves forward for 3 counter cycles
                   if (counter < 6) {
                       RIGHT_FORWARD_SPEED = RIGHTFAST; // Define your desired speed
                       LEFT_FORWARD_SPEED = RIGHTFAST;

                       RIGHT_REVERSE_SPEED = WHEEL_OFF;
                       LEFT_REVERSE_SPEED = WHEEL_OFF;

                       strcpy(display_line[3], " Moving     ");
                   } else {
                       updatetimerflag = 0;
                       RIGHT_FORWARD_SPEED = WHEEL_OFF;  // Stop the car
                       LEFT_FORWARD_SPEED = WHEEL_OFF;

                       RIGHT_REVERSE_SPEED = WHEEL_OFF;
                       LEFT_REVERSE_SPEED = WHEEL_OFF;

                       strcpy(display_line[3], " Stopped    ");
                       // No further task increment; case 7 is the final state
                   }
                   break;

               default:
                   // Handle unexpected values (reset state)
                   counter = 0;
                   task = 0;

                   break;

           }
    }






 if (switch2_pressed == ENABLED)
 {

 if (ircheck){
  strcpy(display_line[3], "  IR ON  ");
  ircheck = 0;
  switch2_pressed = DISABLED;
  P2OUT |= IR_LED; // Initial Value = Low / Off


} else {
 strcpy(display_line[3], "  IR OFF ");
 ircheck = 1;
 switch2_pressed = DISABLED;
 P2OUT &= ~IR_LED; // Initial Value = Low / Off


}
}


HEXtoBCD(ADC_LEFT_DETECT);
adc_line_1(location);
HEXtoBCD_2(ADC_RIGHT_DETECT);
adc_line_2(location);

timer_update(timechanged);


}  //while loop



}

//update display and display change need to be 1 in code
//update display in interrupts

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
                 display_line[0][5] = adc_char3[0];
                 display_line[0][6] = adc_char3[1];
                 display_line[0][7] = adc_char3[2];
                 display_line[0][8] = '.';               // Add decimal point
                 display_line[0][9] = adc_char3[3];      // Add tenths digit

             }





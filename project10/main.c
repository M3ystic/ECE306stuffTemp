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

unsigned int switch1_pressed = DISABLED;
unsigned int switch2_pressed;

extern unsigned int debounce1_in_progress;
extern unsigned int debounce2_in_progress;

extern int task;
extern unsigned int task_duration;
extern int counter;
extern int counter2;
extern int updateDisplayFlag;

//

extern unsigned int ADC_LEFT_DETECT;
extern unsigned int ADC_RIGHT_DETECT;

extern unsigned int location;


extern float Kp;
extern float Ki;
extern int integral;
extern int error;

unsigned int ircheck;
//char adc_char[4];
//char adc_char2[4];
extern char adc_char3[4];
//
//int location = 1;


extern unsigned int timechanged;
unsigned int updatetimerflag = 0;

int active_switch;



extern char usb_tx_buf [128];
extern char usb_rx_buf [128];
extern char iot_rx_buf [128];
extern char iot_tx_buf [128];



extern unsigned int usb_tx;
extern unsigned int usb_rx;
unsigned int iot_tx;
unsigned int iot_rx;


extern int IOT_RECIEVEDFLAG;
extern int USB_RECIEVEDFLAG;
extern volatile unsigned int send_commands_flag;  // Flag to indicate when to send commands
extern volatile unsigned int send_ssid_command_flag;  // Set flag for sending SSID command
extern volatile unsigned int send_ip_command_flag;   // Set flag for sending IP command

extern volatile unsigned int send_cipmux_command_flag;   // Set flag for sending IP command
extern volatile unsigned int send_cipserver_command_flag;   // Set flag for sending IP command

extern int BAUDRATE115200;
extern int BAUDRATE9600;
extern int A0TX_complete_flag;
extern int A1TX_complete_flag;

const char ssid_command[] = "AT+CWJAP?\r\n";
const char ip_command[] = "AT+CIFSR\r\n";
const char cipmux_command[] = "AT+CIPMUX=1\r\n";
const char cipserver_command[] = "AT+CIPSERVER=1,55555\r\n";
extern char stored_ip[16];


extern char stored_command[6];

unsigned int toggle_flag = 2; // Tracks the toggle state (0: 115200, 1: 460800)

extern unsigned int timerflag;
extern unsigned int timer;        // Timer to handle the delay between commands
extern unsigned int command_sent;  // Static flag to track sent command state


unsigned int moving;

extern unsigned int blacklinefollowing;
extern unsigned int leavingCircle;
extern unsigned int FinishedFlag;
char BL_Stop[11] = " BL Stop! ";     // "BL Stop" fits within 10 characters, null terminator added.

 unsigned int calibratingFlag = FALSE;
extern int calibratingTime;

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
//----  --------------------------------------------------------------------------
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
  Init_LCD();                          // Initialize LCD
  Init_UART_A0();
  Init_UART_A1();
  //Init_motors();
  Init_IOT();
  P1OUT &= ~RED_LED; // Initial Value = Low / Off
  memset(iot_rx_buf, '\0', sizeof(iot_rx_buf));
  memset(usb_rx_buf, '\0', sizeof(usb_rx_buf));

  memset(iot_tx_buf, '\0', sizeof(iot_tx_buf));
  memset(usb_tx_buf, '\0', sizeof(usb_tx_buf));

  LCD_BACKLITE_DIMING = PERCENT_60; // P6.0 Right Forward PWM duty cycle
  RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
  RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
  LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
  LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle


  // Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], " Waiting  ");
  strcpy(display_line[1], "for input ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[3], "          ");

  display_changed = TRUE;


//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------
    while(ALWAYS)
    {                      // Can the Operating system run

    Display_Process();                 // Update Display

    // Logic to handle switch 1 press
    if (((switch1_pressed == ENABLED && active_switch == 0) || active_switch == 1)) {
        // Prevent re-triggering
        switch1_pressed = DISABLED;
        P1OUT |= RED_LED; // Toggle LED for debugging


        calibratingFlag = TRUE;
        calibratingTime = 0;     // Reset calibration time counter

        active_switch = 0;  // Lock processing to Switch 1 while transmitting
    }

    // Logic to handle switch 2 press
    if (((switch2_pressed == ENABLED && active_switch == 0) || active_switch == 2) && !debounce2_in_progress) {
        active_switch = 2; // Lock processing to Switch 2
        switch2_pressed = DISABLED; // Prevent re-triggering

        // Toggle baud rate
        if (toggle_flag == 0) {
            toggle_flag = 1;
            UCA0BRW = BAUD_460800;
            UCA0MCTLW = MCTL_460800;
            UCA1BRW = BAUD_460800;
            UCA1MCTLW = MCTL_460800;
            strcpy(display_line[0], "   Wait   ");
            strcpy(display_line[2], "  460800  ");
        } else {
            toggle_flag = 0;
            UCA0BRW = BAUD_115200;
            UCA0MCTLW = MCTL_115200;
            UCA1BRW = BAUD_115200;
            UCA1MCTLW = MCTL_115200;
            strcpy(display_line[0], "   Wait   ");
            strcpy(display_line[2], "  115200  ");
            TB0CCTL1 |= CCIE; // CCR1 enable interrupt
        }

        // Clear buffers
        memset(iot_rx_buf, '\0', sizeof(iot_rx_buf));
        memset(usb_rx_buf, '\0', sizeof(usb_rx_buf));

        active_switch = 0; // Release active switch after processing
    }


    // Handle IOT received data
    if (IOT_RECIEVEDFLAG == ENABLED) {
        parse_and_execute_commands(iot_rx_buf);
//        if (strstr(iot_rx_buf, "AP:")) {
//            parse_ssid_address(iot_rx_buf);  // Parse and display SSID
//        }

        strcpy(usb_tx_buf, iot_rx_buf); // Copy received data to tx buffer

        iot_rx = 0;
        usb_rx = 0;

        // Start transmitting received data
        iot_tx = 0;
        usb_tx = 0;
        UCA1TXBUF = usb_tx_buf[usb_tx++];  // Send first character
        UCA1IE |= UCTXIE; // Enable TX interrupt

        display_changed = TRUE;
        IOT_RECIEVEDFLAG = DISABLED;
    }

    // Handle USB received data
    if (USB_RECIEVEDFLAG == ENABLED) {
        strcpy(iot_tx_buf, usb_rx_buf); // Copy USB data to IOT buffer

        UCA0TXBUF = iot_tx_buf[iot_tx++];  // Send first character
        UCA0IE |= UCTXIE; // Enable TX interrupt

        USB_RECIEVEDFLAG = DISABLED;
    }

    // Clear transmission buffers upon completion
    if (A1TX_complete_flag) {
        memset(usb_tx_buf, 0, sizeof(usb_tx_buf)); // Clear USB TX buffer
        A1TX_complete_flag = 0;

        if (usb_tx == 0) {
            memset(usb_rx_buf, 0, sizeof(usb_rx_buf));  // Clear USB RX buffer
        }
    }

    if (A0TX_complete_flag) {
        memset(iot_tx_buf, 0, sizeof(iot_tx_buf)); // Clear IOT TX buffer
        A0TX_complete_flag = 0;

        if (iot_tx == 0) {
            memset(iot_rx_buf, 0, sizeof(iot_rx_buf));  // Clear IOT RX buffer
        }
    }

    // If the SSID command flag is set, send SSID command
       if (send_ssid_command_flag) {
           send_at_command(ssid_command);  // Send SSID command
           send_ssid_command_flag = FALSE; // Reset flag after sending
           strcpy(display_line[0], "ssidcheck ");
       }

       // If the IP command flag is set, send IP command
       if (send_ip_command_flag) {
           send_at_command(ip_command);    // Send IP command
           send_ip_command_flag = FALSE;   // Reset flag after sending'
           strcpy(display_line[0], " ipcheck  ");
       }

       // If the CIPMUX command flag is set, send CIPMUX command
       if (send_cipmux_command_flag) {
           send_at_command(cipmux_command);  // Send CIPMUX command
           send_cipmux_command_flag = FALSE; // Reset flag after sending
           strcpy(display_line[0], " cipcheck ");
       }

       // If the CIPSERVER command flag is set, send CIPSERVER command
       if (send_cipserver_command_flag) {
           send_at_command(cipserver_command);  // Send CIPSERVER command
           send_cipserver_command_flag = FALSE; // Reset flag after sending
           strcpy(display_line[0], "  server  ");
       }

    // Handle baud rate switch commands
    if (BAUDRATE115200 == TRUE) {
        UCA0BRW = BAUD_115200;
        UCA0MCTLW = MCTL_115200;
        BAUDRATE115200 = FALSE;
        strcpy(display_line[2], "  115200  ");
    }

    if (BAUDRATE9600 == TRUE) {
        UCA0BRW = BAUD_9600;
        UCA0MCTLW = MCTL_9600;
        BAUDRATE9600 = FALSE;
        strcpy(display_line[2], "   9600   ");
    }

    // Handle motor movement tasks
    switch (task) {
        case 0: // Move Forward
            strncpy(display_line[3], stored_command,5);
            if (counter < task_duration) {
                updatetimerflag= 1;
                ControlForward(); // Function to move forward
            } else {
                stop();
                task = 5;
                counter = 0;
                TB1CCTL1 &= ~CCIE; // Disable timer
            }
            break;

        case 1: // Move Backward
        strncpy(display_line[3], stored_command,5);
        updatetimerflag = 1;
            if (counter < task_duration) {
                backwards();
            } else {
                stop();
                task = 5;
                counter = 0;
                TB1CCTL1 &= ~CCIE; // Disable timer
            }
            break;

        case 2: // Turn Right
            strncpy(display_line[3], stored_command,5);
            updatetimerflag= 1;
            if (counter < task_duration) {
                Cturn_right();
            } else {
                stop();
                task = 5;
                counter = 0;
                TB1CCTL1 &= ~CCIE; // Disable timer
            }
            break;

        case 3: // Turn Left
            P6OUT &= ~LCD_BACKLITE; // Initial Value = Low / Off
            strncpy(display_line[3], stored_command,5);
            updatetimerflag= 1;
            if (counter < task_duration) {
                Cturn_left();
            } else {
                stop();
                task = 5;
                counter = 0;
                TB1CCTL1 &= ~CCIE; // Disable timer
            }
            break;

        case 4:
            P6OUT &= ~LCD_BACKLITE; // Initial Value = Low / Off
                stop();
                task = 5;
                counter = 0;
                TB1CCTL1 &= ~CCIE; // Disable timer
            break;
        default:
            lcd_4line();
            counter = 0;
            task = 5;
            TB1CCTL1 &= ~CCIE; // Disable timer
            break;
    }





    if (timerflag == 1 && send_commands_flag == TRUE) {
        // Timer logic to manage command sending
        P6OUT ^= GRN_LED; // Toggle green LED as an example
        timer++;  // Increment the timer at the beginning of each cycle

        switch (command_sent) {
            case 0:  // Waiting for 6 seconds for the IP command
                if (timer < 600) {  // Wait for 6 seconds (600 * 10ms = 6 seconds)
                    // No need to do anything here since timer is already being incremented
                } else {  // After 6 seconds, send IP command
                    send_ip_command_flag = TRUE; // Set flag for sending IP command
                    command_sent = 1; // Mark IP command as sent
                    timer = 0; // Reset timer for the next command
                }
                break;

            case 1:  // Waiting for 1 second for SSID command
                if (timer >= 100) {  // After 1 second (100 * 10ms = 1 second), send SSID command
                    send_ssid_command_flag = TRUE; // Set flag for sending SSID command
                    command_sent = 2; // Mark SSID command as sent
                    timer = 0; // Reset timer for the next command
                }
                break;

            case 2:  // Waiting for 1 second for CIPMUX command
                if (timer >= 100) {  // After 1 second, send CIPMUX command
                    send_cipmux_command_flag = TRUE; // Set flag for sending CIPMUX command
                    command_sent = 3; // Mark CIPMUX command as sent
                    timer = 0; // Reset timer for the next command
                }
                break;

            case 3:  // Waiting for 1 second for CIPSERVER command
                if (timer >= 100) {  // After 1 second, send CIPSERVER command
                    send_cipserver_command_flag = TRUE; // Set flag for sending CIPSERVER command
                    command_sent = 4; // Mark CIPSERVER command as sent
                    timer = 0; // Reset timer for the next command
                }
                break;

            case 4:  // Stop sending commands after 1 second
                if (timer >= 100) {  // After 1 second, stop sending commands
                    send_commands_flag = FALSE; // Reset the flag to stop sending further commands
                    strcpy(display_line[0] , " Connect  ");
                    moving = 1;
                }
                break;

            default:
                break;
        }

        timerflag = 0;  // Reset the timer flag after processing
    }


                        if (updatetimerflag == 1) {

                            timer_update(timechanged);
                            if (moving)
                            {
                            P6OUT |= LCD_BACKLITE; // Initial Value = Low / Off
                            strcpy(display_line[0], "Arrived 0X");
                            changedIPlines(stored_ip);
                            moving = 0;
                            }
                        }

                        if (blacklinefollowing == TRUE)
                        {
                            go_autonomous();
                        }
                        if (leavingCircle)
                        {
                            end_autonomous();
                        }

                        if (FinishedFlag == TRUE)
                        {
                            strncpy(display_line[0], BL_Stop ,10);
                            strcpy(display_line[1] , "Light Work");
                            strcpy(display_line[2] , "NoReaction");

                            display_line[3][0] = 'T';
                            display_line[3][1] = 'i';
                            display_line[3][2] = 'm';
                            display_line[3][3] = 'e';
                            display_line[3][4] = ':';

                            display_line[3][6] = adc_char3[0];
                            display_line[3][7] = adc_char3[1];
                            display_line[3][8] = adc_char3[2];
                            display_line[3][9] = 's';

                        }




}  //while loop



}




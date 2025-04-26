/*
 * April 2021, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains the main routine for the MSP430 microcontroller, implementing
 * a "While" Operating System. The program initializes all system components and
 * continuously processes display updates, switch inputs, and communication commands.
 * It includes functionalities such as toggling UART baud rates, handling IoT-related
 * AT commands, and managing display updates based on system status.
 */
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <msp430.h>
#include <string.h>
#include "include/functions.h"
#include "include/LCD.h"
#include "include/ports.h"
#include "include/macros.h"
//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Display Variables
//------------------------------------------------------------------------------
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
unsigned char display_mode;
//------------------------------------------------------------------------------
// Timing and Sequence Variables
//------------------------------------------------------------------------------
extern unsigned int timerflag;
extern unsigned int timer;             // Timer for delays between commands
extern unsigned int command_sent;      // Tracks the state of sent commands
extern unsigned int timechanged;
unsigned int updatetimerflag = FALSE;
//------------------------------------------------------------------------------
// Debounce and Switch Variables
//------------------------------------------------------------------------------
unsigned volatile int debounce_count1;
unsigned volatile int debounce_count2;
unsigned int switch1_pressed = DISABLED;
unsigned int switch2_pressed;
extern unsigned int debounce1_in_progress;
extern unsigned int debounce2_in_progress;
int active_switch;
//------------------------------------------------------------------------------
// Task and Movement Variables
//------------------------------------------------------------------------------
extern int task;
extern unsigned int task_duration;
extern int counter;
extern int counter2;
unsigned int moving;
unsigned int toggle_flag = 2; // Tracks toggle state (0: 115200, 1: 460800)
unsigned int calibratingFlag = FALSE;
extern int calibratingTime;
//------------------------------------------------------------------------------
// Sensor and Control Variables
//------------------------------------------------------------------------------
extern char adc_char3[4];
//------------------------------------------------------------------------------
// Communication Buffers and Flags
//------------------------------------------------------------------------------
extern char usb_tx_buf[128];
extern char usb_rx_buf[128];
extern char iot_rx_buf[128];
extern char iot_tx_buf[128];
extern unsigned int usb_tx;
extern unsigned int usb_rx;
unsigned int iot_tx;
unsigned int iot_rx;
extern int IOT_RECIEVEDFLAG;
extern int USB_RECIEVEDFLAG;
//------------------------------------------------------------------------------
// Command Flags
//------------------------------------------------------------------------------
extern volatile unsigned int send_commands_flag;       // Indicates when to send commands
extern volatile unsigned int send_ssid_command_flag;   // Flag for sending SSID command
extern volatile unsigned int send_ip_command_flag;     // Flag for sending IP command
extern volatile unsigned int send_cipmux_command_flag; // Flag for sending CIPMUX command
extern volatile unsigned int send_cipserver_command_flag; // Flag for sending CIPSERVER command

//------------------------------------------------------------------------------
// UART and Baud Rate Variables
//------------------------------------------------------------------------------
extern int BAUDRATE115200;
extern int BAUDRATE9600;
extern int A0TX_complete_flag;
extern int A1TX_complete_flag;
//------------------------------------------------------------------------------
// Stored Commands and IP
//------------------------------------------------------------------------------
extern char stored_ip[16];
extern char stored_command[6];
const char ssid_command[] = "AT+CWJAP?\r\n";
const char ip_command[] = "AT+CIFSR\r\n";
const char cipmux_command[] = "AT+CIPMUX=1\r\n";
const char cipserver_command[] = "AT+CIPSERVER=1,55555\r\n";
//------------------------------------------------------------------------------
// Autonomous Behavior Variables
//------------------------------------------------------------------------------
extern unsigned int blacklinefollowing;
extern unsigned int leavingCircle;
extern unsigned int FinishedFlag;
//------------------------------------------------------------------------------
// Miscellaneous Variables
//------------------------------------------------------------------------------
volatile char slow_input_down;
unsigned int test_value;
char chosen_direction;
char change;
char BL_Stop[11] = " BL Stop! "; // "BL Stop" fits within 10 characters, null terminator added.
//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// System Configurations

// Tells the compiler to provide the value in reg as an input to an
// inline assembly block.  Even though the block contains no instructions,
// the compiler may not optimize it away, and is told that the value
// may change and should not be relied upon.

//inline void READ_AND_DISCARD(unsigned int reg) __attribute__((always_inline));
//void READ_AND_DISCARD(unsigned int reg){
//  asm volatile ("" : "=m" (reg) : "r" (reg));
//}

//inline void enable_interrupts(void) __attribute__((always_inline));
void enable_interrupts(void){
  __bis_SR_register(GIE);     // enable interrupts
//  asm volatile ("eint \n");
}

//inline void disable_interrupts(void) __attribute__((always_inline));
//void disable_interrupts(void){
//  asm volatile ("dint \n");
//}


//------------------------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------------------------

// Initialize hardware components
void initialize_hardware(void) {
    Init_Ports();
    Init_Clocks();
    Init_Conditions();
    Init_DAC();
    Init_timer_B0();
    Init_timer_B1();
    Init_Timer_B3();
    Init_ADC();
    Init_LCD();
    Init_UART_A0();
    Init_UART_A1();
    Init_IOT();

    P1OUT &= ~RED_LED; // Turn off RED LED
    memset(iot_rx_buf, '\0', sizeof(iot_rx_buf));
    memset(usb_rx_buf, '\0', sizeof(usb_rx_buf));
    memset(iot_tx_buf, '\0', sizeof(iot_tx_buf));
    memset(usb_tx_buf, '\0', sizeof(usb_tx_buf));

    LCD_BACKLITE_DIMING = PERCENT_60;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}

// Handle switch presses
void handle_switches(void) {
    if (((switch1_pressed == ENABLED && active_switch == FALSE) || active_switch == TRUE)) {
        switch1_pressed = DISABLED;
        P1OUT |= RED_LED; // Debugging LED
        calibratingFlag = TRUE;
        calibratingTime = RESET_STATE;
        active_switch = FALSE;
    }

    if (((switch2_pressed == ENABLED && active_switch == FALSE) || active_switch == 2)) {
        active_switch = 2;
        switch2_pressed = DISABLED;

        if (toggle_flag == FALSE) {
            toggle_flag = TRUE;
            UCA0BRW = BAUD_460800;
            UCA0MCTLW = MCTL_460800;
            UCA1BRW = BAUD_460800;
            UCA1MCTLW = MCTL_460800;
            strcpy(display_line[0], "   Wait   ");
            strcpy(display_line[2], "  460800  ");
        } else {
            toggle_flag = FALSE;
            UCA0BRW = BAUD_115200;
            UCA0MCTLW = MCTL_115200;
            UCA1BRW = BAUD_115200;
            UCA1MCTLW = MCTL_115200;
            strcpy(display_line[0], "   Wait   ");
            strcpy(display_line[2], "  115200  ");
            TB0CCTL1 |= CCIE; // Enable CCR1 interrupt
        }

        clear_buffers();
        active_switch = FALSE;
    }
}

// Handle received data from IOT and USB
void handle_received_data(void) {
    if (IOT_RECIEVEDFLAG == ENABLED) {
        parse_and_execute_commands(iot_rx_buf);
        strcpy(usb_tx_buf, iot_rx_buf);
        iot_rx = RESET_STATE;
        usb_rx = RESET_STATE;
        iot_tx = RESET_STATE;
        usb_tx = RESET_STATE;
        UCA1TXBUF = usb_tx_buf[usb_tx++];
        UCA1IE |= UCTXIE;
        display_changed = TRUE;
        IOT_RECIEVEDFLAG = DISABLED;
    }

    if (USB_RECIEVEDFLAG == ENABLED) {
        strcpy(iot_tx_buf, usb_rx_buf);
        UCA0TXBUF = iot_tx_buf[iot_tx++];
        UCA0IE |= UCTXIE;
        USB_RECIEVEDFLAG = DISABLED;
    }

    clear_buffers();
}

// Handle command flags and timers
void handle_commands(void) {
    if (send_ssid_command_flag) {
        send_at_command(ssid_command);
        send_ssid_command_flag = FALSE;
        strcpy(display_line[0], "ssidcheck ");
    }

    if (send_ip_command_flag) {
        send_at_command(ip_command);
        send_ip_command_flag = FALSE;
        strcpy(display_line[0], " ipcheck  ");
    }

    if (send_cipmux_command_flag) {
        send_at_command(cipmux_command);
        send_cipmux_command_flag = FALSE;
        strcpy(display_line[0], " cipcheck ");
    }

    if (send_cipserver_command_flag) {
        send_at_command(cipserver_command);
        send_cipserver_command_flag = FALSE;
        strcpy(display_line[0], "  server  ");
    }
}

// Handle motor movement tasks
void handle_tasks(void) {
    switch (task) {
        case 0: // Move Forward
            strncpy(display_line[3], stored_command, 5);
            if (counter < task_duration) {
                updatetimerflag = TRUE;
                ControlForward();
            } else {
                stop();
                task = 5;
                counter = RESET_STATE;
                TB1CCTL1 &= ~CCIE;
            }
            break;

        case 1: // Move Backward
            strncpy(display_line[3], stored_command, 5);
            updatetimerflag = TRUE;
            if (counter < task_duration) {
                backwards();
            } else {
                stop();
                task = 5;
                counter = RESET_STATE;
                TB1CCTL1 &= ~CCIE;
            }
            break;

        case 2: // Turn Right
            strncpy(display_line[3], stored_command, 5);
            updatetimerflag = TRUE;
            if (counter < task_duration) {
                Cturn_right();
            } else {
                stop();
                task = 5;
                counter = RESET_STATE;
                TB1CCTL1 &= ~CCIE;
            }
            break;

        case 3: // Turn Left
            P6OUT &= ~LCD_BACKLITE;
            strncpy(display_line[3], stored_command, 5);
            updatetimerflag = TRUE;
            if (counter < task_duration) {
                Cturn_left();
            } else {
                stop();
                task = 5;
                counter = RESET_STATE;
                TB1CCTL1 &= ~CCIE;
            }
            break;

        default:
            lcd_4line();
            counter = RESET_STATE;
            task = 5;
            TB1CCTL1 &= ~CCIE;
            break;
    }
}

// Clear transmission buffers
void clear_buffers(void) {
    if (A1TX_complete_flag) {
        memset(usb_tx_buf, RESET_STATE, sizeof(usb_tx_buf));
        A1TX_complete_flag = FALSE;

        if (usb_tx == 0) {
            memset(usb_rx_buf, RESET_STATE, sizeof(usb_rx_buf));
        }
    }

    if (A0TX_complete_flag) {
        memset(iot_tx_buf, RESET_STATE, sizeof(iot_tx_buf));
        A0TX_complete_flag = FALSE;

        if (iot_tx == RESET_STATE) {
            memset(iot_rx_buf, RESET_STATE, sizeof(iot_rx_buf));
        }
    }
}



// Handle sending commands
void handle_sending_commands(void) {
    if (timerflag == TRUE && send_commands_flag == TRUE) {
        P6OUT ^= GRN_LED; // Toggle green LED as an example
        timer++; // Increment the timer at the beginning of each cycle

        switch (command_sent) {
            case 0: // Waiting for 6 seconds for the IP command
                if (timer >= 600) { // After 6 seconds, send IP command
                    send_ip_command_flag = TRUE; // Set flag for sending IP command
                    command_sent = 1; // Mark IP command as sent
                    timer = RESET_STATE; // Reset timer for the next command
                }
                break;

            case 1: // Waiting for 1 second for SSID command
                if (timer >= 100) { // After 1 second, send SSID command
                    send_ssid_command_flag = TRUE; // Set flag for sending SSID command
                    command_sent = 2; // Mark SSID command as sent
                    timer = RESET_STATE; // Reset timer for the next command
                }
                break;

            case 2: // Waiting for 1 second for CIPMUX command
                if (timer >= 100) { // After 1 second, send CIPMUX command
                    send_cipmux_command_flag = TRUE; // Set flag for sending CIPMUX command
                    command_sent = 3; // Mark CIPMUX command as sent
                    timer = RESET_STATE; // Reset timer for the next command
                }
                break;

            case 3: // Waiting for 1 second for CIPSERVER command
                if (timer >= 100) { // After 1 second, send CIPSERVER command
                    send_cipserver_command_flag = TRUE; // Set flag for sending CIPSERVER command
                    command_sent = 4; // Mark CIPSERVER command as sent
                    timer = RESET_STATE; // Reset timer for the next command
                }
                break;

            case 4: // Stop sending commands after 1 second
                if (timer >= 100) { // After 1 second, stop sending commands
                    send_commands_flag = FALSE; // Reset the flag to stop sending further commands
                    strcpy(display_line[0], " Connect  ");
                    moving = TRUE;
                }
                break;

            default:
                break;
        }

        timerflag = FALSE; // Reset the timer flag after processing
    }
}



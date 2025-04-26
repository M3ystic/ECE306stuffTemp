/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file configures UART modules for communication and manages baud rate settings.
 * It also provides interrupt service routines for UART communication.
 */

 #include "include\\macros.h"
 #include "include\\ports.h"
 #include "include\\functions.h"
 #include "msp430.h"
 #include <string.h>
 
 // Flags for UART transmission and reception
 int A0TX_complete_flag;
 int A1TX_complete_flag;
 int IOT_RECIEVEDFLAG = DISABLED;
 int USB_RECIEVEDFLAG = DISABLED;
 
 // Baud rate configurations
 int BAUDRATE115200;
 int BAUDRATE9600;
 
 // Command detection
 int command_detected = FALSE;
 
 // Buffers for UART communication
 unsigned int usb_tx, usb_rx;
 unsigned int iot_tx, iot_rx;
 char usb_tx_buf[128];
 char usb_rx_buf[128];
 char iot_rx_buf[128];
 char iot_tx_buf[128];
 
 // Buffer for processing commands
 char process_buffer[25];
 char pb_index; // Index for process_buffer
 
 // External variables
 extern int active_switch;
 

 void Init_UART_A0(void) {
     UCA0CTLW0 |= UCSWRST; // Put eUSCI in reset
     UCA0CTLW0 |= UCSSEL__SMCLK; // Use SMCLK as clock source
     UCA0CTLW0 &= ~(UCMSB | UCSPB | UCPEN); // Configure communication settings
 
     // Configure UART pins
     P1SEL1 &= ~(UCA0TXD | UCA0RXD);
     P1SEL0 |= UCA0TXD | UCA0RXD;
 
     UCA0CTLW0 &= ~UCSWRST; // Release from reset
     UCA0IE |= UCRXIE; // Enable RX interrupt
 }
 
 /**
  *  Initializes UART module A1 for communication.
  */
 void Init_UART_A1(void) {
     UCA1CTLW0 = UCSWRST; // Put eUSCI in reset
     UCA1CTLW0 |= UCSSEL__SMCLK; // Use SMCLK as clock source
     UCA1CTLW0 &= ~(UCMSB | UCSPB | UCPEN); // Configure communication settings
 
     UCA1BRW = 4; // Set baud rate to 115,200
     UCA1MCTLW = 0x5551; // Modulation settings
 
     // Configure UART pins
     P4SEL1 &= ~(UCA1TXD | UCA1RXD);
     P4SEL0 |= UCA1TXD | UCA1RXD;
 
     UCA1CTLW0 &= ~UCSWRST; // Release from reset
     UCA1IE |= UCRXIE; // Enable RX interrupt
 }
 
 /**
  *  Initializes IOT-related configurations.
  */
 void Init_IOT(void) {
     P5OUT |= IOT_BOOT_CPU; // Enable IOT boot
     P3OUT &= ~IOT_EN_CPU; // Disable IOT
     P3OUT |= IOT_LINK_CPU; // Set initial value for IOT link
     P2OUT |= IOT_RUN_RED; // Set initial value for IOT run LED
 }
 
 /**
  *  UART A0 interrupt service routine.
  */
 #pragma vector=EUSCI_A0_VECTOR
 __interrupt void eUSCI_A0_ISR(void) {
     char iot_receive;
     static char prev_char = RESET_STATE;
 
     switch (__even_in_range(UCA0IV, 0x08)) {
         case 2: // RX interrupt
             iot_receive = UCA0RXBUF;
             iot_rx_buf[iot_rx++] = iot_receive;
             P2OUT ^= IOT_RUN_RED; // Toggle IOT run LED
 
             if (prev_char == 0x0D || iot_receive == 0x0A) { // End of line
                 IOT_RECIEVEDFLAG = ENABLED;
                 iot_rx = RESET_STATE; // Reset buffer
                 TB0CCTL1 |= CCIE; // Enable CCR1 interrupt
             }
             prev_char = iot_receive;
             break;
 
         case 4: // TX interrupt
             if (iot_tx_buf[iot_tx] != '\0') {
                 UCA0TXBUF = iot_tx_buf[iot_tx++]; // Transmit next character
             } else {
                 UCA0IE &= ~UCTXIE; // Disable TX interrupt
                 iot_tx = RESET_STATE; // Reset index
                 active_switch = FALSE; // Allow other switches
                 A0TX_complete_flag = TRUE; // Mark transmission complete
             }
             break;
 
         default:
             break;
     }
 }
 
 /**
  *  UART A1 interrupt service routine.
  */
 #pragma vector=EUSCI_A1_VECTOR
 __interrupt void eUSCI_A1_ISR(void) {
     char usb_receive;
     static char prev_char = RESET_STATE;
 
     switch (__even_in_range(UCA1IV, 0x08)) {
         case 2: // RX interrupt
             usb_receive = UCA1RXBUF;
             usb_rx_buf[usb_rx++] = usb_receive;
             P2OUT ^= IOT_RUN_RED; // Toggle IOT run LED
 
             if (prev_char == 0x0D || usb_receive == 0x0A) { // End of line
                 USB_RECIEVEDFLAG = ENABLED;
                 command_detected = FALSE;
 
                 if (strstr(usb_rx_buf, "^F")) {
                     BAUDRATE115200 = TRUE;
                     command_detected = TRUE;
                 }
                 if (strstr(usb_rx_buf, "^S")) {
                     BAUDRATE9600 = TRUE;
                     command_detected = TRUE;
                 }
                 usb_rx = RESET_STATE; // Reset buffer
             }
             prev_char = usb_receive;
             break;
 
         case 4: // TX interrupt
             if (usb_tx_buf[usb_tx] != '\0') {
                 UCA1TXBUF = usb_tx_buf[usb_tx++]; // Transmit next character
             } else {
                 UCA1IE &= ~UCTXIE; // Disable TX interrupt
                 usb_tx = RESET_STATE; // Reset index
                 active_switch = FALSE; // Allow other switches
                 A1TX_complete_flag = TRUE; // Mark transmission complete
             }
             break;
 
         default:
             break;
     }
 }

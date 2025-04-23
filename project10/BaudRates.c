/*
 * BaudRates.c
 *
 *  Created on: Mar 26, 2025
 *      Author: Dtwen
 */

#include "include\macros.h"
#include "include\ports.h"
#include "include\functions.h"
#include "msp430.h"
#include <string.h>

int A0TX_complete_flag;
int A1TX_complete_flag;

int IOT_RECIEVEDFLAG = DISABLED;
int USB_RECIEVEDFLAG = DISABLED;

int BAUDRATE115200;
int BAUDRATE9600;

int command_detected = FALSE;

unsigned int usb_tx;
unsigned int usb_rx;

unsigned int iot_tx;
unsigned int iot_rx;

char usb_tx_buf[128];
char usb_rx_buf[128];
char iot_rx_buf[128];
char iot_tx_buf[128];

// Global Variables
char process_buffer[25];
char pb_index; // Size for appropriate Command Length
// Index for process_buffer

extern int active_switch;

void Init_UART_A0(void) {
    // Configure UART 0
    UCA0CTLW0 |= UCSWRST; // Set Software reset enable
    UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK

    UCA0CTLW0 &= ~UCMSB;            // MSB, LSB select
    UCA0CTLW0 &= ~UCSPB;            // UCSPB = 0(1 stop bit) OR 1(2 stop bits)

    UCA0CTLW0 &= ~UCPEN;

    // eUSCIA0 -- A0 transmit & receive --
    P1SEL1 &= ~UCA0TXD; // USCI_A0 UART operation
    P1SEL0 |= UCA0TXD; // USCI_A0 UART operation

    P1SEL1 &= ~UCA0RXD; // USCI_A0 UART operation
    P1SEL0 |= UCA0RXD; // USCI_A0 UART operation

    UCA0CTLW0 &= ~ UCSWRST; // Set Software reset enable
    UCA0IE |= UCRXIE; // Enable RX interrupt
}

void Init_UART_A1(void) {
    // Configure UART 1 (UCA1)
    UCA1CTLW0 = UCSWRST;               // Put eUSCI in reset state
    UCA1CTLW0 |= UCSSEL__SMCLK;        // Select SMCLK as BRCLK source

    UCA1CTLW0 &= ~UCMSB;            // MSB, LSB select
    UCA1CTLW0 &= ~UCSPB;            // UCSPB = 0(1 stop bit) OR 1(2 stop bits)

    UCA1CTLW0 &= ~UCPEN;

    UCA1BRW = 4; // 115,200 baud
    UCA1MCTLW = 0x5551;

    // eUSCIA1 -- A1 transmit & receive --
    P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation
    P4SEL0 |= UCA1TXD; // USCI_A1 UART operation

    P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation
    P4SEL0 |= UCA1RXD; // USCI_A1 UART operation

    UCA1CTLW0 &= ~ UCSWRST; // Set Software reset enable
    UCA1IE |= UCRXIE;                // Enable RX interrupt
}

void Init_IOT(void) {
    P5OUT |= IOT_BOOT_CPU; // Initial Value = ON/Output
    P3OUT &= ~IOT_EN_CPU; // Initial Value = Low / Off

    //dont cares
    P3OUT |= IOT_LINK_CPU; // Initial Value = Low / Off
    P2OUT |= IOT_RUN_RED; // Initial Value = Low / Off
}

#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void) {
    char iot_receive;
    static char prev_char = 0;
    switch(__even_in_range(UCA0IV, 0x08)) {
        case 0:
            break; // Vector 0 - no interrupt
        case 2:
            // In A0 RX
            iot_receive = UCA0RXBUF;
            iot_rx_buf[iot_rx++] = iot_receive;
            P2OUT ^= IOT_RUN_RED; // Initial Value = Low / Off
            if (prev_char == 0x0D || iot_receive == 0x0A) {  // received "\r\n"
            //  iot_rx_buf[usb_rx] = '\0'; // Null-terminate
                IOT_RECIEVEDFLAG = ENABLED;
                iot_rx = 0;
                TB0CCTL1 |= CCIE; // CCR1 enable interrupt

            }

            prev_char = iot_receive;
            break;
        case 4:
            if (iot_tx_buf[iot_tx] != '\0') {
                // Send the next character from usb_tx_buf
                UCA0TXBUF = iot_tx_buf[iot_tx];
                iot_tx++;  // Move to the next character
            } else {
                // End of string, stop transmission
                UCA0IE &= ~UCTXIE;  // Disable TX interrupt to stop transmission
                iot_tx = 0;  // Reset tx index to 0 for the next transmission
                active_switch = 0;  // Allow other switch presses
                A0TX_complete_flag = 1;  // Mark that we're done
            }
            break;
        default:
            break;
    }
}

#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void) {
    char usb_receive;
    static char prev_char = 0;
    switch(__even_in_range(UCA1IV, 0x08)) {
        case 0:
            break; // Vector 0 - no interrupt
        case 2:
            usb_receive = UCA1RXBUF;

            // Store character
            usb_rx_buf[usb_rx++] = usb_receive;
            P2OUT ^= IOT_RUN_RED; // Initial Value = Low / Off
            // If carriage return, handle end-of-message logic
            if (prev_char == 0x0D || usb_receive == 0x0A) {
            //  usb_rx_buf[usb_rx] = '\0'; // Null-terminate
                USB_RECIEVEDFLAG = ENABLED;
                command_detected = FALSE;
                if (strstr(usb_rx_buf, "^F") != NULL) {
                    BAUDRATE115200 = TRUE;
                    command_detected = TRUE;
                }
                if (strstr(usb_rx_buf, "^S") != NULL) {
                    BAUDRATE9600 = TRUE;
                    command_detected = TRUE;
                }

                usb_rx = 0; // Reset buffer for next message
            }

            prev_char = usb_receive;
            break;
        case 4:
            if (usb_tx_buf[usb_tx] != '\0') {
                UCA1TXBUF = usb_tx_buf[usb_tx++];  // Send and move to next
            } else {
                // End of string, stop transmission
                UCA1IE &= ~UCTXIE;  // Disable TX interrupt
                usb_tx = 0;  // Reset for next transmission
                active_switch = 0;
                A1TX_complete_flag = 1;  // Mark that we're done
            }
            break;
        default:
            break;
    }
}

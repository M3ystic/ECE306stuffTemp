/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file configures UART modules for communication and manages baud rate settings.
 * It also provides interrupt service routines for UART communication.
 */
//------------------------------------------------------------------------------
#include "include\\macros.h"
#include "include\\ports.h"
#include "include\\functions.h"
#include "msp430.h"
#include <string.h>
//------------------------------------------------------------------------------
// Flags for UART transmission and reception
//------------------------------------------------------------------------------
int A0TX_complete_flag;
int A1TX_complete_flag;
int IOT_RECIEVEDFLAG = DISABLED;
int CLIENT_RECIEVEDFLAG = DISABLED;
//------------------------------------------------------------------------------
// Baud Rate Configurations
//------------------------------------------------------------------------------
int BAUDRATE115200;
int BAUDRATE9600;
//------------------------------------------------------------------------------
// Command Detection
//------------------------------------------------------------------------------
int command_detected = FALSE;
//------------------------------------------------------------------------------
// Buffers for UART Communication
//------------------------------------------------------------------------------
unsigned int CLIENT_tx, CLIENT_rx;
unsigned int IOTmodule_tx, IOTmodule_rx;
char CLIENT_tx_buf[BUFFERSIZE];
char CLIENT_rx_buf[BUFFERSIZE];
char IOTmodule_rx_buf[BUFFERSIZE];
char IOTmodule_tx_buf[BUFFERSIZE];
//------------------------------------------------------------------------------
// External Variables
//------------------------------------------------------------------------------
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
    char IOTmodule_receive;
    static char prev_char = RESET_STATE;

    switch (__even_in_range(UCA0IV, 0x08)) {
        case 2: // RX interrupt
            IOTmodule_receive = UCA0RXBUF;
            IOTmodule_rx_buf[IOTmodule_rx++] = IOTmodule_receive;
            P2OUT ^= IOT_RUN_RED; // Toggle IOT run LED

            if (prev_char == 0x0D || IOTmodule_receive == 0x0A) { // End of line
                IOT_RECIEVEDFLAG = ENABLED;
                IOTmodule_rx = RESET_STATE; // Reset buffer
                TB0CCTL1 |= CCIE; // Enable CCR1 interrupt
            }
            prev_char = IOTmodule_receive;
            break;

        case 4: // TX interrupt
            if (IOTmodule_tx_buf[IOTmodule_tx] != '\0') {
                UCA0TXBUF = IOTmodule_tx_buf[IOTmodule_tx++]; // Transmit next character
            } else {
                UCA0IE &= ~UCTXIE; // Disable TX interrupt
                IOTmodule_tx = RESET_STATE; // Reset index
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
    char CLIENT_receive;
    static char prev_char = RESET_STATE;

    switch (__even_in_range(UCA1IV, 0x08)) {
        case 2: // RX interrupt
            CLIENT_receive = UCA1RXBUF;
            CLIENT_rx_buf[CLIENT_rx++] = CLIENT_receive;
            P2OUT ^= IOT_RUN_RED; // Toggle IOT run LED

            if (prev_char == 0x0D || CLIENT_receive == 0x0A) { // End of line
                CLIENT_RECIEVEDFLAG = ENABLED;
                command_detected = FALSE;

                if (strstr(CLIENT_rx_buf, "^F")) {
                    BAUDRATE115200 = TRUE;
                    command_detected = TRUE;
                }
                if (strstr(CLIENT_rx_buf, "^S")) {
                    BAUDRATE9600 = TRUE;
                    command_detected = TRUE;
                }
                CLIENT_rx = RESET_STATE; // Reset buffer
            }
            prev_char = CLIENT_receive;
            break;

        case 4: // TX interrupt
            if (CLIENT_tx_buf[CLIENT_tx] != '\0') {
                UCA1TXBUF = CLIENT_tx_buf[CLIENT_tx++]; // Transmit next character
            } else {
                UCA1IE &= ~UCTXIE; // Disable TX interrupt
                CLIENT_tx = RESET_STATE; // Reset index
                active_switch = FALSE; // Allow other switches
                A1TX_complete_flag = TRUE; // Mark transmission complete
            }
            break;

        default:
            break;
    }
}

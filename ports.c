/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains functions for initializing various ports on the MSP430 microcontroller.
 * Each port's pin configuration is set based on the defined macros for specific peripherals.
 * The ports are initialized for different functions such as GPIO operation, input/output,
 * UART, and various other specific functionalities for sensors and peripherals.
 */


#include "include\macros.h"
#include "include\ports.h"
#include "include\functions.h"
#include "msp430.h"



void Init_Ports(void)
{

    Init_Port1();
    Init_Port2();
    Init_Port3();
    Init_Port4();
    Init_Port5();
    Init_Port6();

}

void Init_Port1(void)
{
    P1OUT = 0x00; // P2 set Low
    P1DIR = 0x00; // Set P2 direction to output

    P1SEL0 &= ~RED_LED; // SLOW_CLK GPIO operation
    P1SEL1 &= ~RED_LED; // SLOW_CLK GPIO operation
    P1OUT &= ~RED_LED; // Initial Value = Low / Off
    P1DIR |= RED_LED; // Direction = output

    P1SEL0 |= V_A1_SEEED; // CHECK_BAT GPIO operation
    P2SEL1 |= V_A1_SEEED; // CHECK_BAT GPIO operation
    P1OUT &= ~V_A1_SEEED; // Initial Value = Low / Off
    P1DIR &= ~V_A1_SEEED; // Direction = input

    P1SEL0 |= V_DETECT_L; // CHECK_BAT GPIO operation
    P1SEL1 |= V_DETECT_L; // CHECK_BAT GPIO operation
    P1OUT &= ~V_DETECT_L; // Initial Value = Low / Off
    P1DIR &= ~V_DETECT_L; // Direction = input

    P1SEL0 |= V_DETECT_R; // CHECK_BAT GPIO operation
    P1SEL1 |= V_DETECT_R; // CHECK_BAT GPIO operation
    P1OUT &= ~V_DETECT_R; // Initial Value = Low / Off
    P1DIR &= ~V_DETECT_R; // Direction = input

    P1SEL0 |= V_A4_SEEED; // CHECK_BAT GPIO operation
    P1SEL1 |= V_A4_SEEED; // CHECK_BAT GPIO operation
    P1OUT &= ~V_A4_SEEED; // Initial Value = Low / Off
    P1DIR &= ~V_A4_SEEED; // Direction = input

    P1SEL0 |= V_THUMB; // CHECK_BAT GPIO operation
    P1SEL1 |= V_THUMB; // CHECK_BAT GPIO operation
    P1OUT &= ~V_THUMB; // Initial Value = Low / Off
    P1DIR &= ~V_THUMB; // Direction = input

    P1SEL0 &= ~UCA0RXD; // DAC_ENB GPIO operation
    P1SEL1 |= UCA0RXD; // DAC_ENB GPIO operation
    P1OUT &= ~UCA0RXD; // Initial Value = High
    P1DIR &= ~UCA0RXD; // Direction = input

    P1SEL0 &= ~UCA0TXD; // DAC_ENB GPIO operation
    P1SEL1 |= UCA0TXD; // DAC_ENB GPIO operation
    P1OUT &= ~UCA0TXD; // Initial Value = High
    P1DIR &= ~UCA0TXD; // Direction = input

}

void Init_Port2(void)
{
    P2OUT = 0x00; // P2 set Low
    P2DIR = 0x00; // Set P2 direction to output

    P2SEL0 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
    P2SEL1 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
    P2OUT &= ~SLOW_CLK; // Initial Value = Low / Off
    P2DIR |= SLOW_CLK; // Direction = output

    P2SEL0 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
    P2SEL1 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
    P2OUT &= ~CHECK_BAT; // Initial Value = Low / Off
    P2DIR |= CHECK_BAT; // Direction = output

    P2SEL0 &= ~IR_LED; // P2_2 GPIO operation
    P2SEL1 &= ~IR_LED; // P2_2 GPIO operation
    P2OUT |= IR_LED; // Initial Value = Low / Off
    P2DIR |= IR_LED; // Direction = output

    P2SEL0 &= ~SW2; // SW1 set as I/0
    P2SEL1 &= ~SW2; // SW1 set as I/0
    P2DIR &= ~SW2; // SW1 Direction = input
    P2PUD |= SW2; // Configure pull-up resistor SW1
    P2REN |= SW2; // Enable pull-up resistor SW1
    P2IES |= SW2; // SW1 Hi/Lo edge interrupt
    P2IFG &= ~SW2; // IFG SW1 cleared
    P2IE |= SW2; // SW1 interrupt Enabled

    P2SEL0 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
    P2SEL1 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
    P2OUT &= ~IOT_RUN_RED; // Initial Value = Low / Off
    P2DIR |= IOT_RUN_RED; // Direction = output

    P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
    P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
    P2OUT |= DAC_ENB; // Initial Value = High
    P2DIR |= DAC_ENB; // Direction = output

    P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
    P2SEL1 |= LFXOUT; // LFXOUT Clock operation
    P2SEL0 &= ~LFXIN; // LFXIN Clock operation
    P2SEL1 |= LFXIN; // LFXIN Clock operation
}

void Init_Port3(void)
{
    P3OUT = 0x00; // P2 set Low
    P3DIR = 0x00; // Set P2 direction to output

    P3SEL0 &= ~TEST_PROBE; // SLOW_CLK GPIO operation
    P3SEL1 &= ~TEST_PROBE; // SLOW_CLK GPIO operation
    P3OUT &= ~TEST_PROBE; // Initial Value = Low / Off
    P3DIR |= TEST_PROBE; // Direction = output

    P3SEL0 &= ~OA2O; // SLOW_CLK GPIO operation
    P3SEL1 &= ~OA2O; // SLOW_CLK GPIO operation
    P3OUT &= ~OA2O; // Initial Value = Low / Off
    P3DIR &= ~OA2O; // Direction = output


    P3SEL0 &= ~OA2N; // SLOW_CLK GPIO operation
    P3SEL1 &= ~OA2N; // SLOW_CLK GPIO operation
    P3OUT &= ~OA2N; // Initial Value = Low / Off
    P3DIR &= ~OA2N; // Direction = output


    P3SEL0 &=~ OA2P; // SLOW_CLK GPIO operation
    P3SEL1 &=~ OA2P; // SLOW_CLK GPIO operation
    P3OUT &= ~OA2P; // Initial Value = Low / Off
    P3DIR &= ~OA2P; // Direction = input


    P3SEL0 &= ~SMCLK; // SLOW_CLK GPIO operation
    P3SEL1 &= ~SMCLK; // SLOW_CLK GPIO operation
    P3OUT &= ~SMCLK; // Initial Value = Low / Off
    P3DIR |= SMCLK; // Direction = output

    P3SEL0 &= ~DAC_CNTL; // SLOW_CLK GPIO operation
    P3SEL1 &= ~DAC_CNTL; // SLOW_CLK GPIO operation
    P3OUT &= ~DAC_CNTL; // Initial Value = Low / Off
    P3DIR |= DAC_CNTL; // Direction = output

    P3SEL0 &= ~IOT_LINK_CPU; // SLOW_CLK GPIO operation
    P3SEL1 &= ~IOT_LINK_CPU; // SLOW_CLK GPIO operation
    P3OUT &= ~IOT_LINK_CPU; // Initial Value = Low / Off
    P3DIR |= IOT_LINK_CPU; // Direction = output

    P3SEL0 &= ~IOT_EN_CPU; // SLOW_CLK GPIO operation
    P3SEL1 &= ~IOT_EN_CPU; // SLOW_CLK GPIO operation
    P3OUT &= ~IOT_EN_CPU; // Initial Value = Low / Off
    P3DIR |= IOT_EN_CPU; // Direction = output
}

void Init_Port4(void)
{
    P4OUT = 0x00; // P4 set Low
    P4DIR = 0x00; // Set P4 direction to output

    P4SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P4SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operationx
    P4OUT &= ~RESET_LCD; // Initial Value = Low / Off
    P4DIR |= RESET_LCD; // Direction = output

//    P4SEL0 &= ~SW1; // SW1 GPIO operation
//    P4SEL1 &= ~SW1; // SW1 GPIO operation
//    P4OUT |= SW1; // Configure pullup resistor
//    P4DIR &= ~SW1; // Direction = input
//    P4REN |= SW1; // Enable pullup resistor

    P4SEL0 &= ~SW1; // SW1 set as I/0
    P4SEL1 &= ~SW1; // SW1 set as I/0
    P4DIR &= ~SW1; // SW1 Direction = input
    P4PUD |= SW1; // Configure pull-up resistor SW1
    P4REN |= SW1; // Enable pull-up resistor SW1
    P4IES |= SW1; // SW1 Hi/Lo edge interrupt
    P4IFG &= ~SW1; // IFG SW1 cleared
    P4IE |= SW1; // SW1 interrupt Enabled


    P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

    P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation

    P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
    P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
    P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
    P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output

    P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
    P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

    P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
    P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

    P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
    P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
}

void Init_Port5(void)
{
    P5OUT = 0x00; // P2 set Low
    P5DIR = 0x00; // Set P2 direction to output

    P5SEL0 &= ~V_BAT; // SLOW_CLK GPIO operation
    P5SEL1 &= ~V_BAT; // SLOW_CLK GPIO operation
    P5OUT &= ~V_BAT; // Initial Value = Low / Off
    P5DIR |= V_BAT; // Direction = output

    P5SEL0 &= ~V_5; // SLOW_CLK GPIO operation
    P5SEL1 &= ~V_5; // SLOW_CLK GPIO operation
    P5OUT &= ~V_5; // Initial Value = Low / Off
    P5DIR |= V_5; // Direction = output


    P5SEL0 &= ~V_DAC; // SLOW_CLK GPIO operation
    P5SEL1 &= ~V_DAC; // SLOW_CLK GPIO operation
    P5OUT &= ~V_DAC; // Initial Value = Low / Off
    P5DIR |= V_DAC; // Direction = output


    P5SEL0 &= ~V_3_3; // SLOW_CLK GPIO operation
    P5SEL1 &= ~V_3_3; // SLOW_CLK GPIO operation
    P5OUT &= ~V_3_3; // Initial Value = Low / Off
    P5DIR |= V_3_3; // Direction = output


    P5SEL0 &= ~IOT_BOOT_CPU; // SLOW_CLK GPIO operation
    P5SEL1 &= ~IOT_BOOT_CPU; // SLOW_CLK GPIO operation
    P5OUT &= ~IOT_BOOT_CPU; // Initial Value = Low / Off
    P5DIR |= IOT_BOOT_CPU; // Direction = output
}


void Init_Port6(void)
{
    P6OUT = 0x00; // P2 set Low
    P6DIR = 0x00; // Set P2 direction to output

    P6SEL0 |= LCD_BACKLITE; // SLOW_CLK GPIO operation
    P6SEL1 &= ~LCD_BACKLITE; // SLOW_CLK GPIO operation
   // P6OUT |= LCD_BACKLITE; // Initial Value = Low / Off
    P6DIR |= LCD_BACKLITE; // Direction = output

    P6SEL0 |= R_FORWARD; // SLOW_CLK GPIO operation
    P6SEL1 &= ~R_FORWARD; // SLOW_CLK GPIO operation
  //  P6OUT |= ~R_FORWARD; // Initial Value = Low / Off
    P6DIR |= R_FORWARD; // Direction = output


    P6SEL0 |= R_REVERSE; // SLOW_CLK GPIO operation
    P6SEL1 &= ~R_REVERSE; // SLOW_CLK GPIO operation
   // P6OUT &= ~R_REVERSE; // Initial Value = Low / Off
    P6DIR |= R_REVERSE; // Direction = output


    P6SEL0 |= L_FORWARD; // SLOW_CLK GPIO operation
    P6SEL1 &= ~L_FORWARD; // SLOW_CLK GPIO operation
  //  P6OUT &= ~L_FORWARD; // Initial Value = Low / Off
    P6DIR |= L_FORWARD; // Direction = output


    P6SEL0 |= L_REVERSE; // SLOW_CLK GPIO operation
    P6SEL1 &= ~L_REVERSE; // SLOW_CLK GPIO operation
   // P6OUT &= ~L_REVERSE; // Initial Value = Low / Off
    P6DIR |= L_REVERSE; // Direction = output

    P6SEL0 &= ~DAC_CNTL; // SLOW_CLK GPIO operation
    P6SEL1 &= ~DAC_CNTL; // SLOW_CLK GPIO operation
    P6OUT &= ~DAC_CNTL; // Initial Value = Low / Off
    P6DIR |= DAC_CNTL; // Direction = output

    P6SEL0 &= ~P6_5; // SLOW_CLK GPIO operation
    P6SEL1 &= ~P6_5; // SLOW_CLK GPIO operation
    P6OUT &= ~P6_5; // Initial Value = Low / Off
    P6DIR |= P6_5; // Direction = output

    P6SEL0 &= ~GRN_LED; // SLOW_CLK GPIO operation
    P6SEL1 &= ~GRN_LED; // SLOW_CLK GPIO operation
    P6OUT &= ~GRN_LED; // Initial Value = Low / Off
    P6DIR |= GRN_LED; // Direction = output


}


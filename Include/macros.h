/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This header file defines macros for various constants, configurations,
 * and settings used throughout the project.
 */

#ifndef INCLUDE_MACROS_H_
#define INCLUDE_MACROS_H_

// General Constants
#define ALWAYS (1)
#define RESET_STATE (0)
#define TRUE (0x01)
#define FALSE (0x00)

// LED Definitions
#define RED_LED (0x01) // RED LED 0
#define GRN_LED (0x40) // GREEN LED 1

// Test Probes
#define TEST_PROBE (0x01)

// Port Pull-Up Definitions
#define P4PUD (P4OUT)
#define P2PUD (P2OUT)

// Length Constants
#define LINE_LEN 11
#define LINE_WIDTH 10
#define MAXIPLENGTH 15
#define IPLENGTH 16
#define PASSWORD 5
#define SSIDSIZE 11

// Commands and States
#define NONE ('N')
#define Triangle ('T') // Changed from 'L'
#define Circle ('L')
#define Figure8 ('F')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')

// Timer Constants
#define TB0CCR0_200MS (25000)  // 8,000,000 / 8 / 8 / 5 -> 1 / 0.2s
#define TB0CCR1_10MS (1250)    // 8,000,000 / 2 / 8 / 5 -> 1 / 0.01s
#define TB0CCR2_250MS (25000)  // 8,000,000 / 8 / 8 / 4 -> 1 / 0.25s

#define TB1CCR0_200MS (25000)
#define TB1CCR1_200MS (25000)
#define TB1CCR2_200MS (25000)

// PWM Control
#define PWM_PERIOD (TB3CCR0)
#define LCD_BACKLITE_DIMING (TB3CCR1)
#define LEFT_FORWARD_SPEED (TB3CCR2)
#define LEFT_REVERSE_SPEED (TB3CCR3)
#define RIGHT_FORWARD_SPEED (TB3CCR4)
#define RIGHT_REVERSE_SPEED (TB3CCR5)

// Wheel Speed Settings
#define WHEEL_OFF (0)
#define LEFTSLOW (40000)
#define RIGHTSLOW (LEFTSLOW - 13000)
#define LEFTFAST (50000)
#define RIGHTFAST (LEFTFAST - 13000)

// Speed Percentages
#define PERCENT_100 (45000)
#define PERCENT_80 (40000)
#define PERCENT_60 (35000)

// Speed Limits
#define MAX_SPEED (50000)
#define MIN_SPEED (35000)
#define BASE_SPEED (16000) // Base speed
#define SCALE (4)          // 2^scale = divided by 16 here

// Wheel Period
#define WHEEL_PERIOD (50000)

// Enable/Disable States
#define ENABLED (0x01)
#define DISABLED (0x00)
#define RESET (0x00)
#define RDY (10)

// Miscellaneous
#define LIMIT (1000)
int MIDDLELINE;   //black line value for car to follow
#define HYSTERESIS 35 // Buffer zone to prevent sudden switching
#define DISPLAY_LINE_LENGTH (9)
#define DAC_BEGIN (4000)   // 2V
#define DAC_LIMIT (1650)
#define DAC_Adjust (875)   // 6.00V
#define MAX_LEN (16)
#define FIVESECONDS (25)
#define TIMEINCREMENT (2)
#define MAXMULTIPLIER (.5)
#define MINMULTIPLIER (.5)
#define DACDECREMENTER (10)
#define IOTREADY (20)
#define DEBOUNCEDONE (4)
#define MAXTIMER (9998)
#define NOTHING (0)
#define FMULTIPLIER 3
#define BMULTIPLIER 3
#define LMULTIPLIER 1
#define RMULTIPLIER 1
#define BUFFERSIZE 128
#define COMMANDSIZE 6


// Baud Rate Settings
#define BAUD_115200 4
#define MCTL_115200 0x5551
#define BAUD_460800 17
#define MCTL_460800 0x4A00
#define BAUD_9600 52
#define MCTL_9600 0x4911 // Assuming modulation control based on standard MSP430 settings



#endif /* INCLUDE_MACROS_H_ */

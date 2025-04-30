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
#define LINE_LEN 11        // Maximum length for a line (e.g., display line or data string)
#define LINE_WIDTH 10      // Width of a line (could refer to physical or logical constraints)
#define MAXIPLENGTH 15     // Maximum allowed length for the IP address (in characters)
#define IPLENGTH 16        // Exact length for the IP address storage (in characters, including null terminator)
#define PASSWORD 5         // Length of the password (in characters)
#define SSIDSIZE 11        // Size for storing the SSID (Wi-Fi network name, in characters)

// Commands and States
#define NONE ('N')        // Not used
#define Triangle ('T')    // Not used, Changed from 'L'
#define Circle ('L')      // Not used
#define Figure8 ('F')     // Not used
#define WAIT ('W')        // Not used
#define START ('S')       // Not used
#define RUN ('R')         // Not used
#define END ('E')         // Not used


// Timer Constants
#define TB0CCR0_200MS (25000)  // 8,000,000 / 8 / 8 / 5 -> 1 / 0.2s
#define TB0CCR1_10MS (1250)    // 8,000,000 / 8 / 8 / 100 -> 1 / 0.01s
#define TB0CCR2_200MS (25000)  // 8,000,000 / 8 / 8 / 5 -> 1 / 0.2s

#define TB1CCR0_200MS (25000)  // 8,000,000 / 8 / 8 / 5 -> 1 / 0.2s
#define TB1CCR1_200MS (25000)  // 8,000,000 / 8 / 8 / 5 -> 1 / 0.2s
#define TB1CCR2_200MS (25000)  // 8,000,000 / 8 / 8 / 5 -> 1 / 0.2s

// PWM Control
#define PWM_PERIOD (TB3CCR0)              // Timer period for PWM control (sets the frequency)
#define LCD_BACKLITE_DIMING (TB3CCR1)     // Timer channel for controlling LCD backlight dimming
#define LEFT_FORWARD_SPEED (TB3CCR2)      // Timer channel for controlling the forward speed of the left motor
#define LEFT_REVERSE_SPEED (TB3CCR3)      // Timer channel for controlling the reverse speed of the left motor
#define RIGHT_FORWARD_SPEED (TB3CCR4)     // Timer channel for controlling the forward speed of the right motor
#define RIGHT_REVERSE_SPEED (TB3CCR5)     // Timer channel for controlling the reverse speed of the right motor


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
#define SCALE (4)          // 2^scale = divided by 16 here //NOT USED

// Wheel Period
#define WHEEL_PERIOD (50000)

// Enable/Disable States
#define ENABLED (0x01)
#define DISABLED (0x00)
#define RESET (0x00)
#define RDY (10)

// Miscellaneous
#define LIMIT (1000)                   // A general-purpose limit, possibly for time, iterations, or threshold value
int MIDDLELINE;                        // The black line value that the car should follow (threshold for line detection)
#define HYSTERESIS 35                  // Buffer zone to prevent sudden switching of states (e.g., line following or stopping)
#define DISPLAY_LINE_LENGTH (9)        // Length of the display line (used for controlling the width of display output)
#define DAC_BEGIN (4000)               // Starting DAC value (corresponding to 2V for analog output control)
#define DAC_LIMIT (1650)               // DAC value that corresponds to the maximum current wheel speed (in Volts)
#define DAC_Adjust (875)               //6v
#define MAX_LEN (16)                   // Maximum length for display
#define FIVESECONDS (25)               // Constant representing 5 seconds, for timing purposes
#define TIMEINCREMENT (2)              // Increment value for timers or counters, for adjusting timing
#define MAXMULTIPLIER (.5)             // Maximum multiplier value used for scaling movement length
#define MINMULTIPLIER (.5)             // Minimum multiplier value used for scaling movement length
#define DACDECREMENTER (10)            // Value by which the DAC is decreased (controls speed or power decrement)
#define IOTREADY (20)                  // Value or time threshold to signal when IoT module is ready
#define DEBOUNCEDONE (4)               // Number of debounce cycles or checks before registering a stable input
#define MAXTIMER (9998)                // Maximum timer value, likely for time delays or operations before reset
#define NOTHING (0)                    // Placeholder value, could indicate no action or default state
#define FMULTIPLIER 3                  // Forward multiplier (used to scale or control forward speed)
#define BMULTIPLIER 3                  // Backward multiplier (used to scale or control reverse speed)
#define LMULTIPLIER 1                  // Left turn multiplier (used to scale or control turning speed in left direction)
#define RMULTIPLIER 1                  // Right turn multiplier (used to scale or control turning speed in right direction)
#define BUFFERSIZE 128                 // Size of the buffer for communication
#define COMMANDSIZE 6                  // Size of command buffer, for handling input/output commands
#define LOWESTVALUEREAD 1023           // Threshold or minimum value that can be read from a sensor or ADC


// Baud Rate Settings
#define BAUD_115200 4
#define MCTL_115200 0x5551
#define BAUD_460800 17
#define MCTL_460800 0x4A00
#define BAUD_9600 52
#define MCTL_9600 0x4911 // Assuming modulation control based on standard MSP430 settings



#endif /* INCLUDE_MACROS_H_ */

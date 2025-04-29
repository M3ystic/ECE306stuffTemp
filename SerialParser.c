/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains functions for parsing and executing serial commands
 * on the MSP430 microcontroller. It includes command handling for motor
 * control, blackline following, and displaying IP addresses and task states
 * on an LCD screen.
 */

#include <msp430.h>
#include <string.h>
#include "include/functions.h"
#include "include/LCD.h"
#include "include/ports.h"
#include "include/macros.h"
#include <stdlib.h>  // Required for atoi
#include <stdio.h>


extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

char stored_ip[IPLENGTH] = {NOTHING};   // To store the IP address in dotted format
char stored_ssid[SSIDSIZE] = {NOTHING}; // To store the SSID (up to 10 characters)
char stored_command[COMMANDSIZE] = {'0','0','X','X','X'};

extern char IOTmodule_tx_buf[BUFFERSIZE];
extern char IOTmodule_rx_buf[BUFFERSIZE];
unsigned int IOTmodule_tx;

extern int task;
extern int task2;
extern int task3;
unsigned int task_duration;
extern volatile unsigned int updatetimerflag; // Flag for timer update

extern int counter;
unsigned int blacklinefollowing = FALSE;
unsigned int leavingCircle = FALSE;
unsigned int turnFlag = FALSE;

void parse_and_execute_commands(char* buffer) {
    if (strstr(buffer, "IP,")) {
        parse_ip_address(buffer);  // Parse and display IP
    }
    char* command_start = strchr(buffer, '^');

    if (command_start != NULL) {
        command_start++; // Move past the '^' character

        while (*command_start == '\x00') {
            command_start++; // Skip leading null characters
        }

        if (strlen(command_start) >= 6) {
            char pin[PASSWORD] = {NOTHING};
            strncpy(pin, command_start, 4);

            if (strcmp(pin, "5555") != NOTHING) {
                return;  // Invalid PIN
            }

            char direction = command_start[4];
            int duration = atoi(&command_start[5]);


            stored_command[0] = direction;



            counter = RESET_STATE;
            P2OUT ^= IOT_RUN_RED; // Initial Value = Low / Off
            // Define scaling factors for each task
            const unsigned int forward_multiplier = FMULTIPLIER;
            const unsigned int backward_multiplier = BMULTIPLIER;
            const unsigned int right_multiplier = RMULTIPLIER;
            const unsigned int left_multiplier = LMULTIPLIER;
            // First switch for direction
            switch (direction) {
            case 'F': // Forward
                task = RESET_STATE;
                task_duration = duration * forward_multiplier;
                TB1CCTL1 |= CCIE; // Enable the timer interrupt for the task
                break;
            case 'B': // Backward
                task = 1;
                task_duration = duration * backward_multiplier;
                TB1CCTL1 |= CCIE; // Enable the timer interrupt for the task
                break;
            case 'R': // Right
                task = 2;
                task_duration = duration * right_multiplier;
                TB1CCTL1 |= CCIE; // Enable the timer interrupt for the task
                break;
            case 'L': // Left
                task = 3;
                task_duration = duration * left_multiplier;
                TB1CCTL1 |= CCIE; // Enable the timer interrupt for the task
                break;
            case 'S': //start black line following left side
                task2 = RESET_STATE;
                TB1CCTL2 |= CCIE; // Enable the timer interrupt for the task
                blacklinefollowing = TRUE;
                turnFlag = 1;
                break;
            case 'Y': //start black line following right side
                task2 = RESET_STATE;
                TB1CCTL2 |= CCIE; // Enable the timer interrupt for the task
                blacklinefollowing = TRUE;
                turnFlag = 2;
                break;
            case 'T':
                stop();
                break;
            case 'E': //end black line following
                P1OUT ^= RED_LED; // Initial Value = Low / Off
                updatetimerflag= FALSE;
                leavingCircle = TRUE;
                TB1CCTL2 |= CCIE; // Enable the timer interrupt for the task
                break;
            default:
                // Handle invalid direction if necessary
                // Do not enable the timer if the direction is invalid
                break;
            }

            // Second switch for arrival
            switch (stored_command[0]) {
            case '0':
                P6OUT |= LCD_BACKLITE; // Initial Value = Low / Off
                strcpy(display_line[0], "Arrived 00");
                strncpy(display_line[3], stored_command,5);
                stop();
                task = 5;
                break;
            case '1':
                P6OUT |= LCD_BACKLITE; // Initial Value = Low / Off
                strcpy(display_line[0], "Arrived 01");
                strncpy(display_line[3], stored_command,5);
                task = 5;
                break;
            case '2':
                P6OUT |= LCD_BACKLITE; // Initial Value = Low / Off
                strcpy(display_line[0], "Arrived 02");
                strncpy(display_line[3], stored_command,5);
                task = 5;
                stop();
                break;
            case '3':
                P6OUT |= LCD_BACKLITE; // Initial Value = Low / Off
                strcpy(display_line[0], "Arrived 03");
                strncpy(display_line[3], stored_command,5);
                task = 5;
                stop();
                break;
            case '4':
                P6OUT |= LCD_BACKLITE; // Initial Value = Low / Off
                strcpy(display_line[0], "Arrived 04");
                strncpy(display_line[3], stored_command,5);
                task = 5;
                stop();
                break;
            case '5':
                P6OUT |= LCD_BACKLITE; // Initial Value = Low / Off
                strcpy(display_line[0], "Arrived 05");
                strncpy(display_line[3], stored_command,5);
                task = 5;
                stop();
                break;
            case '6':
                P6OUT |= LCD_BACKLITE; // Initial Value = Low / Off
                strcpy(display_line[0], "Arrived 06");
                strncpy(display_line[3], stored_command,5);
                task = 5;
                stop();
                break;
            case '7':
                P6OUT |= LCD_BACKLITE; // Initial Value = Low / Off
                strcpy(display_line[0], "Arrived 07");
                strncpy(display_line[3], stored_command,5);
                updatetimerflag= FALSE;
                task = 5;
                stop();
                break;
            default:
                // Handle invalid arrival if necessary
                break;
            }

        }
    }
}


void send_at_command(const char* command) {
    strcpy(IOTmodule_tx_buf, command);  // Copy the command to the IOT TX buffer
    IOTmodule_tx = RESET_STATE;  // Reset the transmission index

    UCA0TXBUF = IOTmodule_tx_buf[IOTmodule_tx++];  // Send first character from buffer
    UCA0IE |= UCTXIE;  // Enable TX interrupt to continue sending the rest of the buffer

    display_changed = TRUE;  // Mark the display as changed
}

// This centers characters.
void center_text(char* dest, const char *src, int width) {
    int len = strlen(src);
    int total = width - 1;  // printable characters available
    int left_padding = NOTHING;
    if (len < total) {
        left_padding = (total - len) / 2;
    }
    int i;
    // Fill in left padding with spaces.
    for (i = 0; i < left_padding; i++) {
        dest[i] = ' ';
    }
    // Copy src after the left padding.
    strcpy(dest + left_padding, src);

    // Fill remaining characters with spaces.
    i = left_padding + len;
    while (i < total) {
        dest[i++] = ' ';
    }
    dest[i] = '\0'; // Terminate properly.
}

//-------------------------------------------------------------------
// Parses the IP address from the input string and centers it on two lines.
void parse_ip_address(char *input) {
    char *start = strstr(input, "IP,");
    if (start == NULL) {
        // Default message when no IP is found
        center_text(display_line[2], "No IP", LINE_LEN);
        center_text(display_line[3], "Received", LINE_LEN);
        update_display = TRUE;
        display_changed = TRUE;
        return;
    }

    start += 3; // Move past "IP,"
    if (*start == '"') start++;  // Skip opening quote

    char *end = strchr(start, '"');
    if (end == NULL || (end - start) > MAXIPLENGTH) {
        // Default message if IP format is invalid
        center_text(display_line[2], "No IP", LINE_LEN);
        center_text(display_line[3], "Received", LINE_LEN);
        update_display = TRUE;
        display_changed = TRUE;
        return;
    }

    // Copy extracted IP
    char ip[IPLENGTH] = {NOTHING};
    strncpy(ip, start, end - start);
    ip[end - start] = '\0';
    strncpy(stored_ip, ip, sizeof(stored_ip) - 1);
    stored_ip[sizeof(stored_ip) - 1] = '\0';

    // Tokenize the IP manually into two display lines
    char line1[LINE_LEN] = {NOTHING}, line2[LINE_LEN] = {NOTHING};
    int dotCount = RESET_STATE, idx1 = RESET_STATE, idx2 = RESET_STATE;
    int i;
    for ( i = 0; ip[i] != '\0'; i++) {
        if (ip[i] == '.') dotCount++;

        if (dotCount < 2) {
            if (idx1 < LINE_LEN - 1) line1[idx1++] = ip[i];
        } else {
            if (idx2 < LINE_LEN - 1) line2[idx2++] = ip[i];
        }
    }
    line1[idx1] = '\0';
    line2[idx2] = '\0';

    // Center and update display
    center_text(display_line[2], line1, LINE_LEN);
    center_text(display_line[3], line2, LINE_LEN);
    update_display = TRUE;
    display_changed = TRUE;
}


void changedIPlines(const char* ip) {
    if (ip == NULL || *ip == '\0') {
        // Default message when no IP is available
        char line1[LINE_LEN] = {NOTHING}, line2[LINE_LEN] = {NOTHING};
        center_text(line1, "No IP", LINE_LEN);
        center_text(line2, "Received", LINE_LEN);
        strcpy(display_line[0], line1);
        strcpy(display_line[1], line2);
    } else {
        // Tokenize IP manually into two lines
        char line1[LINE_LEN] = {NOTHING}, line2[LINE_LEN] = {NOTHING};
        int dotCount = RESET_STATE, idx1 = RESET_STATE, idx2 = RESET_STATE;
        int i;
        for (i = 0; ip[i] != '\0'; i++) {
            if (ip[i] == '.') dotCount++;

            if (dotCount < 2) {
                if (idx1 < LINE_LEN - 1) line1[idx1++] = ip[i];
            } else {
                if (idx2 < LINE_LEN - 1) line2[idx2++] = ip[i];
            }
        }

        line1[idx1] = '\0';
        line2[idx2] = '\0';

        center_text(display_line[1], line1, LINE_LEN);
        center_text(display_line[2], line2, LINE_LEN);
    }

    update_display = TRUE;
    display_changed = TRUE;
}



void parse_ssid_address(char* input) {
    // Clear display_line[2].
    memset(display_line[2], '\0', LINE_LEN);

    char *start = strstr(input, "AP:");
    if (start) {
        start += strlen("AP:");  // Move past "AP:"
        if (*start == '"') {
            start++;  // Skip opening quote.
        }
        char *end = strchr(start, '"');
        if (end) {
            int ssid_length = end - start;
            if (ssid_length > 10) {  // Limit length to 10 characters.
                ssid_length = 10;
            }
            char ssid[11] = {NOTHING};
            strncpy(ssid, start, ssid_length);
            ssid[ssid_length] = '\0';

            // Store the SSID for later use
            strncpy(stored_ssid, ssid, sizeof(stored_ssid) - 1);
            stored_ssid[sizeof(stored_ssid) - 1] = '\0';

            char centered[LINE_LEN] = {NOTHING};
            center_text(centered, ssid, LINE_LEN);
            strcpy(display_line[NOTHING], centered);
            update_display = TRUE;
            display_changed = TRUE;
            return;
        }
    }
    // Default message if no SSID is found.
    char centered[LINE_LEN] = {NOTHING};
    center_text(centered, "No SSID", LINE_LEN);
    strcpy(display_line[NOTHING], centered);

}

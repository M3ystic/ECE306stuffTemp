/*
 * SerialParser.c
 *
 *  Created on: Apr 7, 2025
 *      Author: datweneb
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

char stored_ip[16] = {0};   // To store the IP address in dotted format
char stored_ssid[11] = {0}; // To store the SSID (up to 10 characters)
char stored_command[6] = {'0','0','X','X','X'};

extern char iot_tx_buf[128];
extern char iot_rx_buf[128];
unsigned int iot_tx;

extern int task;
extern int task2;
extern int task3;
unsigned int task_duration;
extern volatile unsigned int updatetimerflag; // Flag for timer update

extern int counter;
unsigned int blacklinefollowing = FALSE;
unsigned int leavingCircle = FALSE;
unsigned int turnFlag = 0;

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
            char pin[5] = {0};
            strncpy(pin, command_start, 4);

            if (strcmp(pin, "5555") != 0) {
                return;  // Invalid PIN
            }

            char direction = command_start[4];
            int duration = atoi(&command_start[5]);


            stored_command[0] = direction;



            counter = 0;
            P2OUT ^= IOT_RUN_RED; // Initial Value = Low / Off
            // Define scaling factors for each task
            unsigned int forward_multiplier = 3;
            unsigned int backward_multiplier = 3;
            unsigned int right_multiplier = 1;
            unsigned int left_multiplier = 1;
            // First switch for direction
            switch (direction) {
                case 'F': // Forward
                    task = 0;
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
                case 'S': //start black line following
                    task2 = 0;
                    TB1CCTL2 |= CCIE; // Enable the timer interrupt for the task
                    blacklinefollowing = TRUE;
                    turnFlag = 1;
                    break;
                case 'Y': //start black line following
                    task2 = 0;
                    TB1CCTL2 |= CCIE; // Enable the timer interrupt for the task
                    blacklinefollowing = TRUE;
                    turnFlag = 2;
                    break;
                case 'T':
                     stop();
                     break;
                case 'E': //start black line following
                    P1OUT ^= RED_LED; // Initial Value = Low / Off
                    updatetimerflag= 0;
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
                    updatetimerflag= 0;
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
    strcpy(iot_tx_buf, command);  // Copy the command to the IOT TX buffer
    iot_tx = 0;  // Reset the transmission index

    UCA0TXBUF = iot_tx_buf[iot_tx++];  // Send first character from buffer
    UCA0IE |= UCTXIE;  // Enable TX interrupt to continue sending the rest of the buffer

    display_changed = TRUE;  // Mark the display as changed
}

// This helper computes the centered version of src inside a total of (LINE_LEN - 1) characters.
// It writes the result into dest.
void center_text(char* dest, const char *src, int width) {
    int len = strlen(src);
    int total = width - 1;  // printable characters available
    int left_padding = 0;
    if (len < total) {
        left_padding = (total - len) / 2;
    }
    int i = 0;
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
// The expected input format is:
//    +CIFSR:STAIP,"p1.p2.p3.p4"
// If successful it will fill:
//    display_line[0] --> centered first part ("p1.p2")
//    display_line[1] --> centered second part ("p3.p4")
// If parsing fails, then it shows the default messages centered.
void parse_ip_address(char *input) {
    char *start = strstr(input, "IP,");
    if (start == NULL) {
        // Default message when no IP is found
        center_text(display_line[2], "No IP", LINE_LEN);
        center_text(display_line[3], "Received", LINE_LEN);
        update_display = 1;
        display_changed = 1;
        return;
    }

    start += 3; // Move past "IP,"
    if (*start == '"') start++;  // Skip opening quote

    char *end = strchr(start, '"');
    if (end == NULL || (end - start) > 15) {
        // Default message if IP format is invalid
        center_text(display_line[2], "No IP", LINE_LEN);
        center_text(display_line[3], "Received", LINE_LEN);
        update_display = 1;
        display_changed = 1;
        return;
    }

    // Copy extracted IP
    char ip[16] = {0};
    strncpy(ip, start, end - start);
    ip[end - start] = '\0';
    strncpy(stored_ip, ip, sizeof(stored_ip) - 1);
    stored_ip[sizeof(stored_ip) - 1] = '\0';

    // Tokenize the IP manually into two display lines
    char line1[LINE_LEN] = {0}, line2[LINE_LEN] = {0};
    int dotCount = 0, idx1 = 0, idx2 = 0;
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
    update_display = 1;
    display_changed = 1;
}


void changedIPlines(const char* ip) {
    if (ip == NULL || *ip == '\0') {
        // Default message when no IP is available
        char line1[LINE_LEN] = {0}, line2[LINE_LEN] = {0};
        center_text(line1, "No IP", LINE_LEN);
        center_text(line2, "Received", LINE_LEN);
        strcpy(display_line[0], line1);
        strcpy(display_line[1], line2);
    } else {
        // Tokenize IP manually into two lines
        char line1[LINE_LEN] = {0}, line2[LINE_LEN] = {0};
        int dotCount = 0, idx1 = 0, idx2 = 0;
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

    update_display = 1;
    display_changed = 1;
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
            char ssid[11] = {0};
            strncpy(ssid, start, ssid_length);
            ssid[ssid_length] = '\0';

            // Store the SSID for later use
            strncpy(stored_ssid, ssid, sizeof(stored_ssid) - 1);
            stored_ssid[sizeof(stored_ssid) - 1] = '\0';

            char centered[LINE_LEN] = {0};
            center_text(centered, ssid, LINE_LEN);
            strcpy(display_line[0], centered);
            update_display = 1;
            display_changed = 1;
            return;
        }
    }
    // Default message if no SSID is found.
    char centered[LINE_LEN] = {0};
    center_text(centered, "No SSID", LINE_LEN);
    strcpy(display_line[0], centered);
    update_display = 1;
    display_changed = 1;
}

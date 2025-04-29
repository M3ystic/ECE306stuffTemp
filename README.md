# Embedded Systems Line-Following Car Project
## Project Overview
This project is an introduction to embedded systems through the development of a line-following car. The car is designed to follow a black electrical tape line autonomously after navigating an initial course via Wi-Fi control. The project consists of ten milestones, each building on the previous one, culminating in an end-to-end demonstration of both IoT and autonomous capabilities.

## Objectives
Build foundational skills in embedded systems development, including hardware assembly, software programming, and system integration.

Integrate IoT communication for real-time control and feedback.

Implement autonomous line-following using sensor detection and H-bridge motor control.

Demonstrate real-world application of embedded systems in automation and wireless control.

## Project Milestones
1. Battery Power System
Construct the power system for the car.

Verify functionality using Analog Discovery or other lab test equipment.

2. LCD Display Integration
Install and configure an LCD on the control board.

Demonstrate functionality with the provided project code.

3. Basic Car Assembly
Assemble the car with forward-only motor control via an H-bridge.

Verify functionality through car movement.

4. Controlled Movement
Implement and verify forward-only movement in predetermined shapes within a 36” x 36” area.

5. Full H-Bridge Control
Add forward and reverse motor control.

Verify with timed forward, reverse, and spinning motion tests.

6. Sensor Integration
Add emitter/detector circuits for line detection.

Implement ADC for thumbwheel input and verify transitions between black and white surfaces.

7. Autonomous Circular Tracking
Program the car to navigate around a circle two or more times before exiting.

8. Serial Communication
Develop serial communication routines to support future functionality.

9. Wi-Fi Communication
Enable remote control via Wi-Fi for navigation around designated checkpoints.

10. Autonomous Line Following
Navigate a Wi-Fi-controlled course with eight checkpoints.

After reaching checkpoint 8, transition to autonomous line-following mode to complete the challenge.

##Final Demonstration
IoT Navigation: Drive the car through a Wi-Fi-controlled course , stopping at eight designated pads. At each pad:

Display "Arrived 0X" (X = pad number) on the LCD.

Confirm arrival with the TA for grade logging.

Autonomous Line-Following: Upon reaching the final pad:

Issue a single command to switch the car to autonomous mode.

Detect and follow a black line to a circle, navigating it twice before stopping.

End with a final command to move two feet away and display a success message.

## Challenges and Insights
Sensor Precision: Ensuring reliable black-line detection across varying surface conditions.

Motor Control: Fine-tuning forward and reverse movement for smooth operation.

IoT Communication: Debugging real-time Wi-Fi commands for seamless control.

Integration Complexity: Combining hardware and software elements into a cohesive system.

## Tools and Components
Microcontroller: MSP430FR2355 (or equivalent).

Sensors: Emitter/detector circuits for black-line detection.

Communication: Serial communication and Wi-Fi module.

Display: LCD for status updates.

## Lessons Learned
Embedded systems require a strong understanding of both hardware and software.

Debugging is an iterative process that often reveals new challenges.

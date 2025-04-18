/*
 * macros.h
 *
 *  Created on: Jan 29, 2025
 *      Author: Dtwen
 */

#ifndef INCLUDE_MACROS_H_
#define INCLUDE_MACROS_H_


#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //

#define P4PUD                (P4OUT)
#define P2PUD                (P2OUT)








#define NONE ('N')
#define Triangle ('T')   //changed from L
#define Circle ('L')
#define Figure8 ('F')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')

#define circle  WHEEL_COUNT_TIME (20)
#define circleRIGHT_COUNT_TIME (25)
#define circleLEFT_COUNT_TIME (3)
#define circleTRAVEL_DISTANCE (160)
#define WAITING2START (50)

#define WHEEL_COUNT_TIME (20)


#define f8WHEEL_COUNT_TIME (15)
#define f8RIGHT_COUNT_TIME (15)
#define f8LEFT_COUNT_TIME (1)
#define f8TRAVEL_DISTANCE (45)

#define f8WHEEL_COUNT_TIME2 (15)
#define f8RIGHT_COUNT_TIME2 (1)
#define f8LEFT_COUNT_TIME2 (15)
#define f8TRAVEL_DISTANCE2 (90)

//timer12500
#define TB0CCR0_200MS   (25000)  /* 8 000 000 / 8 / 8 / 5     (1/200ms) --> (1/.2) = 5  */
#define TB0CCR1_10MS    (1250)  /* 8 000 000 / 2 / 8 / 5     (1/200ms) --> (1/.2) = 5  */
#define TB0CCR2_250MS   (31250)  /* 8 000 000 / 8 / 8 / 4     (1/250ms) --> (1/.25) = 4  */

#define TB1CCR0_200MS   (25000)  /* 8 000 000 / 8 / 8 / 5     (1/200ms) --> (1/.2) = 5  */
#define TB1CCR1_200MS   (25000)  /* 8 000 000 / 8 / 8 / 5     (1/200ms) --> (1/.2) = 5  */


#define PWM_PERIOD (TB3CCR0)
#define LCD_BACKLITE_DIMING (TB3CCR1)
#define LEFT_FORWARD_SPEED (TB3CCR2) //left forward
#define LEFT_REVERSE_SPEED (TB3CCR3) //left reverse
#define RIGHT_FORWARD_SPEED (TB3CCR4)  //forward right
#define RIGHT_REVERSE_SPEED (TB3CCR5)  //right reverse


#define WHEEL_OFF (0)


#define LEFTSLOW  (40000)
#define RIGHTSLOW (LEFTSLOW - 15200)
#define LEFTFAST  (50000)
#define RIGHTFAST (LEFTFAST - 15200)




#define PERCENT_100 (45000)
#define PERCENT_80 (40000)
#define PERCENT_60 (35000)

#define MAX_SPEED (50000)
#define MIN_SPEED (35000)
#define BASE_SPEED (16000) // base speed
#define SCALE (4) // 2^scale = divided by 16 here

#define WHEEL_PERIOD (50000)



#define ENABLED  (0x01)
#define DISABLED (0x00)
#define RESET    (0x00)


#define LIMIT (1000)
#define MIDDLELINE (70)//600




#define DISPLAY_LINE_LENGTH (9)

#define DAC_BEGIN (4000) // 2v
#define DAC_LIMIT (1500) // 6.08v
#define DAC_Adjust (875) // 6.00v
///5.174v is 1200
//2100 to start moving

//2030 left wheel wont move


#endif /* INCLUDE_MACROS_H_ */

///*
// * state_machine.c
// *
// *  Created on: Jan 30, 2025
// *      Author: Dtwen
// */
//
//#include "include\globalheader.h"
//#include "include\ports.h"
//#include "include\functions.h"
//#include "include\macros.h"
//
//unsigned int turns;
//unsigned int carTurn = 1;
//static int figure8turns;
//static int f8state;
//unsigned int f8Turn = 1;
//
//// Static variables persist between function calls
//static int forwardCount = 0;  // Tracks how many times the car has moved forward
//static int turnState = 0;      // State to track the current turn progress
//static int triangleDone = 0;   // Track if triangle movement is done
//
//void moveCircle(void)
//{
//        switch(Time_Sequence){
//          case 200:
//              Time_Sequence = 0;
//              circleturned++;
//              if (circleturned == 8) {
//                              motors_off();
//                              Time_Sequence = 0;
//                              return;         // Stop the circle movement
//                          }
//                          break;
//          case 100:
//              right_forward();
//            break;
//          case  50:
//              motors_forward();
//            break;
//          default: break;
//        }
//
//}
//
//void moveTriangle(void)
//{
//    switch(Time_Sequence)
//    {
//     case 150:
//         turns++;
//         motors_off();
//         Time_Sequence = 0;
//         if (turns == 3)
//         {
//             turns = 0;
//             carTurn = 1;
//           while(carTurn == 1)
//           {
//             switch (Time_Sequence)
//                {
//                 case 140:
//                     right_forward();
//                     Time_Sequence = 0;
//                     carTurn = 0;
//                     break;
//                 case 0 :
//                     motors_off();
//                     right_forward();
//                     break;
//                 default: break;
//                }
//            }
//         }
//         break;
//      case 100:
//          motors_forward();
//          break;
//      case  50:
//          motors_forward();
//        break;
//      default: break;
//    }
//
//}
//
//
//
//
//
//void moveFigure8(void)
//{
//
//
//    switch(Time_Sequence){
//      case 180:
//          figure8turns++;
//          Time_Sequence = 0;
//          if (figure8turns == 8)
//          {
//              f8Turn = 1;
//              figure8turns = 0;
//              while (f8Turn == 1)
//              {
//                  switch(Time_Sequence)
//                  {
//                    case 180:
//                        Time_Sequence = 0;
//                        figure8turns++;
//                        if (figure8turns == 10)
//                        {
//                            f8Turn = 0;
//                            figure8turns = 0;
//                            motors_off();
//                            return;
//                        }
//
//                      break;
//                    case 100:
//                        left_forward();
//                      break;
//                    case  50:
//                        motors_forward();
//                      break;
//                    default: break;
//                  }
//              }
//          }
//
//        break;
//      case 100:
//          right_forward();
//        break;
//      case  50:
//          motors_forward();
//        break;
//      default: break;
//    }
//
//
//
//}
//
//
//void Carlson_StateMachine(void){
//    switch(Time_Sequence){
//      case 250:                        //
//        if(one_time){
//          Init_LEDs();
//          lcd_BIG_mid();
//          display_changed = 1;
//          one_time = 0;
//        }
//        Time_Sequence = 0;             //
//        break;
//      case 200:                        //
//        if(one_time){
////          P1OUT &= ~RED_LED;            // Change State of LED 4
//        //  P6OUT |= GRN_LED; changed           // Change State of LED 5
//          one_time = 0;
//        }
//        break;
//      case 150:                         //
//        if(one_time){
//        //  P1OUT |= RED_LED;      changed      // Change State of LED 4
//        //  P6OUT &= ~GRN_LED;    changed        // Change State of LED 5
//          one_time = 0;
//        }
//        break;
//      case 100:                         //
//        if(one_time){
////          lcd_4line   ();
//          lcd_BIG_bot();
//      //   P6OUT |= GRN_LED;      changed      // Change State of LED 5
//          display_changed = 1;
//          one_time = 0;
//        }
//        break;
//      case  50:                        //
//        if(one_time){
//          one_time = 0;
//        }
//        break;                         //
//      default: break;
//    }
//}
//
//

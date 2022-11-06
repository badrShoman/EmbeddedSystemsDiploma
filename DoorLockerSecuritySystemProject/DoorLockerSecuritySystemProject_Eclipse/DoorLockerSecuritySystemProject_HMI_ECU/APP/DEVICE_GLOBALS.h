/******************************************************************************
 * Module: Global Variables
 * File Name: DEVICE_GLOBALS.h
 * Description: Header file for global variables.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef APP_DEVICE_GLOBALS_H_
#define APP_DEVICE_GLOBALS_H_

/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/
extern uint8 key; /* A variable that carries keyboard key */
extern uint8 reenter; /* A variable that indicates password entries */
extern uint8 passwordState; /* A variable that indicates whether the a password is saved */
extern uint8 buttonCount; /* A variable that holds number of keyboard clicks */
extern uint8 password[5]; /* Array holding first password */
extern uint8 passwordReEnter[5]; /* Array holding re-entered password */

#endif /* APP_DEVICE_GLOBALS_H_ */

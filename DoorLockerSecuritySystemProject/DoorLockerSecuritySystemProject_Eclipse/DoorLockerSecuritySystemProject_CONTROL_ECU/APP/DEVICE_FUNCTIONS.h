/******************************************************************************
 * Module: Device Functions
 * File Name: DEVICE_FUNCTIONS.h
 * Description: Header file for device application functions.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef APP_DEVICE_FUNCTIONS_H_
#define APP_DEVICE_FUNCTIONS_H_

/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/
extern uint8 receivedPassword[5]; /* An array for received password*/
extern uint8 receivedPasswordReenter[5]; /* An array for receieved password re-entry */
extern uint8 currentPassword[5]; /* An array for current password in EEPROM */
extern uint8 USARTCommand; /* A variable to store incoming USART commands */
extern uint8 passwordState; /* A variable that saves password in EEPROM state */

/*******************************************************************************
 *                             Functions Prototypes                            *
 *******************************************************************************/
/*
 * [Function Name]	: scanPassword
 * [Description]	:
 * 		Function that scans for existing password and places it in
 * 		currentPassword array.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void scanPassword(void);

/*
 * [Function Name]	: timerSeconds
 * [Description]	:
 * 		Function that starts counting seconds for door motor.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void timerSeconds(void);

/*
 * [Function Name]	: definePassword
 * [Description]	:
 * 		Function that compares a password and it's re-entry.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void definePassword(void);

/*
 * [Function Name]	: checkPassword
 * [Description]	:
 * 		Function that checks if received password and the password saved
 * 		in EEPROM match.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void checkPassword(void);

/*
 * [Function Name]	: doorUnlock
 * [Description]	:
 * 		Function that executes door locking & unlocking mechanisms.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void doorUnlock(void);

/*
 * [Function Name]	: breachDetection
 * [Description]	:
 * 		Function that is executes when user enters wrong password 3 times.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void breachDetection(void);

#endif /* APP_DEVICE_FUNCTIONS_H_ */

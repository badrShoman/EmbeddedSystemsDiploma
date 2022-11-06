/******************************************************************************
 * Module: Device Functions
 * File Name: DEVICE_FUNCTIONS.h
 * Description: Source file for device application functions.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef APP_DEVICE_FUNCTIONS_H_
#define APP_DEVICE_FUNCTIONS_H_

/*******************************************************************************
 *                             Functions Prototypes                            *
 *******************************************************************************/
/*
 * [Function Name]	: LCD_cursorToggle
 * [Description]	:
 * 		Function that toggles LCD cursor every 0.5 second.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void LCD_cursorToggle(void);

/*
 * [Function Name]	: removePassword
 * [Description]	:
 * 		Function that removes password from LCD screen.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void removePassword(void);

/*
 * [Function Name]	: sendNewPassword
 * [Description]	:
 * 		Function that sends a new password set by user to CONTROL_ECU.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void sendNewPassword(void);

/*
 * [Function Name]	: sendPassword
 * [Description]	:
 * 		Function that sends a password to be compared by password saved in
 * 		EEPROM to CONTROL_ECU.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void sendPassword(void);

/*
 * [Function Name]	: breachDetection
 * [Description]	:
 * 		Function that is executes when user enters wrong password 3 times.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void breachDetection(void);

#endif /* APP_DEVICE_FUNCTIONS_H_ */

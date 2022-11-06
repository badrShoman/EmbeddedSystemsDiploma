/******************************************************************************
 * Module: Device Screens
 * File Name: DEVICE_SCREENS.h
 * Description: Header file for device screens that is displayed on LCD.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef HMI_ECU_H_
#define HMI_ECU_H_

/*******************************************************************************
 *                             Functions Prototypes                            *
 *******************************************************************************/
/*
 * [Function Name]	: passwordSuccessful
 * [Description]	:
 * 		Function that is executed upon entering a successful password.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void passwordSuccessful(void);

/*
 * [Function Name]	: passwordNoMatch
 * [Description]	:
 * 		Function that is executed upon entering un-matched passwords.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void passwordNoMatch(void);

/*
 * [Function Name]	: passwordFail
 * [Description]	:
 * 		Function that is executed upon entering a wrong password.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void passwordFail(void);

/*
 * [Function Name]	: enterPassword
 * [Description]	:
 * 		Function that represents the password entry screen.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void enterPassword(void);

#endif /* HMI_ECU_H_ */

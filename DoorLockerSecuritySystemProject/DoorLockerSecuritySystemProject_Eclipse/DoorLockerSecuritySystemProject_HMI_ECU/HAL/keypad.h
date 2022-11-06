/******************************************************************************
 * Module: Keypad
 * File Name: keypad.c
 * Description: Header file for the Keypad driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "../std_types.h"		/* To use standard defined types */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* Keypad standard configuration */
#define STANDARD_KEYPAD					  FALSE
/* Keypad configurations for number of rows and columns */
#define KEYPAD_NUM_COLS                   4
#define KEYPAD_NUM_ROWS                   4
/* Keypad Port Configurations */
#define KEYPAD_ROW_PORT_ID                PORTC_ID
#define KEYPAD_FIRST_ROW_PIN_ID           PIN4_ID
#define KEYPAD_COL_PORT_ID                PORTC_ID
#define KEYPAD_FIRST_COL_PIN_ID           PIN0_ID
/* Keypad button logic configurations */
#define KEYPAD_BUTTON_PRESSED            LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED           LOGIC_HIGH

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * [Function Name]	: Keypad_getPressedKey
 * [Description]	:
 * 		Function that gets the key that is pressed in the keypad.
 * [Args]		: Void.
 * [Return]		: Keypad pressed button corresponding number through mapping.
 */
uint8 Keypad_getPressedKey(void);

#endif /* KEYPAD_H_ */

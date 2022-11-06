/******************************************************************************
 * Module: Keypad
 * File Name: keypad.c
 * Description: Source file for the Keypad driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <util/delay.h>					/* For delay functions usage */
#include "../MCAL/gpio.h"				/* For GPIO usage */
#include "../HAL/keypad.h"				/* For keypad prototypes & definations */

/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/

#if (STANDARD_KEYPAD == FALSE)

#if (KEYPAD_NUM_COLS == 3)

/*
 * [Function Name]	: Keypad_4x3_adjustKeyNumber
 * [Description]	:
 * 		Function that is responsible for mapping the switch number in the keypad to
 * 		it's corresponding number for 4x3 keypad.
 * [Args]	:
 * [In] buttonNumber : Indicates pressed button number.
 * [Return]			 : Keypad pressed button corresponding number.
 */
static uint8 Keypad_4x3_adjustKeyNumber(uint8 buttonNumber);

#elif (KEYPAD_NUM_COLS == 4)

/*
 * [Function Name]	: Keypad_4x4_adjustKeyNumber
 * [Description]	:
 * 		Function that is responsible for mapping the switch number in the keypad to
 * 		it's corresponding number for 4x4 keypad.
 * [Args]	:
 * [In] buttonNumber : Indicates pressed button number.
 * [Return]			 : Keypad pressed button corresponding number.
 */
static uint8 Keypad_4x4_adjustKeyNumber(uint8 buttonNumber);

#endif

#endif

/*******************************************************************************
 *                           Functions Definitions                             *
 *******************************************************************************/
/*
 * [Function Name]	: Keypad_getPressedKey
 * [Description]	:
 * 		Function that gets the key that is pressed in the keypad.
 * [Args]		: Void.
 * [Return]		: Keypad pressed button corresponding number through mapping.
 */
uint8 Keypad_getPressedKey(void)
{
	/* Define variables to be looped about */
	uint8 column, row;
	/* Configure keypad pins */
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID,
			PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID + 1,
			PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID + 2,
			PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID + 3,
			PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID,
			PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + 1,
			PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + 2,
			PIN_INPUT);

#if(KEYPAD_NUM_COLS == 4)

	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + 3,
			PIN_INPUT);

#endif

	while (TRUE)
	{
		/* Loop for rows */
		for (row = 0; row < KEYPAD_NUM_ROWS; row++)
		{
			/* Set the current row pin as output pin */
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,
			KEYPAD_FIRST_ROW_PIN_ID + row, PIN_OUTPUT);
			/* Set or clear the row output pin */
			GPIO_writePin(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID + row,
			KEYPAD_BUTTON_PRESSED);
			/* Loop for columns */
			for (column = 0; column < KEYPAD_NUM_COLS; column++)
			{
				/* Check if the switch is pressed in this column */
				if (GPIO_readPin(KEYPAD_COL_PORT_ID,
				KEYPAD_FIRST_COL_PIN_ID + column) == KEYPAD_BUTTON_PRESSED)
				{

#if (KEYPAD_NUM_COLS == 3)

#if (STANDARD_KEYPAD == TRUE)

					/* An equation to return the pressed button number */
					return ((row * KEYPAD_NUM_COLS) + column + 1);
#else

					/* An equation to return the pressed button number */
					return Keypad_4x3_adjustKeyNumber(
							(row * KEYPAD_NUM_COLS) + column + 1);

#endif

#elif (KEYPAD_NUM_COLS == 4)

#if (STANDARD_KEYPAD == TRUE)

					/* An equation to return the pressed button number */
					return ((row * KEYPAD_NUM_COLS) + column + 1);

#else
					/* An equation to return the pressed button number */
					return Keypad_4x4_adjustKeyNumber(
							(column * KEYPAD_NUM_COLS) + row + 1);
#endif

#endif
				}
			}
			/* Set the current row pin as input pin */
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,
			KEYPAD_FIRST_ROW_PIN_ID + row, PIN_INPUT);
		}
	}
}

#if (STANDARD_KEYPAD == FALSE)

#if (KEYPAD_NUM_COLS == 3)

/*
 * [Function Name]	: Keypad_4x3_adjustKeyNumber
 * [Description]	:
 * 		Function that is responsible for mapping the switch number in the keypad to
 * 		it's corresponding number for 4x3 keypad.
 * [Args]	:
 * [In] buttonNumber : Indicates pressed button number.
 * [Return]			 : Keypad pressed button corresponding number.
 */
static uint8 Keypad_4x3_adjustKeyNumber(uint8 buttonNumber)
{
	/* Define a variable that will be returned from the function */
	uint8 keypadButton = 0;
	/* Select pressed button corresponding value accordingly */
	switch (buttonNumber)
	{
		case 10:
			keypadButton = '*';
		break;
		case 11:
			keypadButton = 0;
		break;
		case 12:
			keypadButton = '#';
		break;
		default:
			keypadButton = buttonNumber;
		break;
	}
	/* Return the selected value */
	return keypadButton;
}

#elif (KEYPAD_NUM_COLS == 4)

/*
 * [Function Name]	: Keypad_4x4_adjustKeyNumber
 * [Description]	:
 * 		Function that is responsible for mapping the switch number in the keypad to
 * 		it's corresponding number for 4x4 keypad.
 * [Args]	:
 * [In] buttonNumber : Indicates pressed button number.
 * [Return]			 : Keypad pressed button corresponding number.
 */
static uint8 Keypad_4x4_adjustKeyNumber(uint8 buttonNumber)
{
	/* Define a variable that will be returned from the function */
	uint8 keypadButton = 0;
	/* Select pressed button corresponding value accordingly */
	switch (buttonNumber)
	{
		case 1:
			keypadButton = 7;
		break;
		case 2:
			keypadButton = 8;
		break;
		case 3:
			keypadButton = 9;
		break;
		case 4:
			keypadButton = '%';
		break;
		case 5:
			keypadButton = 4;
		break;
		case 6:
			keypadButton = 5;
		break;
		case 7:
			keypadButton = 6;
		break;
		case 8:
			keypadButton = '*';
		break;
		case 9:
			keypadButton = 1;
		break;
		case 10:
			keypadButton = 2;
		break;
		case 11:
			keypadButton = 3;
		break;
		case 12:
			keypadButton = '-';
		break;
		case 13:
			keypadButton = 13;
		break;
		case 14:
			keypadButton = 0;
		break;
		case 15:
			keypadButton = '=';
		break;
		case 16:
			keypadButton = '+';
		break;
		default:
			keypadButton = buttonNumber;
		break;
	}
	/* Return the selected value */
	return keypadButton;
}

#endif

#endif /* STANDARD_KEYPAD */

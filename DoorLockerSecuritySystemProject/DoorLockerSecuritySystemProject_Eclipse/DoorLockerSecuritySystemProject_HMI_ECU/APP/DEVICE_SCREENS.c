/******************************************************************************
 * Module: Device Screens
 * File Name: DEVICE_SCREENS.c
 * Description: Source file for device screens that is displayed on LCD.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <util/delay.h>					/* For delay functions */
#include "../MCAL/timer.h"				/* For timer usage */
#include "../HAL/keypad.h"				/* For keypad usage */
#include "../HAL/lcd.h"					/* For LCD usage */
#include "../APP/DEVICE_SCREENS.h"		/* For device screens prototypes */
#include "../APP/DEVICE_GLOBALS.h"		/* For global variables usage */

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/
/*
 * [Function Name]	: passwordSuccessful
 * [Description]	:
 * 		Function that represents device options upon entering successful password.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void passwordSuccessful(void)
{
	/* Reset number of entries */
	reenter = 0;
	/* Reset number of button clicks */
	buttonCount = 0;
	/* Stop cursor blinking */
	Timer1_stop();
	/* Turn LCD cursor OFF */
	LCD_sendCommand(LCD_CURSOR_OFF);
	/* Set password as saved in EEPROM */
	passwordState = 1;
	/* Clear LCD screen & prompt user for options */
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_moveCursor(0, 0);
	LCD_displayString("+ : Open Door");
	LCD_moveCursor(1, 0);
	LCD_displayString("- : Change Pass");
	/* Wait for user to press a key */
	key = Keypad_getPressedKey();
	/* If user selected to open the door */
	if (key == '+')
	{
		/* Tell CONTROL_ECU that user selected opening the door */
		USART_sendByte(0x04);
		/* Wait for CONTROL_ECU to initiate door opening & display status on screen */
		if (USART_recieveByte() == 0x04)
		{
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			LCD_moveCursor(0, 0);
			LCD_displayString("  Opening Door");
		}
		/* Wait for CONTROL_ECU to initiate door holding & display status on screen */
		if (USART_recieveByte() == 0x04)
		{
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			LCD_moveCursor(0, 0);
			LCD_displayString("  Door Opened");
		}
		/* Wait for CONTROL_ECU to initiate door closing & display status on screen */
		if (USART_recieveByte() == 0x04)
		{
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			LCD_moveCursor(0, 0);
			LCD_displayString("  Closing Door");
		}
		/* Wait for CONTROL_ECU to finish and prompt the user to re-enter the password */
		if (USART_recieveByte() == 0x04)
		{
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			LCD_moveCursor(0, 0);
			LCD_displayString("Enter Password: ");
			LCD_moveCursor(1, 0);
			/* Restart timer1 for LCD cursor blink */
			Timer1_start(TIMER01_PRESCALER_256, 0, 15625, 0);
			/* Reset button clicks to (0) */
			buttonCount = 0;
		}
	}
	/* If user selected to change the password */
	else if (key == '-')
	{
		/* Change the state of the password to unsaved in EEPROM */
		passwordState = 0;
		/* Clear the screen and prompt the user to enter the new password */
		LCD_sendCommand(LCD_CLEAR_COMMAND);
		LCD_moveCursor(0, 0);
		LCD_displayString("Enter Password: ");
		LCD_moveCursor(1, 0);
		/* Restart timer1 for LCD cursor blink */
		Timer1_start(TIMER01_PRESCALER_256, 0, 15625, 0);
	}
}

/*
 * [Function Name]	: passwordNoMatch
 * [Description]	:
 * 		Function that represents un-matched passwords screen.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void passwordNoMatch(void)
{
	/* Change the state of the password to unsaved in EEPROM */
	passwordState = 0;
	/* Change re-entry state into first entry */
	reenter = 0;
	/* Reset button clicks to (0) */
	buttonCount = 0;
	/* Stop cursor blinking */
	Timer1_stop();
	LCD_sendCommand(LCD_CURSOR_OFF);
	/* Display a message to user when passwords entered are not matched */
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_moveCursor(0, 0);
	LCD_displayString("  PASSWORDS DO");
	LCD_moveCursor(1, 0);
	LCD_displayString("   NOT MATCH");
	/* Wait for (3) seconds */
	_delay_ms(3000);
	/* Clear the screen and prompt the user to try again */
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_moveCursor(0, 0);
	LCD_displayString("Enter Password: ");
	LCD_moveCursor(1, 0);
	/* Restart timer1 for LCD cursor blink */
	Timer1_start(TIMER01_PRESCALER_256, 0, 15625, 0);
}

/*
 * [Function Name]	: passwordFail
 * [Description]	:
 * 		Function that represents password entry failure screen.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void passwordFail(void)
{
	reenter = 0;
	/* Reset button clicks to (0) */
	buttonCount = 0;
	/* Stop cursor blinking */
	Timer1_stop();
	LCD_sendCommand(LCD_CURSOR_OFF);
	/* Display a message to user when password is incorrect */
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_moveCursor(0, 0);
	LCD_displayString("   PASSWORD");
	LCD_moveCursor(1, 0);
	LCD_displayString("   INCORRECT");
	/* Wait for (3) seconds */
	_delay_ms(3000);
	/* Clear the screen and prompt the user to try again */
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_moveCursor(0, 0);
	LCD_displayString("Enter Password: ");
	LCD_moveCursor(1, 0);
	/* Restart timer1 for LCD cursor blink */
	Timer1_start(TIMER01_PRESCALER_256, 0, 15625, 0);
}

/*
 * [Function Name]	: enterPassword
 * [Description]	:
 * 		Function that represents the password entry screen.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void enterPassword(void)
{
	/* Show a message when user tries to enter a password more than 5 digits */
	if (buttonCount == 5)
	{
		LCD_moveCursor(1, 8);
		LCD_displayString("5 Only");
		LCD_moveCursor(1, 5);
	}
	else
	{
		/* If it is not a re-entry */
		if (reenter == 0)
		{
			LCD_displayCharacter('*'); /* display the pressed keypad switch */
			password[buttonCount] = key; /* Save the key in first password array */
			buttonCount++; /* Count button click */
		}
		/* If it is a re-entry */
		else
		{
			LCD_displayCharacter('*'); /* display the pressed keypad switch */
			passwordReEnter[buttonCount] = key; /* Save the key in second password array */
			buttonCount++; /* Count button click */
		}
	}
	_delay_ms(500); /* Delay for keyboard presses */
}


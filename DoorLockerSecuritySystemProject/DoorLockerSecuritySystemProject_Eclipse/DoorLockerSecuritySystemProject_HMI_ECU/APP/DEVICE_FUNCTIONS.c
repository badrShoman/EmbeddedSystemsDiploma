/******************************************************************************
 * Module: Device Functions
 * File Name: DEVICE_FUNCTIONS.c
 * Description: Source file for device application functions.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <util/delay.h>					/* For delay functions */
#include "../MCAL/timer.h"				/* For timer usage */
#include "../MCAL/usart.h"				/* For USART usage */
#include "../HAL/lcd.h"					/* For LCD usage */
#include "../APP/DEVICE_FUNCTIONS.h"	/* For device functions prototypes */
#include "../APP/DEVICE_GLOBALS.h"		/* For global variables usage */

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/
/*
 * [Function Name]	: LCD_cursorToggle
 * [Description]	:
 * 		Function that toggles LCD cursor every 0.5 second.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void LCD_cursorToggle(void)
{
	/* Define a static toggle variable */
	static volatile uint8 toggle = LOGIC_LOW;
	/* Switch toggle & cursor value */
	switch (toggle)
	{
		case LOGIC_LOW:
			/* Turn cursor ON */
			LCD_sendCommand(LCD_CURSOR_ON);
			toggle = LOGIC_HIGH;
		break;
		case LOGIC_HIGH:
			/* Turn cursor OFF */
			LCD_sendCommand(LCD_CURSOR_OFF);
			toggle = LOGIC_LOW;
		break;
	}
}

/*
 * [Function Name]	: removePassword
 * [Description]	:
 * 		Function that removes password from LCD screen.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void removePassword(void)
{
	/* Remove written password */
	LCD_moveCursor(1, 0);
	LCD_displayString("                ");
	LCD_moveCursor(1, 0);
}

/*
 * [Function Name]	: sendNewPassword
 * [Description]	:
 * 		Function that sends a new password set by user to CONTROL_ECU.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void sendNewPassword(void)
{
	/* A counter variable for loops */
	uint8 counter = 0;
	/* Send password to CONTROL_ECU */
	_delay_ms(10);
	USART_sendByte(0x02);
	for (counter = 0; counter < 5; counter++)
	{
		USART_sendByte(password[counter]);
		_delay_ms(10);
	}
	for (counter = 0; counter < 5; counter++)
	{
		USART_sendByte(passwordReEnter[counter]);
		_delay_ms(10);
	}
}

/*
 * [Function Name]	: sendPassword
 * [Description]	:
 * 		Function that sends a password to be compared by password saved in
 * 		EEPROM to CONTROL_ECU.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void sendPassword(void)
{
	/* A counter variable for loops */
	uint8 counter = 0;
	/* Send password to CONTROL_ECU */
	_delay_ms(10);
	USART_sendByte(0x03);
	for (counter = 0; counter < 5; counter++)
	{
		USART_sendByte(password[counter]);
		_delay_ms(10);
	}
}

/*
 * [Function Name]	: breachDetection
 * [Description]	:
 * 		Function that is executes when user enters wrong password 3 times.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void breachDetection(void)
{
	/* A counter variable for loops */
	uint8 counter = 0;
	/* Display error and stop the micro-controller */
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_moveCursor(0, 0);
	LCD_displayString("      ERROR");
	/* Stop cursor blinking */
	Timer1_stop();
	LCD_sendCommand(LCD_CURSOR_OFF);
	/* Tell CONTROL_ECU user entered wrong password (3) times */
	USART_sendByte(0xF0);
	/* Wait for (60) seconds*/
	for (counter = 0; counter < 5; counter++)
	{
		_delay_ms(10000);
	}
	/* Clear the screen and prompt the user to try again */
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_moveCursor(0, 0);
	LCD_displayString("Enter Password: ");
	LCD_moveCursor(1, 0);
	/* Restart timer1 for LCD cursor blink */
	Timer1_start(TIMER01_PRESCALER_256, 0, 15625, 0);
}

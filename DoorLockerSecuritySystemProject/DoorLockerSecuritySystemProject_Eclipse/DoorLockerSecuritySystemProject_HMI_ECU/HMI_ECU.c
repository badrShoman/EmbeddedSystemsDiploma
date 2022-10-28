/******************************************************************************
 * File Name: HMI_ECI.c
 * Description: Source file for application functions.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/io.h>				/* For AVR registers */
#include <util/delay.h>			/* For delay functions */
#include "common_macros.h"		/* For common macros usage */
#include "keypad.h"				/* For keypad usage */
#include "lcd.h"				/* For LCD usage */
#include "timer.h"				/* For timer usage */
#include "usart.h"				/* For USART usage */

/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/
static uint8 key = 0; /* A variable that carries keyboard key */
static uint8 reenter = 0; /* A variable that indicates password entries */
static uint8 passwordState = 0; /* A variable that indicates whether the a password is saved */
static uint8 buttonCount = 0; /* A variable that holds number of keyboard clicks */

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
 * [Function Name]	: passwordSuccessful
 * [Description]	:
 * 		Function that is executed upon entering a successful password.
 * [Args]	: Void.
 * [Return]	: Void.
 */
static void passwordSuccessful(void)
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
 * 		Function that is executed upon entering un-matched passwords.
 * [Args]	: Void.
 * [Return]	: Void.
 */
static void passwordNoMatch(void)
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
 * 		Function that is executed upon entering a wrong password.
 * [Args]	: Void.
 * [Return]	: Void.
 */
static void passwordFail(void)
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
 * [Function Name]	: main
 * [Description]	:
 * 		The function where the program starts.
 * [Args]	: Void.
 * [Return]	: Void.
 */
int main(void)
{
	/* Variable definations */
	uint8 password[5] = { 0, 0, 0, 0, 0 }; /* Array holding first password */
	uint8 passwordReEnter[5] = { 0, 0, 0, 0, 0 }; /* Array holding re-entered password */
	uint8 failCount = 0; /* Variable indicating number of password entries failure */
	uint8 counter = 0; /* A counter variable for loops */
	/* Enable global interrupt */
	SET_BIT(SREG, 7);
	/* Initialize LCD */
	LCD_init();
	/* Create an instance of timer1 initialization structure */
	Timer_initConfig timerConfig =
			{ TIMER16BIT_CTC_OCR1A, NORMAL_OC, LOGIC_HIGH };
	/* Initialize timer1 */
	Timer1_init(&timerConfig);
	/* Set call-back function for timer1 */
	Timer1_setCallBack(LCD_cursorToggle);
	/* Create an instance of USART initialization structure */
	USART_initConfig USARTConfig = { 9600, USART_DATA_8BIT, PARITY_EVEN,
	LOGIC_LOW };
	/* Initialize USART */
	USART_init(&USARTConfig);
	/* Display text on LCD */
	LCD_displayString("Enter Password: ");
	/* Move to row 0 column 10 */
	LCD_moveCursor(1, 0);
	/* Start timer1 for LCD cursor blink */
	Timer1_start(TIMER01_PRESCALER_256, 0, 15625, 0);
	/* Determine if the password is saved in EEPROM or not */
	if (USART_recieveByte() == 0xFF)
	{
		passwordState = 1;
	}
	/* Execute program loop */
	while (TRUE)
	{
		/* Wait for user to press a key */
		key = Keypad_getPressedKey();
		if ((key <= 9) && (key >= 0))
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
		/* If user clicked '-' */
		else if (key == '-')
		{
			/* Remove written password */
			LCD_moveCursor(1, 0);
			LCD_displayString("                ");
			LCD_moveCursor(1, 0);
			/* Reset button clicks to (0) */
			buttonCount = 0;
		}
		/* If user enters the password */
		else if (key == '=')
		{
			/* Show a message when user tries to enter a password less than 5 digits */
			if (buttonCount != 5)
			{
				LCD_moveCursor(1, 7);
				LCD_displayString("Must Be 5");
				LCD_moveCursor(1, buttonCount);
			}
			else
			{
				/* Reset button clicks to (0) */
				buttonCount = 0;
				/* If the no password exists in EEPROM & it is the first entry */
				if (passwordState == 0 && reenter == 0)
				{
					/* Prompt to re-enter the password for confirmation */
					LCD_sendCommand(LCD_CLEAR_COMMAND);
					LCD_moveCursor(0, 0);
					LCD_displayString("Re-Enter Pass: ");
					LCD_moveCursor(1, 0);
					LCD_displayString("                ");
					LCD_moveCursor(1, 0);
					/* Count the next entry as the second entry */
					reenter = 1;
				}
				/* If the user already re-entered the password */
				else if (reenter == 1)
				{
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
					/* If CONTROL_ECU replied with success */
					if (USART_recieveByte() == 0x02)
					{
						/* Proceed to the next step */
						passwordSuccessful();
					}
					else
					{
						/* Clear screen for repeating */
						passwordNoMatch();
					}
				}
				/* If password already found in EEPROM*/
				else if (passwordState == 1)
				{
					/* Send password to CONTROL_ECU */
					_delay_ms(10);
					USART_sendByte(0x03);
					for (counter = 0; counter < 5; counter++)
					{
						USART_sendByte(password[counter]);
						_delay_ms(10);
					}
					/* If CONTROL_ECU replied with success */
					if (USART_recieveByte() == 0x03)
					{
						/* Proceed to the next step */
						passwordSuccessful();
						failCount = 0;
					}
					else
					{
						/* Clear screen for repeating */
						passwordFail();
						/* Count for maximum number of wrong entries */
						failCount++;
						/* If maximum number of wrong entries reached */
						if (failCount == 3)
						{
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
							/* Reset failure times */
							failCount = 0;
						}
					}
				}
			}
		}
	}
}

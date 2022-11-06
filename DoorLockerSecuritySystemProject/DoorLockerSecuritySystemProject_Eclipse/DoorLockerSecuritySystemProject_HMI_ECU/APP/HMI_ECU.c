/******************************************************************************
 * File Name: HMI_ECI.c
 * Description: Source file for application functions.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/io.h>						/* For AVR registers */
#include <util/delay.h>					/* For delay functions */
#include "../common_macros.h"			/* For common macros usage */
#include "../MCAL/timer.h"				/* For timer usage */
#include "../MCAL/usart.h"				/* For USART usage */
#include "../HAL/keypad.h"				/* For keypad usage */
#include "../HAL/lcd.h"					/* For LCD usage */
#include "../APP/DEVICE_FUNCTIONS.h"	/* For device functions prototypes */
#include "../APP/DEVICE_GLOBALS.h"		/* For global variables usage */
#include "../APP/DEVICE_SCREENS.h"		/* For device screens prototypes */

/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/
uint8 key = 0; /* A variable that carries keyboard key */
uint8 reenter = 0; /* A variable that indicates password entries */
uint8 passwordState = 0; /* A variable that indicates whether the a password is saved */
uint8 buttonCount = 0; /* A variable that holds number of keyboard clicks */
uint8 password[5] = { 0, 0, 0, 0, 0 }; /* Array holding first password */
uint8 passwordReEnter[5] = { 0, 0, 0, 0, 0 }; /* Array holding re-entered password */

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/
/*
 * [Function Name]	: main
 * [Description]	:
 * 		The function where the program starts.
 * [Args]	: Void.
 * [Return]	: Void.
 */
int main(void)
{
	/* Variables definations */
	uint8 failCount = 0; /* Variable indicating number of password entries failure */
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
			/* Enter the password on the screen */
			enterPassword();
		}
		/* If user clicked '-' */
		else if (key == '-')
		{
			/* Remove the password on the screen */
			removePassword();
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
				/* If no password exists in EEPROM & it is the first entry */
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
					/* Send the new password entries to be checked by CONTROL_ECU */
					sendNewPassword();
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
					/* Send the password to be checked by CONTROL_ECU */
					sendPassword();
					/* If CONTROL_ECU replied with success */
					if (USART_recieveByte() == 0x03)
					{
						/* Proceed to the next step */
						passwordSuccessful();
						/* Reset failure times */
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
							/* Execute breach detection protocol */
							breachDetection();
							/* Reset failure times */
							failCount = 0;
						}
					}
				}
			}
		}
	}
}

/******************************************************************************
 * Module: Device Functions
 * File Name: DEVICE_FUNCTIONS.c
 * Description: Header file for device application functions.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <util/delay.h>					/* For delay functions */
#include "../MCAL/timer.h"				/* For timer usage */
#include "../MCAL/usart.h"				/* For USART usage */
#include "../HAL/dc_motor.h"			/* For DC motor usage */
#include "../HAL/external_eeprom.h"		/* For external EEPROM usage */
#include "../APP/DEVICE_FUNCTIONS.h"	/* For device functions prototypes */

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/
/*
 * [Function Name]	: scanPassword
 * [Description]	:
 * 		Function that scans for existing password and places it in
 * 		currentPassword array.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void scanPassword(void)
{
	uint8 counter = 0; /* A counter variable for loops */
	/* Read saved password in EEPROM and copy it to current password array */
	for (counter = 0; counter < 5; counter++)
	{
		EEPROM_readByte(0x0001 + counter, &currentPassword[counter]);
		_delay_ms(10);
	}
	/* Wait for (1) second */
	_delay_ms(1000);
	/* Loop on the current password array */
	for (counter = 0; counter < 5; counter++)
	{
		/* If password was found in EEPROM */
		if (currentPassword[counter] != 0)
		{
			/* Set password state to (1) */
			passwordState = 1;
			/* Tell HMI_ECU that password was found */
			USART_sendByte(0xFF);
			break;
		}
		/* If password was not found */
		if (counter == 4)
		{
			/* Tell HMI_ECU that password was not found */
			USART_sendByte(0x00);
		}
	}
}

/*
 * [Function Name]	: timerSeconds
 * [Description]	:
 * 		Function that starts counting seconds for door motor.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void timerSeconds(void)
{
	static volatile uint8 seconds = 0; /* Define a variable that holds timer seconds */
	seconds++; /* Increase a second each timer interrupt */
	if (seconds == 15)
	{
		USART_sendByte(0x04); /* Tell HMI_ECU the states of the door*/
		DCMotor_Rotate(STOP, 0); /* Stop motor rotation upon reaching 15 seconds */
	}
	else if (seconds == 18)
	{
		USART_sendByte(0x04); /* Tell HMI_ECU the states of the door*/
		DCMotor_Rotate(COUNTER_CLOCKWISE, 100); /* Start motor rotation upon reaching 18 seconds */
	}
	else if (seconds == 33)
	{
		USART_sendByte(0x04); /* Tell HMI_ECU the states of the door*/
		DCMotor_Rotate(STOP, 0); /* Stop motor rotation upon reaching 33 seconds */
		/* Stop counting for mechanism time */
		Timer1_stop();
		/* Reset counter seconds to (0) */
		seconds = 0;
	}
}

/*
 * [Function Name]	: definePassword
 * [Description]	:
 * 		Function that compares a password and it's re-entry.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void definePassword(void)
{
	uint8 counter = 0; /* A counter variable for loops */
	/* Receive password through USART */
	for (counter = 0; counter < 5; counter++)
	{
		receivedPassword[counter] = USART_recieveByte();
	}
	/* Receive re-entered password through USART */
	for (counter = 0; counter < 5; counter++)
	{
		receivedPasswordReenter[counter] = USART_recieveByte();
	}
	/* Compare both received passwords */
	for (counter = 0; counter < 5; counter++)
	{
		/* If passwords matched or didn't, report status in either ways */
		if (receivedPassword[counter] != receivedPasswordReenter[counter])
		{
			_delay_ms(10);
			USART_sendByte(0x00);
			break;

		}
		if (counter == 4)
		{
			for (counter = 0; counter < 5; counter++)
			{
				EEPROM_writeByte(0x0001 + counter, receivedPassword[counter]);
			}
			_delay_ms(10);
			USART_sendByte(0x02);
		}
	}
}

/*
 * [Function Name]	: checkPassword
 * [Description]	:
 * 		Function that checks if received password and the password saved
 * 		in EEPROM match.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void checkPassword(void)
{
	uint8 counter = 0; /* A counter variable for loops */
	/* Receive password through USART */
	for (counter = 0; counter < 5; counter++)
	{
		receivedPassword[counter] = USART_recieveByte();
	}
	/* Check if received password matches the password in EEPROM */
	for (counter = 0; counter < 5; counter++)
	{
		/* If passwords matched or didn't, report status in either ways */
		if (receivedPassword[counter] != currentPassword[counter])
		{
			_delay_ms(10);
			USART_sendByte(0x00);
			break;
		}
		if (counter == 4)
		{
			USART_sendByte(0x03);
		}
	}
}

/*
 * [Function Name]	: doorUnlock
 * [Description]	:
 * 		Function that executes door locking & unlocking mechanisms.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void doorUnlock(void)
{
	uint8 counter = 0; /* A counter variable for loops */
	/* Rotate the motor clock wise */
	DCMotor_Rotate(CLOCKWISE, 100);
	_delay_ms(10);
	/* Tell HMI_ECU the states of the door*/
	USART_sendByte(0x04);
	/* Start counting for mechanism time */
	Timer1_start(TIMER01_PRESCALER_256, 0, 31259, 0);
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
	uint8 counter = 0; /* A counter variable for loops */
	/* Turn on buzzer */
	Buzzer_on();
	/* Wait for (60) seconds*/
	for (counter = 0; counter < 5; counter++)
	{
		_delay_ms(10000);
	}
	/* Turn off buzzer */
	Buzzer_off();
}

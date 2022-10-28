/******************************************************************************
 * File Name: CONTROL_ECU.c
 * Description: Source file for application functions.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/io.h>				/* For AVR registers */
#include <util/delay.h>			/* For delay functions */
#include "buzzer.h"				/* For Buzzer usage */
#include "common_macros.h"		/* For common macros usage */
#include "dc_motor.h"			/* For DC motor usage */
#include "external_eeprom.h"	/* For EEPROM usage */
#include "gpio.h"				/* For GPIO usage */
#include "i2c.h"				/* For I2C usage */
#include "timer.h"				/* For timer usage */
#include "usart.h"				/* For USART usage */

/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/
static uint8 receivedPassword[5] = { 0, 0, 0, 0, 0 }; /* An array for received password*/
static uint8 receivedPasswordReenter[5] = { 0, 0, 0, 0, 0 }; /* An array for receieved password re-entry */
static uint8 currentPassword[5] = { 0, 0, 0, 0, 0 }; /* An array for current password in EEPROM */
static uint8 counter = 0; /* A counter variable for loops */

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/
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
static void definePassword(void)
{
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
static void checkPassword(void)
{
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
static void doorUnlock(void)
{
	/* Rotate the motor clock wise */
	DCMotor_Rotate(CLOCKWISE, 100);
	_delay_ms(10);
	/* Tell HMI_ECU the states of the door*/
	USART_sendByte(0x04);
	/* Start counting for mechanism time */
	Timer1_start(TIMER01_PRESCALER_256, 0, 31259, 0);
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
	SET_BIT(SREG, 7); /* Enable global interrupt I-bit */
	uint8 USARTCommand = 0x00; /* A variable to store incoming USART commands */
	uint8 passwordState = 0; /* A variable that saves password in EEPROM state */
	/* Create an instance of timer0 initialization structure */
	Timer_initConfig Timer_config = { TIMER8BIT_FAST_PWM, CLEAR_OC,
	LOGIC_LOW };
	/* Initialize timer0 for PWM signal to control motor speed */
	Timer0_init(&Timer_config);
	/* Create an instance of timer1 initialization structure */
	Timer_initConfig timerConfig =
			{ TIMER16BIT_CTC_OCR1A, NORMAL_OC, LOGIC_HIGH };
	/* Initialize timer1 for compare mode to control DC motor mechanisms */
	Timer1_init(&timerConfig);
	/* Set call-back function for timer1 */
	Timer1_setCallBack(timerSeconds);
	/* Initialize buzzer */
	Buzzer_init();
	/* Turn off buzzer initially */
	Buzzer_off();
	/* Initialize DC motor */
	DCMotor_init();
	/* Create an instance of I2C initialization structure */
	I2C_initConfig I2CConfig = { 2, 2, I2C_PRESCALER_1, LOGIC_LOW, LOGIC_LOW };
	/* Initialize I2C */
	I2C_init(&I2CConfig);
	/* Create an instance of USART initialization structure */
	USART_initConfig USARTConfig = { 9600, USART_DATA_8BIT, PARITY_EVEN,
	LOGIC_LOW };
	/* Initialize USART */
	USART_init(&USARTConfig);
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
	/* Execute program loop */
	while (TRUE)
	{
		/* Wait for a command for HMI_ECU */
		USARTCommand = USART_recieveByte();
		/* Switch for incoming command */
		switch (USARTCommand)
		{
			/* A case indicating a new password needs to be checked and saved */
			case 0x02:
				definePassword();
			break;
				/* A case indicating an already stored password needs to be checked */
			case 0x03:
				checkPassword();
			break;
				/* A case indicating a successful password entry */
			case 0x04:
				doorUnlock();
			break;
				/* A case indicating an error due to many wrong password entries */
			case 0xF0:
				/* Turn on buzzer */
				Buzzer_on();
				/* Wait for (60) seconds*/
				for (counter = 0; counter < 5; counter++)
				{
					_delay_ms(10000);
				}
				/* Turn off buzzer */
				Buzzer_off();
			break;
		}
	}
}

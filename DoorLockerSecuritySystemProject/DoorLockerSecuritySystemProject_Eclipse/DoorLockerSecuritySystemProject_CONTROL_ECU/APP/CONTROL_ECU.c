/******************************************************************************
 * File Name: CONTROL_ECU.c
 * Description: Source file for application functions.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/io.h>						/* For AVR registers */
#include <util/delay.h>					/* For delay functions */
#include "../common_macros.h"			/* For common macros usage */
#include "../MCAL/gpio.h"				/* For GPIO usage */
#include "../MCAL/i2c.h"				/* For I2C usage */
#include "../MCAL/timer.h"				/* For timer usage */
#include "../MCAL/usart.h"				/* For USART usage */
#include "../HAL/buzzer.h"				/* For Buzzer usage */
#include "../HAL/dc_motor.h"			/* For DC motor usage */
#include "../HAL/external_eeprom.h"		/* For EEPROM usage */
#include "../APP/DEVICE_FUNCTIONS.h"	/* For function prototypes & global variables definations */

/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/
uint8 receivedPassword[5] = { 0, 0, 0, 0, 0 }; /* An array for received password*/
uint8 receivedPasswordReenter[5] = { 0, 0, 0, 0, 0 }; /* An array for receieved password re-entry */
uint8 currentPassword[5] = { 0, 0, 0, 0, 0 }; /* An array for current password in EEPROM */
uint8 USARTCommand = 0x00; /* A variable to store incoming USART commands */
uint8 passwordState = 0; /* A variable that saves password in EEPROM state */

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
	uint8 counter = 0; /* A counter variable for loops */
	SET_BIT(SREG, 7); /* Enable global interrupt I-bit */
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
	/* Scan for an existing password */
	scanPassword();
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
				breachDetection();
			break;
		}
	}
}

/******************************************************************************
 * File Name: fan_controller_project.c
 * Description: Source file for main function.
 * Author: Mohamed Badr
 *******************************************************************************/

#include "adc.h"							/* Initialize ADC */
#include "dc_motor.h"						/* Use DC motor*/
#include "lcd.h"							/* Use LCD */
#include "lm35_three_terminal_sensor.h"		/* Use sensor */
#include "timer.h"							/* Initialize timers */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define LCD_COMMON_COLUMN_INDEX		10

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
	/* A variable to store temperature value */
	uint8 tempValue = 0;
	/* Create an instance of ADC configurations*/
	ADC_ConfigType configType = { ADC_INT_REF_ENABLE, ADC_PRESCALER_8 };
	/* Initialize ADC */
	ADC_init(&configType);
	/* Create an instance of timer configurations*/
	Timer_initConfig Timer_config = { TIMER8BIT_FAST_PWM, CLEAR_OC,
	LOGIC_LOW };
	/* Initialize timer for PWM signal to control motor speed */
	Timer0_init(&Timer_config);
	/* Initialize LCD */
	LCD_init();
	/* Initialize DC motor */
	DCMotor_init();
	/* Display text in the middle of LCD screen */
	LCD_moveCursor(0, 4); /* Move to row 0 column 4 */
	LCD_displayString("Fan is "); /* Write the string */
	LCD_moveCursor(1, 2); /* Move to row 1 column 3 */
	LCD_displayString("Temp =      C"); /* Write the string */
	/* Execute program loop */
	while (TRUE)
	{
		/* Get temperature reading */
		tempValue = LM35_getTemperature();
		/* Check if temperature is in OFF condition */
		if (tempValue < 30)
		{
			LCD_moveCursor(0, 11); /* Move to row 0 and common column */
			LCD_displayString("OFF"); /* Write the string */
			DCMotor_Rotate(STOP, 0); /* Stop the motor */
			LCD_moveCursor(1, LCD_COMMON_COLUMN_INDEX); /* Move to row 1 and common column */
			LCD_intgerToString(tempValue); /* Write the value */
			LCD_displayCharacter(' '); /* Clear numbers after displaying value */
		}
		else
		{
			LCD_moveCursor(0, 11); /* Move to row 0 and common column */
			LCD_displayString("ON "); /* Write the string */
			LCD_moveCursor(1, LCD_COMMON_COLUMN_INDEX); /* Move to row 1 and common column */
			/* If temperature is between 30C and 60C */
			if (tempValue >= 30 && tempValue < 60)
			{
				DCMotor_Rotate(CLOCKWISE, 25); /* Start the motor at 25% speed */
			}
			/* If temperature is between 60C and 90C */
			else if (tempValue >= 60 && tempValue < 90)
			{
				DCMotor_Rotate(CLOCKWISE, 50); /* Start the motor at 50% speed */
			}
			/* If temperature is between 90C and 120C */
			else if (tempValue >= 90 && tempValue < 120)
			{
				DCMotor_Rotate(CLOCKWISE, 75); /* Start the motor at 75% speed */
			}
			/* If temperature is larger than 120C */
			else if (tempValue >= 120)
			{
				DCMotor_Rotate(CLOCKWISE, 100); /* Start the motor at 100% speed */
			}
			LCD_intgerToString(tempValue); /* Write the value */
			LCD_displayCharacter(' '); /* Clear numbers after displaying value */
		}
	}
}

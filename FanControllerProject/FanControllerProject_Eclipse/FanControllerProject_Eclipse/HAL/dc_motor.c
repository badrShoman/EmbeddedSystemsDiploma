/******************************************************************************
 * Module: DC Motor
 * File Name: dc_motor.c
 * Description: Source file for DC motor driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#include "dc_motor.h"			/* For prototypes & definitions */
#include "../MCAL/timer.h"		/* For timer usage */

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/

/*
 * [Function Name]	: DCMotor_init
 * [Description]	:
 * 		Function that initialize DC motor pins.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void DCMotor_init(void)
{
	/* Configure bridge enable pin as output pin */
	GPIO_setupPinDirection(DC_MOTOR_PORT, DC_MOTOR_ENABLE, PIN_OUTPUT);
	/* Configure bridge direction pins as output pin */
	GPIO_setupPinDirection(DC_MOTOR_PORT, DC_MOTOR_IN1, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT, DC_MOTOR_IN2, PIN_OUTPUT);
	/* Stop the motor initially */
	GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_LOW);
}

/*
 * [Function Name]	: DCMotor_Rotate
 * [Description]	:
 * 		Function that controls motor rotation & speed.
 * [Args]	:
 * [In] rotation	: Indicates rotation direction of the motor.
 * [In] speed		: Indicates speed percentage of the motor.
 * [Return]		: Void.
 */
void DCMotor_Rotate(ROTATION_STATE rotation, uint8 speed)
{
	uint8 dutyCycle = 0;	/* Initialize duty-cycle value */
	/* Convert recieved percentage into duty-cycle value */
	dutyCycle = (uint8) ((uint16) (speed * 250) / 100) + 5;
	/* Control the motor as required */
	switch (rotation)
	{
		case STOP:
			Timer0_stop(); /* Stop PWM speed control */
			/* Stop motor rotation */
			GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_LOW);
			GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_LOW);
			return;
		case CLOCKWISE:
			Timer0_start(TIMER01_PRESCALER_8, 5, dutyCycle); /* Start timer & generate PWM */
			/* Rotate motor clock-wise direction */
			GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_HIGH);
			GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_LOW);
			return;
		case COUNTER_CLOCKWISE:
			Timer0_start(TIMER01_PRESCALER_8, 5, dutyCycle); /* Start timer & generate PWM */
			/* Rotate motor counter clock-wise direction */
			GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_LOW);
			GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_HIGH);
			return;
	}
}

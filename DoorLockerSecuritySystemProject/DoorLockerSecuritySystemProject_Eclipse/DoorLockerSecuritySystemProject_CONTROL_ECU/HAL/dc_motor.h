/******************************************************************************
 * Module: DC Motor
 * File Name: dc_motor.h
 * Description: Header file for DC motor driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "../MCAL/gpio.h"			/* For usage of ports & pins definitions */
#include "../std_types.h"			/* For usage of standard defined types */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* DC motor hardware ports & pins IDs */
#define DC_MOTOR_PORT			PORTB_ID
#define DC_MOTOR_ENABLE			PIN3_ID
#define DC_MOTOR_IN1			PIN1_ID
#define DC_MOTOR_IN2			PIN2_ID

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/*
 * [Enumerate Name]	: ROTATION_STATE
 * [Description]	:
 * 		An enumerate that defines values of rotation directions for DC motor.
 */
typedef enum
{
	STOP, CLOCKWISE, COUNTER_CLOCKWISE
} ROTATION_STATE;

/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/

/*
 * [Function Name]	: DCMotor_init
 * [Description]	:
 * 		Function that initialize DC motor pins.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void DCMotor_init(void);

/*
 * [Function Name]	: DCMotor_Rotate
 * [Description]	:
 * 		Function that controls motor rotation & speed.
 * [Args]	:
 * [In] rotation	: Indicates rotation direction of the motor.
 * [In] speed		: Indicates speed percentage of the motor.
 * [Return]		: Void.
 */
void DCMotor_Rotate(ROTATION_STATE rotation, uint8 speed);

#endif /* DC_MOTOR_H_ */

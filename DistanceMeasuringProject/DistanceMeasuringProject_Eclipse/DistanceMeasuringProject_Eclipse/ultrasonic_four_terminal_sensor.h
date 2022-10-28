/******************************************************************************
 * Module: Ultrasonic Four Terminal Sensor
 * File Name: ultrasonic_four_terminal_sensor.h
 * Description: Header file for any four terminal sensor driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef ULTRASONIC_FOUR_TERMINAL_SENSOR_H_
#define ULTRASONIC_FOUR_TERMINAL_SENSOR_H_

#include "std_types.h"		/* To use standard defined types */

/*******************************************************************************
 *                           Functions Prototypes                              *
 *******************************************************************************/
/*
 * [Function Name]	: Ultrasonic_init
 * [Description]	:
 * 		Initialize the ultrasonic:
 * 			1. Initialize ICU.
 * 			2. Set ICU call-back function.
 * 			3. Set trigger output pin.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void Ultrasonic_init(void);

/*
 * [Function Name]	: Ultrasonic_trigger
 * [Description]	:
 * 		Calls Ultrasonic_readDistance and calculates distance value through
 * 		ICU timer value and returns the calculated distance.
 * [Args]	: Void.
 * [Return]	: Measured distance value.
 */
uint16 Ultrasonic_readDistance(void);

/*
 * [Function Name]	: Ultrasonic_edgeProcessing
 * [Description]	:
 * 		Executed upon finding an edge, starts counting upon detecting rising edge,
 * 		stops counting upon detecting falling edge then sets echoHighTime value
 * 		to last ICU timer value and resets number of edges detected.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_FOUR_TERMINAL_SENSOR_H_ */

/******************************************************************************
 * Module: LM35 Three Terminal Sensor
 * File Name: lm35_three_terminal_sensor.h
 * Description: Header file for any three terminal sensor driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef LM35_THREE_TERMINAL_SENSOR_H_
#define LM35_THREE_TERMINAL_SENSOR_H_

#include "adc.h"			/* For determining if interrupt mode is enabled */
#include "std_types.h"		/* For usage of standard defined types */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define SENSOR_CHANNEL_ID         2
#define SENSOR_MAX_VOLT_VALUE     1.5
#define SENSOR_MAX_TEMPERATURE    150

/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/
/*
 * [Function Name]	: LM35_getTemp
 * [Description]	:
 * 		Function that calculates sensor reading value using it's governing equation,
 * 		the readings are taken from ADC digital value and placed in g_LM35TempValue.
 * [Args]	:
 * [Out] g_LM35TempValue	: Holds last successful sensor calculation.
 * [Return]					: Void.
 */
uint8 LM35_getTemperature(void);

#endif /* LM35_THREE_TERMINAL_SENSOR_H_ */

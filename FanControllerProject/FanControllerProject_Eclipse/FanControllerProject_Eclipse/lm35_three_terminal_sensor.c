/******************************************************************************
 * Module: LM35 Three Terminal Sensor
 * File Name: lm35_three_terminal_sensor.h
 * Description: Source file for any three terminal sensor driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#include "lm35_three_terminal_sensor.h"	/* For sensor prototypes & definitions */

/*******************************************************************************
 *                           Timer Useful Equations                            *
 *******************************************************************************/
/*
 * 	Sensor Value = ( ADC Analog Channel Value * Sensor Maximum Value
 * 											* ADC Reference Voltage Value )
 * 				   /
 * 				   ( ADC Maximum Bit Value * Sensor Maximum Voltage Value )
 */

/*******************************************************************************
 *                            Functions Definitions                            *
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
uint8 LM35_getTemperature(void)
{

	uint8 LM35TempValue = 0; /* Hold temperature value */

#if (ADC_INTERRUPT_ENABLE == TRUE)

	/* Read ADC channel where the sensor is connected */
	ADC_readChannel(SENSOR_CHANNEL_ID);
	/* Calculate the temperature from the ADC value*/
	LM35TempValue = (uint8) (((uint32) g_ADCValue
				* SENSOR_MAX_TEMPERATURE * ADC_REF_VOLT_VALUE)
				/ (ADC_MAXIMUM_VALUE * SENSOR_MAX_VOLT_VALUE));

#else

	uint16 ADCRead = 0; /* Hold ADC conversion value */
	/* Get ADC conversion value */
	ADCRead = ADC_readChannel(SENSOR_CHANNEL_ID);
	/* Calculate the temperature from the ADC value*/
	LM35TempValue =
			(uint8) (((uint32) ADCRead * SENSOR_MAX_TEMPERATURE
					* ADC_REF_VOLT_VALUE)
					/ (ADC_MAXIMUM_VALUE * SENSOR_MAX_VOLT_VALUE));

#endif

	return LM35TempValue;
}


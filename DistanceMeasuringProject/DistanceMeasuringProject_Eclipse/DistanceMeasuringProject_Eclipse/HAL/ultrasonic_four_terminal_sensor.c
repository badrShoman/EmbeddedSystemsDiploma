/******************************************************************************
 * Module: Ultrasonic Four Terminal Sensor
 * File Name: ultrasonic_four_terminal_sensor.c
 * Description: Source file for any four terminal sensor driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/delay.h>									/* For delay functions */
#include "../MCAL/gpio.h"								/* For trigger & echo pin configurations */
#include "../MCAL/icu.h"								/* For ICU usage */
#include "../HAL/ultrasonic_four_terminal_sensor.h"		/* For Ultrasonic prototypes */

/*******************************************************************************
 *                         Global Variables & Pointers                         *
 *******************************************************************************/
/* Static global variable that indicates number of detected edges */
volatile static uint8 edgeCount = 0;
/* Static global variable that stores most recent time between rising & falling edges */
volatile static uint16 echoHighTime = 0;

/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/
/*
 * [Function Name]	: Ultrasonic_trigger
 * [Description]	:
 * 		Sets trigger pin to (LOGIC HIGH) for 10 micro-seconds.
 * [Args]	: Void.
 * [Return]	: Void.
 */
static void Ultrasonic_trigger(void);

/*******************************************************************************
 *                            Functions Definitions                            *
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
void Ultrasonic_init(void)
{
	/* Define structure of ICU configurations */
	ICU_ConfigType ICU_Config = { TIMER01_PRESCALER_8, RISING };
	/* Set ICU call-back function */
	ICU_setCallBack(Ultrasonic_edgeProcessing);
	/* Set trigger pin as output */
	GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);
	/* Initialize ICU */
	ICU_init(&ICU_Config);
}

/*
 * [Function Name]	: Ultrasonic_trigger
 * [Description]	:
 * 		Sets trigger pin to (LOGIC HIGH) for 10 micro-seconds.
 * [Args]	: Void.
 * [Return]	: Void.
 */
static void Ultrasonic_trigger(void)
{
	/* Set trigger pin as (LOGIC HIGH) */
	GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_HIGH);
	/* Wait 10 micro-seconds */
	_delay_us(10);
	/* Set trigger pin as (LOGIC LOW) */
	GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_LOW);
}

/*
 * [Function Name]	: Ultrasonic_trigger
 * [Description]	:
 * 		Calls Ultrasonic_readDistance and calculates distance value through
 * 		ICU timer value and returns the calculated distance.
 * [Args]	: Void.
 * [Return]	: Measured distance value.
 */
uint16 Ultrasonic_readDistance(void)
{
	/* Declare & initialize a variable that holds distance */
	uint16 distance = 0;
	/* Trigger ultrasonic */
	Ultrasonic_trigger();
	/* Calculate distance depending on ICU timer value */
	distance = ((float) 0.01731 * echoHighTime);
	/* Return distance calculated to upper layer */
	return distance;
}

/*
 * [Function Name]	: Ultrasonic_edgeProcessing
 * [Description]	:
 * 		Executed upon finding an edge, starts counting upon detecting rising edge,
 * 		stops counting upon detecting falling edge then sets echoHighTime value
 * 		to last ICU timer value and resets number of edges detected.
 * [Args]	: Void.
 * [Return]	: Void.
 */
void Ultrasonic_edgeProcessing(void)
{
	/* Increase edges count */
	edgeCount++;
	/* If the edge detected is a rising edge */
	if (edgeCount == 1)
	{
		ICU_clearTimerValue(); /* Clear previous ICU timer value to start again from this point */
		ICU_setEdgeDetectionType(FALLING); /* Set edge detection to (FALLING) */
	}
	else
	{
		echoHighTime = ICU_getInputCaptureValue(); /* Set the new ICU timer value to echoHighTime */
		ICU_clearTimerValue(); /* Clear previous ICU timer value to start again from this point */
		ICU_setEdgeDetectionType(RISING); /* Set edge detection to (RISING) */
		edgeCount = 0; /* Reset number of detected edges */
	}
}

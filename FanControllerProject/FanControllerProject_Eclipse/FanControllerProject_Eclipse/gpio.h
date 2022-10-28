/******************************************************************************
 * Module: GPIO
 * File Name: gpio.h
 * Description: Header file for the AVR GPIO driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

#include "std_types.h"		/* To use standard defined types */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* Number of ports and pins of ports */
#define NUM_OF_PORTS           4
#define NUM_OF_PINS_PER_PORT   8
/* Ports ID numbers */
#define PORTA_ID               0
#define PORTB_ID               1
#define PORTC_ID               2
#define PORTD_ID               3
/* Pins ID numbers */
#define PIN0_ID                0
#define PIN1_ID                1
#define PIN2_ID                2
#define PIN3_ID                3
#define PIN4_ID                4
#define PIN5_ID                5
#define PIN6_ID                6
#define PIN7_ID                7

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/*
 * [Enumerate Name]	: GPIO_PinDirectionType
 * [Description]	:
 * 		An enumerate that defines pin direction values for GPIO.
 */
typedef enum
{
	PIN_INPUT, PIN_OUTPUT
} GPIO_PinDirectionType;

/*
 * [Enumerate Name]	: GPIO_PinDirectionType
 * [Description]	:
 * 		An enumerate that defines port direction values for GPIO.
 */
typedef enum
{
	PORT_INPUT, PORT_OUTPUT = 0xFF
} GPIO_PortDirectionType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * [Function Name]	: GPIO_setupPinDirection
 * [Description]	:
 * 		Pin direction (INPUT/OUTPUT) setup.
 * 		The function will not handle the request until port & pin numbers are valid.
 * [Args]	:
 * [In] portNum		: Indicates port number.
 * [In] pinNum		: Indicates pin number.
 * [In] direction	: Indicates input (0) or output (1).
 * [Return]			: Void.
 */
void GPIO_setupPinDirection(uint8 portNum, uint8 pinNum,
		GPIO_PinDirectionType direction);
/*
 * [Function Name]	: GPIO_writePin
 * [Description]	:
 * 		Pin logic value (HIGH/LOW) setup.
 * 		Internal pull-resistor (ENABLE/DISABLE) setup. (INPUT PIN ONLY)
 * 		The function will not handle the request until port & pin numbers are valid.
 * [Args]	:
 * [In] portNum		: Indicates port number.
 * [In] pinNum		: Indicates pin number.
 * [In] value		: Indicates logic low (0) or logic high (1).
 * [Return]			: Void.
 */
void GPIO_writePin(uint8 portNum, uint8 pinNum, uint8 value);

/*
 * [Function Name]	: GPIO_readPin
 * [Description]	:
 * 		Read and return value of required pin as logic high (1) or logic low (0).
 * 		The function will not handle the request until port & pin numbers are valid.
 * [Args]	:
 * [In] portNum		: Indicates port number.
 * [In] pinNum		: Indicates pin number.
 * [Return]			: Value of required pin as logic high (1) or logic low (0).
 */
uint8 GPIO_readPin(uint8 portNum, uint8 pinNum);

/*
 * [Function Name]	: GPIO_setupPortDirection
 * [Description]	:
 * 		Port direction (INPUT/OUTPUT) setup.
 * 		Pins in the selected ports will be defined at once as (INPUT/OUTPUT).
 * 		The function will not handle the request until port number is valid.
 * [Args]	:
 * [In] portNum		: Indicates port number.
 * [In] direction	: Indicates a value between (0x00) or (0xFF).
 * [Return]			: Void.
 */
void GPIO_setupPortDirection(uint8 portNum, GPIO_PortDirectionType direction);

/*
 * [Function Name]	: GPIO_writePort
 * [Description]	:
 * 		Port logic value (HIGH/LOW) setup.
 * 		Pins in the selected ports will be defined at once as (HIGH/LOW).
 * 		Internal pull-resistor (ENABLE/DISABLE) setup. (INPUT PIN ONLY)
 * 		The function will not handle the request until port number is valid.
 * [Args]	:
 * [In] portNum		: Indicates port number.
 * [In] value		: Indicates a value between (0x00) or (0xFF).
 * [Return]			: Void.
 */
void GPIO_writePort(uint8 portNum, uint8 value);

/*
 * [Function Name]	: GPIO_readPort
 * [Description]	:
 * 		Read and return the value of the required port.
 * 		The function will return ZERO until port number is valid.
 * [Args]	:
 * [In] portNum		: Indicates port number.
 * [Return]			: Value of required pin as a value between (0x00) or (0xFF).
 */
uint8 GPIO_readPort(uint8 portNum);

#endif /* GPIO_H_ */

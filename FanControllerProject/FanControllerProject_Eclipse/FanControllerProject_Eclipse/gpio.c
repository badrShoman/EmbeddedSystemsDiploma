/******************************************************************************
 * Module: GPIO
 * File Name: gpio.c
 * Description: Source file for the AVR GPIO driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/io.h>				/* For AVR standard definitions & prototypes */
#include "common_macros.h"		/* For common macros usage */
#include "gpio.h"				/* For GPIO definitions & prototypes */

/*******************************************************************************
 *                      Functions Definitions                                  *
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
		GPIO_PinDirectionType direction)
{
	/*
	 * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value
	 * or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
	 */
	if ((pinNum >= NUM_OF_PINS_PER_PORT) || (portNum >= NUM_OF_PORTS))
	{
		/* DO NOTHING */
	}
	else
	{
		/* Setup the pin direction as required */
		switch (portNum)
		{
			case PORTA_ID:
				if (direction == PIN_OUTPUT)
				{
					SET_BIT(DDRA, pinNum);
				}
				else
				{
					CLEAR_BIT(DDRA, pinNum);
				}
			break;
			case PORTB_ID:
				if (direction == PIN_OUTPUT)
				{
					SET_BIT(DDRB, pinNum);
				}
				else
				{
					CLEAR_BIT(DDRB, pinNum);
				}
			break;
			case PORTC_ID:
				if (direction == PIN_OUTPUT)
				{
					SET_BIT(DDRC, pinNum);
				}
				else
				{
					CLEAR_BIT(DDRC, pinNum);
				}
			break;
			case PORTD_ID:
				if (direction == PIN_OUTPUT)
				{
					SET_BIT(DDRD, pinNum);
				}
				else
				{
					CLEAR_BIT(DDRD, pinNum);
				}
			break;
		}
	}
}

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
void GPIO_writePin(uint8 portNum, uint8 pinNum, uint8 value)
{
	/*
	 * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value
	 * or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
	 */
	if ((pinNum >= NUM_OF_PINS_PER_PORT) || (portNum >= NUM_OF_PORTS))
	{
		/* DO NOTHING */
	}
	else
	{
		/* Write the pin value as required */
		switch (portNum)
		{
			case PORTA_ID:
				if (value == LOGIC_HIGH)
				{
					SET_BIT(PORTA, pinNum);
				}
				else
				{
					CLEAR_BIT(PORTA, pinNum);
				}
			break;
			case PORTB_ID:
				if (value == LOGIC_HIGH)
				{
					SET_BIT(PORTB, pinNum);
				}
				else
				{
					CLEAR_BIT(PORTB, pinNum);
				}
			break;
			case PORTC_ID:
				if (value == LOGIC_HIGH)
				{
					SET_BIT(PORTC, pinNum);
				}
				else
				{
					CLEAR_BIT(PORTC, pinNum);
				}
			break;
			case PORTD_ID:
				if (value == LOGIC_HIGH)
				{
					SET_BIT(PORTD, pinNum);
				}
				else
				{
					CLEAR_BIT(PORTD, pinNum);
				}
			break;
		}
	}
}

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
uint8 GPIO_readPin(uint8 portNum, uint8 pinNum) /* @suppress("No return") */
{
	/*
	 * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value
	 * or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
	 */
	if ((pinNum >= NUM_OF_PINS_PER_PORT) || (portNum >= NUM_OF_PORTS))
	{
		/* DO NOTHING */
	}
	else
	{
		/* Read the pin value as required */
		switch (portNum)
		{
			case PORTA_ID:
				if (BIT_IS_SET(PINA, pinNum))
				{
					return LOGIC_HIGH;
				}
				else
				{
					return LOGIC_LOW;
				}
			case PORTB_ID:
				if (BIT_IS_SET(PINB, pinNum))
				{
					return LOGIC_HIGH;
				}
				else
				{
					return LOGIC_LOW;
				}
			case PORTC_ID:
				if (BIT_IS_SET(PINC, pinNum))
				{
					return LOGIC_HIGH;
				}
				else
				{
					return LOGIC_LOW;
				}
			case PORTD_ID:
				if (BIT_IS_SET(PIND, pinNum))
				{
					return LOGIC_HIGH;
				}
				else
				{
					return LOGIC_LOW;
				}
		}
	}
}

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
void GPIO_setupPortDirection(uint8 portNum, GPIO_PortDirectionType direction)
{
	/*
	 * Check if the input number is greater than NUM_OF_PORTS value.
	 */
	if (portNum >= NUM_OF_PORTS)
	{
		/* DO NOTHING */
	}
	else
	{
		/* Setup the port direction as required */
		switch (portNum)
		{
			case PORTA_ID:
				DDRA = direction;
			break;
			case PORTB_ID:
				DDRB = direction;
			break;
			case PORTC_ID:
				DDRC = direction;
			break;
			case PORTD_ID:
				DDRD = direction;
			break;
		}
	}
}

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
void GPIO_writePort(uint8 portNum, uint8 value)
{
	/*
	 * Check if the input number is greater than NUM_OF_PORTS value.
	 */
	if (portNum >= NUM_OF_PORTS)
	{
		/* DO NOTHING */
	}
	else
	{
		/* Write the port value as required */
		switch (portNum)
		{
			case PORTA_ID:
				PORTA = value;
			break;
			case PORTB_ID:
				PORTB = value;
			break;
			case PORTC_ID:
				PORTC = value;
			break;
			case PORTD_ID:
				PORTD = value;
			break;
		}
	}
}

/*
 * [Function Name]	: GPIO_readPort
 * [Description]	:
 * 		Read and return the value of the required port.
 * 		The function will return ZERO until port number is valid.
 * [Args]	:
 * [In] portNum		: Indicates port number.
 * [Return]			: Value of required pin as a value between (0x00) or (0xFF).
 */
uint8 GPIO_readPort(uint8 portNum) /* @suppress("No return") */
{
	/*
	 * Check if the input number is greater than NUM_OF_PORTS value.
	 */
	if (portNum >= NUM_OF_PORTS)
	{
		/* DO NOTHING */
	}
	else
	{
		/* Read the port value as required */
		switch (portNum)
		{
			case PORTA_ID:
				return PINA;
			case PORTB_ID:
				return PINB;
			case PORTC_ID:
				return PINC;
			case PORTD_ID:
				return PIND;
		}
	}
}

/******************************************************************************
 * Module: LCD
 * File Name: lcd.c
 * Description: Source file for the LCD driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <stdlib.h>				/* For itoa function */
#include <util/delay.h> 		/* For delay functions */
#include "../common_macros.h" 	/* For common macros usage */
#include "lcd.h"				/* For LCD prototypes & definitions */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * [Function Name]	: LCD_init
 * [Description]	:
 * 		Initialize the LCD:
 * 			1. Setup the LCD pins directions using GPIO driver.
 * 			2. Setup the LCD data mode (4 BIT MODE/8 BIT MODE).
 * [Args]	: Void.
 * [Return]	: Void.
 */
void LCD_init(void)
{
	/* Configure direction for RS and E pins as output pins */
	GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT_ID, LCD_E_PIN_ID, PIN_OUTPUT);

#if (LCD_RW_GROUND == FALSE)

	/* If RW pin is connected to micro-controller and not grounded */
	GPIO_setupPinDirection(LCD_RW_PORT_ID, LCD_RW_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(LCD_RW_PORT_ID, LCD_RW_PIN_ID, LOGIC_LOW);

#endif

	_delay_ms(20); /* LCD power ON time delay, should be always > 15 ms */

#if(LCD_DATA_BITS_MODE == 4)

	/* Configure 4 pins in the data port as output pins */
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, PIN_OUTPUT);
	/* 4-bit initialization command of LCD  */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);
	/* Use two lines LCD in 4-bits data mode & (5x7) dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);

#elif(LCD_DATA_BITS_MODE == 8)

	/* Configure data port as output port */
	GPIO_setupPortDirection(LCD_DATA_PORT_ID, PORT_OUTPUT);
	/* Use two lines LCD in 8-bits data mode & (5x7) dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);

#endif

	LCD_sendCommand(LCD_CURSOR_OFF); /* Turn cursor off */
	LCD_sendCommand(LCD_CLEAR_COMMAND); /* Clear LCD at the beginning */
}

/*
 * [Function Name]	: LCD_sendCommand
 * [Description]	:
 * 		Send commands to LCD screen.
 * [Args]	:
 * [In] command	: Indicates the command that is sent.
 * [Return]		: Void.
 */
void LCD_sendCommand(uint8 command)
{
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_LOW); /* Instruction Mode RS = 0 */
	_delay_ms(1); /* Delay for processing Tas = 50 ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH); /* Enable LCD E = 1 */
	_delay_ms(1); /* Delay for processing Tpw - Tdws = 190 ns */

#if (LCD_RW_GROUND == FALSE)

	/* If RW pin is connected to micro-controller and not grounded */
	GPIO_setupPinDirection(LCD_RW_PORT_ID, LCD_RW_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(LCD_RW_PORT_ID, LCD_RW_PIN_ID, LOGIC_LOW);

#endif

#if(LCD_DATA_BITS_MODE == 4)

	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command, PIN4_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command, PIN5_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command, PIN6_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command, PIN7_ID));
	_delay_ms(1); /* Delay for processing Tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E = 0 */
	_delay_ms(1); /* Delay for processing Th = 13 ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH); /* Enable LCD E = 1 */
	_delay_ms(1); /* Delay for processing Tpw - Tdws = 190 ns */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command, PIN0_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command, PIN1_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command, PIN2_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command, PIN3_ID));
	_delay_ms(1); /* Delay for processing Tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E = 0 */
	_delay_ms(1); /* Delay for processing Th = 13 ns */

#elif(LCD_DATA_BITS_MODE == 8)

	GPIO_writePort(LCD_DATA_PORT_ID, command); /* Write to the data bus from D0 to D7 */
	_delay_ms(1); /* Delay for processing Tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E = 0 */
	_delay_ms(1); /* Delay for processing Th = 13 ns */

#endif
}

/*
 * [Function Name]	: LCD_displayCharacter
 * [Description]	:
 * 		Display a character on LCD screen
 * [Args]	:
 * [In] data	: Indicates the character that is sent.
 * [Return]		: Void.
 */
void LCD_displayCharacter(uint8 data)
{
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_HIGH); /* Data Mode RS = 1 */
	_delay_ms(1); /* Delay for processing Tas = 50 ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH); /* Enable LCD E = 1 */
	_delay_ms(1); /* Delay for processing Tpw - Tdws = 190 ns */

#if(LCD_DATA_BITS_MODE == 4)

	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(data, PIN4_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(data, PIN5_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(data, PIN6_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(data, PIN7_ID));
	_delay_ms(1); /* Delay for processing Tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E = 0 */
	_delay_ms(1); /* Delay for processing Th = 13 ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH); /* Enable LCD E = 1 */
	_delay_ms(1); /* Delay for processing Tpw - Tdws = 190 ns */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(data, PIN0_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(data, PIN1_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(data, PIN2_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(data, PIN3_ID));
	_delay_ms(1); /* Delay for processing Tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E = 0 */
	_delay_ms(1); /* Delay for processing Th = 13 ns */

#elif(LCD_DATA_BITS_MODE == 8)

	GPIO_writePort(LCD_DATA_PORT_ID, data); /* Write to the data bus from D0 to D7 */
	_delay_ms(1); /* Delay for processing Tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E = 0 */
	_delay_ms(1); /* Delay for processing Th = 13 ns */

#endif
}

/*
 * [Function Name]	: LCD_displayString
 * [Description]	:
 * 		Display a string on the screen.
 * [Args] 	:
 * [In] string	: Indicates the string that is sent.
 * [Return]		: Void.
 */
void LCD_displayString(const uint8 *string)
{
	/* Display each character in the recieved string array */
	uint8 i = 0;
	while (string[i] != '\0')
	{
		LCD_displayCharacter(string[i]);
		i++;
	}
	/***************** Another Method ***********************
	while ((*string) != '\0')
	{
		LCD_displayCharacter(*string);
		string++;
	}
	 *********************************************************/
}

/*
 * [Function Name]	: LCD_moveCursor
 * [Description]	:
 * 		Move the cursor to a specified row and column index on the screen.
 * [Args]	:
 * [In] row		: Indicates the row required.
 * [In] column	: Indicates the column required.
 * [Return]		: Void.
 */
void LCD_moveCursor(uint8 row, uint8 column)
{
	/* Calculate the address in the LCD DDRAM, */
	/* then move the LCD cursor to this specific address. */
	switch (row)
	{
		case 0:
			LCD_sendCommand(column | LCD_SET_CURSOR_LOCATION);
		break;
		case 1:
			LCD_sendCommand((column + 0x40) | LCD_SET_CURSOR_LOCATION);
		break;
		case 2:
			LCD_sendCommand((column + 0x10) | LCD_SET_CURSOR_LOCATION);
		break;
		case 3:
			LCD_sendCommand((column + 0x50) | LCD_SET_CURSOR_LOCATION);
		break;
	}
}

/*
 * [Function Name]	: LCD_displayStringRowColumn
 * [Description]	:
 * 		Display a string in a specified row and column index on the screen.
 * [Args]	:
 * [In] row		: Indicates the row required.
 * [In] column	: Indicates the column required.
 * [In] string	: Indicates the string that is sent.
 * [Return]		: Void.
 */
void LCD_displayStringRowColumn(uint8 row, uint8 column, const uint8 *string)
{
	LCD_moveCursor(row, column); /* Go to to the specified LCD position */
	LCD_displayString(string); /* Display string */
}

/*
 * [Function Name]	: LCD_intgerToString
 * [Description]	:
 * 		Display a decimal value on the screen.
 * [Args]	:
 * [In] data	: Indicates the decimal value required.
 * [Return]		: Void.
 */
void LCD_intgerToString(uint16 data)
{
	uint8 buffer[16]; /* Hold the ASCII result */
	itoa(data, buffer, 10); /* Convert data to it's ASCII value - 10 for decimal */
	LCD_displayString(buffer); /* Display string */
}

/*
 * [Function Name]	: LCD_intgerToString
 * [Description]	:
 * 		Send the clear screen command
 * [Args]	: Void.
 * [Return]	: Void.
 */
void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_COMMAND); /* Send clear display command */
}

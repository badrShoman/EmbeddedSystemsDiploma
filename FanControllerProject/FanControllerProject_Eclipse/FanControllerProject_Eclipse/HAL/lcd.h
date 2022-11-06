/******************************************************************************
 * Module: LCD
 * File Name: lcd.h
 * Description: Header file for the LCD driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "../MCAL/gpio.h"			/* For usage of ports & pins definitions */
#include "../std_types.h"			/* For usage of standard defined types */

/*******************************************************************************
 *                        Definitions & Configurations                         *
 *******************************************************************************/
/* LCD Data bits mode configuration (4-BIT MODE/8-BIT MODE) */
#define LCD_DATA_BITS_MODE 		8
#define LCD_RW_GROUND			TRUE

#if((LCD_DATA_BITS_MODE != 4) && (LCD_DATA_BITS_MODE != 8))

#error "LCD data bits should be (4) or (8)"

#endif

/* LCD hardware ports & pins IDs */
#define LCD_RS_PORT_ID                 PORTD_ID
#define LCD_RS_PIN_ID                  PIN0_ID
#define LCD_E_PORT_ID                  PORTD_ID
#define LCD_E_PIN_ID                   PIN2_ID

#if (LCD_RW_GROUND == FALSE)

#define LCD_RW_PORT_ID				   PORTD_ID
#define LCD_RW_PIN_ID				   PIN1_ID

#endif

#define LCD_DATA_PORT_ID               PORTC_ID

/* If the LCD mode is 4-bit then just define needed pins */
#if (LCD_DATA_BITS_MODE == 4)

#define LCD_DB4_PIN_ID                 PIN3_ID
#define LCD_DB5_PIN_ID                 PIN4_ID
#define LCD_DB6_PIN_ID                 PIN5_ID
#define LCD_DB7_PIN_ID                 PIN6_ID

#endif

/* LCD Commands (SEE DATASHEET) */
#define LCD_CLEAR_COMMAND              		 0x01
#define LCD_GO_TO_HOME                		 0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE 		 0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE   		 0x28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32
#define LCD_CURSOR_OFF               		 0x0C
#define LCD_CURSOR_ON                 		 0x0E
#define LCD_SET_CURSOR_LOCATION      	     0x80
#define LCD_SHIFT_CURSOR_L			  		 0x04
#define LCD_SHIFT_DISP_R 			  		 0x05
#define LCD_SHIFT_CURSOR_R 			 		 0x06
#define LCD_SHIFT_DISP_L			  		 0x07
#define LCD_STATE_OFF 				  		 0x08
#define LCD_STATE_CURSOR_ON			 		 0x0A
#define LCD_DISP_ON_CURSOR_BLINK	  		 0x0F
#define LCD_SHIFT_CURSOR_POS_L		   		 0x10
#define LCD_SHIFT_CURSOR_POS_R		  		 0x14
#define LCD_SHIFT_LCD_L				  		 0x18
#define LCD_SHIFT_LCD_R				  		 0x1C

/*******************************************************************************
 *                      Functions Prototypes                                   *
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
void LCD_init(void);

/*
 * [Function Name]	: LCD_sendCommand
 * [Description]	:
 * 		Send commands to LCD screen.
 * [Args]	:
 * [In] command	: Indicates the command that is sent.
 * [Return]		: Void.
 */
void LCD_sendCommand(uint8 command);

/*
 * [Function Name]	: LCD_displayCharacter
 * [Description]	:
 * 		Display a character on LCD screen
 * [Args]	:
 * [In] data	: Indicates the character that is sent.
 * [Return]		: Void.
 */
void LCD_displayCharacter(uint8 data);

/*
 * [Function Name]	: LCD_displayString
 * [Description]	:
 * 		Display a string on the screen.
 * [Args] 	:
 * [In] string	: Indicates the string that is sent.
 * [Return]		: Void.
 */
void LCD_displayString(const uint8 *string);

/*
 * [Function Name]	: LCD_moveCursor
 * [Description]	:
 * 		Move the cursor to a specified row and column index on the screen.
 * [Args]	:
 * [In] row		: Indicates the row required.
 * [In] column	: Indicates the column required.
 * [Return]		: Void.
 */
void LCD_moveCursor(uint8 row, uint8 column);

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
void LCD_displayStringRowColumn(uint8 row, uint8 column, const uint8 *string);

/*
 * [Function Name]	: LCD_intgerToString
 * [Description]	:
 * 		Display a decimal value on the screen.
 * [Args]	:
 * [In] data	: Indicates the decimal value required.
 * [Return]		: Void.
 */
void LCD_intgerToString(uint16 data);

/*
 * [Function Name]	: LCD_intgerToString
 * [Description]	:
 * 		Send the clear screen command
 * [Args]	: Void.
 * [Return]	: Void.
 */
void LCD_clearScreen(void);

#endif /* LCD_H_ */

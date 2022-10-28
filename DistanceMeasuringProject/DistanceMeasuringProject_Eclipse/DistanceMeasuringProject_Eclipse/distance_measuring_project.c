/******************************************************************************
 * File Name: distance_measuring_project.c
 * Description: Source file for main function.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/io.h>								/* Enable global interrupt */
#include "common_macros.h"						/* For common macros usage */
#include "lcd.h"								/* For LCD usage */
#include "ultrasonic_four_terminal_sensor.h"	/* For Ultrasonic usage */

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/
/*
 * [Function Name]	: main
 * [Description]	:
 * 		The function where the program starts.
 * [Args]	: Void.
 * [Return]	: Void.
 */
int main(void)
{
	/* Enable global interrupt */
	SET_BIT(SREG, 7);
	/* Initialize LCD */
	LCD_init();
	/* Initialize Ultrasonic */
	Ultrasonic_init();
	/* Display text */
	LCD_displayString("Distance:     cm");
	/* Execute program loop */
	while (TRUE)
	{
		LCD_moveCursor(0, 10); /* Move to row 0 column 10 */
		LCD_intgerToString(Ultrasonic_readDistance()); /* Display calculated distance */
		LCD_displayCharacter(' '); /* Clear numbers after displaying value */
	}
}

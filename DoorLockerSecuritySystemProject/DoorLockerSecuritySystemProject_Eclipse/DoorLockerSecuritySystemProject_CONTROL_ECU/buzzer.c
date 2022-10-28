/******************************************************************************
 * Module: Buzzer
 * File Name: buzzer.c
 * Description: Source file for buzzer.
 * Author: Mohamed Badr
 *******************************************************************************/

#include "buzzer.h"				/* For buzzer prototypes & definitions */

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/
/*
 * [Function Name]	: Buzzer_init
 * [Description]	:
 * 		Function that initializes buzzer pin.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Buzzer_init(void)
{

#if (BUZZER_PIN_OUTPUT == TRUE)

	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);

#else

	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_INPUT);

#endif

	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

/*
 * [Function Name]	: Buzzer_on
 * [Description]	:
 * 		Function that turns buzzer on.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Buzzer_on(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}

/*
 * [Function Name]	: Buzzer_off
 * [Description]	:
 * 		Function that turns buzzer off.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Buzzer_off(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

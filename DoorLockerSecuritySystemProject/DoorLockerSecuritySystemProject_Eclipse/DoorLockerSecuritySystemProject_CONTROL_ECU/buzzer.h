/******************************************************************************
 * Module: Buzzer
 * File Name: buzzer.h
 * Description: Header file for buzzer.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"				/* For timer associated GPIO pins */
#include "std_types.h"			/* To use standard defined types */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* Choose buzzer pin direction */
#define BUZZER_PIN_OUTPUT				TRUE
/* Choose buzzer port & pin */
#define BUZZER_PORT_ID					PORTA_ID
#define BUZZER_PIN_ID					PIN0_ID

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * [Function Name]	: Buzzer_init
 * [Description]	:
 * 		Function that initializes buzzer pin.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Buzzer_init(void);

/*
 * [Function Name]	: Buzzer_on
 * [Description]	:
 * 		Function that turns buzzer on.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Buzzer_on(void);

/*
 * [Function Name]	: Buzzer_off
 * [Description]	:
 * 		Function that turns buzzer off.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */

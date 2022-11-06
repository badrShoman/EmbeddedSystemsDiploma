/******************************************************************************
 * Module: ICU
 * File Name: icu.c
 * Description: Source file for ICU driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/io.h> 					/* For ICU related registers */
#include <avr/interrupt.h> 				/* For ICU ISR */
#include "../common_macros.h" 			/* For usage of common macros */
#include "../MCAL/icu.h"				/* For ICU prototypes & definitions */
#include "../MCAL/gpio.h"				/* For ICU pin configuration */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
/* A pointer that holds the address of the call-back function */
static volatile void (*g_interruptCallBack_Ptr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
/*
 * [Interrupt Vector]	: TIMER1_CAPT_vect
 * [Description]		:
 * 		An interrupt that acts upon detecting an edge.
 */
ISR(TIMER1_CAPT_vect)
{
	if (g_interruptCallBack_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_interruptCallBack_Ptr)(); /* Execute callback function */
	}
}

/*******************************************************************************
 *                           Functions Definitions                             *
 *******************************************************************************/
/*
 * [Function Name]	: ICU_init
 * [Description]	:
 * 		Function that initialize ICU.
 * 		By default:
 * 			1. Initialize timer1.
 * 			2. Enable input capture interrupt.
 * 			3. Set edge required to be detected.
 * 			4. Set pre-scaler
 * [Args]	:
 * [In] Config_Ptr	: Includes pre-scaler & edge values.
 * [Return]			: Void.
 */
void ICU_init(const ICU_ConfigType *Config_Ptr)
{
	/* Select edge accordingly (MUST BE SELECTED FIRST) */
	switch ((*Config_Ptr).edge)
	{
		case RISING:
			SET_BIT(TCCR1B, ICES1);
		break;
		case FALLING:
			CLEAR_BIT(TCCR1B, ICES1);
		break;
	}
	/* Create an instance of Timer16Bit_initConfig to initialize timer1*/
	Timer_initConfig Timer1_config = { TIMER16BIT_NORMAL, NORMAL_OC,
			LOGIC_LOW };
	/* Initialize timer1 and disable it's interrupt*/
	Timer1_init(&Timer1_config);
	/* Initialize ICU copied value from timer1 by ZERO */
	ICR1 = 0;
	/* Enable ICU interrupt */
	SET_BIT(TIMSK, TICIE1);
	/* Setup ICU pin direction */
	GPIO_setupPinDirection(PORTD_ID, PIN6_ID, PIN_INPUT);
	/* Start timer1 in normal mode */
	Timer1_start((*Config_Ptr).prescaler, 0, 0, 0);
}

/*
 * [Function Name]	: ICU_setCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void ICU_setCallBack(void (*Ptr2Function)(void))
{
	g_interruptCallBack_Ptr = Ptr2Function;
}

/*
 * [Function Name]	: ICU_setEdgeDetectionType
 * [Description]	:
 * 		Function that sets ICU edge required to be detected.
 * [Args]	:
 * [In] edge	: Inducates edge required.
 * [Return]		: Void.
 */
void ICU_setEdgeDetectionType(const ICU_EDGE_TYPE edge)
{
	/* Select edge accordingly */
	switch (edge)
	{
		case RISING:
			SET_BIT(TCCR1B, ICES1);
		break;
		case FALLING:
			CLEAR_BIT(TCCR1B, ICES1);
		break;
	}
}

/*
 * [Function Name]	: ICU_getInputCaptureValue
 * [Description]	:
 * 		Function that return the value of ICU last copied count of timer1.
 * [Args]		: Void.
 * [Return]		: ICU last copied count of timer1.
 */
uint16 ICU_getInputCaptureValue(void)
{
	return ICR1;
}

/*
 * [Function Name]	: ICU_clearTimerValue
 * [Description]	:
 * 		Function that resets timer1 count value to ZERO.
 * [Args]		: Void.
 * [Return]		: Void
 */
void ICU_clearTimerValue(void)
{
	TCNT1 = 0;
}

/*
 * [Function Name]	: ICU_deInit
 * [Description]	:
 * 		Function that de-initialize timer1, clearing all it's settings & values.
 * [Args]		: Void.
 * [Return]		: Void
 */
void ICU_deInit(void)
{
	Timer1_deInit();
}

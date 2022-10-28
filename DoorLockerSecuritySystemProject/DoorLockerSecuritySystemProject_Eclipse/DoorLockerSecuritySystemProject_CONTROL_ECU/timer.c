/******************************************************************************
 * Module: Timer
 * File Name: timer.h
 * Description: Source file for timer driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/interrupt.h>		/* For ISR of timers */
#include <avr/io.h>				/* For timer registers usage */
#include "common_macros.h"		/* For common macros usage */
#include "gpio.h"				/* For timer associated GPIO pins */
#include "timer.h"				/* For timer prototypes & definitions */

/*******************************************************************************
 *                           Timer Useful Equations                            *
 *******************************************************************************/
/*
 * 		Resolution = Pre-scaler / Frequency
 * 		Time = Resolution * Ticks
 */

/*******************************************************************************
 *                           Timer Hardware Registers                          *
 *******************************************************************************/
/**                               8-Bit Timer                                 **/
/*
 * TCCR0/2 register bits description:
 *
 * 		FOC0/2 	 = 		 (0) Clear in non-PWM mode.
 * 			  	   		 (1) Set in non-PWM mode.
 *
 * 		WGM10:00/12:02 = (00) Normal mode.
 * 				   		 (01) PWM, Phase Correct.
 * 				   		 (10) CTC.
 * 				   		 (11) Fast PWM.
 *
 *		Compare Output Mode, non-PWM Mode:
 * 		COM10:00/12:02 = (00) Normal port operation, OC0 disconnected.
 * 				  		 (01) Toggle OC0 on compare match.
 * 				   		 (10) Clear OC0 on compare match.
 * 				   		 (11) Set OC0 on compare match.
 *
 *		Compare Output Mode, Fast PWM Mode:
 * 		COM10:00/12:02 = (00) Normal port operation, OC0 disconnected.
 * 				   		 (01) Reserved.
 * 				   		 (10) Clear OC0 on compare match, set OC0 at BOTTOM (non-inverting).
 * 				   		 (11) Set OC0 on compare match, set OC0 at BOTTOM (inverting).
 *
 *		Compare Output Mode, Phase Correct PWM Mode:
 * 		COM10:00/12:02 = (00) Normal port operation, OC0 disconnected.
 * 				   		 (01) Reserved.
 * 				   		 (10) Clear OC0 on compare match up-counting, Set OC0 on compare match down-counting
 * 				   		 (11) Set OC0 on compare match up-counting, Set OC0 on compare match down-counting
 */
/** Timer0 **/
/*
 *		CS02:00		   = (000) No clock source.
 * 				   		 (001) Clock source, no pre-scaler.
 * 				   		 (010) Clock source/8.
 * 				   		 (011) Clock source/64.
 * 				   		 (100) Clock source/256.
 * 				   		 (101) Clock source/1024.
 * 				   		 (110) External clock on T0 pin, clock on falling edge.
 * 				   		 (111) External clock on T0 pin, clock on rising edge.
 */
/** Timer2 **/
/*
 *		CS22:20		   = (000) No clock source.
 * 				   		 (001) Clock source, no pre-scaler.
 * 				   		 (010) Clock source/8.
 * 				   		 (011) Clock source/32.
 * 				   		 (100) Clock source/64.
 * 				   		 (101) Clock source/128.
 * 				   		 (110) Clock source/256.
 * 				   		 (111) Clock source/1024.
 *
 * TCNT0/2 register description: Hold timer start value.
 *
 * OCR0/2 register description: Hold timer compare value.
 *
 * TIMSK register bits description:
 *
 * 		OCIE0/2 	 = (0) Interrupt enable on timer compare match.
 * 			  	   	   (1) Interrupt disable on timer compare match.
 *
 * 		TOIE0/2 	 = (0) Interrupt enable on timer overflow.
 * 			  	   	   (1) Interrupt disable on timer overflow.
 *
 * TIFR register bits description:
 *
 * 		OCF0/2 		 = (0) Timer compare match flag unraised. (Automatic clear on interrupt)
 * 			  		   (1) Timer compare match flag raised. (Automatic clear on interrupt)
 *
 * 		TOV0/2 		 = (0) Timer overflow flag unraised. (Automatic clear on interrupt)
 * 			  	 	   (1) Timer overflow flag raised. (Automatic clear on interrupt)
 */
/**                               16-Bit Timer                                 **/
/*
 * TCCR1A register bits description:
 *
 *		Compare Output Mode, non-PWM Mode:
 * 		COM1A1:0/COM1B1:0 = (00) Normal port operation, OC1A/OC1B disconnected.
 * 				  		 	(01) Toggle OC1A/OC1B on compare match.
 * 				   		 	(10) Clear OC1A/OC1B on compare match.
 * 				   		 	(11) Set OC1A/OC1B on compare match.
 *
 *		Compare Output Mode, Fast PWM Mode:
 * 		COM1A1:0/COM1B1:0 = (00) Normal port operation, OC1A/OC1B disconnected.
 * 				   		 	(01) Toggle OC1A on compare match, OC1B disconnected (Except WGM13:00 = 15).
 * 				   		 	(10) Clear OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM (non-inverting).
 * 				   		 	(11) Set OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM (inverting).
 *
 *		Compare Output Mode, Phase Correct PWM Mode:
 * 		COM1A1:0/COM1B1:0 = (00) Normal port operation, OC1A/OC1B disconnected.
 * 				   		 	(01) Toggle OC1A on compare match, OC1B disconnected (Except WGM13:00 = 9 or 15).
 * 				   		 	(10) Clear OC1A/OC1B on compare match up-counting, Set OC1A/OC1B on compare match down-counting
 * 				   		 	(11) Set OC1A/OC1B on compare match up-counting, Set OC1A/OC1B on compare match down-counting
 *
 * 		FOC1A/FOC1B 	  = (00) Clear in non-PWM mode.
 * 			  	   		    (11) Set in non-PWM mode.
 *
 * 		WGM13:10		  = (0000) Normal mode.
 * 				   		 	(0001) PWM, Phase Correct, 8-bit.
 * 				   		 	(0010) PWM, Phase Correct, 9-bit.
 * 				   		 	(0011) PWM, Phase Correct, 10-bit.
 * 				   		 	(0100) CTC, OCR1A
 * 				   		 	(0101) Fast PWM, 8-bit.
 * 				   		 	(0110) Fast PWM, 9-bit.
 * 				   		 	(0111) Fast PWM, 10-bit.
 * 				   		 	(1000) PWM, Phase & Frequency Correct, ICR1.
 * 				   		 	(1001) PWM, Phase & Frequency Correct, OCR1A.
 * 				   		 	(1010) PWM, Phase Correct, ICR1.
 * 				   		 	(1011) PWM, Phase Correct, OCR1A.
 * 				   		 	(1100) CTC, ICR1.
 * 				   		 	(1101) Reserved.
 * 				   		 	(1110) Fast PWM, ICR1
 * 				   		 	(1111) Fast PWM, OCR1A
 *
 *		CS12:10			  = (000) No clock source.
 * 				   		 	(001) Clock source, no pre-scaler.
 * 				   		 	(010) Clock source/8.
 * 				   		 	(011) Clock source/64.
 * 				   		 	(100) Clock source/256.
 * 				   		 	(101) Clock source/1024.
 * 				   		 	(110) External clock on T1 pin, clock on falling edge.
 * 				   		 	(111) External clock on T1 pin, clock on rising edge.
 *
 * TCNT1 register description: Hold timer start value.
 *
 * OCR1A register description: Hold timer unit A compare value.
 *
 * OCR1B register description: Hold timer unit B compare value.
 *
 * TIMSK register bits description:
 *
 * 		OCIE1A	 	 = (0) Interrupt enable on timer unit A compare match.
 * 			  	   	   (1) Interrupt disable on timer unit A compare match.
 *
 * 		OCIE1B	 	 = (0) Interrupt enable on timer unit B compare match.
 * 			  	   	   (1) Interrupt disable on timer unit B compare match.
 *
 * 		TOIE1	 	 = (0) Interrupt enable on timer overflow.
 * 			  	   	   (1) Interrupt disable on timer overflow.
 *
 * TIFR register bits description:
 *
 * 		OCF1A 		 = (0) Timer unit A compare match flag unraised. (Automatic clear on interrupt)
 * 			  		   (1) Timer unit A compare match flag raised. (Automatic clear on interrupt)
 *
 * 		OCF1B 		 = (0) Timer unit B compare match flag unraised. (Automatic clear on interrupt)
 * 			  		   (1) Timer unit B compare match flag raised. (Automatic clear on interrupt)
 *
 * 		TOV1 		 = (0) Timer overflow flag unraised. (Automatic clear on interrupt)
 * 			  	 	   (1) Timer overflow flag raised. (Automatic clear on interrupt)
 */

/*******************************************************************************
 *                            Global Pointers                                  *
 *******************************************************************************/
/* Pointers that hold the addresses of the call-back functions */

#if (TIMER0_ENABLE == TRUE)

static volatile void (*g_timer0CallBack_Ptr)(void) = NULL_PTR;

#endif

#if (TIMER1_ENABLE == TRUE)

static volatile void (*g_timer1CallBackUnitA_Ptr)(void) = NULL_PTR;
static volatile void (*g_timer1CallBackUnitB_Ptr)(void) = NULL_PTR;

#endif

#if (TIMER2_ENABLE == TRUE)

static volatile void (*g_timer2CallBack_Ptr)(void) = NULL_PTR;

#endif

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

#if (TIMER0_ENABLE == TRUE)

/*
 * [Interrupt Vector]	: TIMER0_OVF_vect
 * [Description]		:
 * 		An interrupt that acts upon overflow of timer0.
 */
ISR(TIMER0_OVF_vect)
{
	if (g_timer0CallBack_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_timer0CallBack_Ptr)(); /* Execute callback function */
	}
}

/*
 * [Interrupt Vector]	: TIMER0_COMP_vect
 * [Description]		:
 * 		An interrupt that acts upon compare match of timer0.
 */
ISR(TIMER0_COMP_vect)
{
	if (g_timer0CallBack_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_timer0CallBack_Ptr)(); /* Execute callback function */
	}
}

#endif

#if (TIMER1_ENABLE == TRUE)

/*
 * [Interrupt Vector]	: TIMER1_OVF_vect
 * [Description]		:
 * 		An interrupt that acts upon overflow of timer1.
 */
ISR(TIMER1_OVF_vect)
{
	if (g_timer1CallBackUnitA_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_timer1CallBackUnitA_Ptr)(); /* Execute callback function */
	}
}

/*
 * [Interrupt Vector]	: TIMER1_COMPA_vect
 * [Description]		:
 * 		An interrupt that acts upon compare match of timer1 unit A.
 */
ISR(TIMER1_COMPA_vect)
{
	if (g_timer1CallBackUnitA_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_timer1CallBackUnitA_Ptr)(); /* Execute callback function */
	}
}

/*
 * [Interrupt Vector]	: TIMER1_COMPB_vect
 * [Description]		:
 * 		An interrupt that acts upon compare match of timer1 unit B.
 */
ISR(TIMER1_COMPB_vect)
{
	if (g_timer1CallBackUnitB_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_timer1CallBackUnitB_Ptr)(); /* Execute callback function */
	}
}

#endif

#if (TIMER2_ENABLE == TRUE)

/*
 * [Interrupt Vector]	: TIMER2_OVF_vect
 * [Description]		:
 * 		An interrupt that acts upon overflow of timer2.
 */
ISR(TIMER2_OVF_vect)
{
	if (g_timer2CallBack_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_timer2CallBack_Ptr)(); /* Execute callback function */
	}
}

/*
 * [Interrupt Vector]	: TIMER2_COMP_vect
 * [Description]		:
 * 		An interrupt that acts upon compare match of timer2.
 */
ISR(TIMER2_COMP_vect)
{
	if (g_timer2CallBack_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_timer2CallBack_Ptr)(); /* Execute callback function */
	}
}

#endif

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/

#if (TIMER0_ENABLE == TRUE)

/*
 * [Function Name]	: Timer0_init
 * [Description]	:
 * 		Function that initialize timer0 to selected mode, sets compare output mode if
 * 		chosen and sets it's interrupt.
 * [Args]	:
 * [In] timerConfig		: Contains selected mode, selected output compare mode and
 * 						  interrupt state.
 * [Return]				: Void.
 */
void Timer0_init(const Timer_initConfig *timerConfig)
{
	/* Clear previous timer0 initialization data */
	CLEAR_REG(TCCR0);
	/* Select timer mode */
	switch ((*timerConfig).mode)
	{
		case TIMER8BIT_NORMAL:
			SET_BIT(TCCR0, FOC0); /* Only enable in non-PWM mode */
		break;
		case TIMER8BIT_PHASE_PWM:
			SET_BIT(TCCR0, WGM00);
		break;
		case TIMER8BIT_CTC:
			SET_BIT(TCCR0, FOC0); /* Only enable in non-PWM mode */
			SET_BIT(TCCR0, WGM01);
		break;
		case TIMER8BIT_FAST_PWM:
			SET_BIT(TCCR0, WGM01);
			SET_BIT(TCCR0, WGM00);
		break;
	}
	if ((*timerConfig).mode == TIMER8BIT_CTC)
	{
		/* Change interrupt state accordingly */
		switch ((*timerConfig).interruptEnable)
		{
			case LOGIC_HIGH:
				SET_BIT(TIMSK, OCIE0);
				CLEAR_BIT(TIMSK, TOIE0);
			break;
			case LOGIC_LOW:
				CLEAR_BIT(TIMSK, OCIE0);
				CLEAR_BIT(TIMSK, TOIE0);
			break;
		}
		/* Set compare output mode accordingly */
		switch ((*timerConfig).compareOutputMode)
		{
			case NORMAL_OC:
				CLEAR_BIT(TCCR0, COM01);
				CLEAR_BIT(TCCR0, COM00);
			break;
			case TOGGLE_OC:
				CLEAR_BIT(TCCR0, COM01);
				SET_BIT(TCCR0, COM00);
				GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
			break;
			case CLEAR_OC:
				SET_BIT(TCCR0, COM01);
				CLEAR_BIT(TCCR0, COM00);
				GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
			break;
			case SET_OC:
				SET_BIT(TCCR0, COM01);
				SET_BIT(TCCR0, COM00);
				GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
			break;
		}
	}
	else
	{
		/* Change interrupt state accordingly */
		switch ((*timerConfig).interruptEnable)
		{
			case LOGIC_HIGH:
				CLEAR_BIT(TIMSK, OCIE0);
				SET_BIT(TIMSK, TOIE0);
			break;
			case LOGIC_LOW:
				CLEAR_BIT(TIMSK, OCIE0);
				CLEAR_BIT(TIMSK, TOIE0);
			break;
		}
		/* Set compare output mode accordingly */
		switch ((*timerConfig).compareOutputMode)
		{
			case NORMAL_OC:
				CLEAR_BIT(TCCR0, COM01);
				CLEAR_BIT(TCCR0, COM00);
			break;
			case TOGGLE_OC:
			break;
			case CLEAR_OC:
				SET_BIT(TCCR0, COM01);
				CLEAR_BIT(TCCR0, COM00);
				GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
			break;
			case SET_OC:
				SET_BIT(TCCR0, COM01);
				SET_BIT(TCCR0, COM00);
				GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
			break;
		}
	}
}

/*
 * [Function Name]	: Timer0_setCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for timer0 interrupt
 * 		for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer0_setCallBack(void (*Ptr2Function)(void))
{
	g_timer0CallBack_Ptr = Ptr2Function;
}

/*
 * [Function Name]	: Timer0_start
 * [Description]	:
 * 		Function that starts timer0 in by setting it's pre-scaler, compare value
 * 		& start value.
 * [Args]	:
 * [In] prescaler			: Indicates pre-scaler value.
 * [In] start				: Indicates start value.
 * [In] compareValue		: Indicates compare value.
 * [Return]					: Void.
 */
void Timer0_start(TIMER01_PRESCALER prescaler, uint8 start, uint8 compareValue)
{
	prescaler &= 0x07; /* Set pre-scaler to (111) if it is larger */
	TCNT0 = start; /* Set timer starting value */
	OCR0 = compareValue; /* Set timer compare value */
	/* Clear previous pre-scaler value & set new pre-scaler */
	OVERWRITE_REG(TCCR0, 0xF8, prescaler);
}

/*
 * [Function Name]	: Timer0_stop
 * [Description]	:
 * 		Function that stops timer0 in any mode, by setting it's pre-scaler to zero.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer0_stop(void)
{
	OVERWRITE_REG(TCCR0, 0xF8, 0x00); /* Clear pre-scaler */
}

/*
 * [Function Name]	: Timer0_deInit
 * [Description]	:
 * 		Function that clears all timer0 settings.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer0_deInit(void)
{
	/* Clear timer0 registers */
	CLEAR_REG(TCCR0);
	CLEAR_REG(TCNT0);
	CLEAR_REG(OCR0);
	/* Disable timer0 interrupts */
	CLEAR_BIT(TIMSK, OCIE0);
	CLEAR_BIT(TIMSK, TOIE0);
	/* Clear timer0 flags */
	SET_BIT(TIFR, OCF0);
	SET_BIT(TIFR, TOV0);
}

#endif

#if (TIMER1_ENABLE == TRUE)

void Timer1_init(const Timer_initConfig *timerConfig)
{
	/* Clear previous timer0 initialization data */
	CLEAR_REG(TCCR1A);
	CLEAR_REG(TCCR1B);
	/* Select timer mode */
	switch ((*timerConfig).mode)
	{
		case TIMER16BIT_NORMAL:
			SET_BIT(TCCR1A, FOC1A); /* Only enable in non-PWM mode */
			SET_BIT(TCCR1A, FOC1B); /* Only enable in non-PWM mode */
			CLEAR_BIT(TCCR1B, WGM13);
			CLEAR_BIT(TCCR1B, WGM12);
			CLEAR_BIT(TCCR1A, WGM11);
			CLEAR_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_PHASE_PWM_8BIT:
			CLEAR_BIT(TCCR1B, WGM13);
			CLEAR_BIT(TCCR1B, WGM12);
			CLEAR_BIT(TCCR1A, WGM11);
			SET_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_PHASE_PWM_9BIT:
			CLEAR_BIT(TCCR1B, WGM13);
			CLEAR_BIT(TCCR1B, WGM12);
			SET_BIT(TCCR1A, WGM11);
			CLEAR_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_PHASE_PWM_10BIT:
			CLEAR_BIT(TCCR1B, WGM13);
			CLEAR_BIT(TCCR1B, WGM12);
			SET_BIT(TCCR1A, WGM11);
			SET_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_CTC_OCR1A:
			SET_BIT(TCCR1A, FOC1A); /* Only enable in non-PWM mode */
			SET_BIT(TCCR1A, FOC1B); /* Only enable in non-PWM mode */
			CLEAR_BIT(TCCR1B, WGM13);
			SET_BIT(TCCR1B, WGM12);
			CLEAR_BIT(TCCR1A, WGM11);
			CLEAR_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_FAST_PWM_8BIT:
			CLEAR_BIT(TCCR1B, WGM13);
			SET_BIT(TCCR1B, WGM12);
			CLEAR_BIT(TCCR1A, WGM11);
			SET_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_FAST_PWM_9BIT:
			CLEAR_BIT(TCCR1B, WGM13);
			SET_BIT(TCCR1B, WGM12);
			SET_BIT(TCCR1A, WGM11);
			CLEAR_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_FAST_PWM_10BIT:
			CLEAR_BIT(TCCR1B, WGM13);
			SET_BIT(TCCR1B, WGM12);
			SET_BIT(TCCR1A, WGM11);
			SET_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_PHASE_FREQ_PWM_ICR1:
			SET_BIT(TCCR1B, WGM13);
			CLEAR_BIT(TCCR1B, WGM12);
			CLEAR_BIT(TCCR1A, WGM11);
			CLEAR_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_PHASE_FREQ_PWM_OCR1A:
			SET_BIT(TCCR1B, WGM13);
			CLEAR_BIT(TCCR1B, WGM12);
			CLEAR_BIT(TCCR1A, WGM11);
			SET_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_PHASE_PWM_ICR1:
			SET_BIT(TCCR1B, WGM13);
			CLEAR_BIT(TCCR1B, WGM12);
			SET_BIT(TCCR1A, WGM11);
			CLEAR_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_PHASE_PWM_OCR1A:
			SET_BIT(TCCR1B, WGM13);
			CLEAR_BIT(TCCR1B, WGM12);
			SET_BIT(TCCR1A, WGM11);
			SET_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_CTC_ICR1:
			SET_BIT(TCCR1A, FOC1A); /* Only enable in non-PWM mode */
			SET_BIT(TCCR1A, FOC1B); /* Only enable in non-PWM mode */
			SET_BIT(TCCR1B, WGM13);
			SET_BIT(TCCR1B, WGM12);
			CLEAR_BIT(TCCR1A, WGM11);
			CLEAR_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_FAST_PWM_ICR1:
			SET_BIT(TCCR1B, WGM13);
			SET_BIT(TCCR1B, WGM12);
			SET_BIT(TCCR1A, WGM11);
			CLEAR_BIT(TCCR1A, WGM10);
		break;
		case TIMER16BIT_FAST_PWM_OCR1A:
			SET_BIT(TCCR1B, WGM13);
			SET_BIT(TCCR1B, WGM12);
			SET_BIT(TCCR1A, WGM11);
			SET_BIT(TCCR1A, WGM10);
		break;
	}
	if (((*timerConfig).mode == TIMER16BIT_CTC_OCR1A)
			|| ((*timerConfig).mode == TIMER16BIT_CTC_ICR1))
	{
		/* Change interrupt state accordingly */
		switch ((*timerConfig).interruptEnable)
		{
			case LOGIC_HIGH:
				SET_BIT(TIMSK, OCIE1A);
				SET_BIT(TIMSK, OCIE1B);
				CLEAR_BIT(TIMSK, TOIE1);
			break;
			case LOGIC_LOW:
				CLEAR_BIT(TIMSK, OCIE1A);
				CLEAR_BIT(TIMSK, OCIE1B);
				CLEAR_BIT(TIMSK, TOIE1);
			break;
		}
		/* Select compare mode */
		switch ((*timerConfig).compareOutputMode)
		{
			case NORMAL_OC:
				CLEAR_BIT(TCCR1A, COM1A1);
				CLEAR_BIT(TCCR1A, COM1B1);
				CLEAR_BIT(TCCR1A, COM1A0);
				CLEAR_BIT(TCCR1A, COM1B0);
			break;
			case TOGGLE_OC:
				CLEAR_BIT(TCCR1A, COM1A1);
				CLEAR_BIT(TCCR1A, COM1B1);
				SET_BIT(TCCR1A, COM1A0);
				SET_BIT(TCCR1A, COM1B0);
				GPIO_setupPinDirection(PORTD_ID, PIN4_ID, PIN_OUTPUT);
				GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);
			break;
			case CLEAR_OC:
				SET_BIT(TCCR1A, COM1A1);
				SET_BIT(TCCR1A, COM1B1);
				CLEAR_BIT(TCCR1A, COM1A0);
				CLEAR_BIT(TCCR1A, COM1B0);
				GPIO_setupPinDirection(PORTD_ID, PIN4_ID, PIN_OUTPUT);
				GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);
			break;
			case SET_OC:
				SET_BIT(TCCR1A, COM1A1);
				SET_BIT(TCCR1A, COM1B1);
				SET_BIT(TCCR1A, COM1A0);
				SET_BIT(TCCR1A, COM1B0);
				GPIO_setupPinDirection(PORTD_ID, PIN4_ID, PIN_OUTPUT);
				GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);
			break;
		}
	}
	else
	{
		/* Change interrupt state accordingly */
		switch ((*timerConfig).interruptEnable)
		{
			case LOGIC_HIGH:
				CLEAR_BIT(TIMSK, OCIE1A);
				CLEAR_BIT(TIMSK, OCIE1B);
				SET_BIT(TIMSK, TOIE1);
			break;
			case LOGIC_LOW:
				CLEAR_BIT(TIMSK, OCIE1A);
				CLEAR_BIT(TIMSK, OCIE1B);
				CLEAR_BIT(TIMSK, TOIE1);
			break;
		}
		/* Set compare output mode accordingly */
		switch ((*timerConfig).compareOutputMode)
		{
			case NORMAL_OC:
				CLEAR_BIT(TCCR1A, COM1A1);
				CLEAR_BIT(TCCR1A, COM1B1);
				CLEAR_BIT(TCCR1A, COM1A0);
				CLEAR_BIT(TCCR1A, COM1B0);
			break;
			case TOGGLE_OC:
				CLEAR_BIT(TCCR1A, COM1A1);
				CLEAR_BIT(TCCR1A, COM1B1);
				SET_BIT(TCCR1A, COM1A0);
				SET_BIT(TCCR1A, COM1B0);
				GPIO_setupPinDirection(PORTD_ID, PIN4_ID, PIN_OUTPUT);
				if ((*timerConfig).mode != TIMER16BIT_PHASE_FREQ_PWM_OCR1A
						|| (*timerConfig).mode != TIMER16BIT_FAST_PWM_OCR1A)
				{
					GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);
				}
			break;
			case CLEAR_OC:
				SET_BIT(TCCR1A, COM1A1);
				SET_BIT(TCCR1A, COM1B1);
				CLEAR_BIT(TCCR1A, COM1A0);
				CLEAR_BIT(TCCR1A, COM1B0);
				GPIO_setupPinDirection(PORTD_ID, PIN4_ID, PIN_OUTPUT);
				GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);
			break;
			case SET_OC:
				SET_BIT(TCCR1A, COM1A1);
				SET_BIT(TCCR1A, COM1B1);
				SET_BIT(TCCR1A, COM1A0);
				SET_BIT(TCCR1A, COM1B0);
				GPIO_setupPinDirection(PORTD_ID, PIN4_ID, PIN_OUTPUT);
				GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);
			break;
		}
	}
}

/*
 * [Function Name]	: Timer1_setCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for timer1
 * 		normal mode or compare unit A interrupt for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer1_setCallBack(void (*Ptr2Function)(void))
{
	g_timer1CallBackUnitA_Ptr = Ptr2Function;
}

/*
 * [Function Name]	: Timer1_setCallBackUnitB
 * [Description]	:
 * 		Function that sets the call-back function address for timer1
 * 		compare unit B interrupt for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer1_setCallBackUnitB(void (*Ptr2Function)(void))
{
	g_timer1CallBackUnitB_Ptr = Ptr2Function;
}

/*
 * [Function Name]	: Timer1_start
 * [Description]	:
 * 		Function that starts timer1 in by setting it's pre-scaler, compare values
 * 		& start value.
 * [Args]	:
 * [In] prescaler		: Indicates pre-scaler value.
 * [In] start			: Indicates start value.
 * [In] compareValueA	: Indicates compare value for unit A.
 * [In] compareValueB	: Indicates compare value for unit B.
 * [Return]				: Void.
 */
void Timer1_start(TIMER01_PRESCALER prescaler, uint16 start,
		uint16 compareValueA, uint16 compareValueB)
{
	prescaler &= 0x07; /* Set pre-scaler to (111) if it is larger */
	TCNT1 = start; /* Set timer starting value */
	/* Choose between compare unit A or compare unit B or both */
	OCR1A = compareValueA;
	OCR1B = compareValueB;
	/* If interrupt is enable in initialization, enable it for
	 * it's corresponding compare unit chosen */
	if (compareValueA != 0 && compareValueB == 0)
	{
		if (BIT_IS_SET(TIMSK, OCIE1A))
		{
			CLEAR_BIT(TIMSK, OCIE1B);
		}
	}
	else if (compareValueA == 0 && compareValueB != 0)
	{
		if (BIT_IS_SET(TIMSK, OCIE1B))
		{
			CLEAR_BIT(TIMSK, OCIE1A);
		}
	}
	else if (compareValueA == 0 && compareValueB == 0)
	{
		CLEAR_BIT(TIMSK, OCIE1A);
		CLEAR_BIT(TIMSK, OCIE1B);
	}
	/* Clear previous pre-scaler value & set new pre-scaler */
	OVERWRITE_REG(TCCR1B, 0xF8, prescaler);
}

/*
 * [Function Name]	: Timer1_stop
 * [Description]	:
 * 		Function that stops timer1 in any mode, by setting it's pre-scaler to zero.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer1_stop(void)
{
	OVERWRITE_REG(TCCR1B, 0xF8, 0x00); /* Clear pre-scaler */
}

/*
 * [Function Name]	: Timer1_deInit
 * [Description]	:
 * 		Function that clears all timer1 settings.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer1_deInit(void)
{
	/* Clear timer1 registers */
	CLEAR_REG(TCCR1A);
	CLEAR_REG(TCCR1B);
	CLEAR_REG(TCNT1);
	CLEAR_REG(OCR1A);
	CLEAR_REG(OCR1B);
	CLEAR_REG(ICR1);
	/* Disable timer1 interrupts */
	CLEAR_BIT(TIMSK, TICIE1);
	CLEAR_BIT(TIMSK, OCIE1A);
	CLEAR_BIT(TIMSK, OCIE1B);
	CLEAR_BIT(TIMSK, TOIE1);
	/* Clear timer1 flags */
	SET_BIT(TIFR, ICF1);
	SET_BIT(TIFR, OCF1A);
	SET_BIT(TIFR, OCF1B);
	SET_BIT(TIFR, TOV1);
}

#endif

#if (TIMER2_ENABLE == TRUE)

/*
 * [Function Name]	: Timer2_init
 * [Description]	:
 * 		Function that initialize timer2 to selected mode, sets compare output mode if
 * 		chosen and sets it's interrupt.
 * [Args]	:
 * [In] timerConfig		: Contains selected mode, selected output compare mode and
 * 						  interrupt state.
 * [Return]				: Void.
 */
void Timer2_init(const Timer_initConfig *timerConfig)
{
	/* Clear previous timer0 initialization data */
	CLEAR_REG(TCCR2);
	/* Select timer mode */
	switch ((*timerConfig).mode)
	{
		case TIMER8BIT_NORMAL:
			SET_BIT(TCCR2, FOC2); /* Only enable in non-PWM mode */
		break;
		case TIMER8BIT_PHASE_PWM:
			SET_BIT(TCCR2, WGM20);
		break;
		case TIMER8BIT_CTC:
			SET_BIT(TCCR2, FOC2); /* Only enable in non-PWM mode */
			SET_BIT(TCCR2, WGM21);
		break;
		case TIMER8BIT_FAST_PWM:
			SET_BIT(TCCR2, WGM21);
			SET_BIT(TCCR2, WGM20);
		break;
	}
	if ((*timerConfig).mode == TIMER8BIT_CTC)
	{
		/* Change interrupt state accordingly */
		switch ((*timerConfig).interruptEnable)
		{
			case LOGIC_HIGH:
				SET_BIT(TIMSK, OCIE2);
				CLEAR_BIT(TIMSK, TOIE2);
			break;
			case LOGIC_LOW:
				CLEAR_BIT(TIMSK, OCIE2);
				CLEAR_BIT(TIMSK, TOIE2);
			break;
		}
		/* Set compare output mode accordingly */
		switch ((*timerConfig).compareOutputMode)
		{
			case NORMAL_OC:
				CLEAR_BIT(TCCR2, COM21);
				CLEAR_BIT(TCCR2, COM20);
			break;
			case TOGGLE_OC:
				CLEAR_BIT(TCCR2, COM21);
				SET_BIT(TCCR2, COM20);
				GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);
			break;
			case CLEAR_OC:
				SET_BIT(TCCR2, COM21);
				CLEAR_BIT(TCCR2, COM20);
				GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);
			break;
			case SET_OC:
				SET_BIT(TCCR2, COM21);
				SET_BIT(TCCR2, COM20);
				GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);
			break;
		}
	}
	else
	{
		/* Change interrupt state accordingly */
		switch ((*timerConfig).interruptEnable)
		{
			case LOGIC_HIGH:
				CLEAR_BIT(TIMSK, OCIE2);
				SET_BIT(TIMSK, TOIE2);
			break;
			case LOGIC_LOW:
				CLEAR_BIT(TIMSK, OCIE2);
				CLEAR_BIT(TIMSK, TOIE2);
			break;
		}
		/* Set compare output mode accordingly */
		switch ((*timerConfig).compareOutputMode)
		{
			case NORMAL_OC:
				CLEAR_BIT(TCCR2, COM21);
				CLEAR_BIT(TCCR2, COM20);
			break;
			case TOGGLE_OC:
			break;
			case CLEAR_OC:
				SET_BIT(TCCR2, COM21);
				CLEAR_BIT(TCCR2, COM20);
				GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);
			break;
			case SET_OC:
				SET_BIT(TCCR2, COM21);
				SET_BIT(TCCR2, COM20);
				GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);
			break;
		}
	}
}

/*
 * [Function Name]	: Timer2_setCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for timer2 interrupt
 * 		for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer2_setCallBack(void (*Ptr2Function)(void))
{
	g_timer2CallBack_Ptr = Ptr2Function;
}

/*
 * [Function Name]	: Timer2_start
 * [Description]	:
 * 		Function that starts timer2 in by setting it's pre-scaler, compare value
 * 		& start value.
 * [Args]	:
 * [In] prescaler			: Indicates pre-scaler value.
 * [In] start				: Indicates start value.
 * [In] compareValue		: Indicates compare value.
 * [Return]					: Void.
 */
void Timer2_start(TIMER2_PRESCALER prescaler, uint8 start, uint8 compareValue)
{
	prescaler &= 0x07; /* Set pre-scaler to (111) if it is larger */
	TCNT2 = start; /* Set timer starting value */
	OCR2 = compareValue; /* Set timer compare value */
	/* Clear previous pre-scaler value & set new pre-scaler */
	OVERWRITE_REG(TCCR2, 0xF8, prescaler);
}

/*
 * [Function Name]	: Timer2_stop
 * [Description]	:
 * 		Function that stops timer2 in any mode, by setting it's pre-scaler to zero.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer2_stop(void)
{
	OVERWRITE_REG(TCCR2, 0xF8, 0x00); /* Clear pre-scaler */
}

/*
 * [Function Name]	: Timer2_deInit
 * [Description]	:
 * 		Function that clears all timer2 settings.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer2_deInit(void)
{
	/* Clear timer2 registers */
	CLEAR_REG(TCCR2);
	CLEAR_REG(TCNT2);
	CLEAR_REG(OCR2);
	/* Disable timer2 interrupts */
	CLEAR_BIT(TIMSK, OCIE2);
	CLEAR_BIT(TIMSK, TOIE2);
	/* Clear timer2 flags */
	SET_BIT(TIFR, OCF2);
	SET_BIT(TIFR, TOV2);
}

#endif

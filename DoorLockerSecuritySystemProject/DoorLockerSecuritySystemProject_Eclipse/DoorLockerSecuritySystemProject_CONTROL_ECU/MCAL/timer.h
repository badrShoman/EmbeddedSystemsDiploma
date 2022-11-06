/******************************************************************************
 * Module: Timer
 * File Name: timer.h
 * Description: Header file for timer driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "../std_types.h"		/* To use standard defined types */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* Choose timers to enable, disable never used timers to decrease code size */
#define TIMER0_ENABLE			TRUE
#define TIMER1_ENABLE			TRUE
#define TIMER2_ENABLE			FALSE

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/*
 * [Enumerate Name]	: TIMER_MODE
 * [Description]	:
 * 		An enumerate that defines chosen timer mode.
 */
typedef enum
{
	TIMER8BIT_NORMAL, TIMER8BIT_PHASE_PWM, TIMER8BIT_CTC, TIMER8BIT_FAST_PWM
} TIMER8BIT_MODE;

/*
 * [Enumerate Name]	: TIMER_COMP_OUTPUT_MODE
 * [Description]	:
 * 		An enumerate that defines 8-bit timer compare output mode.
 */
typedef enum
{
	NORMAL_OC, TOGGLE_OC, CLEAR_OC, SET_OC
} TIMER_COMP_OUTPUT_MODE;

#if (TIMER1_ENABLE == TRUE)

/*
 * [Enumerate Name]	: TIMER16BIT_PHASE_PWM_MODE
 * [Description]	:
 * 		An enumerate that defines 16-bit timer mode.
 */
typedef enum
{
	TIMER16BIT_NORMAL,
	TIMER16BIT_PHASE_PWM_8BIT,
	TIMER16BIT_PHASE_PWM_9BIT,
	TIMER16BIT_PHASE_PWM_10BIT,
	TIMER16BIT_CTC_OCR1A,
	TIMER16BIT_FAST_PWM_8BIT,
	TIMER16BIT_FAST_PWM_9BIT,
	TIMER16BIT_FAST_PWM_10BIT,
	TIMER16BIT_PHASE_FREQ_PWM_ICR1,
	TIMER16BIT_PHASE_FREQ_PWM_OCR1A,
	TIMER16BIT_PHASE_PWM_ICR1,
	TIMER16BIT_PHASE_PWM_OCR1A,
	TIMER16BIT_CTC_ICR1,
	TIMER16BIT_FAST_PWM_ICR1 = 14,
	TIMER16BIT_FAST_PWM_OCR1A
} TIMER16BIT_MODE;

#endif

#if (TIMER0_ENABLE == TRUE || TIMER1_ENABLE == TRUE)

/*
 * [Enumerate Name]	: TIMER01_PRESCALER
 * [Description]	:
 * 		An enumerate that defines timer0 and timer1 pre-scaler.
 */
typedef enum
{
	TIMER01_NO_PRESCALER,
	TIMER01_PRESCALER_1,
	TIMER01_PRESCALER_8,
	TIMER01_PRESCALER_64,
	TIMER01_PRESCALER_256,
	TIMER01_PRESCALER_1024,
	TIMER01_PRESCALER_EXT_CLOCK_FALLING,
	TIMER01_PRESCALER_EXT_CLOCK_RISING
} TIMER01_PRESCALER;

#endif

#if (TIMER2_ENABLE == TRUE)

/*
 * [Enumerate Name]	: TIMER2_PRESCALER
 * [Description]	:
 * 		An enumerate that defines timer2 pre-scaler.
 */
typedef enum
{
	TIMER2_NO_PRESCALER,
	TIMER2_PRESCALER_1,
	TIMER2_PRESCALER_8,
	TIMER2_PRESCALER_32,
	TIMER2_PRESCALER_64,
	TIMER2_PRESCALER_128,
	TIMER2_PRESCALER_256,
	TIMER2_PRESCALER_1024
} TIMER2_PRESCALER;

#endif

/*******************************************************************************
 *                                 Structures                                  *
 *******************************************************************************/
/*
 * [Structure Name]	: Timer_initConfig
 * [Description]	:
 * 		A structure in which it's instance hold timer mode,
 * 		compare output mode & interrupt enable in 1 byte
 * 		to be used in all timer initialization functions.
 */
typedef struct
{
	uint8 mode :4;
	TIMER_COMP_OUTPUT_MODE compareOutputMode :2;
	uint8 interruptEnable :1;
	uint8 :0;
} Timer_initConfig;

/*******************************************************************************
 *                              Functions Prototypes                           *
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
void Timer0_init(const Timer_initConfig *timerConfig);

/*
 * [Function Name]	: Timer0_setCallBack
 * [Description]	:
 * 		Function that sets the call-back function addresses for timer0 interrupts
 * 		for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer0_setCallBack(void (*Ptr2Function)(void));

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
void Timer0_start(TIMER01_PRESCALER prescaler, uint8 start, uint8 compareValue);

/*
 * [Function Name]	: Timer0_stop
 * [Description]	:
 * 		Function that stops timer0 in any mode, by setting it's pre-scaler to zero.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer0_stop(void);

/*
 * [Function Name]	: Timer0_deInit
 * [Description]	:
 * 		Function that clears all timer0 settings.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer0_deInit(void);

#endif

#if (TIMER1_ENABLE == TRUE)

void Timer1_init(const Timer_initConfig *timerConfig);

/*
 * [Function Name]	: Timer1_setCallBack
 * [Description]	:
 * 		Function that sets the call-back function addresses for timer1
 * 		normal mode or compare unit A interrupts for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer1_setCallBack(void (*Ptr2Function)(void));

/*
 * [Function Name]	: Timer1_setCallBackUnitB
 * [Description]	:
 * 		Function that sets the call-back function addresses for timer1
 * 		compare unit B interrupt for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer1_setCallBackUnitB(void (*Ptr2Function)(void));

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
		uint16 compareValueA, uint16 compareValueB);

/*
 * [Function Name]	: Timer1_stop
 * [Description]	:
 * 		Function that stops timer1 in any mode, by setting it's pre-scaler to zero.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer1_stop(void);

/*
 * [Function Name]	: Timer1_deInit
 * [Description]	:
 * 		Function that clears all timer1 settings.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer1_deInit(void);

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
void Timer2_init(const Timer_initConfig *timerConfig);

/*
 * [Function Name]	: Timer2_setCallBack
 * [Description]	:
 * 		Function that sets the call-back function addresses for timer2 interrupts
 * 		for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer2_setCallBack(void (*Ptr2Function)(void));

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
void Timer2_start(TIMER2_PRESCALER prescaler, uint8 start, uint8 compareValue);

/*
 * [Function Name]	: Timer2_stop
 * [Description]	:
 * 		Function that stops timer2 in any mode, by setting it's pre-scaler to zero.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer2_stop(void);

/*
 * [Function Name]	: Timer2_deInit
 * [Description]	:
 * 		Function that clears all timer2 settings.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void Timer2_deInit(void);

#endif

#endif /* TIMER_H_ */

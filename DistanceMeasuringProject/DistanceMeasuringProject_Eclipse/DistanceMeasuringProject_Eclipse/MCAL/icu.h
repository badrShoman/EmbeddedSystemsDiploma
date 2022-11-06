/******************************************************************************
 * Module: ICU
 * File Name: icu.h
 * Description: Header file for ICU driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef ICU_H_
#define ICU_H_

#include "../std_types.h"			/* To use standard defined types */
#include "../MCAL/timer.h"			/* To use timer1 */

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
/*
 * [Enumerate Name]	: ICU_EDGE_TYPE
 * [Description]	:
 * 		An enumerate that defines values edges detected in ICU.
 */
typedef enum
{
	FALLING, RISING
} ICU_EDGE_TYPE;

/*
 * [Structure Name]	: ICU_ConfigType
 * [Description]	:
 * 		A structure in which it's instance hold pre-scaler
 * 		 & edge vlue in 1 byte to be used in ICU_init.
 */
typedef struct
{
	TIMER01_PRESCALER prescaler :3;
	ICU_EDGE_TYPE edge :1;
} ICU_ConfigType;

/*******************************************************************************
 *                           Functions Prototypes                              *
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
void ICU_init(const ICU_ConfigType *Config_Ptr);

/*
 * [Function Name]	: ICU_setCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void ICU_setCallBack(void (*Ptr2Function)(void));

/*
 * [Function Name]	: ICU_setEdgeDetectionType
 * [Description]	:
 * 		Function that sets ICU edge required to be detected.
 * [Args]	:
 * [In] edge	: Inducates edge required.
 * [Return]		: Void.
 */
void ICU_setEdgeDetectionType(const ICU_EDGE_TYPE edge);

/*
 * [Function Name]	: ICU_getInputCaptureValue
 * [Description]	:
 * 		Function that return the value of ICU last copied count of timer1.
 * [Args]		: Void.
 * [Return]		: ICU last copied count of timer1.
 */
uint16 ICU_getInputCaptureValue(void);

/*
 * [Function Name]	: ICU_clearTimerValue
 * [Description]	:
 * 		Function that resets timer1 count value to ZERO.
 * [Args]		: Void.
 * [Return]		: Void
 */
void ICU_clearTimerValue(void);

/*
 * [Function Name]	: ICU_deInit
 * [Description]	:
 * 		Function that de-initialize timer1, clearing all it's settings & values.
 * [Args]		: Void.
 * [Return]		: Void
 */
void ICU_deInit(void);

#endif /* ICU_H_ */

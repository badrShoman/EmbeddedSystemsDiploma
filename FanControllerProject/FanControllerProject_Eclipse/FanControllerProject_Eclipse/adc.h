/******************************************************************************
 * Module: ADC
 * File Name: adc.h
 * Description: Header file for ADC driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"		/* For usage of standard defined types */

/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/
#define ADC_INTERRUPT_ENABLE		FALSE
#define ADC_MAXIMUM_VALUE    		1023
#define ADC_LCD_VALUE_ADJUSTMENT	1000
#define ADC_REF_VOLT_VALUE  		2.56

/*******************************************************************************
 *                               Global Variables                              *
 *******************************************************************************/
/* A global variable that stores the value of ADC conversion */
extern volatile uint16 g_ADCValue;

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/*
 * [Enumerate Name]	: ADC_REF_VOLT_MODE
 * [Description]	:
 * 		An enumerate that defines values of reference voltage for ADC.
 */
typedef enum
{
	ADC_AREF_ENABLE, ADC_AVCC_ENABLE, ADC_INT_REF_ENABLE = 3
} ADC_REF_VOLT_MODE;

/*
 * [Enumerate Name]	: ADC_RESULT_ALLIGN
 * [Description]	:
 * 		An enumerate that defines values of result allignment for ADC.
 */
typedef enum
{
	ADC_RESULT_RIGHT, ADC_RESULT_LEFT
} ADC_RESULT_ALLIGN;

/*
 * [Enumerate Name]	: ADC_PRESCALER
 * [Description]	:
 * 		An enumerate that defines clock pre-scaler values for ADC.
 */
typedef enum
{
	ADC_PRESCALER_2,
	ADC_PRESCALER_4,
	ADC_PRESCALER_8,
	ADC_PRESCALER_16,
	ADC_PRESCALER_32,
	ADC_PRESCALER_64,
	ADC_PRESCALER_128
} ADC_PRESCALER;

/*
 * [Enumerate Name]	: ADC_REF_VOLT_MODE
 * [Description]	:
 * 		An enumerate that defines trigger mode selection value for ADC.
 */
typedef enum
{
	ADC_TRIG_FREE,
	ADC_ANALOG_COMP,
	ADC_EXT_INT_0,
	ADC_TRIG_TIMER0_CTC,
	ADC_TRIG_TIMER0_OVF,
	ADC_TRIG_TIMER1_CTC_B,
	ADC_TRIG_TIMER1_OVF,
	ADC_TRIG_TIMER1_CAPTURE
} ADC_AUTOTRIG_MODE;

/*******************************************************************************
 *                                 Structures                                  *
 *******************************************************************************/
/*
 * [Structure Name]	: ADC_REF_VOLT_MODE
 * [Description]	:
 * 		A structure in which it's instance hold reference
 * 		voltage and pre-scaler value in 1 byte to be used
 * 		in ADC_init, ADC_setRefVolt or ADC_setPrescaler functions.
 */
typedef struct
{
	ADC_REF_VOLT_MODE refVoltage :2;
	ADC_PRESCALER prescaler :3;
	uint8 :0;
} ADC_ConfigType;

/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/
/*
 * [Function Name]	: ADC_init
 * [Description]	:
 * 		Function that initialize ADC.
 * 		By default:
 * 			1. ADC is enabled
 * 			2. Result adjustment is right.
 * 			3. Channel 0 is selected.
 * 			4. Auto-trigger is disabled.
 * 			5. ADC interrupt is disabled.
 * [Args]	:
 * [In] ADCControl	: Includes reference voltage & pre-scaler value.
 * [Return]			: Void.
 */
void ADC_init(const ADC_ConfigType *ADCControl_Ptr);

/*
 * [Function Name]	: ADC_setRefVolt
 * [Description]	:
 * 		Function sets ADC reference voltage.
 * [Args]	:
 * [In] ADCControl	: Includes reference voltage & pre-scaler value, but only reference
 * 					  voltage will be used.
 * [Return]			: Void.
 */
void ADC_setRefVolt(const ADC_ConfigType *ADCControl_Ptr);

/*
 * [Function Name]	: ADC_setResultAllign
 * [Description]	:
 * 		Function that sets the allignment of ADC conversion result.
 * [Args]	:
 * [In] resultBit	: Indicates allignment bit state.
 * [Return]			: Void.
 */
void ADC_setResultAllign(uint8 resultBit);

/*
 * [Function Name]	: ADC_setAutoTrig
 * [Description]	:
 * 		Function that enables or disabled ADC auto-trigger.
 * [Args]	:
 * [In] trigEnable	: Indicates auto-trigger state.
 * [In] trigSource	: Indicates auto-trigger source.
 * [Return]		: Void.
 */
void ADC_setAutoTrig(uint8 trigEnable, uint8 trigSource);

/*
 * [Function Name]	: ADC_setPrescaler
 * [Description]	:
 * 		Function that set pre-scaler value of ADC with CPU clock.
 * [Args]	:
 * [In] ADCControl	: Includes reference voltage & pre-scaler value, but only pre-scaler
 * 					  will be used.
 * [Return]			: Void.
 */
void ADC_setPrescaler(const ADC_ConfigType *ADCControl_Ptr);

/*
 * [Function Name]	: ADC_readChannel
 * [Description]	:
 * 		Function that selects a channel & reads analog data from a channel
 * 		and convert it to digital data using the ADC driver (polling technique).
 * [Args]	:
 * [In] channelNum	: Indicates channel number.
 * [Return]			: Conversion result (No return in case of interrupt usage).
 */
uint16 ADC_readChannel(uint8 channelNum);

/*
 * [Function Name]	: ADC_deInit
 * [Description]	:
 * 		Function that de-initialize the ADC driver.
 * 		By default:
 * 			1. ADC is disabled
 * 			2. Auto-trigger is disabled.
 * 			3. ADC interrupt is disabled.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void ADC_deInit(void);

#endif /* ADC_H_ */

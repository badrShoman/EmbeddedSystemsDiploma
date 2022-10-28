/******************************************************************************
 * Module: ADC
 * File Name: adc.c
 * Description: Source file for ADC driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/io.h> 			/* To use the ADC Registers */
#include "adc.h"				/* For ADC prototypes & definitions */

#if (ADC_INTERRUPT_ENABLE == TRUE)

#include <avr/interrupt.h> 		/* For ADC ISR */

#endif

#include "common_macros.h" 		/* For common macros usage */

/*******************************************************************************
 *                         Global Variables & Pointers                         *
 *******************************************************************************/
/* A global variable that stores the value of ADC conversion */
volatile uint16 g_ADCValue = 0;

/*******************************************************************************
 *                          Interrupt Service Routines                         *
 *******************************************************************************/

#if (ADC_INTERRUPT_ENABLE == TRUE)

/*
 * [Interrupt Vector]	: ADC_vect
 * [Description]		:
 * 		An interrupt that acts upon completing ADC conversion.
 */
ISR(ADC_vect)
{
	g_ADCValue = ADC; /* Store ADC conversion value in the global variable */
}

#endif

/*******************************************************************************
 *                      Functions Definitions                                  *
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
void ADC_init(const ADC_ConfigType *ADCControl_Ptr)
{
	/* ADMUX register bits description:
	 * 		REFS1:0 = (00) To choose AREF, internal Vref off, set external Vref.
	 * 				  (01) To choose AVCC, internal Vref On = (5) volts.
	 * 				  (10) Reserved.
	 * 				  (11) To choose AVCC, internal Vref On = (2.56) volts.
	 *
	 * 		ADLAR   = (0) ADC conversion result right adjusted.
	 * 				  (1) ADC conversion result left adjusted.
	 *
	 * 		MUX4:0  = (00000) to (00111) to choose channels from (0) to (7).
	 * 				  (01000) to (11101) to choose channels from (0) to (7) and gain factors.
	 */

	/* Set reference selection bits */
	if ((*ADCControl_Ptr).refVoltage == 2)
	{
		/* DO NOTHING */
	}
	else
	{
		/* Clear previous reference voltage value &  Write new value in 2 MSB of ADMUX */
		OVERWRITE_REG(ADMUX, 0x3F,
				(*ADCControl_Ptr).refVoltage << (MICROCONTROLLER_BITS - 2));
	}
	/* Clear left adjust result bit */
	CLEAR_BIT(ADMUX, ADLAR);
	/* Set analog channel to 0 */
	OVERWRITE_REG(ADMUX, 0xE0, 0x00);

	/* ADCSRA register bits description:
	 * 		ADEN  	  = (0) To disable ADC.
	 * 			        (1) To enable ADC.
	 *
	 * 		ADSC 	  = (0) Stop conversion.
	 * 			   	    (1) Start conversion.
	 *
	 * 		ADATE     = (0) ADC auto-trigger disable.
	 * 				    (1) ADC auto-trigger enable.
	 *
	 * 		ADIF  	  : ADC flag on conversion complete, cleared by hardware on interrupt.
	 *
	 * 		ADIE  	  = (0) ADC Interrupt disable.
	 * 				    (1) ADC Interrupt enable, global interrupt must also be enabled.
	 *
	 * 		ADPS2:0  = (000) to (111) to choose pre-scaler bits.
	 *
	 * 		ADCL:ADCH = Result is stored in these register when conversion is completed,
	 * 					can obtain complete result by reading the value (ADC).
	 */

	/* Clear ADC state bit */
	SET_BIT(ADCSRA, ADEN); /* Enable ADC */
	/* Coversion state bit */
	CLEAR_BIT(ADCSRA, ADSC); /* Do not start conversion on initialization */
	/* Clear auto-trigger bit */
	CLEAR_BIT(ADCSRA, ADATE);

#if (ADC_INTERRUPT_ENABLE == TRUE)

	/* Set ADC interrupt bit */
	SET_BIT(ADCSRA, ADIE);

#else

	/* Clear ADC interrupt bit */
	CLEAR_BIT(ADCSRA, ADIE);

#endif

	/* Set pre-scaler bits */
	OVERWRITE_REG(ADCSRA, 0xF8, 0x00); /* Set ADPS2:0 values */

	/* SFIOR register bits description:
	 * 		REFS2:0 = (000) to (111) to choose auto-trigger source.
	 */

	/* Set auto-trigger source bit */
	OVERWRITE_REG(SFIOR, 0x1F, 0x00);
}

/*
 * [Function Name]	: ADC_setRefVolt
 * [Description]	:
 * 		Function sets ADC reference voltage.
 * [Args]	:
 * [In] ADCControl	: Includes reference voltage & pre-scaler value, but only reference
 * 					  voltage will be used.
 * [Return]			: Void.
 */
void ADC_setRefVolt(const ADC_ConfigType *ADCControl_Ptr)
{
	if ((*ADCControl_Ptr).refVoltage == 2)
	{
		/* DO NOTHING */
	}
	else
	{
		/* Clear previous reference voltage value &  Write new value in 2 MSB of ADMUX */
		OVERWRITE_REG(ADMUX, 0x3F,
				(*ADCControl_Ptr).refVoltage << (MICROCONTROLLER_BITS - 2));
	}
}

/*
 * [Function Name]	: ADC_setResultAllign
 * [Description]	:
 * 		Function that sets the allignment of ADC conversion result.
 * [Args]	:
 * [In] resultBit	: Indicates allignment bit state.
 * [Return]			: Void.
 */
void ADC_setResultAllign(uint8 resultBit)
{
	switch (resultBit)
	{
		case ADC_RESULT_RIGHT:
			CLEAR_BIT(ADMUX, ADLAR);
		break;
		case ADC_RESULT_LEFT:
			SET_BIT(ADMUX, ADLAR);
		break;
	}
}

/*
 * [Function Name]	: ADC_setAutoTrig
 * [Description]	:
 * 		Function that enables or disabled ADC auto-trigger.
 * [Args]	:
 * [In] trigEnable	: Indicates auto-trigger state.
 * [In] trigSource	: Indicates auto-trigger source.
 * [Return]		: Void.
 */
void ADC_setAutoTrig(uint8 trigEnable, uint8 trigSource)
{
	/* Set sate & reference selection bits */
	switch (trigEnable)
	{
		case LOGIC_HIGH:
			SET_BIT(ADCSRA, ADATE); /* Enable auto-trigger bit*/
			/* Remove auto-trigger selection bits previous value & write new value in 3 MSB of SFIOR */
			OVERWRITE_REG(SFIOR, 0x1F,
					trigSource << (MICROCONTROLLER_BITS - 3));
		break;
		case LOGIC_LOW:
			CLEAR_BIT(ADCSRA, ADATE); /* disable auto-trigger bit*/
			OVERWRITE_REG(SFIOR, 0x1F, 0x00); /* remove auto-trigger selection bits previous value */
		break;
	}
}

/*
 * [Function Name]	: ADC_setPrescaler
 * [Description]	:
 * 		Function that set pre-scaler value of ADC with CPU clock.
 * [Args]	:
 * [In] ADCControl	: Includes reference voltage & pre-scaler value, but only pre-scaler
 * 					  will be used.
 * [Return]			: Void.
 */
void ADC_setPrescaler(const ADC_ConfigType *ADCControl_Ptr)
{
	/* Clear previous pre-scaler & set new pre-scaler */
	OVERWRITE_REG(ADCSRA, 0xF8, (*ADCControl_Ptr).prescaler);
}

/*
 * [Function Name]	: ADC_readChannel
 * [Description]	:
 * 		Function that selects a channel & reads analog data from a channel
 * 		and convert it to digital data using the ADC driver.
 * [Args]	:
 * [In] channelNum	: Indicates channel number.
 * [Return]			: Conversion result (No return in case of interrupt usage).
 */
uint16 ADC_readChannel(uint8 channelNum)
{

#if (ADC_INTERRUPT_ENABLE == TRUE)

	channelNum &= 0x1F; /* Set channel = (11111) if it is larger */
	/* Clear previous channel value & set new analog channel & gain selection bits */
	OVERWRITE_REG(ADMUX,0xE0,channelNum);
	SET_BIT(ADCSRA, ADSC); /* Start conversion */
	return -1; /* Indicating no-return */

#else

	channelNum &= 0x1F; /* Set channel = (11111) if it is larger */
	/* Clear previous channel value & set new analog channel & gain selection bits */
	OVERWRITE_REG(ADMUX, 0xE0, channelNum);
	SET_BIT(ADCSRA, ADSC); /* Start conversion */
	while (BIT_IS_CLEAR(ADCSRA, ADIF)); /* Wait for conversion */
	SET_BIT(ADCSRA, ADIF); /* Clear ADIF after conversion by writing LOGIC_HIGH to it */
	return ADC;

#endif

}

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
void ADC_deInit(void)
{
	CLEAR_REG(ADMUX); /* Clear ADMUX register */
	CLEAR_REG(ADCSRA); /* Clear ADCSRA register */
	SET_BIT(ADCSRA, ADIF); /* Clear ADC flag */
	OVERWRITE_REG(SFIOR, 0x1F, 0x00); /* Clear auto-trigger value */
}

/******************************************************************************
 * Module: I2C (TWI)
 * File Name: i2c.c
 * Description: Source file for the I2C (TWI) AVR driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#include <avr/io.h>				/* For I2C registers usage */
#include "common_macros.h"		/* For common macros usage */
#include "i2c.h"				/* For I2C prototypes & definitions */

#if (I2C_INTERRUPT_ENABLE == TRUE)

#include <avr/interrupt.h>		/* For ISR of USART */

#endif

/*******************************************************************************
 *                            I2C Useful Equations                             *
 *******************************************************************************/
/*
 * 		SCL Frequency = (CPU Frequency) / ( 16 + (2) * (TWBR) * (4) ^ (TWPS) )
 */

/*******************************************************************************
 *                           I2C Hardware Registers                            *
 *******************************************************************************/
/*
 * TWBR register description: A register that selects division factor for I2C
 * 							  SCL frequency equation.
 *
 * TWCR register bits description:
 *
 * 		TWINT 	 = 		 (0) TWI interrupt flag unraised.
 * 			  	   		 (1) TWI interrupt flag unraised.
 *
 * 		TWEA	 = 		 (0) TWI do not send acknowledge bit (NACK).
 * 				   		 (1) TWI send acknowledge bit (ACK).
 *
 * 		TWSTA	 = 		 (0) TWI do not send start bit.
 * 				   		 (1) TWI send start bit.
 *
 * 		TWSTO	 = 		 (0) TWI do not send stop bit.
 * 				   		 (1) TWI send stop bit.
 *
 * 		TWWC	 = 		 (0) TWI write collision flag unraised.
 * 				   		 (1) TWI write collision flag raised.
 *
 * 		TWWC	 = 		 (0) TWI disable.
 * 				   		 (1) TWI enable.
 *
 * 		TWIE	 = 		 (0) TWI interrupt disable.
 * 				   		 (1) TWI interrupt enable.
 *
 * TWBR register description: A register that holds I2C data to be exchanged.
 *
 * TWCR register bits description:
 *
 * 		TWS7:0 	 	= 	 Holds the status of I2C which are described in datasheet.
 *
 * 		TWPS1:0 	= 	 (00) Pre-scaler value is 1.
 * 						 (01) Pre-scaler value is 4.
 * 						 (10) Pre-scaler value is 16.
 * 						 (11) Pre-scaler value is 64.
 *
 * TWAR register bits description:
 *
 * 		TWA6:0 	 	= 	 Holds the address of current device in case of usage as slave.
 *
 * 		TWPGCE	 	= 	 (0) TWI general call recognition disable.
 * 						 (1) TWI general call recognition enable.
 */

/*******************************************************************************
 *                              Global Pointers                                *
 *******************************************************************************/

#if (I2C_INTERRUPT_ENABLE == TRUE)

/* Pointers that hold the addresses of the call-back functions */
static volatile void (*g_I2CCallBack_Ptr)(void) = NULL_PTR;

#endif

/*******************************************************************************
 *                         Interrupt Service Routines                          *
 *******************************************************************************/

#if (I2C_INTERRUPT_ENABLE == TRUE)

/*
 * [Interrupt Vector]	: TWI_vect
 * [Description]		:
 * 		An interrupt that acts upon an action happens in I2C.
 */
ISR(TWI_vect)
{
	if (g_I2CCallBack_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_I2CCallBack_Ptr)(); /* Execute callback function */
	}
}

#endif

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/
/*
 * [Function Name]	: I2C_init
 * [Description]	:
 * 		Function that initializes I2C, sets division factor and device address
 * 		and selects pre-scaler, interrupt stat and general call recognition bit.
 * [Args] I2CConfig		: Contains division factor, device address, pre-scaler
 * 						  interrupt state and general call recognition.
 * [Return]				: Void.
 */
void I2C_init(const I2C_initConfig *I2CConfig)
{
	/* Set I2C division factor for SCL frequency equation */
	TWBR = (*I2CConfig).divisionFactor;
	/* Enable I2C */
	SET_BIT(TWCR, TWEN);
	/* Select interrupt state */
	switch ((*I2CConfig).interruptEnable)
	{
		case LOGIC_HIGH:
			SET_BIT(TWCR, TWIE);
		break;
		case LOGIC_LOW:
			CLEAR_BIT(TWCR, TWIE);
		break;
	}
	/* Set I2C pre-scaler for SCL frequency equation */
	OVERWRITE_REG(TWDR, 0xFC, (*I2CConfig).prescaler);
	/* Set I2C address for current device for slave case */
	OVERWRITE_REG(TWAR, 0x01, (*I2CConfig).address);
	/* Set general call recognition */
	switch ((*I2CConfig).GCREnable)
	{
		case LOGIC_HIGH:
			SET_BIT(TWAR, TWGCE);
		break;
		case LOGIC_LOW:
			CLEAR_BIT(TWAR, TWGCE);
		break;
	}
}

#if (I2C_INTERRUPT_ENABLE == TRUE)

/*
 * [Function Name]	: I2C_setCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for I2C interrupt
 * 		for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void I2C_setCallBack(void (*Ptr2Function)(void))
{
	g_I2CCallBack_Ptr = Ptr2Function;
}

#endif

/*
 * [Function Name]	: I2C_start
 * [Description]	:
 * 		Function that sends start bit through I2C.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void I2C_start(void)
{
	/* Clear interrupt flag */
	SET_BIT(TWCR, TWINT);
	/* Send start bit */
	SET_BIT(TWCR, TWSTA);
	/* Wait for interrupt flag to be set, which means start bit was sent successfully */
	while (BIT_IS_CLEAR(TWCR, TWINT));
}

/*
 * [Function Name]	: I2C_stop
 * [Description]	:
 * 		Function that sends stop bit through I2C.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void I2C_stop(void)
{
	/* Clear interrupt flag */
	SET_BIT(TWCR, TWINT);
	/* Send stop bit */
	SET_BIT(TWCR, TWSTO);
}

/*
 * [Function Name]	: I2C_writeByte
 * [Description]	:
 * 		Function that sends data through I2C.
 * [Args]	:
 * [In] data		: Indicates data value.
 * [Return]			: Void.
 */
void I2C_writeByte(uint8 data)
{
	/* Clear interrupt flag */
	SET_BIT(TWCR, TWINT);
	/* Set data to be sent */
	TWDR = data;
	/* Wait for interrupt flag to be set, which means data was sent successfully */
	while (BIT_IS_CLEAR(TWCR, TWINT));
}

/*
 * [Function Name]	: I2C_readByteWithACK
 * [Description]	:
 * 		Function that returns received data through I2C and sends ACK.
 * [Args]		: Void
 * [Return]		: Received data through I2C.
 */
uint8 I2C_readByteWithACK(void)
{
	/* Clear interrupt flag */
	SET_BIT(TWCR, TWINT);
	/* Send acknowledge bit*/
	SET_BIT(TWCR, TWEA);
	/* Wait for interrupt flag to be set, which means data was received successfully */
	while (BIT_IS_CLEAR(TWCR, TWINT));
	/* Read data */
	return TWDR;
}

/*
 * [Function Name]	: I2C_readByteWithNACK
 * [Description]	:
 * 		Function that returns received data through I2C and does not send ACK.
 * [Args]		: Void
 * [Return]		: Received data through I2C.
 */
uint8 I2C_readByteWithNACK(void)
{
	/* Clear interrupt flag */
	SET_BIT(TWCR, TWINT);
	/* Do not send acknowledge bit*/
	/* Wait for interrupt flag to be set, which means data was received successfully */
	while (BIT_IS_CLEAR(TWCR, TWINT));
	/* Read data */
	return TWDR;
}

/*
 * [Function Name]	: I2C_getStatus
 * [Description]	:
 * 		Function that return I2C status.
 * [Args]		: Void
 * [Return]		: I2C status.
 */
uint8 I2C_getStatus(void)
{
	/* Define a variable that will carry I2C status */
	uint8 status;
	/* Eliminate 3 LSBs and get 5 MSBs */
	status = TWSR & 0xF8;
	/* Read status */
	return status;
}

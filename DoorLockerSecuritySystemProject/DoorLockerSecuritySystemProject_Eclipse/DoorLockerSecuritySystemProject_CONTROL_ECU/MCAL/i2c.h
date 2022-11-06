/******************************************************************************
 * Module: I2C (TWI)
 * File Name: i2c.h
 * Description: Header file for the I2C (TWI) AVR driver.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef I2C_H_
#define I2C_H_

#include "../std_types.h"		/* To use standard defined types */

/*******************************************************************************
 *                                 Definitions                                 *
 *******************************************************************************/
/* I2C status bits */
#define I2C_START         		0x08 /* Start sent */
#define I2C_REP_START     		0x10 /* Repeated start */
#define I2C_MT_SLA_W_ACK  		0x18 /* Master transmitted to slave for writing & ACK received. */
#define I2C_MT_SLA_R_ACK  		0x40 /* Master transmitted to slave for reading & ACK received. */
#define I2C_MT_DATA_ACK  		0x28 /* Master transmitted data & ACK received. */
#define I2C_MR_DATA_ACK   		0x50 /* Master received data & ACK sent. */
#define I2C_MR_DATA_NACK  		0x58 /* Master received data & ACK was not sent. */
/* I2C interrupt enable */
#define I2C_INTERRUPT_ENABLE	FALSE

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/*
 * [Enumerate Name]	: I2C_PRESCALER
 * [Description]	:
 * 		An enumerate that defines chosen I2C pre-scaler.
 */
typedef enum
{
	I2C_PRESCALER_1, I2C_PRESCALER_4, I2C_PRESCALER_16, I2C_PRESCALER_64
} I2C_PRESCALER;

/*******************************************************************************
 *                                 Structures                                  *
 *******************************************************************************/
/*
 * [Structure Name]	: I2C_initConfig
 * [Description]	:
 * 		A structure in which it's instance hold I2C initializations,
 * 		division factor, device address, pre-scaler, interrupt state
 * 		& general call recognition in 2 bytes to be used in initialization function.
 */
typedef struct
{
	uint8 divisionFactor :8;
	uint8 :0;
	uint8 address :7;
	uint8 :0;
	I2C_PRESCALER prescaler :2;
	uint8 interruptEnable :1;
	uint8 GCREnable :1;
	uint8 :0;
} I2C_initConfig;

/*******************************************************************************
 *                           Functions Prototypes                              *
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
void I2C_init(const I2C_initConfig *I2CConfig);

#if (I2C_INTERRUPT_ENABLE == TRUE)

/*
 * [Function Name]	: I2C_setCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for I2C interrupt
 * 		for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void I2C_setCallBack(void (*Ptr2Function)(void));

#endif

/*
 * [Function Name]	: I2C_start
 * [Description]	:
 * 		Function that sends start bit through I2C.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void I2C_start(void);

/*
 * [Function Name]	: I2C_stop
 * [Description]	:
 * 		Function that sends stop bit through I2C.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void I2C_stop(void);

/*
 * [Function Name]	: I2C_writeByte
 * [Description]	:
 * 		Function that sends data through I2C.
 * [Args]	:
 * [In] data		: Indicates data value.
 * [Return]			: Void.
 */
void I2C_writeByte(uint8 data);

/*
 * [Function Name]	: I2C_readByteWithACK
 * [Description]	:
 * 		Function that returns received data through I2C and sends ACK.
 * [Args]		: Void
 * [Return]		: Received data through I2C.
 */
uint8 I2C_readByteWithACK(void);

/*
 * [Function Name]	: I2C_readByteWithNACK
 * [Description]	:
 * 		Function that returns received data through I2C and does not send ACK.
 * [Args]		: Void
 * [Return]		: Received data through I2C.
 */
uint8 I2C_readByteWithNACK(void);

/*
 * [Function Name]	: I2C_getStatus
 * [Description]	:
 * 		Function that return I2C status.
 * [Args]		: Void
 * [Return]		: I2C status.
 */
uint8 I2C_getStatus(void);

#endif /* I2C_H_ */

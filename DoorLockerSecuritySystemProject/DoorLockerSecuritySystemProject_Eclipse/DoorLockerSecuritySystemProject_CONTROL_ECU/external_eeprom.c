/******************************************************************************
 * Module: External EEPROM
 * File Name: external_eeprom.c
 * Description: Source file for external EEPROM Memory.
 * Author: Mohamed Badr
 *******************************************************************************/

#include "external_eeprom.h"	/* For external EEPROM prototypes & definitions */
#include "i2c.h"				/* For I2C prototypes & definitions */

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/
/*
 * [Function Name]	: EEPROM_writeByte
 * [Description]	:
 * 		Function that writes a byte to external EEPROM.
 * [Args]	:
 * [In] u16address			: Indicates EEPROM memory location address.
 * [In] u8data				: Indicates data that is sent.
 * [Return]					: Success or failure state.
 */
uint8 EEPROM_writeByte(uint16 u16address, uint8 u8data)
{
	/* Send start bit */
	I2C_start();
	/* Generate an error if operation failed */
	if (I2C_getStatus() != I2C_START)
		return ERROR;
	/* Send the 3 MSBs of u16address in device address in addition
	 * to write bit & the rest of the data in word address */
	I2C_writeByte((uint8) (0xA0 | ((u16address & 0x0700) >> 7)));
	/* Generate an error if operation failed */
	if (I2C_getStatus() != I2C_MT_SLA_W_ACK)
		return ERROR;
	/* Send memory location address */
	I2C_writeByte((uint8) (u16address & 0xFF));
	/* Generate an error if operation failed */
	if (I2C_getStatus() != I2C_MT_DATA_ACK)
		return ERROR;
	/* Write the byte to EEPROM */
	I2C_writeByte(u8data);
	/* Generate an error if operation failed */
	if (I2C_getStatus() != I2C_MT_DATA_ACK)
		return ERROR;
	/* Send stop bit */
	I2C_stop();
	/* Generate a success if operation succeeded */
	return SUCCESS;
}

/*
 * [Function Name]	: EEPROM_writeByte
 * [Description]	:
 * 		Function that reads a byte from external EEPROM.
 * [Args]	:
 * [In] u16address			: Indicates EEPROM memory location address.
 * [Out] u8data				: Indicates data that is received.
 * [Return]					: Success or failure state.
 */
uint8 EEPROM_readByte(uint16 u16address, uint8 *u8data)
{
	/* Send start bit */
	I2C_start();
	/* Generate an error if operation failed */
	if (I2C_getStatus() != I2C_START)
		return ERROR;
	/* Send the 3 MSBs of u16address in device address in addition
	 * to write bit & the rest of the data in word address */
	I2C_writeByte((uint8) ((0xA0) | ((u16address & 0x0700) >> 7)));
	/* Generate an error if operation failed */
	if (I2C_getStatus() != I2C_MT_SLA_W_ACK)
		return ERROR;
	/* Send memory location address */
	I2C_writeByte((uint8) (u16address));
	/* Generate an error if operation failed */
	if (I2C_getStatus() != I2C_MT_DATA_ACK)
		return ERROR;
	/* Send repeated start bit */
	I2C_start();
	/* Generate an error if operation failed */
	if (I2C_getStatus() != I2C_REP_START)
		return ERROR;
	/* Send the 3 MSBs of u16address in device address in addition
	 * to read bit & the rest of the data in word address */
	I2C_writeByte((uint8) ((0xA0) | ((u16address & 0x0700) >> 7) | 1));
	/* Generate an error if operation failed */
	if (I2C_getStatus() != I2C_MT_SLA_R_ACK)
		return ERROR;
	/* Read the byte from memory without sending ACK */
	*u8data = I2C_readByteWithNACK();
	/* Generate an error if operation failed */
	if (I2C_getStatus() != I2C_MR_DATA_NACK)
		return ERROR;
	/* Send stop bit */
	I2C_stop();
	/* Generate a success if operation succeeded */
	return SUCCESS;
}

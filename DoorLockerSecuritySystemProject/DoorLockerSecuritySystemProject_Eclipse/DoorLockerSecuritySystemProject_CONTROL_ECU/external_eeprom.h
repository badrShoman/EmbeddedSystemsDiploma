/******************************************************************************
 * Module: External EEPROM
 * File Name: external_eeprom.h
 * Description: Header file for external EEPROM Memory.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"		/* To use standard defined types */

/*******************************************************************************
 *                                 Definitions                                 *
 *******************************************************************************/
/* Set macros for success & failure of operations */
#define ERROR			0
#define SUCCESS			1

/*******************************************************************************
 *                            Functions Prototypes                             *
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
uint8 EEPROM_writeByte(uint16 u16address, uint8 u8data);

/*
 * [Function Name]	: EEPROM_writeByte
 * [Description]	:
 * 		Function that reads a byte from external EEPROM.
 * [Args]	:
 * [In] u16address			: Indicates EEPROM memory location address.
 * [Out] u8data				: Indicates data that is received.
 * [Return]					: Success or failure state.
 */
uint8 EEPROM_readByte(uint16 u16address, uint8 *u8data);

#endif /* EXTERNAL_EEPROM_H_ */

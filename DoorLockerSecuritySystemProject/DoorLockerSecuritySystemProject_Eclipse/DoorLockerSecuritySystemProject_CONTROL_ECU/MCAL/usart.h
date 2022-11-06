/******************************************************************************
 * Module: USART
 * File Name: usart.c
 * Description: Header file for the USART AVR driver
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef USART_H_
#define USART_H_

#include "../std_types.h"		/* To use standard defined types */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* Choose USART features to enable */
#define USART_MULTI_PROCESSOR_MODE_ENABLE		FALSE
#define USART_INTERRUPT_ENABLE					FALSE
#define USART_SYNCHRONOUS_MODE_ENABLE			FALSE
#define USART_9BIT_MODE_ENABLE					FALSE

#if (USART_SYNCHRONOUS_MODE_ENABLE == TRUE)

#define USART_CLOCK_POLARITY					LOGIC_LOW

#else

#define USART_DOUBLE_SPEED_MODE					TRUE

#endif

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/*
 * [Enumerate Name]	: USART_PARITY_MODE
 * [Description]	:
 * 		An enumerate that defines USART parity mode.
 */
typedef enum
{
	PARITY_DISABLE, PARITY_EVEN = 2, PARITY_ODD
} USART_PARITY_MODE;

/*
 * [Enumerate Name]	: USART_CHARACTER_SIZE
 * [Description]	:
 * 		An enumerate that defines USART data size.
 */
typedef enum
{
	USART_DATA_5BIT,
	USART_DATA_6BIT,
	USART_DATA_7BIT,
	USART_DATA_8BIT,
	USART_DATA_9BIT = 7
} USART_CHARACTER_SIZE;

/*******************************************************************************
 *                                 Structures                                  *
 *******************************************************************************/
/*
 * [Structure Name]	: USART_initConfig
 * [Description]	:
 * 		A structure in which it's instance hold USART character
 * 		size, parity mode & stop bit selection in 1 byte and
 * 		baud rate in 4 bytes, to be used in USART_init function.
 */
typedef struct
{
	uint32 baudRate :32;
	uint8 :0;
	USART_CHARACTER_SIZE characterSize :3;
	USART_PARITY_MODE parityMode :2;
	uint8 stopBit :1;
	uint8 :0;
} USART_initConfig;

/*******************************************************************************
 *                           Functions Prototypes                              *
 *******************************************************************************/
/*
 * [Function Name]	: USART_init
 * [Description]	:
 * 		Function that initializes USART:
 * 			1. Enable USART.
 * 			2. Sets frame format.
 * 			3. Sets UART baud rate.
 * [Args] USARTConfig	: Contains baud rate, character size, parity mode & stop bit.
 * [Return]				: Void.
 */
void USART_init(const USART_initConfig *USARTConfig);

#if (USART_INTERRUPT_ENABLE == TRUE)

/*
 * [Function Name]	: USART_RXCsetCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for USART
 * 		receive complete interrupt for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void USART_RXCsetCallBack(void (*Ptr2Function)(void));

/*
 * [Function Name]	: USART_TXCsetCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for USART
 * 		transmit complete interrupt for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void USART_TXCsetCallBack(void (*Ptr2Function)(void));

/*
 * [Function Name]	: USART_UDREsetCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for USART
 * 		UDR register is empty interrupt for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void USART_UDREsetCallBack(void (*Ptr2Function)(void));

#endif


/*
 * [Function Name]	: USART_sendByte
 * [Description]	:
 * 		Function that sends data through USART.
 * [Args] data	: Indicates data to be sent.
 * [Return]		: Void.
 */

#if (USART_9BIT_MODE_ENABLE == TRUE)

void USART_sendByte(const uint16 data);

#else

void USART_sendByte(const uint8 data);

#endif

/*
 * [Function Name]	: USART_recieveByte
 * [Description]	:
 * 		Function that receives data through USART.
 * [Args] 		: Void.
 * [Return]		: Data existing in UDR.
 */

#if (USART_9BIT_MODE_ENABLE == TRUE)

uint16 USART_recieveByte(void);

#else

uint8 USART_recieveByte(void);

#endif

/*
 * [Function Name]	: USART_sendString
 * [Description]	:
 * 		Function that sends string through USART.
 * [Args] string	: Indicates string to be sent.
 * [Return]			: Void.
 */
void USART_sendString(const uint8 *string);

/*
 * [Function Name]	: USART_recieveByte
 * [Description]	:
 * 		Function that receives string through USART.
 * [Args] 			: Void.
 * [Out] string		: Indicates the whole string that is received until (#) symbol.
 * [Return]			: Void.
 */
void USART_receiveString(uint8 *string);

#endif /* USART_H_ */

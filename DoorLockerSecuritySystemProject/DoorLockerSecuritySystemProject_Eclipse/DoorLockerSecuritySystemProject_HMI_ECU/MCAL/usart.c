/******************************************************************************
 * Module: USART
 * File Name: usart.c
 * Description: Source file for the USART AVR driver
 * Author: Mohamed Badr
 *******************************************************************************/

#include "../MCAL/usart.h"					/* For USART prototypes & definitions */
#include <avr/io.h>							/* For USART registers usage */
#include "../common_macros.h"				/* For common macros usage */

#if (USART_INTERRUPT_ENABLE == TRUE)

#include <avr/interrupt.h>					/* For ISR of USART */

#endif

/*******************************************************************************
 *                           USART Useful Equations                            *
 *******************************************************************************/
/*
 * Asynchronous normal mode:
 *
 * 		BAUD = (fOSC) / ( (16) * (UBRR + 1) )
 * 		UBRR = ( (fOSC) / ( (16) * (BAUD) ) ) - 1
 *
 * Asynchronous double speed mode:
 *
 * 		BAUD = (fOSC) / ( (8) * (UBRR + 1) )
 * 		UBRR = ( (fOSC) / ( (8) * (BAUD) ) ) - 1
 *
 * Synchronous master mode:
 *
 * 		BAUD = (fOSC) / ( (2) * (UBRR + 1) )
 * 		UBRR = ( (fOSC) / ( (2) * (BAUD) ) ) - 1
 */

/*******************************************************************************
 *                           USART Hardware Registers                          *
 *******************************************************************************/
/*
 * UDR register bits description: Transmit & receive data buffer register, they
 * 								  both use the same register.
 *
 * UCSRA register bits description:
 *
 * 		RXC  	= (0) Receive complete flag unraised. (Automatic clear on data read)
 * 			      (1) Receive complete flag raised. (Automatic clear on data read)
 *
 * 		TXC  	= (0) Transmit complete flag unraised. (Automatic clear on interrupt)
 * 			   	  (1) Transmit complete flag raised. (Automatic clear on interrupt)
 *
 * 		UDRE  	= (0) USART Data register is empty flag unraised.
 * 				  (1) USART Data register is empty flag raised.
 *
 * 		FE	 	= (0) Frame error flag unraised.
 * 			      (1) Frame error flag raised.
 *
 * 		DOR	 	= (0) Data overrun flag unraised.
 * 			  	  (1) Data overrun flag raised.
 *
 * 		PE	 	= (0) Parity error flag unraised.
 * 			  	  (1) Parity error flag raised.
 *
 * 		U2X	 	= (0) Normal transmission speed.
 * 			  	  (1) Double transmission speed.
 *
 * 		MPCM 	= (0) Normal transmission speed.
 * 			  	  (1) Double transmission speed.
 *
 * UCSRB register bits description:
 *
 * 		RXCIE  	= (0) RX complete interrupt disable.
 * 			      (1) RX complete interrupt enable.
 *
 * 		TXCIE  	= (0) TX complete interrupt disable.
 * 			   	  (1) TX complete interrupt enable.
 *
 * 		UDRIE  	= (0) USART Data register empty interrupt disable.
 * 				  (1) USART Data register empty interrupt enable.
 *
 * 		RXEN 	= (0) Receiver enable.
 * 			      (1) Receiver disable.
 *
 * 		TXEN 	= (0) Transmitter enable.
 * 			  	  (1) Transmitter disable.
 *
 * 		UCSZ2 	= Combined with UCSZ1 & UCSZ0 in USCRC, sets frame data bits.
 *
 * 		RXB8 	= Ninth bit received along with UDR buffer when 9-bit frame data is chosen.
 *
 * 		TXB8 	= Ninth bit transmitted along with UDR buffer when 9-bit frame data is chosen.
 *
 * UCSRC register bits description:
 *
 * 		URSEL 	 	= (0) Clear when writing to UBRRL & UBRRH.
 * 			  	      (1) Set when writing to UCSRC.
 *
 * 		UMSEL  		= (0) Operate in asynchronous mode.
 * 			   		  (1) Operate in synchronous mode.
 *
 * 		UPM1:0  	= (00) Parity disabled.
 * 					  (01) Reserved.
 * 					  (10) Enabled, even parity.
 * 					  (11) Enabled, odd parity.
 *
 * 		USBS 		= (0) Stop bit is 1-bit.
 * 			   	      (1) Stop bits are 2-bits.
 *
 * 		UCSZ1:0 	= (DO NOT FORGET UCSZ2)
 * 					  (000) 5-bit data.
 * 				   	  (001) 6-bit data.
 * 				   	  (010) 7-bit data.
 * 				   	  (011) 8-bit data.
 * 				   	  (100) Reserved.
 * 				   	  (101) Reserved.
 * 				   	  (110) Reserved.
 * 				   	  (111) 9-bit data.
 *
 * 		UCPOL	 	= (0) TXD with rising XCK edge, RXD with falling XCK edge.
 * 					  (1) TXD with falling XCK edge, RXD with rising XCK edge.
 *
 * UBRRL & UBRRH register bits description:
 *
 * 		URSEL 	 	= (0) Clear when writing to UBRRL & UBRRH.
 * 			  	      (1) Set when writing to UCSRC.
 *
 * 		UBRR11:8  	= Write the most significant 4-bits from UBRR value in
 * 					  the least significant 4-bits in UBRRH, and the rest
 * 					  8-bits in UBRRL, knowing that UBRR value is 12-bit.
 */

/*******************************************************************************
 *                            Global Pointers                                  *
 *******************************************************************************/
/* Pointers that hold the addresses of the call-back functions */

#if (USART_INTERRUPT_ENABLE == TRUE)

static volatile void (*g_USART_RXCCallBack_Ptr)(void) = NULL_PTR;
static volatile void (*g_USART_TXCCallBack_Ptr)(void) = NULL_PTR;
static volatile void (*g_USART_UDRECCallBack_Ptr)(void) = NULL_PTR;

#endif

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

#if (USART_INTERRUPT_ENABLE == TRUE)

/*
 * [Interrupt Vector]	: USART_RXC_vect
 * [Description]		:
 * 		An interrupt that acts upon USART receive complete.
 */
ISR(USART_RXC_vect)
{
	if (g_USART_RXCCallBack_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_USART_RXCCallBack_Ptr)(); /* Execute callback function */
	}
}

/*
 * [Interrupt Vector]	: USART_TXC_vect
 * [Description]		:
 * 		An interrupt that acts upon USART transmit complete.
 */
ISR(USART_TXC_vect)
{
	if (g_USART_TXCCallBack_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_USART_TXCCallBack_Ptr)(); /* Execute callback function */
	}
}

/*
 * [Interrupt Vector]	: USART_UDRE_vect
 * [Description]		:
 * 		An interrupt that acts upon USART UDR register is empty.
 */
ISR(USART_UDRE_vect)
{
	if (g_USART_UDRECCallBack_Ptr != NULL_PTR) /* If callback function pointer is not void */
	{
		(*g_USART_UDRECCallBack_Ptr)(); /* Execute callback function */
	}
}

#endif

/*******************************************************************************
 *                          Functions Definitions                              *
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
void USART_init(const USART_initConfig *USARTConfig)
{
	/* Define a variable that will hold UBRR value*/
	uint16 UBRRValue = 0;
	/* Clear previous USART initialization data */
	CLEAR_REG(UCSRA);
	CLEAR_REG(UCSRB);
	CLEAR_REG(UCSRC);
	OVERWRITE_REG(UBRRH, 0xF0, 0x00);
	CLEAR_REG(UBRRL);

#if (USART_DOUBLE_SPEED_MODE == TRUE)

	/* Enable double transmission speed */
	SET_BIT(UCSRA, U2X);

#endif

#if(USART_MULTI_PROCESSOR_MODE_ENABLE == TRUE)

	/* Enable multi-processor communication mode */
	SET_BIT(UCSRA, MPCM);

#endif

#if (USART_INTERRUPT_ENABLE == TRUE)

	/* Enable RX complete interrupt, TX complete interrupt & USART data register
	 * is empty interrupt */
	OVERWRITE_REG(UCSRB, 0x1F, 0xE0);

#endif

	/* Enable receiver & override receiver pin settings */
	SET_BIT(UCSRB, RXEN);
	/* Enable transmitter & override transmitter pin settings */
	SET_BIT(UCSRB, TXEN);
	/* Set most significant bit of character size */
	OVERWRITE_REG(UCSRB, 0xFB, GET_BIT((*USARTConfig).characterSize, 2) << 2);
	/* Write in UCSRC */
	SET_BIT(UCSRC, URSEL);

#if(USART_SYNCHRONOUS_MODE_ENABLE == TRUE)

	/* Select synchronous mode */
	SET_BIT(UCSRC, UMSEL);

#endif

	/* Set parity mode */
	OVERWRITE_REG(UCSRC, 0xCF, (*USARTConfig).parityMode << 4);
	/* Set stop bit */
	OVERWRITE_REG(UCSRC, 0xF7, GET_BIT((*USARTConfig).stopBit, 0) << 3);
	/* Set two least significant bits of character size */
	OVERWRITE_REG(UCSRC, 0xF9, ((*USARTConfig).characterSize & 0x03) << 1);

#if (USART_SYNCHRONOUS_MODE_ENABLE == TRUE)

	SET_BIT(UCSRC, UCPOL);

#endif

	/* Write in UBRRL & UBRRH */
	CLEAR_BIT(UCSRC, URSEL);

#if (USART_DOUBLE_SPEED_MODE == TRUE)

	/* Calculate the UBRR register value */
	UBRRValue = (uint16) (((F_CPU / ((*USARTConfig).baudRate * 8UL))) - 1);

#elif (USART_SYNCHRONOUS_MODE_ENABLE == TRUE)

	/* Calculate the UBRR register value */
	UBRRValue = (uint16) (((F_CPU / ((*USARTConfig).baudRate * 2UL))) - 1);

#else

	/* Calculate the UBRR register value */
	UBRRValue = (uint16) (((F_CPU / ((*USARTConfig).baudRate * 16UL))) - 1);

#endif

	/* Set 4 most significant bits of UBRRValue in UBRRH */
	UBRRH = UBRRValue >> 8;
	/* Set 8 least significant bits of UBRRValue in UBRRL */
	UBRRL = UBRRValue;
}

#if (USART_INTERRUPT_ENABLE == TRUE)

/*
 * [Function Name]	: USART_RXCsetCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for USART
 * 		receive complete interrupt for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void USART_RXCsetCallBack(void (*Ptr2Function)(void))
{
	g_USART_RXCCallBack_Ptr = Ptr2Function;
}

/*
 * [Function Name]	: USART_TXCsetCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for USART
 * 		transmit complete interrupt for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void USART_TXCsetCallBack(void (*Ptr2Function)(void))
{
	g_USART_TXCCallBack_Ptr = Ptr2Function;
}

/*
 * [Function Name]	: USART_UDREsetCallBack
 * [Description]	:
 * 		Function that sets the call-back function address for USART
 * 		UDR register is empty interrupt for the upper layer layer.
 * [Args]		: Void.
 * [Return]		: Void.
 */
void USART_UDREsetCallBack(void (*Ptr2Function)(void))
{
	g_USART_UDRECCallBack_Ptr = Ptr2Function;
}

#endif

/*
 * [Function Name]	: USART_sendByte
 * [Description]	:
 * 		Function that sends data through USART.
 * [Args] data	: Indicates data to be sent.
 * [Return]		: Void.
 */

#if (USART_9BIT_MODE_ENABLE == TRUE)

void USART_sendByte(const uint16 data)

#else

void USART_sendByte(const uint8 data)

#endif

{

#if (USART_9BIT_MODE_ENABLE == TRUE)

	/* Wait for data register empty flag to be raised indicating UDR is ready */
	while (BIT_IS_CLEAR(UCSRA, UDRE));
	/* Set ninth bit */
	OVERWRITE_REG(UCSRB, 0xFE, GET_BIT(data, 8));
	/* Put the data in the UDR, flag is automatically cleared */
	UDR = (data & 0xFF);

#else

	/* Wait for data register empty flag to be raised indicating UDR is ready */
	while (BIT_IS_CLEAR(UCSRA, UDRE));
	/* Put the data in the UDR, flag is automatically cleared */
	UDR = data;
	/************************* Another Method *************************
	 UDR = data;
	 while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	 SET_BIT(UCSRA,TXC); // Clear the TXC flag
	 *******************************************************************/

#endif

}

/*
 * [Function Name]	: USART_recieveByte
 * [Description]	:
 * 		Function that receives data through USART.
 * [Args] 		: Void.
 * [Return]		: Data existing in UDR.
 */

#if (USART_9BIT_MODE_ENABLE == TRUE)

uint16 USART_recieveByte(void)

#else

uint8 USART_recieveByte(void)

#endif

{

#if (USART_9BIT_MODE_ENABLE == TRUE)

	/* Define a variable to be returned */
	uint16 UDRValue = 0;
	/* Wait for receive complete flag to be raised indicating UDR is ready */
	while (BIT_IS_CLEAR(UCSRA, RXC));
	/* Get ninth bit */
	UDRValue = (GET_BIT(UCSRB, RXB8) << 8);
	/* Read received data from UDR, flag is automatically cleared */
	UDRValue |= UDR;
	/* Return the variable */
	return UDRValue;

#else

	/* Wait for receive complete flag to be raised indicating UDR is ready */
	while (BIT_IS_CLEAR(UCSRA, RXC));
	/* Read received data from UDR, flag is automatically cleared */
	return UDR;

#endif

}

/*
 * [Function Name]	: USART_sendString
 * [Description]	:
 * 		Function that sends string through USART.
 * [Args] string	: Indicates string to be sent.
 * [Return]			: Void.
 */
void USART_sendString(const uint8 *string)
{
	/* Define a counter variable */
	uint8 i = 0;
	/* Send the string */
	while (string[i] != '\0')
	{
		USART_sendByte(string[i]);
		i++;
	}
	/************************* Another Method *************************
	 while(*Str != '\0')
	 {
	 UART_sendByte(*Str);
	 Str++;
	 }
	 *******************************************************************/
}

/*
 * [Function Name]	: USART_recieveByte
 * [Description]	:
 * 		Function that receives string through USART.
 * [Args] 			: Void.
 * [Out] string		: Indicates the whole string that is received until (#) symbol.
 * [Return]			: Void.
 */
void USART_receiveString(uint8 *string)
{
	/* Define a counter variable */
	uint8 i = 0;
	/* Receive the first byte */
	string[i] = USART_recieveByte();
	/* Receive the string until the (#) */
	while (string[i] != '#')
	{
		i++;
		string[i] = USART_recieveByte();
	}
	/* Replace (#) with (\0) After receiving */
	string[i] = '\0';
}

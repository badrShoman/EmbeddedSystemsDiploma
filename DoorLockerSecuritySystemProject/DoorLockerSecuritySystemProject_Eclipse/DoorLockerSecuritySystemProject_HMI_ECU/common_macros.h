/******************************************************************************
 * Module: Common - Macros
 * File Name: common_macros.h
 * Description: Commonly used Macros.
 * Author: Mohamed Badr
 *******************************************************************************/

#ifndef COMMON_MACROS
#define COMMON_MACROS

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Set microcontroller bits */
#define MICROCONTROLLER_BITS	8

/* Set a bit in a register */
#define SET_BIT(REG,BIT) (REG |= (1 << BIT))

/* Clear a bit a register */
#define CLEAR_BIT(REG,BIT) (REG &= (~(1 << BIT)))

/* Toggle a bit in a register */
#define TOGGLE_BIT(REG,BIT) (REG ^= (1 << BIT))

/* Rotate right register value with specific number of rotates */
#define ROR(REG,num) (REG = (REG >> num) | (REG << (MICROCONTROLLER_BITS - num)))

/* Rotate left register value with specific number of rotates */
#define ROL(REG,num) (REG = (REG << num) | (REG >> (MICROCONTROLLER_BITS - num)))

/* Check if a bit is set in a register and return (TRUE) if valid */
#define BIT_IS_SET(REG,BIT) (REG & (1 << BIT))

/* Check if a bit is cleared in a register and return (TRUE) if valid */
#define BIT_IS_CLEAR(REG,BIT) (!(REG & (1 << BIT)))

/* Get the value of a specific bit as (0) or a (1) */
#define GET_BIT(REG,BIT) (( REG & (1 << BIT)) >> BIT)

/* Set a register */
#define WRITE_REG(REG) ((REG) |= (0xFF))

/* Clear a register */
#define CLEAR_REG(REG) ((REG) &= (0x00))

/* Overwrite a value in a register */
#define OVERWRITE_REG(REG,CLEAR,WRITE) ((REG) = ((REG) & (CLEAR)) | (WRITE))

#endif

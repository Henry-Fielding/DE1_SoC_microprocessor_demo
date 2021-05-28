/*
 * DE1SoC_SevenSeg.h
 *
 *  Created on: 12 Feb 2021
 *      Author: Harry Clegg
 */

/***********************************************************************
 * DO NOT MODIFY THIS FILE
 * YOU CAN COMPLETE THE ASSIGNMENT WITHOUT ANY CHANGES TO THIS FILE
 ***********************************************************************/

#ifndef DE1SoC_SEVENSEG_H_
#define DE1SoC_SEVENSEG_H_

/**
 * DE1SoC_SevenSeg_Write
 *
 * Low level set function to send a byte value to a selected display.
 *
 * Inputs:
 * 		display:		index of the display to update (0-5)
 * 		value: 			byte to set display to.
 */
void DE1SoC_SevenSeg_Write(unsigned int display, unsigned char value);

/**
 * DE1SoC_SevenSeg_SetSingle
 *
 * Set a single seven segment display from a hexadecimal (0x0-0xF) value.
 *
 * Inputs:
 * 		display:		index of the display to update (0-5)
 * 		value: 			value to assign to the display (0x0-0xF)
 */
void DE1SoC_SevenSeg_SetSingle(unsigned int display, unsigned int value);

/**
 * DE1SoC_SevenSeg_SetDoubleHex
 *
 * Set a pair of seven segment displays from a hexadecimal (0x00-0xFF) value.
 *
 * Inputs:
 * 		display:		index of the pair of displays to update (0-4)
 * 		value: 			value to assign to the display (0x00-0xFF)
 */
void DE1SoC_SevenSeg_SetDoubleHex(unsigned int display, unsigned int value);

/**
 * DE1SoC_SevenSeg_SetDoubleDec
 *
 * Set a pair of seven segment displays from a decimal (00-99) value.
 *
 * Inputs:
 * 		display:		index of the pair of displays to update (0-4)
 * 		value: 			value to assign to the display (00-99)
 */
void DE1SoC_SevenSeg_SetDoubleDec(unsigned int display, unsigned int value);

#endif /* DE1SoC_SEVENSEG_H_ */

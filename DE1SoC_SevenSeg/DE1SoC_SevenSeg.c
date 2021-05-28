/*
 * DE1SoC_SevenSeg.c
 *
 *  Created on: 12 Feb 2021
 *      Author: Harry Clegg
 *      		Henry Fielding
 */

#include "DE1SoC_SevenSeg.h"

// ToDo: Add the base addresses of the seven segment display peripherals.
volatile unsigned char *sevenseg_base_lo_ptr = (unsigned char *) 0xFF200020;
volatile unsigned char *sevenseg_base_hi_ptr = (unsigned char *) 0xFF200030;

// There are four HEX displays attached to the low (first) address.
#define SEVENSEG_N_DISPLAYS_LO 4

// There are two HEX displays attached to the high (second) address.
#define SEVENSEG_N_DISPLAYS_HI 2

void DE1SoC_SevenSeg_Write(unsigned int display, unsigned char value) {
    // Select between the two addresses so that the higher level functions
    // have a seamless interface.
    if (display < SEVENSEG_N_DISPLAYS_LO) {
        // If we are targeting a low address, use byte addressing to access
        // directly.
        sevenseg_base_lo_ptr[display] = value;
    } else {
        // If we are targeting a high address, shift down so byte addressing
        // works.
        display = display - SEVENSEG_N_DISPLAYS_LO;
        sevenseg_base_hi_ptr[display] = value;
    }
}

void DE1SoC_SevenSeg_SetSingle(unsigned int display, unsigned int value) {
    // ToDo: Write the code for driving a single seven segment display here.
    // Your function should turn a real value 0-F into the correctly encoded
    // bits to enable the correct segments on the seven segment display to
    // illuminate. Use the DE1SoC_SevenSeg_Write function you created earlier
    // to set the bits of the display.

	// conversion array stores the seven-segment display value corresponding to the array index value
	int conversionArray[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7c, 0x39, 0x5E, 0x79, 0x71};

	if (value <= 0xF) {
		// If input value is within the display range (0-15) convert to the
		// corresponding seven-segment display value.
		value = conversionArray[value];

	} else {
		// If input value is outside the display range (0-15), convert to -
		// seven-segment display value
		value = 0x40;
	}

	// pass converted display value to the display writing function
	DE1SoC_SevenSeg_Write(display, value);
}

void DE1SoC_SevenSeg_SetDoubleHex(unsigned int display, unsigned int value) {
    // ToDo: Write the code for setting a pair of seven segment displays
    // here. Good coding practice suggests your solution should call
    // DE1SoC_SevenSeg_SetSingle() twice.
    // This function should show the first digit of a HEXADECIMAL number on
    // the specified 'display', and the second digit on the display to
    // the left of the specified display.

    /** Some examples:
     *
     *    input | output | HEX(N+1) | HEX(N)
     *    ----- | ------ | -------- | ------
     *        5 |     05 |        0 |      5
     *       30 |     1E |        1 |      E
     *     0x60 |     60 |        6 |      0
     */

	// initialised required variables
	int digitFirst;
	int digitSecond;

	// conversion array stores the seven-segment display value corresponding
	// to the array index value
	int conversionArray[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7c, 0x39, 0x5E, 0x79, 0x71};

	if (value <= 0xFF) {
		// isolate first digit (lowest 4 bits) and convert to corresponding
		// seven-segment display value
		digitFirst = value & 0x0F;
		digitFirst = conversionArray[digitFirst];

		// isolate second digit (upper 4 bits) and convert to corresponding
		// seven-segment display value
		digitSecond = (value & 0xF0) >> 4;
		digitSecond = conversionArray[digitSecond];

	} else {
		// both digits display - on LEDs
		digitFirst = 0x40;
		digitSecond = 0x40;
	}

	// pass converted display value to the display writing function
	DE1SoC_SevenSeg_Write(display, digitFirst);
	DE1SoC_SevenSeg_Write(display + 1, digitSecond);
}

void DE1SoC_SevenSeg_SetDoubleDec(unsigned int display, unsigned int value) {
    // ToDo: Write the code for setting a pair of seven segment displays
    // here. Good coding practice suggests your solution should call
    // DE1SoC_SevenSeg_SetSingle() twice.
    // This function should show the first digit of a DECIMAL number on
    // the specified 'display', and the second digit on the display to
    // the left of the specified display.

    /** Some examples:
     *
     *	  input | output | HEX(N+1) | HEX(N)
     *    ----- | ------ | -------- | ------
     *        5 |     05 |        0 |      5
     *       30 |     30 |        3 |      0
     *     0x5A |     90 |        9 |      0
     */
	// initialised required variables
	int digitFirst;
	int digitSecond;

	// conversion array stores the seven-segment display value corresponding
	// to the array index value
	int conversionArray[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7c, 0x39, 0x5E, 0x79, 0x71};

	if (value <= 99) {
		// first digit is the modulo of input % 10 then converted to corresponding
		// seven-segment display value
		digitFirst = value % 10;
		digitFirst = conversionArray[digitFirst];

		// second digit is found by / 10 then converted to corresponding
		// seven-segment display value
		digitSecond = value / 10;
		digitSecond = conversionArray[digitSecond];

	} else {
		// both digits display - on LEDs
		digitFirst = 0x40;
		digitSecond = 0x40;
	}

	// pass converted display value to the display writing function
	DE1SoC_SevenSeg_Write(display, digitFirst);
	DE1SoC_SevenSeg_Write(display + 1, digitSecond);
}

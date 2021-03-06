/*
 *	ScreenBuffer (source file)
 *	------------------------------
 *	Created on: 31 May 2021
 *	Author: Henry Fielding
 *
 *	Description
 *	-------------
 *	A library of functions to handle various interaction with the screen buffer array for the 240*320 LCD module
 */

#include "ScreenBuffer.h"

// function to reset the screen buffer
// copies the RGB565 value of each address of the background bitmap to the correspoding adress in the screenBuffer
void ScreenBuffer_resetBuffer (unsigned short* screenBuffer, const unsigned short* background) {
	unsigned int index;
	for (index = 0; index < 76800; index++){
		screenBuffer[index] = background [index];
	}
}

// TODO: modify to draw object, feed in only object and frame
// TODO: create object for water sprite
// function to draw a sprite/bitmap to the correct position in the screen buffer array
void ScreenBuffer_drawSprite (unsigned short* screenBuffer, const unsigned short* newLayer, signed int xOrigin, signed int yOrigin, unsigned int width, unsigned int height) {
	signed int xInit, yInit, xEnd, yEnd;
	signed int xAddr, yAddr, newLayerIndex, screenBufferIndex;

	// select section of sprite that is on screen
	xInit	= xOrigin;			// set target position range
	yInit	= yOrigin;
	xEnd	= xOrigin + width - 1;
	yEnd	= yOrigin + height - 1;

	if (xInit < 0)	xInit = 0;	// check the range is within the LCD bounds
	if (yInit < 0)	yInit = 0;
	if (xEnd > 239)	xEnd = 239;
	if (yEnd > 319)	yEnd = 319;

	// transfer sprite Array to current Frame Array
	for(yAddr = yOrigin; yAddr <= yEnd ; yAddr++) {
		for (xAddr = xOrigin; xAddr <= xEnd ; xAddr++) {
			newLayerIndex		= (yAddr - yOrigin) * width + (xAddr - xOrigin);	// index of target pixel within sprite array
			screenBufferIndex	= (yAddr * 240) + xAddr;						// index of target pixel within screen buffer array

			// if sprite pixel is not clear (clear = 0x0001) copy pixel to screen buffer
			if (newLayer[newLayerIndex] != 0x0001 && !(xAddr >= xOrigin + width) && !( yAddr >= yOrigin + height)) screenBuffer[screenBufferIndex] = newLayer[newLayerIndex];
		}
	}
}

// function to draw the score to the screen
// creates a string from the score value
// writes the string to the screen
void ScreenBuffer_drawScore(unsigned short* screenBuffer, int score, unsigned int size, unsigned int colour, unsigned int xOrigin, unsigned int yOrigin) {
	char scoreChar[10], digitChar[1];
	int i, digit;

	// create string of characters to be displayed
	strncpy(scoreChar, "Score: ", 8);		// write score to string
	for (i = 2; i >= 0; i--) {				// calculate each digit of score
		digit 			= score/(pow(10,i));
		digit 			= digit % 10;
		digitChar[0] 	= digit + '0';		// convert digit to ASCII code
		strcat(scoreChar, digitChar);		// add digit to string
	}

	// draw string to screen buffer
	ScreenBuffer_drawString(screenBuffer, scoreChar, size, 0XFFFF, xOrigin, yOrigin);
}

// function to write a string to the screen
// generates a bitmap for each char of the score string
// prints each bitmap to the screen buffer
void ScreenBuffer_drawString(unsigned short* screenBuffer, char* string, unsigned int size, unsigned int colour, unsigned int xOrigin, unsigned int yOrigin) {
	unsigned short charSprite[640];
	int i;

	// convert each character of string until null character found
	for (i = 0; string[i] != '\0'; i++) {
		ScreenBuffer_generateFontBitmap (charSprite, string[i], size, colour);
		ScreenBuffer_drawSprite(screenBuffer, charSprite, (xOrigin + (i*6*size)), yOrigin, 5*size, 8*size);
	}
}

// function to create bitmaps for characters from the font map
void ScreenBuffer_generateFontBitmap(unsigned short* charSprite, char letter, unsigned int size , unsigned short colour)  {
	signed char line;
	int i, j;

	// check font size is within range
	if (size < 1) size = 1;
	else if (size > 4) size = 4;

	for (i = 0; i < (5 * size); i++) {
		line = BF_fontMap[letter - ' '][i/size];									// find desired column value in font map
		for (j = 0; j < 8*size; j++){												// convert font map to true bitmap
			if ((line & 0x01) != 0x00)	charSprite[(5 * size * j) + i] = colour;
			else  						charSprite[(5 * size * j) + i] = 0x0001;

			if ((j + 1) % size == 0)	line = line >> 1;
		}
	}
}

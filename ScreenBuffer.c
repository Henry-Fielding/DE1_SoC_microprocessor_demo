/*
 * ScreenBuffer.c
 *
 *  Created on: 31 May 2021
 *      Author: Henry
 */

#include "ScreenBuffer.h"

void ScreenBuffer_resetBuffer (unsigned short* screenBuffer, const unsigned short* background) {
	unsigned int index;
	for (index = 0; index < 76800; index++){
		screenBuffer[index] = background [index];
	}
}

void ScreenBuffer_drawSprite (unsigned short* screenBuffer, const unsigned short* newLayer, signed int xOrigin, signed int yOrigin, unsigned int width, unsigned int height) {
	signed int xInit, yInit, xEnd, yEnd;
	signed int xAddr, yAddr, newLayerIndex, screenBufferIndex;
	// select section of sprite that is on screen
	xInit = xOrigin;
	if (xInit < 0) xInit = 0;

	yInit = yOrigin;
	if (yInit < 0) yInit = 0;

	xEnd = xOrigin + width;
	if (xEnd > 319) xEnd = 319;

	yEnd = yOrigin + height;
	if (yEnd > 319) yEnd = 319;

	// transfer sprite to current Frame
	for(yAddr = yOrigin; yAddr <= yEnd ; yAddr++) {
		for (xAddr = xOrigin; xAddr <= xEnd ; xAddr++) {
			newLayerIndex = (yAddr - yOrigin) * width + (xAddr - xOrigin);
			screenBufferIndex = (yAddr * 240) + xAddr;

			if (xAddr < xOrigin || xAddr >= xOrigin + width || yAddr < yOrigin || yAddr >= yOrigin + height || newLayer[newLayerIndex] == 0x0001) {} // do nothing
			else screenBuffer[screenBufferIndex] = newLayer[newLayerIndex];
		}
	}
}

void ScreenBuffer_drawScore(unsigned short* screenBuffer, int score, unsigned int size, unsigned int colour, unsigned int xOrigin, unsigned int yOrigin) {
	char scoreChar[10], digitChar[1];
	int i, digit;

	strncpy(scoreChar, "Score: ", 8);
	for (i = 2; i >= 0; i--) {
		digit = score/(pow(10,i));
		digit = digit % 10;
		digitChar[0] = digit + '0';
		strcat(scoreChar, digitChar);
	}

	ScreenBuffer_drawString(screenBuffer, scoreChar, size, 0XFFFF, xOrigin, yOrigin);
}

void ScreenBuffer_drawString(unsigned short* screenBuffer, char* string, unsigned int size, unsigned int colour, unsigned int xOrigin, unsigned int yOrigin) {
	unsigned short charSprite[640];
	int i;

	for (i = 0; string[i] != '\0'; i++) {
		ScreenBuffer_generateChar (charSprite, string[i], size, colour);
		ScreenBuffer_drawSprite(screenBuffer, charSprite, (xOrigin + (i*6*size)), yOrigin, 5*size, 8*size);
	}
}


void ScreenBuffer_generateChar (unsigned short* charSprite, char letter, unsigned int size , unsigned short colour)  {
	signed char line;
	int i, j;

	if (size < 1) size = 1;
	else if (size > 4) size = 4;

	for (i = 0; i < (5 * size); i++) {
		line = BF_fontMap[letter -' '][i/size];
		for (j = 0; j < 8*size; j++){

			if ((line & 0x01) != 0x00 ) charSprite[(5 * size * j) + i] = colour;
			else  charSprite[(5 * size * j) + i] = 0x0001;

			if ((j + 1) % size == 0)	line = line >> 1;
		}
	}
}

/*
 *	ScreenBuffer (header file)
 *	------------------------------
 *	Created on: 31 May 2021
 *	Author: Henry Fielding
 *
 *	Description
 *	-------------
 *	A library of functions to handle various interaction with the screen buffer array for the 240*320 LCD module
 */


#ifndef SCREENBUFFER_H_
#define SCREENBUFFER_H_

#include <string.h>
#include <math.h>

#include "BasicFont/BasicFont.h"

// function to reset the screen buffer
// copies the RGB565 value of each address of the background bitmap to the correspoding adress in the screenBuffer
void ScreenBuffer_resetBuffer (unsigned short* screenBuffer, const unsigned short* background);

// function to draw a sprite/bitmap to the correct position in the screen buffer array
void ScreenBuffer_drawSprite (unsigned short* screenBuffer, const unsigned short* newLayer, signed int xOrigin, signed int yOrigin, unsigned int width, unsigned int height);

// function to draw the score to the screen
// creates a string from the score value
// writes the string to the screen
void ScreenBuffer_drawScore(unsigned short* screenBuffer, int score, unsigned int size, unsigned int colour, unsigned int xOrigin, unsigned int yOrigin);

// function to write a string to the screen
// generates a bitmap for each char of the score string
// prints each bitmap to the screen buffer
void ScreenBuffer_drawString(unsigned short* screenBuffer, char* string, unsigned int size, unsigned int colour, unsigned int xOrigin, unsigned int yOrigin);

// function to create bitmaps for characters from the font map
void ScreenBuffer_generateFontBitmap (unsigned short* charSprite, char letter, unsigned int size , unsigned short colour);

#endif /* SCREENBUFFER_H_ */



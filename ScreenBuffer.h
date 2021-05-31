/*
 * ScreenBuffer.h
 *
 *  Created on: 31 May 2021
 *      Author: Henry
 */

#ifndef SCREENBUFFER_H_
#define SCREENBUFFER_H_

#include <string.h>
#include <math.h>

#include "BasicFont/BasicFont.h"

void ScreenBuffer_resetBuffer (unsigned short* screenBuffer, const unsigned short* background);

void ScreenBuffer_drawSprite (unsigned short* screenBuffer, const unsigned short* newLayer, signed int xOrigin, signed int yOrigin, unsigned int width, unsigned int height);

void ScreenBuffer_drawScore(unsigned short* screenBuffer, int score, unsigned int size, unsigned int colour, unsigned int xOrigin, unsigned int yOrigin);

void ScreenBuffer_drawString(unsigned short* screenBuffer, char* string, unsigned int size, unsigned int colour, unsigned int xOrigin, unsigned int yOrigin);

void ScreenBuffer_generateChar (unsigned short* charSprite, char letter, unsigned int size , unsigned short colour);

#endif /* SCREENBUFFER_H_ */



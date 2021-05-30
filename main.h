/*
 * Mini Project Top Level (header file)
 * ------------------------------------
 * Created on: 28 May 2021
 * Author: Henry Fielding
 *
 * Description
 * -----------
 * Program to handle the top level functionality of the mini-project
 *
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <string.h>

// include required header files
#include "HPS_Watchdog/HPS_Watchdog.h"			// hardware drivers
#include "DE1SoC_LT24/DE1SoC_LT24.h"
#include "DE1Soc_SevenSeg/DE1Soc_SevenSeg.h"
#include "HPS_PrivateTimer/HPS_PrivateTimer.h"

#include "Sprites/Sprites.h"						// sprite bitmaps


// define hardware base addresses
volatile unsigned int *key_edge_ptr = (unsigned int *)0xFF20005C;	// KEYS 0-3 (push buttons)
volatile unsigned int *LEDR_ptr = (unsigned int *)0xFF200000; 		// LEDS 0-9
volatile unsigned int *key_ptr = (unsigned int *)0xFF200050; // KEYS 0-3 (push buttons)


struct position {
	signed int x, y;
	float dx, dy;
	const unsigned short* spriteId;
};

//
// function declarations
//

// function to clear inputs
// clears all set bits in the key edge register by writing 1 to them
void pushButtons_clear (void);

void addToFrame (unsigned short* currentFrame, const unsigned short* newLayer, signed int xOrigin, signed int yOrigin, unsigned int width, unsigned int height);

void clearbackground (const unsigned short*, unsigned int, unsigned int, unsigned int, unsigned int);

void configure_privateTimer (void);

void resetFrame (unsigned short* currentFrame, const unsigned short* background);

void initPlayer (struct position*);

void initPlatforms (struct position*);

void updatePlayer (struct position* player, struct position* platform);

void checkCollisions(struct position* player, struct position* platform);

void updateScreenPosition(struct position* player, struct position* platform);

void updateWaterAnimation(const unsigned short* waterSprite);

#endif /* MAIN_H_ */

/*
 *	Game Functions (header file)
 *	------------------------------
 *	Created on: 2 Jun May 2021
 *	Author: Henry Fielding
 *
 *	Description
 *	-------------
 *	A library of functions to handle various game play functions/ interactions
 */


#ifndef GAMEFUNCTION_H_
#define GAMEFUNCTION_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "../Sprites/Sprites.h"			// sprite bitmaps

struct object { //TODO: add size to object
	signed int x, y, width, height;
	float dx, dy;
	const unsigned short* spriteId;
};

// set initial values for the player object
void GameFunction_initPlayer (struct object*);

// set initial values for the water object
void GameFunction_initWater (struct object* water);

// set initial values for the platform objects
void GameFunction_initPlatforms (struct object*, unsigned int difficulty);

// function to update the player object attributes based on device inputs
void GameFunction_updatePlayer (struct object* player, struct object* platform);

// function to check for collisions between player object and platform objects
// and update positions accordingly
void GameFunction_checkCollisions(struct object* player, struct object* platform);

// function to update the position of screen objects based on player object position
void GameFunction_updateScreenobject(struct object* player, struct object* platform, float* score, unsigned int difficulty);

// function to increment the water sprite pointer to next water bitmap
void GameFunction_updateWaterAnimation(struct object* waterSprite);

#endif /* GAMEFUNCTION_H_ */

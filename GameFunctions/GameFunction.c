/*
 *	Game Functions (source file)
 *	------------------------------
 *	Created on: 2 Jun May 2021
 *	Author: Henry Fielding
 *
 *	Description
 *	-------------
 *	A library of functions to handle various game play functions/ interactions
 */
#include "GameFunction.h"

#define MAX_VERT_VELOCITY 15	// maximum velocity of player when bouncing
#define VERT_ACCELERATION 1		// downward acceleration of player when bouncing
#define HORZ_ACCELERATION 1		// side to side acceleration of player
#define MAX_HEIGHT 120			// max screen height reached by player when bouncing (screen scrolls beyond this)

// define hardware base addresses
extern volatile unsigned int *key_ptr;	// KEYS 0-3 (buttons)

// set initial values for the player object
void GameFunction_initPlayer (struct object* player) {
	// set initial values for the player object
	player->x 			= 102;
	player->y 			= 208;
	player->width		= 32;
	player->height		= 64;
	player->dx			= 0;
	player->dy			= 0;
	player->spriteId	= marioRightStand;
}

// set initial values for the water object
void GameFunction_initWater (struct object* water) {
	// set initial values for the water object
	water->x 			= 0;
	water->y 			= 288;
	water->width		= 240;
	water->height		= 32;
	water->dx			= 0;
	water->dy			= 0;
	water->spriteId		= water0;
}

// set initial values for the platform objects
void GameFunction_initPlatforms (struct object* platform, unsigned int difficulty) {
	int i;
	// set initial value for the first platform object (under players feet)
	platform[0].x			= 96;
	platform[0].y			= 272;
	platform[0].width		= 48;
	platform[0].height		= 16;
	platform[0].dx			= 0;
	platform[0].dy			= 0;
	platform[0].spriteId	= platformSprite;

	// randomly generate initials values for the remaining platform objects
	for (i = 1; i < 15; i++) {
		platform[i].x = rand()%191;										// x is randomly generated within the LCD screen width (- platform width)
		platform[i].y = platform[i-1].y - ((rand() % difficulty) + 16);	// y is randomly generated within an appropriate range of previous platform
		platform[i].width		= 48;
		platform[i].height		= 16;
		platform[i].dx = 0;
		platform[i].dy = 0;
		platform[i].spriteId = platformSprite;
	}
}

// function to update the player object attributes based on device inputs
void GameFunction_updatePlayer (struct object* player, struct object* platform) {
	// player motion in y direction
	player->dy = player->dy + VERT_ACCELERATION; // reduce player velocity
	player->y = player->y + player->dy;

	// player motion in x direction
	if (*key_ptr & 0x01) {
		player->dx = player->dx + HORZ_ACCELERATION;
		player->spriteId = marioRightJump;
	} else if (*key_ptr & 0x02) {
		player->dx = player->dx - HORZ_ACCELERATION;
		player->spriteId = marioLeftJump;
	} else {
		if (player->dx >= 0) player->spriteId = marioRightJump;
		if (player->dx < 0 ) player->spriteId = marioLeftJump;
	}

	player->x  = player->x + player->dx;
	if (player->x < 0) player->x = 0;
	if (player->x > 207) player->x = 207;

	GameFunction_checkCollisions(player, platform);
}

// function to check for collisions between player object and platform objects
// and update positions accordingly
void GameFunction_checkCollisions(struct object* player, struct object* platform) { //TODO: clean
	int i;

	// check for collision with each platform
	for (i = 0; i < 15; i++){
		if ((player->x + 32 > platform[i].x) && (player->x < platform[i].x + 48)
		&& (player->y + 64 > platform[i].y) && (player->y +64 < platform[i].y + 16)
		&& (player->dy > 0)){
			player->dy = -MAX_VERT_VELOCITY;											// update player velocity
			if (player->spriteId == marioRightJump) player->spriteId = marioRightStand;	// set appropriate animation for player
			if (player->spriteId == marioLeftJump) player->spriteId = marioLeftStand;
		}
	}
}

// function to update the position of screen objects based on player object position
void GameFunction_updateScreenobject(struct object* player, struct object* platform, float* score, unsigned int difficulty) { //TODO: clean
	int i;

	if (player->y < MAX_HEIGHT) {
		player->y = MAX_HEIGHT;						// player cannot move higher than max height
		*score = *score - (player->dy / 100);		// update score
		for (i = 0; i < 15; i++) {
			platform[i].dy	= player->dy;						// instead platforms move downwards at player velocity
			platform[i].y	= platform[i].y - platform[i].dy;

			if (platform[i].y > 300){							// if platforms exit the screen re-generate them randomly
				platform[i].x = rand()%191;
				if (i == 0)	platform[i].y = platform[14].y - ((rand() % difficulty) + 16);
				else 		platform[i].y = platform[i-1].y - ((rand() % difficulty) + 16);
			}
		}
	}
}

// function to increment the water sprite pointer to next water bitmap
void GameFunction_updateWaterAnimation(struct object* water) {
	if 		(water->spriteId == water0)	water->spriteId = water1;
	else if	(water->spriteId == water1)	water->spriteId = water2;
	else if	(water->spriteId == water2)	water->spriteId = water3;
	else if	(water->spriteId == water3)	water->spriteId = water0;
}

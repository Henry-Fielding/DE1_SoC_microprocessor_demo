/*
 * Mini Project Top Level (source file)
 * ------------------------------------
 * Created on: 28 May 2021
 * Author: Henry Fielding
 *
 * Description
 * -----------
 * Program to handle the top level functionality of the mini-project
 *
 */

#include "main.h"

#define MAX_VERT_VELOCITY 15 // maximum velocity of player when bouncing
#define VERT_ACCELERATION 1	// VERT_ACCELERATION rate of player when bouncing
#define HORZ_ACCELERATION 1
#define MAX_HEIGHT 120	// max screen height reached by player when bouncing (screen scrolls beyond this)

main() {
	// declare task scheduler variables
	unsigned int timerLast;

	// declare game variables
	int gameState = INTRO;
	unsigned short screenBuffer[76800];
	float score = 0;
	struct object player;
	struct object water;
	struct object platform[10];
	int i;

	//configure drivers
	configure_privateTimer();	//TODO: add these functions
	Timer_setControl(224, 0, 1, 1);
	timerLast = Timer_readTimer();

	LT24_initialise(0xFF200060,0xFF200080);
	//HPS_ResetWatchdog();

	while (1) {
		if (timerLast - Timer_readTimer() >= 16666){
			timerLast = Timer_readTimer();

			switch (gameState) {
				case INTRO :
					// update screen buffer
					ScreenBuffer_resetBuffer(screenBuffer, background);						// clear to background
					ScreenBuffer_drawSprite(screenBuffer, water0, 0, 288, 240, 32);			// draw water sprite
					ScreenBuffer_drawSprite(screenBuffer, introSprite, 0, 100, 240, 70);	// draw intro screen

					// update displays
					LT24_copyFrameBuffer(screenBuffer, 0, 0, 240, 320);	// copy screen buffer to display
					display_ScoreSevenSeg(score);						// display score on seven seg LCD

					// change state conditions
					if(*key_edge_ptr & 0x01) {	// if key0 is pressed
						gameState = INIT;
						pushButtons_clear();
					}
				break;

				case INIT :
					srand(Timer_readTimer());
					score = 0;
					initPlayer(&player);
					initPlatforms(platform);
					water.spriteId = water0;

					// change state conditions
					gameState = GAMELOOP;
				break;

				case GAMELOOP :
					// update game object positions and animations
					updatePlayer(&player, platform);				// update player position and animation
					updateScreenobject(&player, platform, &score);	// update platforms positions and animations
					updateWaterAnimation(&water);					// update water animation

					// update screen buffer
					ScreenBuffer_resetBuffer(screenBuffer, background);																			// clear to background
					for (i = 0; i < 10; i++) ScreenBuffer_drawSprite(screenBuffer, platform[i].spriteId, platform[i].x, platform[i].y, 48, 16);	// draw platforms
					ScreenBuffer_drawSprite(screenBuffer, player.spriteId, player.x, player.y, 32, 64);											// draw player sprite
					ScreenBuffer_drawSprite(screenBuffer, water.spriteId, 0, 288, 240, 32);														// draw water sprite
					ScreenBuffer_drawScore(screenBuffer, score, 2, 0xFFFF, 120, 305);															// draw score sprite

					// update displays
					LT24_copyFrameBuffer(screenBuffer, 0, 0, 240, 320);	// copy screen buffer to display
					display_ScoreSevenSeg(score);						// display score on seven seg LCD

					// change state conditions
					if (player.y > 288) {	// if player falls in water
						gameState = GAMEOVER;
						pushButtons_clear();
					}

				break;

				case GAMEOVER :
					// update screen buffer
					ScreenBuffer_drawSprite(screenBuffer, gameoverSprite, 0, 100, 240, 91);	// draw game over screen
					ScreenBuffer_drawSprite(screenBuffer, water.spriteId, 0, 288, 240, 32);	// hide old score position (bottom right)
					ScreenBuffer_drawScore(screenBuffer, score, 2, 0xFFFF, 60,  137);		// write current score to screen centre

					// update displays
					LT24_copyFrameBuffer(screenBuffer, 0, 0, 240, 320);	// copy screen buffer to display
					display_ScoreSevenSeg(score);						// display score on seven seg LCD

					//change state conditions
					if(*key_edge_ptr & 0x01) {	// if key 0 is pressed
						gameState = INIT;
						pushButtons_clear();
					}
				break;
			}
		}
			// Finally, reset the watchdog timer.
			HPS_ResetWatchdog();
	}
}


void initPlayer (struct object* player) {
	player->x 			= 102;
	player->y 			= 208;
	player->dx			= 0;
	player->dy			= 0;
	player->spriteId	= marioRightStand;
}

void initPlatforms (struct object* platform) {
	//struct object platform[10];
// generate 10 platforms and assign objects randomly on screen
	int i;
	platform[0].x = 96;
	platform[0].y = 272;
	platform[0].dx = 0;
	platform[0].dy = 0;
	platform[0].spriteId = platformSprite;

	for (i = 1; i < 10; i++) {
		platform[i].x = rand()%191;
		platform[i].y = platform[i-1].y - (rand()%70 + 16);
		platform[i].dx = 0;
		platform[i].dy = 0;
		platform[i].spriteId = platformSprite;
	}
}

void updatePlayer (struct object* player, struct object* platform) {
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

	checkCollisions(player, platform);
}

// update animation using statemachine

void checkCollisions(struct object* player, struct object* platform) {
	int i;

	for (i = 0; i < 10; i++){
		if ((player->x + 32 > platform[i].x) && (player->x < platform[i].x + 48)
		&& (player->y + 64 > platform[i].y) && (player->y +64 < platform[i].y + 16)
		&& (player->dy > 0)){
			player->dy = -MAX_VERT_VELOCITY;
			if (player->spriteId == marioRightJump) player->spriteId = marioRightStand;
			if (player->spriteId == marioLeftJump) player->spriteId = marioLeftStand;
		}
	}
}

void updateScreenobject(struct object* player, struct object* platform, float* score) {
	int i;

	if (player->y < MAX_HEIGHT) {
		player->y = 120;
		*score = *score - (player->dy / 100);
		for (i = 0; i < 10; i++) {
			platform[i].dy = player->dy;
			platform[i].y = platform[i].y - platform[i].dy;

			if (platform[i].y > 300){
				platform[i].x = rand()%191;
				if (i == 0)	platform[i].y = platform[9].y - (rand()%84 + 16);
				else 		platform[i].y = platform[i-1].y - (rand()%84 + 16);
			}
		}
	}

}

void updateWaterAnimation(struct object* water) {
	if 		(water->spriteId == water0)	water->spriteId = water1;
	else if	(water->spriteId == water1)	water->spriteId = water2;
	else if	(water->spriteId == water2)	water->spriteId = water3;
	else if	(water->spriteId == water3)	water->spriteId = water0;
}

//void update screen() {};




void display_ScoreSevenSeg (float score) {
	DE1SoC_SevenSeg_SetSingleAlpha(5, 19);
	DE1SoC_SevenSeg_SetSingleAlpha(4, 3);
	DE1SoC_SevenSeg_SetSingleAlpha(3, 18);
	DE1SoC_SevenSeg_SetSingle(2, (int)(score/100)%10);
	DE1SoC_SevenSeg_SetDoubleDec(0, (int)score%100);
}

// function to clear inputs
// clears all set bits in the key edge register by writing 1 to them
void pushButtons_clear () {
	unsigned int temp = *key_edge_ptr;
	*key_edge_ptr = temp;
}

void configure_privateTimer () {
	Timer_initialise(0xFFFEC600);	// set private timer base address
	Timer_setLoadValue(0xFFFFFFFF);	// load maximum value
	Timer_setControl(224, 0, 1, 0);	// timer initialised to disabled mode
}

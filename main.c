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



main() {
	const unsigned short* playerSprite;
	const unsigned short* waterSprite;
	unsigned short currentFrame[76800];
	unsigned int timerLast = 0;
	int i;



	struct position player;
	struct position platform[10];

	initPlayer(&player);
	initPlatforms(platform);



	//configure drivers
	configure_privateTimer();
	Timer_setControl(224, 0, 1, 1);
	timerLast = Timer_readTimer();

	LT24_initialise(0xFF200060,0xFF200080);

	HPS_ResetWatchdog();

	pushButtons_clear ();
	while (!*key_edge_ptr & 0x01) HPS_ResetWatchdog();// do nothing while waiting for key press
	pushButtons_clear ();

	srand(Timer_readTimer());

	LT24_copyFrameBuffer(background, 0, 0, 240, 320);




	// set player position
//	player.x = 102;
//	player.y = 240;


	while (1) {
		if (timerLast - Timer_readTimer() >= 16666){
			timerLast = Timer_readTimer();

			playerSprite = marioRightStand;

			// player motion in y direction

			player.dy = player.dy + 0.5; // reduce player velocity
			player.y = player.y + player.dy;
			// bounce on bottom of screen
			if (player.y > 240) {
				player.dy = -10;
				player.y = 240;
			} else {
				for (i = 0; i < 10; i++){
					if ((player.x + 32 > platform[i].x) && (player.x < platform[i].x + 48)
					&& (player.y + 64 > platform[i].y) && (player.y +64 < platform[i].y + 16)
					&& (player.dy > 0)){
						player.dy = -10;
					}
				}
			}

			// player motion in x direction
			if (*key_ptr & 0x01) player.x  = player.x + 5;
			else if (*key_ptr & 0x02) player.x  = player.x - 5;

			// platform motion
			if (player.y < 120) {
				player.y = 120;
				for (i = 0; i < 10; i++) {
					platform[i].dy = player.dy;
					platform[i].y = platform[i].y - platform[i].dy;

					if (platform[i].y > 320){
						platform[i].x = rand()%191;
						if (i == 0)	platform[i].y = platform[9].y - (rand()%84 + 16);
						else 		platform[i].y = platform[i-1].y - (rand()%84 + 16);
					}
				}
			}

			// update water animation
			waterSprite = water0;

			// draw sprites
			//addToFrame(currentFrame, background, 0, 0, 240, 320);
//			memcpy (currentFrame, background, 5*sizeof(unsigned short));
			resetFrame(currentFrame, background);

			for (i = 0; i < 10; i++) {
				if (platform[i].y >= -16 && platform[i].y < 350) {
					addToFrame(currentFrame, platform[i].spriteId, platform[i].x, platform[i].y, 48, 16);
				}
			}

				addToFrame(currentFrame, playerSprite, player.x, player.y, 32, 64); 	// draw player sprite
				addToFrame(currentFrame, waterSprite, 0, 288, 240, 32); 					//draw water sprite


				LT24_copyFrameBuffer(currentFrame, 0, 0, 240, 320);
		}

			// Finally, reset the watchdog timer.
			HPS_ResetWatchdog();
	}
}

void initPlayer (struct position* player) {
	player->x 			= 102;
	player->y 			= 240;
	player->dx			= 0;
	player->dy			= -10;
	player->spriteId	= marioRightStand;
}


void initPlatforms (struct position* platform) {
	//struct position platform[10];
// generate 10 platforms and assign positions randomly on screen
	int i;
	platform[0].x = 96;
	platform[0].y = 304;
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


void resetFrame (unsigned short* currentFrame, const unsigned short* background) {
	unsigned int index;
	for (index = 0; index < 76800; index++){
		currentFrame[index] = background [index];
	}
}

void addToFrame (unsigned short* currentFrame, const unsigned short* newLayer, signed int xOrigin, signed int yOrigin, unsigned int width, unsigned int height) {
	signed int xInit, yInit, xEnd, yEnd;
	signed int xAddr, yAddr, newLayerIndex, currentFrameIndex;

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
			currentFrameIndex = (yAddr * 240) + xAddr;

			if (xAddr < xOrigin || xAddr >= xOrigin + width || yAddr < yOrigin || yAddr >= yOrigin + height || newLayer[newLayerIndex] == 0x0001) {} // do nothing
			else currentFrame[currentFrameIndex] = newLayer[newLayerIndex];

		}
	}
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












//
// Defunct
//



// function to clear all previous sprites are reset background
//void clearbackground (const unsigned short* background, unsigned int xorigin, unsigned int yorigin, unsigned int width, unsigned int height) {
//	unsigned int i;
//	unsigned int j;
//
//	unsigned int index = (yorigin * width) + xorigin; // set initial index position
//	unsigned int cnt = 0;
//	//const unsigned int patchsize = (width * height);
//	unsigned short patch [2048];
//
//	//copy first row
//	for (i = 0; i < height; i = i + 1){
//		for (j = 0; j < width; j = j + 1) {
//			patch[cnt] = background[index + j];
//			cnt = cnt + 1;
//		}
//		index = index + 240;
//	}
//
//	//LT24_drawSprite(patch, xorigin, yorigin, width, height);
//}


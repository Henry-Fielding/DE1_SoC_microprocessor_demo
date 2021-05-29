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

struct position {
	signed int xNew, yNew, xPrev, yPrev;
};

main() {
	const unsigned short* playerSprite;
	unsigned int timerLast = 0;
	int i;

	float dx = 0, dy = 0;

	struct position player;
	struct position platform[10];
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
	player.xNew = 102;
	player.yNew = 240;
	player.xPrev = player.xNew;
	player.yPrev = player.yNew;

	// generate 10 platforms and assign positions randomly on screen
	platform[0].xNew = 96;
	platform[0].yNew = 304;
	platform[0].xPrev = platform[i].xNew;
	platform[0].yPrev = platform[i].yNew;

	for (i = 1; i < 10; i++) {
		platform[i].xNew = rand()%191;
		platform[i].yNew = platform[i-1].yNew - (rand()%84 + 16);
		platform[i].xPrev = platform[i].xNew;
		platform[i].yPrev = platform[i].yNew;
		HPS_ResetWatchdog();
	}

	while (1) {
		if (timerLast - Timer_readTimer() >= 25000){
			timerLast = Timer_readTimer();

//			if (*key_ptr & 0x01) xNew = xNew + 1;
//			else if (*key_ptr & 0x02) xNew = xNew - 1;
//			else if (*key_ptr & 0x04) xNew = xNew + 5;
//			else if (*key_ptr & 0x08) xNew = xNew - 5;
//
//			pushButtons_clear();
//
//			if (temp == 1)		playerSprite = marioRightStand;
//			else if (temp == 2)	playerSprite = marioRightWalk1;
//			else if (temp == 3)	playerSprite = marioRightWalk2;
//			else if (temp == 4)	playerSprite = marioRightWalk3;
//
			playerSprite = marioRightStand;

			player.yPrev = player.yNew;

			dy = dy + 0.5; // reduce player velocity
			player.yNew = player.yNew + dy;

			// bounce on bottom of screen
			if (player.yNew > 240) {
				dy = -10;
				player.yNew = 240;
			}






			// draw sprites
			for (i = 0; i < 10; i++) {
				if (platform[i].yNew >= 0) {
					LT24_drawSprite2(background, platformSprite, platform[i].xNew, platform[i].yNew, 48, 16, platform[i].xNew - platform[i].xPrev, platform[i].yNew - platform[i].yPrev);
				}
			}
				LT24_drawSprite2(background, playerSprite, player.xNew, player.yNew, 32, 64, player.xNew - player.xPrev, player.yNew - player.yPrev);
		}


			// Finally, reset the watchdog timer.
			HPS_ResetWatchdog();
	}
}


// function to clear all previous sprites are reset background
void clearbackground (const unsigned short* background, unsigned int xorigin, unsigned int yorigin, unsigned int width, unsigned int height) {
	unsigned int i;
	unsigned int j;

	unsigned int index = (yorigin * width) + xorigin; // set initial index position
	unsigned int cnt = 0;
	//const unsigned int patchsize = (width * height);
	unsigned short patch [2048];

	//copy first row
	for (i = 0; i < height; i = i + 1){
		for (j = 0; j < width; j = j + 1) {
			patch[cnt] = background[index + j];
			cnt = cnt + 1;
		}
		index = index + 240;
	}

	//LT24_drawSprite(patch, xorigin, yorigin, width, height);
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

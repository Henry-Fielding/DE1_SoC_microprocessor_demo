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
	unsigned short currentFrame[76800];
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
		platform[i].yNew = platform[i-1].yNew - (rand()%70 + 16);
		platform[i].xPrev = platform[i].xNew;
		platform[i].yPrev = platform[i].yNew;
		HPS_ResetWatchdog();
	}

	while (1) {
		if (timerLast - Timer_readTimer() >= 16666){
			timerLast = Timer_readTimer();

			playerSprite = marioRightStand;

			// player motion in y direction
			player.yPrev = player.yNew;
			dy = dy + 0.5; // reduce player velocity
			player.yNew = player.yNew + dy;
			// bounce on bottom of screen
			if (player.yNew > 240) {
				dy = -10;
				player.yNew = 240;
			} else {
				for (i = 0; i < 10; i++){
					if ((player.xNew + 32 > platform[i].xNew) && (player.xNew < platform[i].xNew + 48)
					&& (player.yNew + 64 > platform[i].yNew) && (player.yNew +64 < platform[i].yNew + 16)
					&& (dy > 0)){
						dy = -10;
					}
				}
			}

			// player motion in x direction
			player.xPrev = player.xNew;
			if (*key_ptr & 0x01) player.xNew  = player.xNew + 5;
			else if (*key_ptr & 0x02) player.xNew  = player.xNew - 5;

			// platform motion
			if (player.yNew < 120) {
				player.yNew = 120;
				for (i = 0; i < 10; i++) {
					platform[i].yPrev = platform[i].yNew;
					platform[i].yNew = platform[i].yNew - dy;

					if (platform[i].yNew > 320){
						platform[i].xNew = rand()%191;
						if (i == 0)	platform[i].yNew = platform[9].yNew - (rand()%84 + 16);
						else 		platform[i].yNew = platform[i-1].yNew - (rand()%84 + 16);
						platform[i].xPrev = platform[i].xNew;
						platform[i].yPrev = platform[i].yNew;
					}
				}
			}

			// draw sprites
			//addToFrame(currentFrame, background, 0, 0, 240, 320);
//			memcpy (currentFrame, background, 5*sizeof(unsigned short));
			resetFrame(currentFrame, background);

			for (i = 0; i < 10; i++) {
				if (platform[i].yNew >= 0 && platform[i].yNew < 300) {
					//LT24_drawSprite2(background, platformSprite, platform[i].xNew, platform[i].yNew, 48, 16, platform[i].xNew - platform[i].xPrev, platform[i].yNew - platform[i].yPrev);
					addToFrame(currentFrame, platformSprite, platform[i].xNew, platform[i].yNew, 48, 16);
				}
			}

				addToFrame(currentFrame, playerSprite, player.xNew, player.yNew, 32, 64);
				//LT24_drawSprite2(currentFrame, playerSprite, player.xNew, player.yNew, 32, 64, player.xNew - player.xPrev, player.yNew - player.yPrev);

				LT24_copyFrameBuffer(currentFrame, 0, 0, 240, 320);
		}

			// Finally, reset the watchdog timer.
			HPS_ResetWatchdog();
	}
}

void resetFrame (unsigned short* currentFrame, const unsigned short* background) {
	unsigned int index;
	for (index = 0; index < 76800; index++){
		currentFrame[index] = background [index];
	}
}

void addToFrame (unsigned short* currentFrame, const unsigned short* newLayer,unsigned int xOrigin, unsigned int yOrigin, unsigned int width, unsigned int height) {
	// at beginning of each loop start with background layer
	// after each write add the new layer to the current Frame
	// feed the new frame into the LCD
	unsigned int xAddr, yAddr, newLayerIndex, currentFrameIndex;

	unsigned int xEnd = xOrigin + width;
	unsigned int yEnd = yOrigin + height;

	for(yAddr = yOrigin; yAddr < yEnd ; yAddr++) {
		for (xAddr = xOrigin; xAddr < xEnd ; xAddr++) {
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


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
	signed int temp = 1;
	signed int xOld = 0;
	signed int yOld = 0;
	signed int xNew = 50;
	signed int yNew = 50;
	unsigned int timerLast = 0;

	//configure drivers
	configure_privateTimer();
	Timer_setControl(224, 0, 1, 1);
	timerLast = Timer_readTimer();

	LT24_initialise(0xFF200060,0xFF200080);

	HPS_ResetWatchdog();

	LT24_copyFrameBuffer(background, 0, 0, 240, 320);


	while (1) {
		if (timerLast - Timer_readTimer() >= 25000){
			timerLast = Timer_readTimer();

			xOld = xNew;
			yOld = yNew;
			if (*key_ptr & 0x01) xNew = xNew + 1;
			else if (*key_ptr & 0x02) xNew = xNew - 1;
			else if (*key_ptr & 0x04) xNew = xNew + 5;
			else if (*key_ptr & 0x08) xNew = xNew - 5;

			pushButtons_clear();
	//		else if (*key_edge_ptr & 0x04)
	//		else if (*key_edge_ptr & 0x08)



			if (temp == 1)		playerSprite = marioRightStand;
			else if (temp == 2)	playerSprite = marioRightWalk1;
			else if (temp == 3)	playerSprite = marioRightWalk2;
			else if (temp == 4)	playerSprite = marioRightWalk3;

			//

			//if (xNew != xOld || yNew != yOld) clearbackground(background, xOld, yOld, 32, 64);
			LT24_drawSprite(background, playerSprite, xNew, yNew, 32, 64, xNew - xOld, yNew - yOld);
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

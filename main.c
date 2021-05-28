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
	signed int temp;

	LT24_initialise(0xFF200060,0xFF200080);

	HPS_ResetWatchdog();

	while (1) {
		if (*key_edge_ptr & 0x01) {
			temp = temp + 1;
			if (temp > 4) temp = 1;
			if (temp < 1) temp = 4;
			pushButtons_clear();
		}

		if (temp == 1)		playerSprite = marioRightStand;
		else if (temp == 2)	playerSprite = marioRightWalk1;
		else if (temp == 3)	playerSprite = marioRightWalk2;
		else if (temp == 4)	playerSprite = marioRightWalk3;


		LT24_copyFrameBuffer(playerSprite, 0, 50, 32, 64);


		// Finally, reset the watchdog timer.
		HPS_ResetWatchdog();
	}
}

// function to clear inputs
// clears all set bits in the key edge register by writing 1 to them
void pushButtons_clear () {
	unsigned int temp = *key_edge_ptr;
	*key_edge_ptr = temp;
}

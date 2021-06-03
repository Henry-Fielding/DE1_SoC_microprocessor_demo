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

#define FRAME_RATE 30

// define hardware base addresses
volatile unsigned int *key_ptr = (unsigned int *)0xFF200050; 		// KEYS 0-3 (buttons)
volatile unsigned int *key_edge_ptr = (unsigned int *)0xFF20005C;	// KEYS 0-3 (button edge)
volatile unsigned int *switch_ptr = (unsigned int *)0xFF200040;		// Switches 0-9
volatile unsigned int *LEDR_ptr = (unsigned int *)0xFF200000; 		// LEDS 0-9

main() {
	// declare task scheduler variables
	unsigned int timerLast;

	// declare game variables
	int gameState = INTRO;
	unsigned short screenBuffer[76800];
	float score = 0;
	struct object player;
	struct object water;
	struct object platform[15];
	int i;
	volatile unsigned int difficulty;

	//configure drivers
	configure_privateTimer();
	timerLast = Timer_readTimer();

	LT24_initialise(0xFF200060, 0xFF200080);
	//HPS_ResetWatchdog();

	while (1) {
		if (timerLast - Timer_readTimer() >= 1000000 / FRAME_RATE){
			timerLast = Timer_readTimer();
			difficulty = 20 * (*switch_ptr & 0x03) + 30;

			switch (gameState) {
				case INTRO :
					// update screen buffer
					ScreenBuffer_resetBuffer(screenBuffer, background);													// clear to background
					ScreenBuffer_drawSprite(screenBuffer, water.spriteId, water.x, water.y, water.width, water.height);	// draw water sprite
					ScreenBuffer_drawSprite(screenBuffer, introSprite, 0, 100, 240, 70);								// draw intro screen

					// update displays
					LT24_copyFrameBuffer(screenBuffer, 0, 0, 240, 320);	// copy screen buffer to display
					display_ScoreSevenSeg(score);						// display score on seven seg LCD

					// change state conditions
					if(*key_edge_ptr & 0x01) {		// if key0 is pressed
						gameState = INIT;
						pushButtons_clear();
					}
				break;

				case INIT :
					srand(Timer_readTimer());
					score = 0;
					GameFunction_initPlayer(&player);
					GameFunction_initWater(&water);
					GameFunction_initPlatforms(platform, difficulty);

					// change state conditions
					gameState = GAMELOOP;
				break;

				case GAMELOOP :
					// update game object positions and animations
					GameFunction_updatePlayer(&player, platform);							// update player position and animation
					GameFunction_updateScreenobject(&player, platform, &score, difficulty);	// update platforms positions and animations
					GameFunction_updateWaterAnimation(&water);								// update water animation

					// update screen buffer
					ScreenBuffer_resetBuffer(screenBuffer, background);																											// clear to background
					for (i = 0; i < 15; i++) ScreenBuffer_drawSprite(screenBuffer, platform[i].spriteId, platform[i].x, platform[i].y, platform[i].width, platform[i].height);	// draw platforms
					ScreenBuffer_drawSprite(screenBuffer, player.spriteId, player.x, player.y, player.width,player.height);														// draw player sprite
					ScreenBuffer_drawSprite(screenBuffer, water.spriteId, water.x, water.y, water.width, water.height);															// draw water sprite
					ScreenBuffer_drawScore(screenBuffer, score, 2, 0xFFFF, 120, 305);																							// draw score sprite

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
					ScreenBuffer_drawSprite(screenBuffer, gameoverSprite, 0, 100, 240, 91);								// draw game over screen
					ScreenBuffer_drawSprite(screenBuffer, water.spriteId, water.x, water.y, water.width, water.height);	// hide old score position (bottom right)
					ScreenBuffer_drawScore(screenBuffer, score, 2, 0xFFFF, 60,  137);									// write current score to screen centre

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
			HPS_ResetWatchdog();	// Finally, reset the watchdog timer.
	}
}

// function to display score value of 7 segment display
void display_ScoreSevenSeg (float score) {
	DE1SoC_SevenSeg_SetSingleAlpha(5, 19);				// S
	DE1SoC_SevenSeg_SetSingleAlpha(4, 3);				// C
	DE1SoC_SevenSeg_SetSingleAlpha(3, 18);				// R
	DE1SoC_SevenSeg_SetSingle(2, (int)(score/100)%10);	// score[Digit 2]
	DE1SoC_SevenSeg_SetDoubleDec(0, (int)score%100);	// score[Digit 1: Digit 0]
}

// function to clear inputs
// clears all set bits in the key edge register by writing 1 to them
void pushButtons_clear () {
	unsigned int temp = *key_edge_ptr;
	*key_edge_ptr = temp;
}

// function to configure the private timer
// initialises to prescaler = 224 (freq = 1MHz) and timer to enabled
void configure_privateTimer () {
	Timer_initialise(0xFFFEC600);	// set private timer base address
	Timer_setLoadValue(0xFFFFFFFF);	// load maximum value
	Timer_setControl(224, 0, 1, 1);	// timer initialised to enabled mode
}

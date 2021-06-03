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

// include required header files
#include "HPS_Watchdog/HPS_Watchdog.h"			// hardware drivers
#include "DE1SoC_LT24/DE1SoC_LT24.h"
#include "DE1Soc_SevenSeg/DE1Soc_SevenSeg.h"
#include "HPS_PrivateTimer/HPS_PrivateTimer.h"
#include "ScreenBuffer/ScreenBuffer.h"
#include "GameFunctions/GameFunction.h"

enum State {
	INTRO,
	INIT,
	GAMELOOP,
	GAMEOVER
};

// function to display score value of 7 segment display
void display_ScoreSevenSeg (float score);

// function to clear inputs
// clears all set bits in the key edge register by writing 1 to them
void pushButtons_clear (void);

// function to configure the private timer
// initialises to prescaler = 224 (freq = 1MHz) and timer to enabled
void configure_privateTimer (void);

#endif /* MAIN_H_ */

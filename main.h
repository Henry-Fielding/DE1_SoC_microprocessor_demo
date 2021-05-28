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
#include "Sprites/MarioSprites.h"						// sprite bitmaps

// define hardware base addresses
volatile unsigned int *key_edge_ptr = (unsigned int *)0xFF20005C;	// KEYS 0-3 (push buttons)
volatile unsigned int *LEDR_ptr = (unsigned int *)0xFF200000; 		// LEDS 0-9

//
// function declarations
//

// function to clear inputs
// clears all set bits in the key edge register by writing 1 to them
void pushButtons_clear (void);

#endif /* MAIN_H_ */

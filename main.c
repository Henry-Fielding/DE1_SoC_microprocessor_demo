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
	const unsigned short* waterSprite = water0;
	unsigned short currentFrame[76800];
	unsigned int timerLast = 0;
	float score = 0;

	int i;
	int j;


	char scoreChar[10];
	char test[1];
	volatile int digit;


	//unsigned short letter[160];
//	signed char line;
////	unsigned short colour;
//	int size;
//	char test;
	unsigned short charSprite[640];

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

	// introduction
	pushButtons_clear ();
	while (!*key_edge_ptr & 0x01) HPS_ResetWatchdog(); // do nothing while waiting for key press
	pushButtons_clear ();

	srand(Timer_readTimer());

	LT24_copyFrameBuffer(background, 0, 0, 240, 320);

	while (1) {
		if (timerLast - Timer_readTimer() >= 16666){
			timerLast = Timer_readTimer();

			updatePlayer(&player, platform);
			updateScreenPosition(&player, platform, &score);
			updateWaterAnimation(&waterSprite); // add to some sort of timer function for the sake of it

			// draw score to screen and to BCD counter

			// display score on seven seg lcd
			display_ScoreSevenSeg(score);

			resetFrame(currentFrame, background);

			// read the font map
			// add the values to an array in the correct colour

			// draw the array to the correct position

//			colour = 0xF000;
//
//			size = 2;
//
//			test = 'A';
//
//			if (size < 1) size = 1;
//			else if (size > 4) size = 4;
//
//			for (i = 0; i < (5 * size); i++) {
//				line = BF_fontMap[test-' '][i/size];
//				for (j = 0; j < 8*size; j++){
//
//					if ((line & 0x01) != 0x00 ) letter[(5 * size * j) + i] = colour;
//					else  letter[(5 * size * j) + i] = 0x0001;
//
//					if ((j + 1) % size == 0)	line = line >> 1;
//				}
//			}
//







			// draw sprites



			for (i = 0; i < 10; i++) {
				if (platform[i].y >= -16 && platform[i].y < 350) {
					addToFrame(currentFrame, platform[i].spriteId, platform[i].x, platform[i].y, 48, 16);
				}
			}

			addToFrame(currentFrame, player.spriteId, player.x, player.y, 32, 64); 	// draw player sprite
			addToFrame(currentFrame, waterSprite, 0, 288, 240, 32); 					//draw water sprite

			//ScreenBuffer_generateChar (charSprite, 'F', 2, 0xF000);
			//addToFrame(currentFrame, charSprite, 50, 50, 5*2, 8*2);

			strncpy(scoreChar, "Score: ", 10);
			for (i = 2; i >= 0; i--) {
				digit = score/(pow(10,i));
				digit = digit % 10;
				test[0] = digit + '0';
				strcat(scoreChar, test);
			}

			ScreenBuffer_addCharsToFrame(currentFrame, scoreChar, 2, 0XFFFF, 120, 305);


			LT24_copyFrameBuffer(currentFrame, 0, 0, 240, 320);
		}

			// Finally, reset the watchdog timer.
			HPS_ResetWatchdog();
	}
}

void ScreenBuffer_generateChar (unsigned short* charSprite, char letter, unsigned int size , unsigned short colour)  {
	signed char line;
	int i, j;

	if (size < 1) size = 1;
	else if (size > 4) size = 4;

	for (i = 0; i < (5 * size); i++) {
		line = BF_fontMap[letter -' '][i/size];
		for (j = 0; j < 8*size; j++){

			if ((line & 0x01) != 0x00 ) charSprite[(5 * size * j) + i] = colour;
			else  charSprite[(5 * size * j) + i] = 0x0001;

			if ((j + 1) % size == 0)	line = line >> 1;
		}
	}
}

void ScreenBuffer_addCharsToFrame(unsigned short* currentFrame, char* string, unsigned int size, unsigned int colour, unsigned int xOrigin, unsigned int yOrigin) {
	unsigned short charSprite[640];
	int i;

	for (i = 0; string[i] != '\0'; i++) {
		ScreenBuffer_generateChar (charSprite, string[i], size, colour);
		addToFrame(currentFrame, charSprite, (xOrigin + (i*6*size)), yOrigin, 5*size, 8*size);
	}
}

void initPlayer (struct position* player) {
	player->x 			= 102;
	player->y 			= 208;
	player->dx			= 0;
	player->dy			= 0;
	player->spriteId	= marioRightStand;
}

void initPlatforms (struct position* platform) {
	//struct position platform[10];
// generate 10 platforms and assign positions randomly on screen
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

void updatePlayer (struct position* player, struct position* platform) {
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

void checkCollisions(struct position* player, struct position* platform) {
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

void updateScreenPosition(struct position* player, struct position* platform, float* score) {
	int i;

	if (player->y < MAX_HEIGHT) {
		player->y = 120;
		*score = *score - (player->dy / 100);
		for (i = 0; i < 10; i++) {
			platform[i].dy = player->dy;
			platform[i].y = platform[i].y - platform[i].dy;

			if (platform[i].y > 320){
				platform[i].x = rand()%191;
				if (i == 0)	platform[i].y = platform[9].y - (rand()%84 + 16);
				else 		platform[i].y = platform[i-1].y - (rand()%84 + 16);
			}
		}
	}

}

void updateWaterAnimation(const unsigned short** waterSprite) {
	if 		(*waterSprite == water0)	*waterSprite = water1;
	else if	(*waterSprite == water1)	*waterSprite = water2;
	else if	(*waterSprite == water2)	*waterSprite = water3;
	else if	(*waterSprite == water3)	*waterSprite = water0;
}

//void update screen() {};

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


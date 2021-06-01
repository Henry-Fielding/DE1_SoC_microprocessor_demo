/*
 * Sprites.h (header file)
 * --------------------------
 * Created on: 30/05/2021
 * Author: Henry Fielding
 *
 * Description
 * -----------
 * A unified header file for all sprite bitmaps declared within the the various sprite source files
 */

#ifndef SPRITES_H_
#define SPRITES_H_

//
// declare bitmap arrays
//
// background bitmaps
extern const unsigned short background[76800];	// 240x320

// popup bitmaps
extern const unsigned short gameoverSprite[21840];	// 240x91
extern const unsigned short introSprite[16800];		// 240x70

// player character (mario) bitmaps
extern const unsigned short marioRightStand[2048];	// 32x64
extern const unsigned short marioLeftStand[2048];	// 32x64
extern const unsigned short marioRightJump[2048];	// 32x64
extern const unsigned short marioLeftJump[2048];	// 32x64

// platform bitmaps
extern const unsigned short platformSprite[768];	// 48x16

// water bitmaps
extern const unsigned short water0[7680];	// 240x32
extern const unsigned short water1[7680];	// 240x32
extern const unsigned short water2[7680];	// 240x32
extern const unsigned short water3[7680];	// 240x32

#endif /* SPRITES_H_ */

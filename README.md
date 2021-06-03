# ELEC5620M Mini-Project Repository - Miniproject
This repository contains the files required for the mini-project demonstration game.
The included files and project hierarchy are as follows:

## Original IP 
### 1st level
| Filename 				| Descriptions                 			 |
|---|---|
| main.c/.h  	| Program to handle the top level functionality of the game using a statemachine taking inputs from keys [1:0], and switches [1:0] and outputting to 7 segment LCDS and LT24 LCD. |

### 2nd level
| Filename 				| Descriptions                 			 |
|---|---|
| GameFunctions.c/.h | A library of functions to handle various game play functions/ interactions|
| ScreenBuffer.c/.h |	A library of functions to handle various interaction with the screen buffer array for the 240*320 LCD module |
| HPS_PrivateTimer.c/.h | Functions for interacting with the private timer of the the DE1-SoC computer. |
| Sprites.c/.h | A unified header file for all sprite bitmaps declared within the the various sprite source files |

## Externally Sourced Driver Files
### 2nd level
| Filename | Description | Company | Author/s |
|---|---|---|---|
| HPS_Watchdog.c./.h | Simple inline functions for resetting watchdog and returning the current watchdog timer value. | University of Leeds | T Carpenter |
| DE1SocSevenSeg.c/.h | Functions for displaying input values on the seven segment LEDs in the DE1-SoC computer | University of Leeds | Harry Clegg, Henry Fielding |




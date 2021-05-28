/*
 *	Private timer driver (header file)
 *	-----------------------------------
 *	Created on: 15 Mar 2021
 *	Author: Henry Fielding
 *
 *	Description
 *	-------------
 *	Functions for interacting with the private timer of the the DE1-SoC computer.
 */

#include <stdbool.h>

#define TIMER_SUCCESS       0
#define TIMER_ERRORNOINIT  -1

// function to initialise the Timer
signed int Timer_initialise(unsigned int base_address);

// check if driver initialised
bool Timer_isInitialised( void );

// set the private timer load value
// - timer begins at load value and counts down to 0
// - min value 0x0
// - max value 0xFFFFFFFF
signed int Timer_setLoadValue(unsigned int loadvalue);

// set the private timer control register
// - "prescaler" determines counting rate, Timer decrements every (prescaler + 1) clock cycles.
// - "I" Processor interrupts, true to enable, false to disable.
// - "A" Automatic counter resets, true to enable, false to disable.
// - "E" Timer enable, true to enable, false to disable.
signed int Timer_setControl(unsigned int prescaler, bool I, bool A, bool E);

// read the private timer counter register value
// - min value 0x0
// - max value = private timer load value
signed int Timer_readTimer( void );

// read the private timer interrupt flag
// - interupt flag set high when private timer counter reaches 0 and stays high until reset.
signed int Timer_readInterrupt( void );

// read the private timer interrupt flag
// - interrupt flag set high when private timer counter reaches 0.
// - interrupt flag reset to low by writing 0x1 to it.
signed int Timer_clearInterrupt( void );

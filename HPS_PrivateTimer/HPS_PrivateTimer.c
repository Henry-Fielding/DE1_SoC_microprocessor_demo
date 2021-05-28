/*
 *	Private timer driver (source file)
 *	-----------------------------------
 *	Created on: 15 Mar 2021
 *	Author: Henry Fielding
 *
 *	Description
 *	-------------
 *	Functions for interacting with the private timer of the the DE1-SoC computer.
 */

#include "HPS_PrivateTimer.h"

//Driver Base Addresses
volatile unsigned int   *timer_base_ptr     = 0x0;  //0xFFFEC600

//Driver Initialised
bool timer_initialised = false;

// Private Timer Register Offsets
#define TIMER_LOAD      (0x00/sizeof(unsigned int))
#define TIMER_VALUE     (0x04/sizeof(unsigned int))
#define TIMER_CONTROL   (0x08/sizeof(unsigned int))
#define TIMER_INTERRUPT (0x0C/sizeof(unsigned int))

//Function to initialise the Timer
signed int Timer_initialise(unsigned int base_address){
    timer_base_ptr = (unsigned int *)  base_address;	// Set base address pointer

    timer_base_ptr[TIMER_CONTROL] = 0;					//Ensure timer initialises to disabled

    timer_initialised = true;							//Timer now initialised
    return TIMER_SUCCESS;
}

//Check if driver initialised
bool Timer_isInitialised() {
    return timer_initialised;
}

//Set the private timer load value
// - timer begins at load value and counts down to 0
// - min value 0x0
// - max value 0xFFFFFFFF
signed int Timer_setLoadValue(unsigned int loadvalue) {
	if (!timer_initialised) return TIMER_ERRORNOINIT;	// return error code if timer not initialised

	timer_base_ptr[TIMER_LOAD] = loadvalue;
	return TIMER_SUCCESS;
}

//Set the private timer control register
// - "prescaler" determines counting rate, Timer decrements every (prescaler + 1) clock cycles.
// - "I" Processor interrupts, true to enable, false to disable.
// - "A" Automatic counter resets, true to enable, false to disable.
// - "E" Timer enable, true to enable, false to disable.
signed int Timer_setControl(unsigned int prescaler, bool I, bool A, bool E) {
	if (!timer_initialised) return TIMER_ERRORNOINIT;	// return error code if timer not initialised

	timer_base_ptr[TIMER_CONTROL] = (prescaler << 8 | I << 2 | A << 1 | E << 0);
	return TIMER_SUCCESS;
}

//Read the private timer counter register value
// - min value 0x0
// - max value = private timer load value
signed int Timer_readTimer() {
	if (!timer_initialised) return TIMER_ERRORNOINIT;	// return error code if timer not initialised

	return timer_base_ptr[TIMER_VALUE];
}

// Read the private timer interrupt flag
// - interupt flag set high when private timer counter reaches 0 and stays high until reset.
signed int Timer_readInterrupt() {
	if (!timer_initialised) return TIMER_ERRORNOINIT;	// return error code if timer not initialised

	return timer_base_ptr[TIMER_INTERRUPT];
}

// Read the private timer interrupt flag
// - interrupt flag set high when private timer counter reaches 0.
// - interrupt flag reset to low by writing 0x1 to it.
signed int Timer_clearInterrupt() {
	if (!timer_initialised) return TIMER_ERRORNOINIT;	// return error code if timer not initialised

	timer_base_ptr[TIMER_INTERRUPT] = 0x1;
	return TIMER_SUCCESS;
}

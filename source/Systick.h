/*
 *  File name:Systick.h
 *  Created on: Dec 06, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This H file contains the declaration of Systick timer related functions
 *  Reference: PES Assignment 4
 */

#ifndef TIMING_H_
#define TIMING_H_

typedef uint32_t ticktime_t; //Time since boot, in sixteenth of a second


/*
 * Function for initialization of SysTick timer
 * Purpose: This function initializes the registers related to SysTick timer
 * Parameter passed: None
 * Parameters returned: None
 */
void Init_SysTick(void);


/*
 * Function to reset a variable that is used for calculating certain delay
 * Purpose: This function resets a variable every time it is called,
 * so that the counting can start again. But it doesn't affect the now() values
 * Parameter passed: None
 * Parameters returned: None
 */
void reset_timer(void);


/*
 * Function to return the variable count (used for calculating delay) since
 * last call to reset function
 * Parameter passed: None
 * Parameters returned: Variable's count value since reset: TimerMS_62_5 - Start_Timer
 */
ticktime_t get_timer(void);

#endif /* TIMING_H_ */

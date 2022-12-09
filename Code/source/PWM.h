/*
 *  Filename: PWM.h
 *  Created on: Dec 06, 2022
 *  Author: Shashank Chandrasekaran
 *	Description: This H file contains the declaration of of PWM initialization function
 *	Reference: PES Assignment 4
 */

#ifndef PWM_H_
#define PWM_H_

/*
 * Function for initialization of TPM0 and TPM2 channels for RGB LEDs
 * Purpose: This function initializes the registers for TPM0 and TPM2 for enabling
 * the PWM functionality of the LEDs
 * Parameter passed: Variable for loading the mod value of TPM: period
 * Parameters returned: None
 */
void Init_RGB_LED_PWM (uint16_t period);

#endif /* PWM_H_ */

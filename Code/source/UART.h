/*
 *  Filename: UART.h
 *  Created on: Dec 06, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This H file contains the declaration of UART0 initialization function
 *  Reference: PES Assignment 6
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define BAUD_RATE (9600) //9600 baud rate

/*
 * Function for initializing UART0
 * Parameter passed: Desired baud rate: baud_rate
 * Parameters returned: None
 */
void Init_UART0(uint32_t baud_rate);


#endif /* UART_H_ */

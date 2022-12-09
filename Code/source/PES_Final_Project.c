/*
 *  Filename: main.c
 *  Created on: Dec 06, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This C fie contains the main subroutine which calls the command processor
 *  Reference: None
 */

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include <MKL25Z4.H>
#include <math.h>
#include "test_cases.h"
#include "command_processor.h"
#include "UART.h"
#include "accelerometer.h"
#include "i2c.h"
#include "PWM.h"
#include "TSI.h"
#include "Systick.h"

#define PWM_PERIOD 256 //For obtaining intensity values from 0-255

int main(void)
{
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	//Initailizations
	Init_UART0(BAUD_RATE);
	Init_RGB_LED_PWM(PWM_PERIOD);
	Touch_Init();
	Init_SysTick();
	i2c_init();

	//Running test cases
	test_cbfifo();
	test_accelerometer();
	test_systick();
	test_TSI();
	printf("\r\nAll individual test cases are executed\r\n");

	//Command Processor called to execute position tracking
	command_processor();
}

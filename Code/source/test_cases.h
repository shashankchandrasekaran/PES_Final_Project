/*
 * Filename: test_cases.h
 * Created on: Dec 08, 2022
 * Author: Shashank Chandrasekaran
 * Description: This H file contains the declaration of test functions for CBFIFO, accelerometer
 * ,Systick and TSI
 * Reference: PES Assignment 6 for CBFIFO test cases
 */

#ifndef TEST_CASES_H_
#define TEST_CASES_H_

/**************************************CBFIFO TEST CASE*****************************************************************
 * Overall test function: This test function calls all the above test case functions
 * and returns 1 on success and 0 on faiure
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
void test_cbfifo(void);


/**************************************ACCELEROMETER TEST CASE*********************************************************
 * If the mma sensor is not initialized correctly, RED LED is turned on and test case fails. If it is initialized
 * properly, test case has passed
 * Parameters Passed: None
 * Parameters returned: None
 */
void test_accelerometer(void);


/**************************************SYSTICK TEST CASE***************************************************************
 * Systick timer test case is such that after 1 second is elapsed, it is checked if the systick timer value in seconds
 * is greater than 1 sec or not. If it is, test case has passed, else failed
 * Parameters Passed: None
 * Parameters returned: None
 */
void test_systick(void);


/*********************************************TSI TEST CASE************************************************************
 * IF a touch is detected within 10 seconds, test case passes else fails
 * Parameters Passed: None
 * Parameters returned: None
 */
void test_TSI(void);

#endif /* TEST_CASES_H_ */

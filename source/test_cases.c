/*
 * Filename: test_cases.c
 * Created on: Dec 08, 2022
 * Author: Shashank Chandrasekaran
 * Description: This C file contains the definition of test functions for CBFIFO, accelerometer
 * Systick and TSI
 * Reference: PES Assignment 6 for CBFIFO test cases
 */

#include <MKL25Z4.H>
#include <stdio.h>
#include <string.h>
#include "test_cases.h"
#include "cbfifo.h"
#include "accelerometer.h"
#include "Systick.h"
#include "TSI.h"

Q_T RX; //Declare a buffer

/******************************************CBFIFO TEST CASES START*****************************************************************/

/**************Test case 1: Enqueuing nbytes first time that is less than capacity of array*************
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 ******************************************************************************************************/
int cbfifo_test1(void)
{

	size_t expenqbytes=28;
	char values[]="Thisiscircularbuffertestcase";
	size_t calenqbytes= cbfifo_enqueue(&values[0],strlen(values),&RX);
	if(calenqbytes==expenqbytes)
		return 1;
	else
		return 0;
}

/*****************************Test 2: Enqueuing a null element*****************************************
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 ******************************************************************************************************/
int cbfifo_test2(void)
{
	size_t enqbytes=cbfifo_enqueue(NULL,3,&RX);
	if (enqbytes==-1)
		return 1;
	else
		return 0;
}

/********************Test 3: Dequeuing the dequeued value in a null pointer***************************
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 *****************************************************************************************************/
int cbfifo_test3(void)
{
	size_t deqbytes=cbfifo_dequeue(NULL,100,&RX);
	if (deqbytes==0)
		return 1;
	else
		return 0;
}

/****************Test 4: Dequeuing more than the available space in buffer*****************************
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 ******************************************************************************************************/
int cbfifo_test4(void)
{
	int i;
	char deqdata[28];
	int expdeqbytes=28;
	char expdeqdata[28]="Thisiscircularbuffertestcase";
	size_t deqbytes=cbfifo_dequeue(deqdata,30,&RX);
	for(i=0;i<28;i++)
	{
		if(deqdata[i]!=expdeqdata[i])
			return 0;
	}
	if (deqbytes==expdeqbytes)
		return 1;
	else
		return 0;
}

/******************Test 5: Enqueuing more than the capacity of buffer*********************************
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 *****************************************************************************************************/
int cbfifo_test5(void)
{
	size_t expenqbytes=256;
	char values[]="fgyxagnjzuqjsliuzyapxddsvxyypavfhtntfweqgidxtmheyyqqaslqxyaerfxxksskolgipxzdbishtvxtmdhwzdcirpfdnlgvtnwtezfgbzlkdxqzrhdupivipska"
				  "fgyxagnjzuqjsliuzyapxddsvxyypavfhtntfweqgidxtmheyyqqaslqxyaerfxxksskolgipxzdbishtvxtmdhwzdcirpfdnlgvtnwtezfgbzlkdxqzrhdupivipska";
	size_t enqbytes=cbfifo_enqueue(values,strlen(values)+3,&RX);
	if(enqbytes==expenqbytes)
		return 1;
	else
		return 0;
}

/************************Test 6: Combination of enqueuing and dequeuing*******************************
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 *****************************************************************************************************/
int cbfifo_test6(void)
{
	int i;
	size_t enqbytes,deqbytes,expenqbytes=20,expdeqbytes=20;
	char deqdata[20];
	char expdeqdata[]="fgyxagnjzuqjsliuzyap";
	char values[]="IamdoingPESassignment6testcases";
	deqbytes=cbfifo_dequeue(deqdata,20,&RX);
	enqbytes=cbfifo_enqueue(values,30,&RX);
	for(i=0;i<20;i++)
	{
		if(deqdata[i]!=expdeqdata[i])
			return 0;
	}
	if ((deqbytes==expdeqbytes)&&(enqbytes==expenqbytes))
		return 1;
	else
		return 0;
}

/***************Test 7: When nbytes=0 is passed in enqueue and dequeue function*************************
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 *******************************************************************************************************/
int cbfifo_test7(void)
{
	char values[]="IamdoingPESassignment6testcases";
	char deqdata[20];
	if ((cbfifo_enqueue(values,0,&RX)==0)&&(cbfifo_dequeue(deqdata,0,&RX)==0))
		return 1;
	else
		return 0;
}

/*********Test 8: Checking if length is correctly returned after performing dequeue operation***********
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 ******************************************************************************************************/
int cbfifo_test8(void)
{
	size_t callength,explength=156;
	char deqdata[100];
	cbfifo_dequeue(deqdata,100,&RX);
	callength=cbfifo_length(&RX);
	if(callength==explength)
		return 1;
	else
		return 0;
}

/**************************Test 9: Passing integer as input*********************************************
* Parameters Passed: None
* Parameters returned: 1 on success and 0 on failure
*******************************************************************************************************/
int cbfifo_test9(void)
{
	size_t enqbytes,expenqbytes=4;
	int x=310;
	enqbytes=cbfifo_enqueue(&x,4,&RX);
	if(enqbytes==expenqbytes)
		return 1;
	else
		return 0;
}

/*************************************Overall test function********************************/
void test_cbfifo(void)
{
	printf("\r\nTesting CBFIFO");
	cbfifo_init(&RX);
	int returnvalue1=cbfifo_test1();
	int returnvalue2=cbfifo_test2();
	int returnvalue3=cbfifo_test3();
	int returnvalue4=cbfifo_test4();
	int returnvalue5=cbfifo_test5();
	int returnvalue6=cbfifo_test6();
	int returnvalue7=cbfifo_test7();
	int returnvalue8=cbfifo_test8();
	int returnvalue9=cbfifo_test9();
	int finalreturn=returnvalue1+returnvalue2+returnvalue3+returnvalue4+returnvalue5
					+returnvalue6+returnvalue7+returnvalue8+returnvalue9;
	if(finalreturn==9)
		printf("\r\nSuccess: All the CBFIFO test cases have passed\r\n");
	else
		printf("\r\nFailure: CBFIFO test cases failed\r\n");
}

/******************************************CBFIFO TEST CASES END*******************************************************************/

/******************************************ACCELROMETER TEST CASE START***********************************************************/
void test_accelerometer(void)
{
	printf("\r\nTesting accelerometer");
	if (!init_mma()) //init mma peripheral
	{
		TPM2->CONTROLS[0].CnV=255;
		printf("\r\nFailure: Accelerometer initialization test failed\r\n");
	}
	else
		printf("\r\nSuccess: Accelerometer initialization test passed\r\n");
}
/********************************************ACCELROMETER TEST CASE END***********************************************************/

/*********************************************SYSTICK TEST CASE START*************************************************************/
void test_systick(void)
{
	printf("\r\nTesting if systick timer crosses 1 second");
	reset_timer();
	while(get_timer()<16); //Wait for 1 second to elapse
	if (get_timer()*63.5>=1000) //Check if time is greater or equal to 1 sec
		printf("\r\nSuccess: Systick timer test case passed\r\n");
	else
		printf("\r\nFailure: Systick timer test cases failed\r\n");
}
/******************************************SYSTICK TEST CASE END***************************************************************/

/********************************************TSI TEST CASE START***************************************************************/
void test_TSI(void)
{
	int Touch_Val=0,success=0,count=0;
	printf("\r\nTesting the TSI. Kindly touch the TSI one time within 10 seconds. If sense detected, Green LED should glow and message should be displayed");
	while(Touch_Val<5 && count<=40)
	{
		reset_timer();
		while(get_timer()<=4); //Provide slight delay for detection
		count++;
		Touch_Val=Touch_Scan_LH();
		if(Touch_Val>5) //If touch is detected
		{
			TPM2->CONTROLS[1].CnV=255;
			printf("\r\nSuccess: Touch detected. TSI test case passed\r\n");
			success=1;
			reset_timer();
			while(get_timer()<8);
			TPM2->CONTROLS[1].CnV=0;
		}
	}
	if(success==0)
		printf("\r\nFailure: Touch not detected within 10 seconds. TSI test case failed\r\n");
}

/*
 *  Filename: Command_Processor.c
 *  Created on: Dec 06, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This C file contains the definitions of functions related to command processor
 *  Reference: PES Assignment 6
 */

#include <MKL25Z4.H>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "Command_Processor.h"
#include "accelerometer.h"
#include "TSI.h"
#include "Systick.h"

//Macros used for lexical analysis
#define ARGV_MAX_SIZE (10)
#define SPACE (32)
#define BUFFER_SIZE (1000)
#define INIT_VAL (-1)
#define PI_DEGREE (180)

//Macro used in handler functions
#define MULTIPLE (5)
#define PI_BTWO_DEGREE (90)

//Macro used for position tracking function
#define	CAL (0)
#define ROLL (1)
#define PITCH (2)
#define TOUCH_MIN (5)
#define DELAY_250MS (4)
#define DELAY_500MS (8)
#define DELAY_5S (5)
#define FULL_INTENSITY (255)
#define OFFSET (5)
#define TRANSITION (16)

//Variable to store the reference zero degree angle
float roll_calibr =0;

typedef int (*command_handler_t)(int, char *argv[]); //Function pointer

typedef struct
{
	const char *name;
	command_handler_t handler;
	const char *help_string;
} command_table_t;

//Declared as it needs to be used by the handler table
int help(int argc, char *argv[]);


/************************Function handler for roll angle setpoint command*************************
 * Function handler when command entered by user is "roll_sp".Once the roll angle reaches to this
 * setpoint, LED will transition smoothly from red to green and will blink green for 5 seconds.
 * Parameter passed: The number of arguments: argc and address of the arguments: argv[]
 * Parameters returned: roll_sp: roll angle setpoint if its within the range or 0 if not
 *************************************************************************************************/
int roll_sp(int argc, char *argv[])
{
	int roll_sp;
	//Convert setpoint to decimal
	roll_sp=atoi(argv[1]);
	//If setpoint in range and multiple of 5
	if((roll_sp> 0 && roll_sp<=PI_DEGREE-roll_calibr) && roll_sp%MULTIPLE==0)
		return roll_sp;
	else
		printf("\r\nInvalid roll angle setpoint\r\n");
	return 0;
}


/************************Function handler for pitch angle setpoint command*************************
 * Function handler when command entered by user is "pitch_sp".Once the pitch angle reaches to this
 * setpoint, LED will will blink blue for 5 seconds.
 * Parameter passed: The number of arguments: argc and address of the arguments: argv[]
 * Parameters returned: pitch_sp: pitch angle setpoint if its within the range or 0 if not
 *************************************************************************************************/
int pitch_sp(int argc, char *argv[])
{
	int pitch_sp;
	//Convert SP to decimal
	pitch_sp=atoi(argv[1]);
	if((pitch_sp>0 && pitch_sp< PI_BTWO_DEGREE) && pitch_sp%MULTIPLE==0)
		return pitch_sp;
	else
		printf("\r\nInvalid pitch angle setpoint\r\n");
	return 0;
}


/************************Function handler for calibrating roll angle******************************
 * Function handler when command entered by user is "cal_roll". The user then can touch TSI at
 * any roll angle which would then be the reference zero angle.
 * Parameter passed: The number of arguments: argc and address of the arguments: argv[]
 * Parameters returned: 1 after operation is completed
 *************************************************************************************************/
int calibrate_roll(int argc, char *argv[])
{
	roll_calibr=0.0;
	TPM2->CONTROLS[0].CnV=0;
	return 1;
}


/************************Function handler for resetting the calibration angle**********************
 * Function handler when command entered by user is "cal_reset". The reference gets set to zero degrees.
 * Parameter passed: The number of arguments: argc and address of the arguments: argv[]
 * Parameters returned: 1 after operation is completed
 *************************************************************************************************/
int calibrate_reset(int argc, char *argv[])
{
	roll_calibr=0.0;
	printf("\r\nThe roll reference angle is reset to zero degrees\r\n");
	return 1;
}


/************************Function handler for displaying the angles*******************************
 * Function handler when command entered by user is "disp_angles". Roll, pitch and refrence/calibration
 * angles are displayed
 * Parameter passed: The number of arguments: argc and address of the arguments: argv[]
 * Parameters returned: 1 after operation is completed
 *************************************************************************************************/
int display_angles(int argc, char *argv[])
{
	read_full_xyz();
	float roll=convert_xyz_to_roll();
	float pitch=convert_xyz_to_pitch();
	printf("\r\nThe roll angle in degrees is: %f",fabs(roll));
	printf("\r\nThe pitch angle in degrees is: %f",fabs(pitch));
	printf("\r\nThe current calibration roll angle in degrees is: %f\r\n",roll_calibr);
	return 1;
}


//Array of structures with each element corresponding to the command, handler and help string
static const command_table_t commands[] = {

		{"Cal_Roll",calibrate_roll,"\r\nPosition Tracking System Command Processor\r\nCal_Roll\r\n"
		"           Place the object at a roll angle and touch TSI to set that angle as reference zero degree\r\n"
		"           The LED will glow RED when TSI is detected"},

		{"Roll_SP",roll_sp,"\r\nRoll_SP <set point>\r\n           Enter the setpoint in degrees for desired roll angle (0-180) in multiples of 5\r\n"},

		{"Pitch_SP",pitch_sp,"\r\nPitch_SP <set_point>\r\n           Enter the setpoint in degrees for desired pitch angle (0-85) in multiples of 5\r\n"},

		{"Disp_Angles",display_angles,"\r\nDisp_Angles\r\n           Displays the roll, pitch and calibration angle\r\n"},

		{"Cal_Reset",calibrate_reset,"\r\nCal_Reset\r\n           Reset the roll calibration angle to zero\r\n"},

		{"Help",help,"\r\nHelp\r\n           Print this help message\r\n"}
};


//Number of commands
static const int num_commands = sizeof(commands) / sizeof(command_table_t);


/************************Function handler for help*************************************************
 * Function handler when command entered by user is "help". Help info regarding all the commands
 * is displayed.
 * Parameter passed: The number of arguments: argc and address of the arguments: argv[]
 * Parameters returned: 1 after operation is completed
 *************************************************************************************************/
int help(int argc, char *argv[])
{
	for (int i=0; i < num_commands; i++) //Run loop till number of commands
		printf("%s",commands[i].help_string); //Print the help string for each command
	return 1;
}


/************************Position tracking system function*****************************************
 * Based on the command entered by user and the values returned by the handlers, the appropriate
 * action is taken
 * Parameter passed: value: value returned by the handler, command: command entered by user
 * Parameters returned: None
 *************************************************************************************************/
void position_tracking(int value, int command)
{
	//Variables for detecting touch on TSI
	int Touch_flag=0;
	int Touch_Val=0;
	float roll,pitch;
	if(value!=0)
	{
		if(command==CAL) //If command is to calibrate roll angle
		{
			printf("\r\nMove the object in roll angle and press TSI when angle to be reference zero degrees is needed");
			//Loop to detect when the touch occurs
			while(Touch_flag==0)
			{
				reset_timer();
				while(get_timer()<DELAY_250MS); //Provide slight delay for detection
				Touch_Val=Touch_Scan_LH();
				if(Touch_Val>TOUCH_MIN) //If touch is detected
				{
					read_full_xyz();
					roll=convert_xyz_to_roll();
					roll_calibr=fabs(roll); //Store the current roll angle
					TPM2->CONTROLS[0].CnV=FULL_INTENSITY; //Glow red LED
					printf("\r\nThe reference angle is set to %f degrees\r\n",roll_calibr);
					reset_timer();
					while(get_timer()<DELAY_250MS);
					TPM2->CONTROLS[0].CnV=0; ////Turn off red LED
					Touch_flag=1;
				}
			}
		}

		else if(command==ROLL) //If command is to provide roll angle setpoint
		{
			printf("\r\nMove the object in roll angle and once roll setpoint reaches, green LED will blink");
			read_full_xyz();
			roll=convert_xyz_to_roll();
			if(fabs(roll)>=roll_calibr) //If calibration angle is detected, turn on red LED
				TPM2->CONTROLS[0].CnV=FULL_INTENSITY;
			while(fabs(roll)<roll_calibr) //Loop till calibration angle is found
			{
				read_full_xyz();
				roll=convert_xyz_to_roll();
				if(fabs(roll)>=roll_calibr) //If calibration angle is detected, turn on red LED
					TPM2->CONTROLS[0].CnV=FULL_INTENSITY;
			}
			while(fabs(roll)-roll_calibr<=value+OFFSET) //Till roll angle setpoint has reached, remain in this loop
			{
				read_full_xyz();
				roll=convert_xyz_to_roll();
				if(fabs(roll)-roll_calibr>=value && fabs(roll)-roll_calibr<=value+OFFSET) //If setpoint reached
				{
					printf("\r\nThe roll angle setpoint has reached\r\n");
					reset_timer();
					int store=get_timer();
					TPM2->CONTROLS[0].CnV=FULL_INTENSITY;
					while(get_timer()<TRANSITION && roll_calibr!=0) //Loop to transition smoothly from red to green through PWM
					{
						if(get_timer()-store>0)
						{
							TPM2->CONTROLS[0].CnV-=TRANSITION;
							TPM2->CONTROLS[1].CnV+=TRANSITION;
							store = get_timer();
						}
					}
					Delay(1);
					TPM2->CONTROLS[0].CnV=0;
					//Blink green LED for 5 seconds
					for(int i=0;i<DELAY_5S;i++)
					{
						reset_timer();
						while(get_timer()<DELAY_500MS)
							TPM2->CONTROLS[1].CnV=FULL_INTENSITY;
						reset_timer();
						while(get_timer()<DELAY_500MS)
							TPM2->CONTROLS[1].CnV=0;
					}
					break;
				}
			}
		}

		else if(command==PITCH) //If command is to provide pitch angle setpoint
		{
			printf("\r\nMove the object in pitch angle and once pitch setpoint reaches, blue LED will blink");
			read_full_xyz();
			pitch=convert_xyz_to_pitch();
			while(fabs(pitch)<=value+OFFSET)  //Till pitch angle setpoint has reached, remain in this loop
			{
				read_full_xyz();
				pitch=convert_xyz_to_pitch();
				if(fabs(pitch)>=value && fabs(pitch)<=value+OFFSET)  //If setpoint reached
				{
					printf("\r\nThe pitch angle setpoint has reached\r\n");
					TPM2->CONTROLS[1].CnV=0;
					TPM2->CONTROLS[0].CnV=0;
					//Blink blue LED for 5 seconds
					for(int i=0;i<DELAY_5S;i++)
					{
						reset_timer();
						while(get_timer()<DELAY_500MS)
							TPM0->CONTROLS[1].CnV=FULL_INTENSITY;
						reset_timer();
						while(get_timer()<DELAY_500MS)
							TPM0->CONTROLS[1].CnV=0;
					}
					break;
				}
			}
		}
	}
}


/**************Process Command Function for Lexical analysis and calling handlers******************
 * Function performs lexical analysis on the input string and calls the appropriate handler based on
 * the command received. After analysis, calls the position tracking function
 * Parameter passed: The input string: input
 * Parameters returned: None
 **************************************************************************************************/
void process_command(char *input)
{
	char *p = input;
	char *end;
	char store_prev=*input; //Use this variable to store the previous value in input string
	bool handler_status = false; //Used to check if a valid command is received or not
	char *argv[ARGV_MAX_SIZE]; //Array of character pointers to store address of arguments
	int argc = 0; //Variable to store number of arguments
	int value; //To store value received from handler
	int command; //To store the command i.e. the handler called
	memset(argv, 0, sizeof(argv));

	for (end = input; *end != '\0'; end++); // find end of string

	//Lexical Analysis
	for (p = input; p<end; p++)
	{
		if(*p>SPACE) //If the value is above space in ascii table
		{
			//If the previous character is  first address or space or null
			if(p==input || store_prev==SPACE || store_prev=='\0')
			{
				argv[argc]=p; //Store the start address of argument
				argc++; //Increment number of argument
			}
		}
		//If current value is space and previous value not space or null
		if(*p==SPACE && (store_prev!=SPACE || store_prev!='\0'))
		{
			*p='\0'; //Force to null
		}
		store_prev=*p; //Store previous byte in string
	}
	argv[argc] = NULL;
	if (argc == 0) // no command
		return;

	//Call the handlers based on the commands received
	for (int i=0; i < num_commands; i++)
	{
		if (strcasecmp(argv[0], commands[i].name) == 0) //For valid commands
		{
			value=commands[i].handler(argc, argv); //Call the appropriate handler function
			command=i;
			handler_status=true;
			break;
		}
	}

	//If invalid command
	if(handler_status==false)
	{
		char *i;
		printf("\r\nUnknown Command:");
		for(i=argv[0];*i!='\0';i++) //To print the invalid command
			printf("%c",*i);
		printf("\r\n");
	}
	//After lexical analysis, call the position tracking function
	position_tracking(value,command);
}


/*****************************Main Command Processor function*********************************/
void command_processor(void)
{
	char buffer[BUFFER_SIZE]; //Used to store the string containing various arguments
	memset(buffer,0,BUFFER_SIZE);
	int i=INIT_VAL;
	printf("\r\nWelcome to position tracking system!\r\n");
	while(1)
	{
		printf("\r\n? ");

		//Accumulating line
		while(buffer[i]!='\r')
		{
			i++;
			buffer[i]= getchar(); //Store the character received from user

			//To handle backspace
			if((buffer[i] == '\b') && (i > 0))
			{
				buffer[i] =' ';
				i--;
				buffer[i]=' ';
				i--;
				printf(" \b");
			}
		}
		buffer[i]='\0'; //Null terminate the string
		process_command(buffer); //Pass the string to the process command function
		//Reset the variables before processing new string
		memset(buffer,0,BUFFER_SIZE);
		i=INIT_VAL;
	}
}

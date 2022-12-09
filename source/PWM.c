/*
 * Filename: PWM.c
 * Created on: Dec 06, 2022
 * Author: Shashank Chandrasekaran
 * Description: This C file contains the definition of PWM initialization function
 * Reference: PES Assignment 4
 */

#include <MKL25Z4.H>
#include "PWM.h"

#define RED_LED_POS (18) //Defining PTB18 for Red LED
#define GREEN_LED_POS (19) //Defining PTB19 for Green LED
#define BLUE_LED_POS (1)  //Defining PTD1 for Blue LED


/**********************Initializing PWM channels for RGB LEDs*****************************/
void Init_RGB_LED_PWM (uint16_t period)
{
SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK| //Clock for red and green LED ports enabled
		      SIM_SCGC5_PORTD_MASK; //Clock for blue LED port enabled

SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK|SIM_SCGC6_TPM2_MASK; //TPM0 and TPM2 Clock enabled

PORTB->PCR [RED_LED_POS] &=~PORT_PCR_MUX_MASK;
PORTB->PCR [RED_LED_POS]|=PORT_PCR_MUX(3); //Red LED port MUX configured to ALT 3

PORTB->PCR [GREEN_LED_POS] &=~PORT_PCR_MUX_MASK;
PORTB->PCR [GREEN_LED_POS]|=PORT_PCR_MUX(3); //Green LED port MUX configured to ALT 3

PORTD->PCR [BLUE_LED_POS] &=~PORT_PCR_MUX_MASK;
PORTD->PCR [BLUE_LED_POS]|=PORT_PCR_MUX(4); //Blue LED port MUX configured to ALT 4

//Clock source for TPM set to 48 MHz
SIM->SOPT2 |= (SIM_SOPT2_TPMSRC (1) | SIM_SOPT2_PLLFLLSEL_MASK);

TPM0->MOD=period-1; //Load MOD for TPM0 channels connected to blue LED
TPM2->MOD=period-1; //Load MOD for TPM2 channels connected to red and green LED

TPM0->SC= TPM_SC_PS(1); //Divide by 2 prescaler for TPM0 with count direction up
TPM2->SC= TPM_SC_PS(1); //Divide by 2 prescaler for TPM2 with count direction up

//Continue operation in debug mode for TPM0 and TPM2
TPM0->CONF|=TPM_CONF_DBGMODE(3);
TPM2->CONF|=TPM_CONF_DBGMODE(3);

TPM0->CONTROLS[1]. CnSC= TPM_CnSC_MSB_MASK| //edge aligned low true Blue LED port
		                 TPM_CnSC_ELSA_MASK;
TPM0->CONTROLS [1].CnV=0; //Set initial duty cycle
TPM0->SC |=TPM_SC_CMOD(1); //Start TPM0

TPM2->CONTROLS[0]. CnSC= TPM_CnSC_MSB_MASK| //edge aligned low true Red LED port
		                 TPM_CnSC_ELSA_MASK;
TPM2->CONTROLS [0].CnV=0; //initial duty cycle
TPM2->CONTROLS[1]. CnSC= TPM_CnSC_MSB_MASK| //edge aligned low true Green LED port
		                 TPM_CnSC_ELSA_MASK;
TPM2->CONTROLS [1].CnV=0; //Set initial duty cycle
TPM2->SC |=TPM_SC_CMOD(1); //Start TPM2
}



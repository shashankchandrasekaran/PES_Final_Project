/*
 * Filename: i2c.c
 * Created on: Dec 06, 2022
 * Author: Shashank Chandrasekaran
 * Description: This C file contains the definition of i2c functions
 * Reference: https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8/I2C-Demo/src/i2c.c
 */

#include <MKL25Z4.H>
#include "i2c.h"

#define I2C_M_START 	I2C0->C1 |= I2C_C1_MST_MASK      //To generate start bit
#define I2C_M_STOP  	I2C0->C1 &= ~I2C_C1_MST_MASK     //To generate stop bit
#define I2C_M_RSTART 	I2C0->C1 |= I2C_C1_RSTA_MASK     // To generate repeated start bit

#define I2C_TRAN		I2C0->C1 |= I2C_C1_TX_MASK   //Transmit mode
#define I2C_REC			I2C0->C1 &= ~I2C_C1_TX_MASK  //Receive mode

#define I2C_WAIT 		i2c_wait();  //wait for completion of transfer

#define NACK 	        I2C0->C1 |= I2C_C1_TXAK_MASK  // Acknowledgment from slave
#define ACK             I2C0->C1 &= ~I2C_C1_TXAK_MASK  //No acknowledgment from master

#define LOCK_DETECT_LIM (200) //Limit lock_detect to 200

//For busy wait logic
int lock_detect=0;
int i2c_lock=0;


/**********************Initializing the i2c*****************************/
void i2c_init(void)
{
	//clock i2c peripheral and port E
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);

	//set pins to I2C function
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);

	//set to 100k baud
	//baud = bus freq/(scl_div+mul)
	//~400k = 24M/(64); icr=0x12 sets scl_div to 64
	I2C0->F = (I2C_F_ICR(0x10) | I2C_F_MULT(0));

	//enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);

	//Select high drive mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
}


/**********************i2c_ busy function*****************************
 * To check if the i2C bus is busy transmitting or receiving data
 * Parameter passed: None
 * Parameter returned: None
 *********************************************************************/
void i2c_busy(void)
{
	// Start Signal
	lock_detect=0;
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C_TRAN;
	I2C_M_START;
	I2C0->C1 |=  I2C_C1_IICEN_MASK;
	// Write to clear line
	I2C0->C1 |= I2C_C1_MST_MASK; /* set MASTER mode */
	I2C0->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C0->D = 0xFF;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0U) {
	} /* wait interrupt */
	I2C0->S |= I2C_S_IICIF_MASK; /* clear interrupt bit */

	/* Clear arbitration error flag*/
	I2C0->S |= I2C_S_ARBL_MASK;

	/* Send start */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C0->C1 |= I2C_C1_MST_MASK; /* START signal generated */

	I2C0->C1 |= I2C_C1_IICEN_MASK;
	/*Wait until start is send*/

	/* Send stop */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_MST_MASK; /* set SLAVE mode */
	I2C0->C1 &= ~I2C_C1_TX_MASK; /* Set Rx */
	I2C0->C1 |= I2C_C1_IICEN_MASK;

	/* wait */
	/* Clear arbitration error & interrupt flag*/
	I2C0->S |= I2C_S_IICIF_MASK;
	I2C0->S |= I2C_S_ARBL_MASK;
	lock_detect=0;
	i2c_lock=1;
}


/**********************i2c_ wait function*****************************
 * To wait if the i2c bus is busy
 * Parameter passed: None
 * Parameter returned: None
 *********************************************************************/
void i2c_wait(void)
{
	lock_detect = 0;
	while(((I2C0->S & I2C_S_IICIF_MASK)==0) & (lock_detect < LOCK_DETECT_LIM))
	{
		lock_detect++;
	}
	if (lock_detect >= LOCK_DETECT_LIM)
		i2c_busy();
	I2C0->S |= I2C_S_IICIF_MASK;
}


/**************************Send start bit********************************/
void i2c_start()
{
	I2C_TRAN;	//set to transmit mode
	I2C_M_START; //send start
}


/****************Send  device and register addresses*********************/
void i2c_read_setup(uint8_t dev, uint8_t address)
{
	I2C0->D = dev; //send dev address
	I2C_WAIT	 //wait for completion

	I2C0->D =  address;	//send read address
	I2C_WAIT

	I2C_M_RSTART;  //repeated start
	I2C0->D = (dev|0x1);  //send dev address (read)
	I2C_WAIT

	I2C_REC;  //set to receive mode
}


/********************Read repeated bytes and ack/nack as appropriate************/
uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	uint8_t data;
	lock_detect = 0;

	if(isLastRead)
		NACK;  //set NACK after read
	else
		ACK;  //ACK after read

	data = I2C0->D;	//dummy read
	I2C_WAIT  //wait for completion

	if(isLastRead)
		I2C_M_STOP;	//send stop
	data = I2C0->D;	//read data

	return  data;
}


/********************Reading a single byte***********************************/
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;

	I2C_TRAN;  //set to transmit mode
	I2C_M_START; //send start
	I2C0->D = dev; //send dev address
	I2C_WAIT  //wait for completion

	I2C0->D =  address;	//send read address
	I2C_WAIT

	I2C_M_RSTART; //repeated start
	I2C0->D = (dev|0x1); //send dev address (read)
	I2C_WAIT

	I2C_REC; //set to recieve mode
	NACK; //set NACK after read

	data = I2C0->D;	//dummy read
	I2C_WAIT

	I2C_M_STOP;	//send stop
	data = I2C0->D;	 //read data

	return data;
}


/********************Writing a single byte***********************************/
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{
	I2C_TRAN; //set to transmit mode
	I2C_M_START; //send start
	I2C0->D = dev; //send dev address
	I2C_WAIT  //wait for ack

	I2C0->D =  address;	//send write address
	I2C_WAIT

	I2C0->D = data;	//send data
	I2C_WAIT
	I2C_M_STOP; //send stop bit
}



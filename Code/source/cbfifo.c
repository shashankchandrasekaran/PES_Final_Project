/*
 *  Filename: cbfifo.c
 *  Created on: Dec 06, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This C file contains the definitions of CBFIFO functions
 *  Reference: PES Assignment 6
 */

#include <MKL25Z4.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cbfifo.h"

#define RW_INIT (-1) //Initializing value of read and write
#define FAIL (-1) //Fail indication


/****************************Function to initialize CBFIFO************************************/
void cbfifo_init(Q_T * q)
{
	q->write= RW_INIT;
	q->read= RW_INIT;
	q->length= 0;
	memset(q->qdata,0,Q_MAX_SIZE);
}


/******************************Function to check if CBFIFO is empty****************************/
int cbfifo_empty(Q_T * q)
{
	return q->length == 0;
}


/******************************Function to check if CBFIFO is full*****************************/
int cbfifo_full(Q_T * q)
{
	return q->length == Q_MAX_SIZE;
}


/****************************************CBFIFO Enqueue function******************************/
size_t cbfifo_enqueue(void *buf, size_t nbyte,Q_T *q)
{
	int store=0,enqbytes=0,temp=0,i=0;
	char *input=buf; //Character pointer created to point to buf
	uint32_t masking_state;

	// save current masking state
	masking_state = __get_PRIMASK();
	// disable interrupts
	__disable_irq();

	if(input==NULL) //If the buffer is NULL, error condition
	{
		__set_PRIMASK(masking_state); //Enable the interrupts again before returning
		return FAIL;
	}
	else if((q->write==RW_INIT)&&(q->read==RW_INIT)) //If no bytes is inserted, insert the first byte of data
	{
		temp=RW_INIT; //variable used for enqueuing 1st byte in array
		q->write=q->read=0; //read and write now at 0th index of array
		q->qdata[0]=*input; //First byte enqueued in first element of array
		input++;
		q->length++;
	}
	if (q->length==Q_MAX_SIZE||nbyte==0) //If the array is full or nbyte requested to enqueue is zero
	{
		__set_PRIMASK(masking_state);
		return 0; //Indicate no bytes enqueued
	}
	else if(nbyte<=(Q_MAX_SIZE-q->length)) //If bytes to enqueue is less or equal to available elements in array
	{
		if(temp==RW_INIT) //To enqueue remaining bytes after 1st byte was inserted in array when write and read was -1
		{
			for(i=0;i<nbyte-1;i++)
			{
				q->write=(q->write+1)&(Q_MAX_SIZE-1); //To update write in such a way that on last index of array, after incrementing goes to start address
				q->qdata[q->write]=*input; //1 byte inserted in array
				input++;
				q->length++;
			}
			temp=0; //Temp changed to zero as now the array has element
			__set_PRIMASK(masking_state);
			return nbyte; //Returns number of bytes enqueued
		}
		else //To enqueue nbytes when enqueue is not called first time and above if condition is met
		{
			for(i=0;i<nbyte;i++)
			{
				q->write=(q->write+1)&(Q_MAX_SIZE-1);
				q->qdata[q->write]=*input;
				input++;
				q->length++;
			}
			__set_PRIMASK(masking_state);
			return nbyte;
		}
	}
	else if(nbyte>(Q_MAX_SIZE-q->length)) //When bytes to enqueue is more than the available space in array
	{
		if(temp==-1) //To enqueue remaining bytes after 1st byte was inserted in array when write and read was -1
		{
			store=Q_MAX_SIZE-q->length-1;
			for(i=0;i<store;i++)
			{
				q->write=(q->write+1)&(Q_MAX_SIZE-1);
				q->qdata[q->write]=*input;
				input++;
				q->length++;
				enqbytes++;
			}
			temp=0;
			__set_PRIMASK(masking_state);
			return enqbytes;
		}
		else //To enqueue nbytes when enqueue is not called first time and above if condition is met
		{
			store=Q_MAX_SIZE-q->length;
			for(i=0;i<store;i++)
			{
				q->write=(q->write+1)&(Q_MAX_SIZE-1);
				q->qdata[q->write]=*input;
				input++;
				q->length++;
				enqbytes++;
			}
			__set_PRIMASK(masking_state);
			return enqbytes;
		}
	}
	else
	{
		__set_PRIMASK(masking_state);
		return 0; //If no conditions above met, enqueue operation is not performed
	}
}


/****************************************CBFIFO Dequeue function************************************/
size_t cbfifo_dequeue(void *buf, size_t nbyte, Q_T *q)
{
	int i;
	size_t deqbyte; //Variable is to store the number of bytes dequeued
	char *output=buf; //Character pointer created to point to buf
	uint32_t masking_state;

	// save current masking state
	masking_state = __get_PRIMASK();
	// disable interrupts
	__disable_irq();

	if(((q->write==RW_INIT)&&(q->read==RW_INIT))||q->length==0||buf==NULL||nbyte==0) //If the array is empty , return that 0 bytes were dequeued
	{
		__set_PRIMASK(masking_state);
		return 0;
	}
	else if (nbyte>q->length) //If byte to dequeue is more than length, dequeue all bytes available and return that
	{
		deqbyte=q->length; //Since array contains length elements, all are dequeued
		for(i=0;i<deqbyte;i++)
		{
			*output=q->qdata[q->read]; //Dequeued data stored
			q->read=(q->read+1)&(Q_MAX_SIZE-1); //Read updated to next array index
			q->length--; //Length reduced
			output++;
		}
		__set_PRIMASK(masking_state);
		return deqbyte; //Return bytes dequeued
	}
	else if (nbyte<=q->length) //If byte to dequeue is less than or equal to length, dequeue those bytes
	{
		for(i=0;i<nbyte;i++)
		{
			*output=q->qdata[q->read];
			q->read=(q->read+1)&(Q_MAX_SIZE-1);
			q->length--;
			output++;
		}

		__set_PRIMASK(masking_state);
		return nbyte; //Returns bytes dequeued
	}
	else
	{__set_PRIMASK(masking_state);
	return 0; //If no conditions above met, dequeue operation is not performed
	}
}


/***************************************CBFIFO Length function***************************************/
size_t cbfifo_length(Q_T *q)
{
	return q->length; //No. of elements in array returned
}





/*
 *  Filename: cbfifo.h
 *  Created on: Dec 06, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This H file contains the declaration of CBFIFO functions
 *  Reference: PES Assignment 6
 */


#ifndef _CBFIFO_H_
#define _CBFIFO_H_

#include <stdlib.h>  // for size_t

#define Q_MAX_SIZE (256) //Maximum size of buffer


//Structure defined for transmit and receive buffer
typedef struct{
	int write;
	int read;
	size_t length;
	char qdata[Q_MAX_SIZE];
}Q_T;


/*
 * Initializes the FIFO
 *
 * Parameters:
 *	 *q       The buffer which needs to be enqueued (TxQ or RxQ)
 *
 * Returns: None
 */
void cbfifo_init(Q_T * q);


/*
 * Checks if the FIFO is empty
 *
 * Parameters:
 *	 *q       The buffer which needs to be enqueued (TxQ or RxQ)
 *
 * Returns:
 *   1 if the FIFO is empty and 0 if it is not
 */
int cbfifo_empty(Q_T * q);


/*
 * Checks if the FIFO is full
 *
 * Parameters:
 *	 *q       The buffer which needs to be enqueued (TxQ or RxQ)
 *
 * Returns:
 *   1 if the FIFO is full and 0 if it is not
 */
int cbfifo_full(Q_T * q);


/*
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   buf      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 *	 *q       The buffer which needs to be enqueued (TxQ or RxQ)
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns (size_t) -1.
 */
size_t cbfifo_enqueue(void *buf, size_t nbyte, Q_T *q);


/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buf      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 *   *q       The buffer which needs to be dequeued (TxQ or RxQ)
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 */
size_t cbfifo_dequeue(void *buf, size_t nbyte, Q_T *q);


/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   *q       Buffer (TxQ or RxQ)
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(Q_T *q);

#endif // _CBFIFO_H_

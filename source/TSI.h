/*
 *  File name: TSI.h
 *  Created on: Dec 06, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This H file contains TSI initialization and scan function declarations
 *  Reference: PES Assignment 3
 */

#ifndef TOUCH_H_
#define TOUCH_H_


/*
 * Function for initialization of capacitive touch slider (TSI)
 * Parameter passed: None
 * Parameters returned: None
 */
void Touch_Init(void);


/*
 * Function to scan/poll value from TSI and detect if there is a touch on the capacitive touch slider.
 * Parameters passed: None
 * Parameters returned: scan-TOUCH_OFFSET: Touch value from TSI removing the offset
 */
int Touch_Scan_LH(void);

#endif /* TOUCH_H_ */

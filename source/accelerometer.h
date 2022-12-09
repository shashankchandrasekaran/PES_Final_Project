/*
 * Filename: accelerometer.h
 * Created on: Dec 06, 2022
 * Author: Shashank Chandrasekaran
 * Description: This H file contains the declaration of accelerometer (mmA8451) functions
 * Reference: https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8/I2C-Demo/inc/mma8451.h
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <stdint.h>

/*
 * Function for generating delay through for loop
 * Parameter passed: dly: number based on which delay is generated
 * Parameters returned: None
 */
void Delay (uint32_t dly);


/*
 * Function for initialization of mmA8451 accelerometer
 * Parameter passed: None
 * Parameters returned: None
 */
int init_mma(void);


/*
 * Function to read X,Y and Z coordinates from the sensor through i2c i2c_read_setup function
 * Parameter passed: None
 * Parameters returned: None
 */
void read_full_xyz(void);


/*
 * Function to convert the X, Y and Z coordinates to roll angle
 * Parameter passed: None
 * Parameters returned: pitch: Roll angle
 */
float convert_xyz_to_roll(void);


/*
 * Function to convert the Y and Z coordinates to pitch angle
 * Parameter passed: None
 * Parameters returned: pitch: Pitch angle
 */
float convert_xyz_to_pitch(void);

#endif /* ACCELEROMETER_H_ */

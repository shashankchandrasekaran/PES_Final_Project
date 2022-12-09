/*
 * Filename: i2c.h
 * Created on: Dec 06, 2022
 * Author: Shashank Chandrasekaran
 * Description: This H file contains the declaration of i2c functions
 * Reference: https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8/I2C-Demo/inc/i2c.h
 */


#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

/*
 * Function for initialization of i2c
 * Parameter passed: None
 * Parameters returned: None
 */
void i2c_init(void);


/*
 * Function for generating start bit
 * Parameter passed: None
 * Parameters returned: None
 */
void i2c_start(void);


/*
 * Function for transmitting device and read address
 * Parameter passed: dev: Device address, address: read address
 * Parameters returned: None
 */
void i2c_read_setup(uint8_t dev, uint8_t address);


/*
 * Function for reading repeated bytes of data
 * Parameter passed: isLastRead: The last byte to be read
 * Parameters returned: data: Byte received
 */
uint8_t i2c_repeated_read(uint8_t isLastRead);


/*
 * Function for reading a single byte of data
 * Parameter passed: dev: Device address, address: read address
 * Parameters returned: data: Byte received
 */
uint8_t i2c_read_byte(uint8_t dev, uint8_t address);


/*
 * Function for writing a single byte of data
 * Parameter passed: dev: Device address, address: read address,
 * data: byte of data to be written
 * Parameters returned: data: None
 */
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);

#endif /* I2C_H_ */

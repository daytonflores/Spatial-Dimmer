/**
 * @file    i2c.h
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief   Macros and function headers for I2C
 */

#ifndef I2C_H_
#define I2C_H_



/**
 * @brief   Initialize the on-board I2C0
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
void init_onboard_i2c0(void);



/**
 * @brief   Perform set-up required to write byte to on-board I2C0 module
 * @param	device_address - The I2C address of the device to write byte to
 * @param	register_address - The register address on the device to write byte to
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
void i2c0_write_setup(uint8_t device_address, uint8_t register_address);



/**
 * @brief   Write byte to on-board I2C0 module
 * @param	device_address - The I2C address of the device to write byte to
 * @param	register_address - The register address on the device to write byte to
 * @param	data - The byte of data to send
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
void i2c0_write_byte(uint8_t device_address, uint8_t register_address, uint8_t data);



/**
 * @brief   Perform set-up required to read byte from on-board I2C0 module
 * @param	device_address - The I2C address of the device to write byte to
 * @param	register_address - The register address on the device to write byte to
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
void i2c0_read_setup(uint8_t device_address, uint8_t register_address);



/**
 * @brief   Read byte from on-board I2C0 module
 * @param	device_address - The I2C address of the device to write byte to
 * @param	register_address - The register address on the device to write byte to
 * @return	The byte of data received
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
uint8_t i2c0_read_byte(uint8_t device_address, uint8_t register_address);



/**
 * @brief   Repeated read byte from on-board I2C0 module
 * @param	is_final_byte - True only if the byte about to be read is the
 * 			final byte to be read in the sequence, otherwise false
 * @return	The byte of data received
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
uint8_t i2c_repeated_read_byte(bool is_final_byte);



/**
 * @brief   Send Start sequence to on-board I2C0
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
void i2c0_start(void);



/**
 * @brief   Send Stop sequence to on-board I2C0
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
void i2c0_stop(void);



#endif /* I2C_H_ */

/**
 * @file    i2c.c
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief   Function definitions for I2C
 */



/**
 * Include pre-defined libraries
 */
#include <stdbool.h>
#include "board.h"



/**
 * User-defined libraries
 */
#include "i2c.h"



/**
 * @brief	PCR is a 32-bit register where bits 8:10 are a MUX field
 * @detail
 * 		000: Pin disabled (analog)
 * 		001: Alternative
 * 		010: Alternative
 * 		011: TPM0_CH0
 * 		100: Alternative
 * 		101: I2C0_SCL
 * 		110: Alternative
 * 		111: Alternative
 */
#define PCR_MUX_SCL_I2C0\
	(5)



/**
 * @brief	PCR is a 32-bit register where bits 8:10 are a MUX field
 * @detail
 * 		000: Pin disabled (analog)
 * 		001: Alternative
 * 		010: Alternative
 * 		011: TPM0_CH1
 * 		100: Alternative
 * 		101: I2C0_SDA
 * 		110: Alternative
 * 		111: Alternative
 */
#define PCR_MUX_SDA_I2C0\
	(5)



/**
 * @brief	On-board SCL for I2C0 is located at PTE24
 */
#define PORTE_I2C0_SCL_PIN\
	(24)



/**
 * @brief	On-board SDA for I2C0 is located at PTE25
 */
#define PORTE_I2C0_SDA_PIN\
	(25)



/**
 * @brief	Prescales the bus clock for bit rate selection. This field and
 * 			the MULT field determine the I2C baud rate, the SDA hold time,
 * 			the SCL start hold time, and the SCL stop hold time. For a list
 * 			of values corresponding to each ICR setting, see I2C divider
 * 			and hold values
 * @detail
 * 		The SCL divider multiplied by multiplier factor (mul) determines the I2C baud rate
 * 			I2C baud rate = bus speed (Hz)/(mul × SCL divider)
 *
 * 		The SDA hold time is the delay from the falling edge of SCL (I2C clock) to the
 * 		changing of SDA (I2C data)
 * 			SDA hold time = bus period (s) × mul × SDA hold value
 *
 * 		The SCL start hold time is the delay from the falling edge of SDA (I2C data) while
 * 		SCL is high (start condition) to the falling edge of SCL (I2C clock)
 * 			SCL start hold time = bus period (s) × mul × SCL start hold value
 *
 * 		The SCL stop hold time is the delay from the rising edge of SCL (I2C clock) to the
 * 		rising edge of SDA (I2C data) while SCL is high (stop condition)
 * 			SCL stop hold time = bus period (s) × mul × SCL stop hold value
 */
#define F_ICR\
	(17)



/**
 * @brief	The MULT bits define the multiplier factor. This factor is
 * 			used along with the SCL divider to generate the I2C baud rate
 * @detail
 * 		00: x1
 * 		01: x2
 * 		10: x4
 * 		11: Reserved
 */
#define F_MULT\
	(0)



/**
 * @brief	Instruct I2C0 to send start condition
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8
 * 		of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
#define I2C0_START()\
	(I2C0->C1 |= I2C_C1_MST_MASK)



/**
 * @brief	Instruct I2C0 to send stop condition
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8
 * 		of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
#define I2C0_STOP()\
	(I2C0->C1 &= ~I2C_C1_MST_MASK)



/**
 * @brief	Instruct I2C0 to send repeated start condition
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8
 * 		of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
#define I2C0_REPEATED_START()\
	(I2C0->C1 |= I2C_C1_RSTA_MASK)



/**
 * @brief	Set I2C0 to transmit mode
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8
 * 		of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
#define I2C0_TRANSMIT_MODE()\
	(I2C0->C1 |= I2C_C1_TX_MASK)



/**
 * @brief	Set I2C0 to receive mode
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8
 * 		of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
#define I2C0_RECEIVE_MODE()\
	(I2C0->C1 &= ~I2C_C1_TX_MASK)



/**
 * @brief	Waits until I2C0 interrupt pending flag is set. This indicates
 * 			that a byte and acknowledgment have transferred. Once flag is
 * 			checked, this macro will clear that flag (by writing 1 to it)
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8
 * 		of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
#define I2C0_WAIT()\
	do{\
		while(!(I2C0->S & I2C_S_IICIF_MASK));\
		I2C0->S |= I2C_S_IICIF_MASK;\
	}while(0)



/**
 * @brief	Instruct I2C0 to send acknowledgment
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8
 * 		of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
#define I2C0_ACK()\
	(I2C0->C1 &= ~I2C_C1_TXAK_MASK)



/**
 * @brief	Instruct I2C0 to send no acknowledgment
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8
 * 		of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
#define I2C0_NACK()\
	(I2C0->C1 |= I2C_C1_TXAK_MASK)



/**
 * @brief	Standard I2C 7-bit addressing states device address | 0x01
 * 			is the command for read
 */
#define I2C0_READ()\
	(I2C0->D = (device_address | (uint8_t)(1)))



/**
 * @brief	Standard I2C 7-bit addressing states device address | 0x00
 * 			is the command for write
 */
#define I2C0_WRITE()\
	(I2C0->D = (device_address | (uint8_t)(0)))



void init_onboard_i2c0(void){

	/**
	 * Enable clock to I2C0
	 * Enable clock to Port E for I2C0 SCL + SDA pins
	 */
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;



    /**
     * Set PTE24 as SCL for on-board I2C0
     * Set PTE25 as SDA for on-board I2C0
     * The MUX selection in PCR is done with bits 10:8, where 101 is configuration as I2C0 SCL/SDA
     */
	PORTE->PCR[PORTE_I2C0_SCL_PIN] |= PORT_PCR_MUX(PCR_MUX_SCL_I2C0);
	PORTE->PCR[PORTE_I2C0_SDA_PIN] |= PORT_PCR_MUX(PCR_MUX_SDA_I2C0);



	/**
	 * Generate I2C baud rate by configuring:
	 * 	- ICR to 16
	 * 	- MULT to x1
	 */
 	I2C0->F =
 		I2C_F_ICR(F_ICR) |
 		I2C_F_MULT(F_MULT);



 	/**
 	 * Enable I2C and set to master mode
 	 */
	I2C0->C1 |= (I2C_C1_IICEN_MASK);



	/**
	 * Configure I2C0 as:
	 * 	- High Drive (in terms of drive capability of I2C pads)
	 */
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
}



void i2c0_write_setup(uint8_t device_address, uint8_t register_address){

	/**
	 * Send device address and wait for ACK bit
	 */
	I2C0->D = device_address;
	I2C0_WAIT();



	/**
	 * Send register address and wait for ACK bit
	 */
	I2C0->D = register_address;
	I2C0_WAIT();
}



void i2c0_write_byte(uint8_t device_address, uint8_t register_address, uint8_t data){

	/**
	 * Start on-board I2C0
	 */
	i2c0_start();



	/**
	 * Perform required set-up in order to write to on-board I2C0 module
	 */
	i2c0_write_setup(device_address, register_address);



	/**
	 * Send data and wait for ACK bit
	 */
	I2C0->D = data;
	I2C0_WAIT();



	/**
	 * Byte has been written so stop on-board I2C0
	 */
	i2c0_stop();
}



void i2c0_read_setup(uint8_t device_address, uint8_t register_address){

	/**
	 * Send device address and wait for ACK bit
	 */
	I2C0->D = device_address;
	I2C0_WAIT();



	/**
	 * Send register address and wait for ACK bit
	 */
	I2C0->D = register_address;
	I2C0_WAIT();



	/**
	 * Send Repeated Start, read command to device and wait for ACK bit
	 */
	I2C0_REPEATED_START();
	I2C0_READ();
	I2C0_WAIT();



	/**
	 * Configure I2C0 to Receive mode
	 */
	I2C0_RECEIVE_MODE();
}



uint8_t i2c0_read_byte(uint8_t device_address, uint8_t register_address){

	/**
	 * Used to hold dummy data and then valid data afterwards
	 */
	uint8_t data;



	/**
	 * Start on-board I2C0
	 */
	i2c0_start();



	/**
	 * Perform required set-up in order to read from on-board I2C0 module
	 */
	i2c0_read_setup(device_address, register_address);



	/**
	 *
	 */
	I2C0_NACK();



	/**
	 * Read dummy data and then wait for ACK bit
	 */
	data = I2C0->D;
	I2C0_WAIT();



	/**
	 * ACK bit has been detected so stop on-board I2C0
	 */
	i2c0_stop();



	/**
	 * Read valid data and return it
	 */
	data = I2C0->D;
	return data;
}



uint8_t i2c_repeated_read_byte(bool is_final_byte){

	/**
	 * Used to hold dummy data and then valid data afterwards
	 */
	uint8_t data;



	/**
	 * If this is the final byte to be read, set NACK after read
	 * Else, set ACK after read
	 */
	if(is_final_byte)	{
		I2C0_NACK();
	}
	else{
		I2C0_ACK();
	}



	/**
	 * Read dummy data and then wait for ACK bit
	 */
	data = I2C0->D;
	I2C0_WAIT();



	/**
	 * If this is the final byte to be read, stop on-board I2C0
	 */
	if(is_final_byte){
		i2c0_stop();
	}



	/**
	 * Read valid data and return it
	 */
	data = I2C0->D;
	return data;
}



void i2c0_start(void){

	/**
	 * Configure I2C0 to Transmit mode and then send Start sequence
	 */
	I2C0_TRANSMIT_MODE();
	I2C0_START();
}



void i2c0_stop(void){

	/**
	 * Send Stop sequence to I2C0
	 */
	I2C0_STOP();
}

/**
 * @file	mma8451q.c
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief	Function definitions for on-board accelerometer
 */



/**
 * Include pre-defined libraries
 */
#include <stdbool.h>
#include <stdlib.h>
#include "board.h"



/**
 * User-defined libraries
 */
#include "bitops.h"
#include "led.h"		// Keep led.h included before mma8451q.h for led_color_t typedef
#include "mma8451q.h"
#include "i2c.h"
#include "tpm.h"



/**
 * @brief	I2C address for MMA8451Q
 */
#define MMA8451Q_ADDRESS\
	(0x3A)



/**
 * @brief	Address of WHO_AM_I register for MMA8451Q
 */
#define WHO_AM_I_REG\
	(0x0D)



/**
 * @brief	Device ID returned by reading WHO_AM_I register for MMA8451Q
 */
#define DEVICE_ID\
	(0x1A)



/**
 * @brief	Address of X[13:8] data register for MMA8451Q
 */
#define X_HI_REG\
	(0x01)



/**
 * @brief	Address of X[7:0] data register for MMA8451Q
 */
#define X_LO_REG\
	(0x02)



/**
 * @brief	Address of Y[13:8] data register for MMA8451Q
 */
#define Y_HI_REG\
	(0x03)



/**
 * @brief	Address of Y[7:0] data register for MMA8451Q
 */
#define Y_LO_REG\
	(0x04)



/**
 * @brief	Address of Z[13:8] data register for MMA8451Q
 */
#define Z_HI_REG\
	(0x05)



/**
 * @brief	Address of Z[7:0] data register for MMA8451Q
 */
#define Z_LO_REG\
	(0x06)



/**
 * @brief	Address of CTRL1 register for MMA8451Q
 */
#define CTRL1_REG_ADDRESS\
	(0x2A)



/**
 * @brief	CTRL1[0] - Full-scale selection
 * @detail
 * 		0: Standby mode
 * 		1: Active mode
 */
#define CTRL1_ACTIVE\
	(MASK(1UL, 0))



/**
 * @brief	CTRL1[1] - Fast read mode: Data format limited to single byte
 * @detail
 * 		0: Normal read mode
 * 		1: Fast read mode
 */
#define CTRL1_F_READ\
	(MASK(0UL, 1))



/**
 * @brief	CTRL1[1] - Fast read mode: Data format limited to single byte
 * @detail
 * 		0: Normal read mode
 * 		1: Fast read mode
 */
#define CTRL1_F_READ\
	(MASK(0UL, 1))



/**
 * @brief	CTRL1[2] - Reduced noise reduced maximum range mode
 * @detail
 * 		0: Normal mode
 * 		1: Low noise mode
 */
#define CTRL1_LNOISE\
	(MASK(0UL, 2))



/**
 * @brief	CTRL1[5:3] - Data-rate selection
 * @detail
 * 		000: 800.00 Hz (period of   1.25 ms)
 * 		001: 400.00 Hz (period of   2.50 ms)
 * 		010: 200.00 Hz (period of   5.00 ms)
 * 		011: 100.00 Hz (period of  10.00 ms)
 * 		100: 50.00  Hz (period of  20.00 ms)
 * 		101: 12.50  Hz (period of  80.00 ms)
 * 		110:  6.25  Hz (period of 160.00 ms)
 * 		111:  1.56  Hz (period of 640.00 ms)
 */
#define CTRL1_DR\
	(MASK(0, 3))



/**
 * @brief	CTRL1[7:6] - Configures the auto-wake sample frequency when the
 * 			device is in sleep mode
 * @detail
 * 		00: 50.00 Hz
 * 		01: 12.50 Hz
 * 		10:  6.25 Hz
 * 		11:  1.56 Hz
 */
#define CTRL1_ASLP_RATE\
	(MASK(0, 6))



/**
 * @brief	The total amount of possible XYZ values
 */
int xyz_levels = ((XYZ_MAX - XYZ_MIN) + 1);



/**
 * @brief	Current x value of data read from on-board accelerometer
 * 			at 14-bit resolution
 */
int16_t current_x = 0;



/**
 * @brief	Current y value of data read from on-board accelerometer
 * 			at 14-bit resolution
 */
int16_t current_y = 0;



/**
 * @brief	Current z value of data read from on-board accelerometer
 * 			at 14-bit resolution
 */
int16_t current_z = 0;



int init_onboard_accelerometer(void){

	/**
	 * Used to hold CTRL1 configuration during initialization
	 */
	uint8_t data;



	/**
	 * Configure all fields of CTRL1:
	 * 	- Active mode
	 * 	- 14-bit samples
	 * 	- 800 Hz ODR (period of 1.25 ms)
	 */
	data =
		CTRL1_ASLP_RATE |
		CTRL1_DR |
		CTRL1_LNOISE |
		CTRL1_F_READ |
		CTRL1_ACTIVE;



	/**
	 * Configure on-board accelerometer
	 */
	if(i2c0_read_byte(MMA8451Q_ADDRESS, WHO_AM_I_REG) == DEVICE_ID){
		for(int i = 0; i < 1250000; i++);
		i2c0_write_byte(MMA8451Q_ADDRESS, CTRL1_REG_ADDRESS, data);
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}
}



void read_onboard_accelerometer_values(void){

	/**
	 * Used to hold XYZ data bytes from on-board accelerometer
	 */
	uint8_t x_hi_data, x_lo_data;
	uint8_t y_hi_data, y_lo_data;
	uint8_t z_hi_data, z_lo_data;



	/**
	 * Used to hold temp calculated 16-bit values for read XYZ data
	 */
	int16_t x_temp, y_temp, z_temp;



	/**
	 * Start on-board I2C0
	 */
	i2c0_start();



	/**
	 * Perform required set-up in order to read from on-board I2C0 module
	 *
	 * Since we are using repeated read mode, the register address to set up
	 * with will be the lowest memory-mapped data register. This happens to
	 * be X_HI_REG (0x01)
	 */
	i2c0_read_setup(MMA8451Q_ADDRESS, X_HI_REG);



	/**
	 * Read data bytes 1-5 in repeated mode. Order is based on
	 * the memory-mapped addresses of data registers for XYZ
	 *
	 * Read final data byte 6 and end repeated mode
	 */
	x_hi_data = i2c0_repeated_read_byte(false);
	x_lo_data = i2c0_repeated_read_byte(false);
	y_hi_data = i2c0_repeated_read_byte(false);
	y_lo_data = i2c0_repeated_read_byte(false);
	z_hi_data = i2c0_repeated_read_byte(false);
	z_lo_data = i2c0_repeated_read_byte(true);



	/**
	 * Temp calculation for merging all bytes of written data
	 */
	x_temp = (int16_t)((x_hi_data << 8) | (x_lo_data));
	y_temp = (int16_t)((y_hi_data << 8) | (y_lo_data));
	z_temp = (int16_t)((z_hi_data << 8) | (z_lo_data));



	/**
	 * Align the calculated data from 16-bits to 14-bits
	 */
	current_x = (x_temp >> 2);
	current_y = (y_temp >> 2);
	current_z = (z_temp >> 2);
}



void calculate_rgb_from_xyz(accelerometer_axis_t accelerometer_axis, led_color_t led_color){

	/**
	 * Used to hold values for XYZ to RGB calculations
	 */
	int32_t xyz;
	int32_t xyz_offset;
	int32_t rgb;



	/**
	 * Calculate XYZ offset
	 */
	if(XYZ_MIN >= 0){
		xyz_offset = 0;
	}
	else{
		xyz_offset = -(XYZ_MIN);
	}



	/**
	 * Grab the requested XYZ value
	 */
	switch(accelerometer_axis){
	case x:
		xyz = current_x + xyz_offset;
		break;
	case y:
		xyz = current_y + xyz_offset;
		break;
	case z:
		xyz = current_z + xyz_offset;
		break;
	default:
		break;
	}



	/**
	 * Map according to ranges of XYZ and RGB
	 */
	if(rgb_levels == xyz_levels){

	}
	else if(rgb_levels > xyz_levels){

	}
	else if(rgb_levels < xyz_levels){
		rgb = xyz / (xyz_levels / rgb_levels);
	}



	/**
	 * Set the calculated RGB value
	 */
	switch(led_color){
	case red:
		current_red_level = rgb;
		break;
	case green:
		current_green_level = rgb;
		break;
	case blue:
		current_blue_level = rgb;
		break;
	default:
		break;
	}
}

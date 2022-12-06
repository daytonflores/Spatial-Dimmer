/**
 * @file	mma8451q.h
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief	Macros and function headers for on-board accelerometer
 */

#ifndef MMA8451Q_H_
#define MMA8451Q_H_



/**
 * @brief	Used to refer to aspect of on-board accelerometer XYZ
 */
typedef enum accelerometer_axis_e{
	x,
	y,
	z
} accelerometer_axis_t;



/**
 * @brief	Lowest possible value when working with XYZ (14-bit resolution)
 */
#define XYZ_MIN\
	(-4096)



/**
 * @brief	Highest possible value when working with XYZ (14-bit resolution)
 */
#define XYZ_MAX\
	(4095)



/**
 * @brief	Defined in mma8451q.c
 */
extern int xyz_levels;



/**
 * @brief	Defined in mma8451q.c
 */
extern int16_t current_x;



/**
 * @brief	Defined in mma8451q.c
 */
extern int16_t current_y;



/**
 * @brief	Defined in mma8451q.c
 */
extern int16_t current_z;



/**
 * @brief	Initialize the on-board accelerometer
 * @return	EXIT_SUCCESS or EXIT_FAILURE
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
int init_onboard_accelerometer(void);



/**
 * @brief	Read the values from on-board accelerometer
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 8 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 */
void read_onboard_accelerometer_values(void);



/**
 * @brief	Map an XYZ value to RGB level(s)
 * @param	accelerometer_axis - The XYZ value to map from
 * @param	led_color - The LED color(s) to map to
 */
void calculate_rgb_from_xyz(accelerometer_axis_t accelerometer_axis, led_color_t led_color);



#endif /* MMA8451Q_H_ */

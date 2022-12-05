/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */



/**
 * @file	main.c
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief	Application entry point
 */



/**
 * Include pre-defined libraries
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"



/**
 * Include user-defined libraries
 */
#include "bitops.h"
#include "led.h"
#include "tpm.h"
#include "i2c.h"
#include "mma8451q.h"



/*
 * @brief	Application entry point
 */
int main(void){

	/**
	 * Used to check return code for any method calls
	 */
	volatile int return_code;

	/**
	 * Init board hardware
	 */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();



#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL

	/**
	 * Init FSL debug console
	 */
	BOARD_InitDebugConsole();
#endif



	/**
	 * Initialize on-board LEDs
	 */
	init_red_onboard_led(analog);
	init_green_onboard_led(analog);
	init_blue_onboard_led(analog);



	/**
	 * Initialize on-board TPM modules
	 * 	- TPM2 channel 0 connects to red on-board LED
	 * 	- TPM2 channel 1 connects to green on-board LED
	 * 	- TPM0 channel 1 connects to blue on-board LED
	 */
	init_onboard_tpm2(TPM2_RED_LED_CHANNEL, TPM_MOD);
	init_onboard_tpm2(TPM2_GREEN_LED_CHANNEL, TPM_MOD);
	init_onboard_tpm0(TPM0_BLUE_LED_CHANNEL, TPM_MOD);



	/**
	 * Initialize on-board I2C0
	 */
	init_onboard_i2c0();



	/**
	 * Initialize on-board accelerometer
	 */
	return_code = init_onboard_accelerometer();
	if(return_code != EXIT_SUCCESS){
		return(EXIT_FAILURE);
	}



	/**
	 * Enter an infinite loop where accelerometer values will be
	 * read and RGB LED will be set according to XYZ values
	 */
	while(1) {

		/**
		 * Read XYZ values from on-board accelerometer
		 */
		read_onboard_accelerometer_values();
		printf("XYZ = (%d, %d, %d)\r\n", current_x, current_y, current_z);



		/**
		 * Calculate new RGB levels from current XYZ values
		 */
		calculate_rgb_from_xyz(x, red);
		calculate_rgb_from_xyz(y, green);
		calculate_rgb_from_xyz(z, blue);
		printf("RGB = (%d, %d, %d)\r\n\n", current_red_level, current_green_level, current_blue_level);



		/**
		 * Set new RGB levels to physical RGB LED
		 */
		analog_control_onboard_leds(red, analog_set);
		analog_control_onboard_leds(green, analog_set);
		analog_control_onboard_leds(blue, analog_set);



		/**
		 * Delay for testing
		 */
		for(int i = 0; i < 1250000; i++);



		/**
		for(int i = 0; i < 125000; i++);
		if(current_red_level >= RGB_MAX || current_red_level <= RGB_MIN){
			step_red_level = (-step_red_level);
		}
		if(current_green_level >= RGB_MAX || current_green_level <= RGB_MIN){
			step_green_level = (-step_green_level);
		}
		if(current_blue_level >= RGB_MAX || current_blue_level <= RGB_MIN){
			step_blue_level = (-step_blue_level);
		}

		current_red_level += step_red_level;
		current_green_level += step_green_level;
		current_blue_level += step_blue_level;

		if(current_red_level < RGB_MIN){
			current_red_level = RGB_MIN;
		}
		else if(current_red_level > RGB_MAX){
			current_red_level = RGB_MAX;
		}

		if(current_green_level < RGB_MIN){
			current_green_level = RGB_MIN;
		}
		else if(current_green_level > RGB_MAX){
			current_green_level = RGB_MAX;
		}

		if(current_blue_level < RGB_MIN){
			current_blue_level = RGB_MIN;
		}
		else if(current_blue_level > RGB_MAX){
			current_blue_level = RGB_MAX;
		}

		analog_control_onboard_leds(white, analog_set);
		 */
	}



	/**
	 * Program should never reach this point
	 */
	return EXIT_SUCCESS;
}

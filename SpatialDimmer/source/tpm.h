/**
 * @file	tpm.h
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief	Macros and function headers for TPM (Timer PWM Module)
 */



#ifndef TPM_H_
#define TPM_H_



/**
 * @brief	Red on-board LED is tied to TPM2 channel 0
 */
#define TPM2_RED_LED_CHANNEL\
	(0)



/**
 * @brief	Green on-board LED is tied to TPM2 channel 1
 */
#define TPM2_GREEN_LED_CHANNEL\
	(1)



/**
 * @brief	Blue on-board LED is tied to TPM0 channel 1
 */
#define TPM0_BLUE_LED_CHANNEL\
	(1)



/**
 * @brief	Lowest possible value when working with RGB
 */
#define RGB_MIN\
	(0)



/**
 * @brief	Highest possible value when working with RGB
 */
#define RGB_MAX\
	(255)



/**
 * @brief	The value to load into TPM->MOD register. This is the amount
 * 			of levels the PWM can pulse to. Set to 255 because we are
 * 			working with RGB
 */
#define TPM_MOD\
	(RGB_MAX)



/**
 * @brief	Defined in tpm.c
 */
extern int rgb_levels;



/**
 * @brief	Defined in tpm.c
 */
extern int16_t current_red_level;



/**
 * @brief	Defined in tpm.c
 */
extern int16_t current_green_level;



/**
 * @brief	Defined in tpm.c
 */
extern int16_t current_blue_level;



/**
 * @brief	Defined in tpm.c
 */
extern int8_t step_red_level;



/**
 * @brief	Defined in tpm.c
 */
extern int8_t step_green_level;



/**
 * @brief	Defined in tpm.c
 */
extern int8_t step_blue_level;



/**
 * @brief	Initialize the on-board timer PWM module 0
 * @param	channel - The TPM0 channel to initialize
 * @param	mod - The value to load into TPM0->MOD register (this is what
 * 			TPM timer will count up to) */
void init_onboard_tpm0(uint32_t channel, uint16_t mod);



/**
 * @brief	Initialize the on-board timer PWM module 2
 * @param	channel - The TPM2 channel to initialize
 * @param	mod - The value to load into TPM2->MOD register (this is what
 * 			TPM timer will count up to)
 */
void init_onboard_tpm2(uint32_t channel, uint16_t mod);



/**
 * @brief	Calculate the smallest necessary TPM prescaler to provide the
 * 			highest granularity
 * @param	tpm_clock_hz - The frequency of the clock TPM will take
 * 			its reference from
 * @param	tpm_pwm_hz - The desired frequency of TPM output
 * @return	x for 2^x, where 2^x is the TPM prescaler
 */
uint8_t get_smallest_prescaler(uint32_t tpm_clock_hz, uint32_t tpm_pwm_hz);



/**
 * @brief	Control analog signals of on-board LED for specific color(s)
 * @param	led_color - The LED color(s) to perform the action on
 * @param	led_action - The action to perform on LED color(s)
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 2
 * 		of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 *
 * 		PCOR:	Port Clear Output Register is a register used to clear some
 * 				bits on the respective port A-E (e.g. writing 0x00000070 to
 * 				PTA->PCOR will clear bits 6:4 on Port A)
 * 		PSOR:	Port Set Output Register is a register used to set some bits
 * 				on the respective port A-E (e.g. writing 0x00000070 to PTA->PSOR
 * 				will set bits 6:4 on Port A)
 *		PTOR:	Port Toggle Output Register is a register used to toggle some
 *				bits on the respective port A-E (e.g. writing 0x00000070 to
 *				PTA-PTOR will toggle bits 6:4 on Port A)
 */
void analog_control_onboard_leds(led_color_t led_color, led_action_t led_action);



/**
 * @brief	Calculate current red LED level with respective to current x
 * @detail
 *
 */
void init_onboard_tpm2(uint32_t channel, uint16_t mod);

#endif /* TPM_H_ */

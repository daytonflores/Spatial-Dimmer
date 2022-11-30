/**
 * @file    tpm.h
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief   Macros and function headers for TPM (Timer PWM Module)
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
 * @brief   Initialize the on-board timer PWM module 0
 */
void init_onboard_tpm0(uint32_t channel, uint16_t mod);



/**
 * @brief   Initialize the on-board timer PWM module 2
 */
void init_onboard_tpm2(uint32_t channel, uint16_t mod);



/**
 * @return	x for 2^x, where 2^x is the TPM prescaler
 * @brief   Calculate the smallest necessary TPM prescaler to provide the highest granularity
 */
uint8_t get_smallest_prescaler(uint32_t tpm_clock_hz, uint32_t tpm_pwm_hz);



/**
 * @brief   Control analog signals of on-board LED for specific color(s)
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 2 of Embedded Systems
 * 		Fundamentals with ARM Cortex-M Based Microcontrollers)
 *
 * 		PCOR:	Port Clear Output Register is a register used to clear some bits on the respective
 * 				port A-E (e.g. writing 0x00000070 to PTA->PCOR will clear bits 6:4 on Port A)
 * 		PSOR:	Port Set Output Register is a register used to set some bits on the respective port
 * 				A-E (e.g. writing 0x00000070 to PTA->PSOR will set bits 6:4 on Port A)
 *		PTOR:	Port Toggle Output Register is a register used to toggle some bits on the respective
 *				port A-E (e.g. writing 0x00000070 to PTA-PTOR will toggle bits 6:4 on Port A)
 */
void analog_control_onboard_leds(led_t led, led_action_t led_action);



#endif /* TPM_H_ */

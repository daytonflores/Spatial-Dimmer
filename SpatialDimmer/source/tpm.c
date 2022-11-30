/**
 * @file    tpm.c
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief   Function definitions for TPM (Timer PWM Module)
 */



/**
 * Include pre-defined libraries
 */
#include "board.h"



/**
 * User-defined libraries
 */
#include "bitops.h"
#include "led.h"
#include "tpm.h"



/**
 * @brief	Configuration for TPM clock source select
 * @detail
 * 		0: Disabled
 * 		1: MCGFLLCLK (or MCGPLLCLK / 2)
 * 		2: OSCERCLK
 * 		3: MCGIRCLK
 */
#define SOPT2_TPMSRC\
	(1)



/**
 * @brief	Selects the MCGPLLCLK or MCGFLLCLK clock for various
 * 			peripheral clocking options
 * @detail
 * 		0: MCGPLLCLK clock
 * 		1: MCGPLLCLK clock with fixed divide by 2
 */
#define SOPT2_PLLFLLSEL\
	(1)



/**
 * @brief	The frequency of MCGFLLCLK clock in Hz
 */
#define MCGFLLCLK_HZ\
	(48000000)



/**
 * @brief	The frequency of TPM clock in Hz
 */
#define TPM_CLOCK_HZ\
	(MCGFLLCLK_HZ)



/**
 * @brief	The desired frequency of the PWM in Hz
 */
#define TPM_PWM_HZ\
	(500)



/**
 * @brief	The max division factor for 16-bit TPM->MOD register
 */
#define MAX_TPM_MOD_VALUE\
	(65536)



/**
 * @brief	Configuration for TPM debug mode
 * @detail
 * 		0: LPTPM counter does not increment during debug. Trigger inputs and
 * 		   input capture events are also ignored
 * 		3: LPTPM counter continues to increment in debug mode
 */
#define CONF_DBGMODE\
	(3)



/**
 * @brief	Selects the LPTPM counter clock modes. When disabling the counter,
 * 			this field remain set until acknowledged in the LPTPM clock domain
 * @detail
 * 		00: LPTPM counter is disabled
 * 		01: LPTPM counter increments on every LPTPM counter clock
 * 		10: LPTPM counter increments on rising edge of LPTPM_EXTCLK synchronized
 * 			to the LPTPM counter
 * 		11: Reserved
 */
#define SC_CMOD\
	(1)



/**
 * @brief	Turn off on-board red LED through analog TPM
 */
#define ANALOG_CLEAR_RED_LED()\
	(TPM2->CONTROLS[TPM2_RED_LED_CHANNEL].CnV = 0)



/**
 * @brief	Turn off on-board green LED through analog TPM
 */
#define ANALOG_CLEAR_GREEN_LED()\
	(TPM2->CONTROLS[TPM2_GREEN_LED_CHANNEL].CnV = 0)



/**
 * @brief	Turn off on-board blue LED through analog TPM
 */
#define ANALOG_CLEAR_BLUE_LED()\
	(TPM0->CONTROLS[TPM0_BLUE_LED_CHANNEL].CnV = 0)



/**
 * @brief	Set on-board red LED through analog TPM
 */
#define ANALOG_SET_RED_LED(x)\
	(TPM2->CONTROLS[TPM2_RED_LED_CHANNEL].CnV = x)



/**
 * @brief	Set on-board green LED through analog TPM
 */
#define ANALOG_SET_GREEN_LED(x)\
	(TPM2->CONTROLS[TPM2_GREEN_LED_CHANNEL].CnV = x)



/**
 * @brief	Set on-board blue LED through analog TPM
 */
#define ANALOG_SET_BLUE_LED(x)\
	(TPM0->CONTROLS[TPM0_BLUE_LED_CHANNEL].CnV = x)



/**
 * @brief	Current analog level of red LED between 0 and 255. Needs to
 * 			be signed to support both dimming and brightening
 */
int16_t current_red_level = 0;



/**
 * @brief	Current analog level of red LED between 0 and 255. Needs to
 * 			be signed to support both dimming and brightening
 */
int16_t current_green_level = 0;



/**
 * @brief	Current analog level of red LED between 0 and 255. Needs to
 * 			be signed to support both dimming and brightening
 */
int16_t current_blue_level = 0;



/**
 * @brief	The amount to step red LED analog level. Needs to
 * 			be signed to support both dimming and brightening
 */
int8_t step_red_level = 4;



/**
 * @brief	The amount to step red LED analog level. Needs to
 * 			be signed to support both dimming and brightening
 */
int8_t step_green_level = 4;



/**
 * @brief	The amount to step red LED analog level. Needs to
 * 			be signed to support both dimming and brightening
 */
int8_t step_blue_level = 4;



void init_onboard_tpm0(uint32_t channel, uint16_t mod){

	/**
	 * Enable clock to TPM module 0
	 */
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;



	/**
     * Configure SOPT2:
     * 	- To use MCGFLLCLK as clock source
     */
	SIM->SOPT2 &=
		~(SIM_SOPT2_TPMSRC_MASK |
		SIM_SOPT2_PLLFLLSEL_MASK);
	SIM->SOPT2 |=
		SIM_SOPT2_TPMSRC(SOPT2_TPMSRC) |
		SIM_SOPT2_PLLFLLSEL(SOPT2_PLLFLLSEL);



	/**
     * Set the smallest needed prescaler along with PWM period for
     * desired PWM frequency
     *
     * TPM->SC[PS] holds x for 2^x, where 2^x is the tpm_prescaler
     *
     * Configure the TPM SC register:
     * 	- Count up with divide by tpm_prescaler
     */
	uint8_t tpm_sc_ps;
	tpm_sc_ps = get_smallest_prescaler(TPM_CLOCK_HZ, TPM_PWM_HZ);
	TPM0->SC = TPM_SC_PS(tpm_sc_ps);



	/**
     * Load the TPM MOD register
     */
	TPM0->MOD = mod;



	/**
     * Configure the TPM CONF register:
     * 	- Continue counting operation in debug mode
     */
	TPM0->CONF |= TPM_CONF_DBGMODE(CONF_DBGMODE);



	/**
     * Configure TPM CnSC:
     * 	- Edge-aligned PWM
     * 	- Low-true pulses (set output on match, clear output on reload)
     */
	TPM0->CONTROLS[channel].CnSC =
		TPM_CnSC_MSB_MASK |
		TPM_CnSC_ELSA_MASK;



	/**
     * Set initial duty cycle
     */
	TPM0->CONTROLS[channel].CnV = 0;



	/**
     * Configure the TPM SC register:
     * 	- Start TPM
     */
	TPM0->SC |= TPM_SC_CMOD(SC_CMOD);
}



void init_onboard_tpm2(uint32_t channel, uint16_t mod){

	/**
	 * Enable clock to TPM module 2
	 */
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;



	/**
     * Configure SOPT2:
     * 	- To use MCGFLLCLK as clock source
     */
	SIM->SOPT2 &=
		~(SIM_SOPT2_TPMSRC_MASK |
		SIM_SOPT2_PLLFLLSEL_MASK);
	SIM->SOPT2 |=
		SIM_SOPT2_TPMSRC(SOPT2_TPMSRC) |
		SIM_SOPT2_PLLFLLSEL(SOPT2_PLLFLLSEL);



	/**
     * Set the smallest needed prescaler along with PWM period for
     * desired PWM frequency
     *
     * TPM->SC[PS] holds x for 2^x, where 2^x is the tpm_prescaler
     *
     * Configure the TPM SC register:
     * 	- Count up with divide by tpm_prescaler
     */
	uint8_t tpm_sc_ps;
	tpm_sc_ps = get_smallest_prescaler(TPM_CLOCK_HZ, TPM_PWM_HZ);
	TPM2->SC = TPM_SC_PS(tpm_sc_ps);



	/**
     * Load the TPM MOD register
     */
	TPM2->MOD = mod;



	/**
     * Configure the TPM CONF register:
     * 	- Continue counting operation in debug mode
     */
	TPM2->CONF |= TPM_CONF_DBGMODE(CONF_DBGMODE);



	/**
     * Configure TPM CnSC:
     * 	- Edge-aligned PWM
     * 	- Low-true pulses (set output on match, clear output on reload)
     */
	TPM2->CONTROLS[channel].CnSC =
		TPM_CnSC_MSB_MASK |
		TPM_CnSC_ELSA_MASK;



	/**
     * Set initial duty cycle
     */
	TPM2->CONTROLS[channel].CnV = 0;



	/**
     * Configure the TPM SC register:
     * 	- Start TPM
     */
	TPM2->SC |= TPM_SC_CMOD(SC_CMOD);
}



uint8_t get_smallest_prescaler(uint32_t tpm_clock_hz, uint32_t tpm_pwm_hz){


	/**
     * Calculate the smallest needed prescaler to allow for largest
     * possible TPM->MOD value and thus more granular control
     */

	uint8_t return_value;

	if(((tpm_clock_hz / tpm_pwm_hz) / MAX_TPM_MOD_VALUE) < 1){
		return_value = 0;
	}
	else if(((tpm_clock_hz / tpm_pwm_hz) / MAX_TPM_MOD_VALUE) < 2){
		return_value = 1;
	}
	else if(((tpm_clock_hz / tpm_pwm_hz) / MAX_TPM_MOD_VALUE) < 4){
		return_value = 2;
	}
	else if(((tpm_clock_hz / tpm_pwm_hz) / MAX_TPM_MOD_VALUE) < 8){
		return_value = 3;
	}
	else if(((tpm_clock_hz / tpm_pwm_hz) / MAX_TPM_MOD_VALUE) < 16){
		return_value = 4;
	}
	else if(((tpm_clock_hz / tpm_pwm_hz) / MAX_TPM_MOD_VALUE) < 32){
		return_value = 5;
	}
	else if(((tpm_clock_hz / tpm_pwm_hz) / MAX_TPM_MOD_VALUE) < 64){
		return_value = 6;
	}
	else{
		return_value = 7;
	}

	return (return_value);
}



void analog_control_onboard_leds(led_t led, led_action_t led_action){

	/**
	 * Control LEDs based on specified LED(s), then action
	 */
	switch(led){
	case red:
		switch(led_action){
		case analog_clear:
			ANALOG_CLEAR_RED_LED();
			break;
		case analog_set:
			ANALOG_SET_RED_LED(current_red_level);
			break;
		default:
			break;
		}
		break;
	case green:
		switch(led_action){
		case analog_clear:
			ANALOG_CLEAR_GREEN_LED();
			break;
		case analog_set:
			ANALOG_SET_GREEN_LED(current_green_level);
			break;
		default:
			break;
		}
		break;
	case blue:
		switch(led_action){
		case analog_clear:
			ANALOG_CLEAR_BLUE_LED();
			break;
		case analog_set:
			ANALOG_SET_BLUE_LED(current_blue_level);
			break;
		default:
			break;
		}
		break;
	case cyan:
		switch(led_action){
		case analog_clear:
			ANALOG_CLEAR_GREEN_LED();
			ANALOG_CLEAR_BLUE_LED();
			break;
		case analog_set:
			ANALOG_SET_GREEN_LED(current_green_level);
			ANALOG_SET_BLUE_LED(current_blue_level);
			break;
		default:
			break;
		}
		break;
	case magenta:
		switch(led_action){
		case analog_clear:
			ANALOG_CLEAR_RED_LED();
			ANALOG_CLEAR_BLUE_LED();
			break;
		case analog_set:
			ANALOG_SET_RED_LED(current_red_level);
			ANALOG_SET_BLUE_LED(current_blue_level);
			break;
		default:
			break;
		}
		break;
	case yellow:
		switch(led_action){
		case analog_clear:
			ANALOG_CLEAR_RED_LED();
			ANALOG_CLEAR_GREEN_LED();
			break;
		case analog_set:
			ANALOG_SET_RED_LED(current_red_level);
			ANALOG_SET_GREEN_LED(current_green_level);
			break;
		default:
			break;
		}
		break;
	case white:
		switch(led_action){
		case analog_clear:
			ANALOG_CLEAR_RED_LED();
			ANALOG_CLEAR_GREEN_LED();
			ANALOG_CLEAR_BLUE_LED();
			break;
		case analog_set:
			ANALOG_SET_RED_LED(current_red_level);
			ANALOG_SET_GREEN_LED(current_green_level);
			ANALOG_SET_BLUE_LED(current_blue_level);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

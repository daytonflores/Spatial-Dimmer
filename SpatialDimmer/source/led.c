/**
 * @file    led.c
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief   Function definitions for on-board LEDs
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
 * @brief	PCR is a 32-bit register where bits 8:10 are a MUX field
 * @detail
 * 		000: Pin disabled (analog)
 * 		001: GPIO
 * 		010: Alternative
 * 		011: TPM2_CH0
 * 		100: Alternative
 * 		101: Alternative
 * 		110: Alternative
 * 		111: Alternative
 */
#define PCR_MUX_GPIO_RED_LED\
	(1)
#define PCR_MUX_TPM_RED_LED\
	(3)



/**
 * @brief	PCR is a 32-bit register where bits 8:10 are a MUX field
 * @detail
 * 		000: Pin disabled (analog)
 * 		001: GPIO
 * 		010: Alternative
 * 		011: TPM2_CH1
 * 		100: Alternative
 * 		101: Alternative
 * 		110: Alternative
 * 		111: Alternative
 */
#define PCR_MUX_GPIO_GREEN_LED\
	(1)
#define PCR_MUX_TPM_GREEN_LED\
	(3)



/**
 * @brief	PCR is a 32-bit register where bits 8:10 are a MUX field
 * @detail
 * 		000: Pin disabled (analog)
 * 		001: GPIO
 * 		010: Alternative
 * 		011: Alternative
 * 		100: TPM0_CH1
 * 		101: Alternative
 * 		110: Alternative
 * 		111: Alternative
 */
#define PCR_MUX_GPIO_BLUE_LED\
	(1)
#define PCR_MUX_TPM_BLUE_LED\
	(4)



/**
 * @brief	On-board red LED is located at PTB18
 */
#define PORTB_RED_LED_PIN\
	(18)



/**
 * @brief	On-board green LED is located at PTB19
 */
#define PORTB_GREEN_LED_PIN\
	(19)



/**
 * @brief	On-board blue LED is located at PTD1
 */
#define PORTD_BLUE_LED_PIN\
	(1)



/**
 * @brief	Turn off on-board red LED through digital GPIO port
 */
#define DIGITAL_CLEAR_RED_LED()\
	(PTB->PSOR |= MASK(PORTB_RED_LED_PIN))



/**
 * @brief	Turn off on-board green LED through digital GPIO port
 */
#define DIGITAL_CLEAR_GREEN_LED()\
	(PTB->PSOR |= MASK(PORTB_GREEN_LED_PIN))



/**
 * @brief	Turn off on-board blue LED through digital GPIO port
 */
#define DIGITAL_CLEAR_BLUE_LED()\
	(PTD->PSOR |= MASK(PORTD_BLUE_LED_PIN))



/**
 * @brief	Turn on on-board red LED through digital GPIO port
 */
#define DIGITAL_SET_RED_LED()\
	(PTB->PCOR |= MASK(PORTB_RED_LED_PIN))



/**
 * @brief	Turn on on-board green LED through digital GPIO port
 */
#define DIGITAL_SET_GREEN_LED()\
	(PTB->PCOR |= MASK(PORTB_GREEN_LED_PIN))



/**
 * @brief	Turn on on-board blue LED through digital GPIO port
 */
#define DIGITAL_SET_BLUE_LED()\
	(PTD->PCOR |= MASK(PORTD_BLUE_LED_PIN))



/**
 * @brief	Toggle on-board red LED through digital GPIO port
 */
#define DIGITAL_TOGGLE_RED_LED()\
	(PTB->PTOR |= MASK(PORTB_RED_LED_PIN))



/**
 * @brief	Toggle on-board green LED through digital GPIO port
 */
#define DIGITAL_TOGGLE_GREEN_LED()\
	(PTB->PTOR |= MASK(PORTB_GREEN_LED_PIN))



/**
 * @brief	Toggle on-board blue LED through digital GPIO port
 */
#define DIGITAL_TOGGLE_BLUE_LED()\
	(PTD->PTOR |= MASK(PORTD_BLUE_LED_PIN))



void init_red_onboard_led(led_type_t led_type){

	/**
     * Enable clock to Port B for red on-board LED
     */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;



    /**
     * Set PTB18 as GPIO for red on-board LED
     *
     * The MUX selection in PCR is done with bits 10:8
     */
	PORTB->PCR[PORTB_RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	switch(led_type){
	case digital:
		PORTB->PCR[PORTB_RED_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_GPIO_RED_LED);
		break;
	case analog:
		PORTB->PCR[PORTB_RED_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_TPM_RED_LED);
		break;
	default:
		break;
	}



	/**
	 * Set PTB18 as output for red on-board LED
	 */
    PTB->PDDR |= MASK(PORTB_RED_LED_PIN);



    /**
     * Turn red on-board LED off. Note that on-board LEDs are active-low
     */
    digital_control_onboard_leds(red, digital_clear);
}



void init_green_onboard_led(led_type_t led_type){


	/**
     * Enable clock to Port B for green on-board LED
     */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;



    /**
     * Set PTB19 as GPIO for green on-board LED
     *
     * The MUX selection in PCR is done with bits 10:8
     */
	PORTB->PCR[PORTB_GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	switch(led_type){
	case digital:
		PORTB->PCR[PORTB_GREEN_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_GPIO_GREEN_LED);
		break;
	case analog:
		PORTB->PCR[PORTB_GREEN_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_TPM_GREEN_LED);
		break;
	default:
		break;
	}



	/**
	 * Set PTB19 as output for green on-board LED
	 */
    PTB->PDDR |= MASK(PORTB_GREEN_LED_PIN);



    /**
     * Turn green on-board LED off. Note that on-board LEDs are active-low
     */
    digital_control_onboard_leds(green, digital_clear);
}



void init_blue_onboard_led(led_type_t led_type){


	/**
     * Enable clock to Port D for blue on-board LED
     */
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;



    /**
     * Set PTD1 as GPIO for blue on-board LED
     *
     * The MUX selection in PCR is done with bits 10:8
     */
	PORTD->PCR[PORTD_BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	switch(led_type){
	case digital:
		PORTD->PCR[PORTD_BLUE_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_GPIO_BLUE_LED);
		break;
	case analog:
		PORTD->PCR[PORTD_BLUE_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_TPM_BLUE_LED);
		break;
	default:
		break;
	}



	/**
	 * Set PTD1 as output for blue on-board LED
	 */
    PTD->PDDR |= MASK(PORTD_BLUE_LED_PIN);



    /**
     * Turn all on-board LEDs off. Note that on-board LEDs are active-low
     */
    digital_control_onboard_leds(blue, digital_clear);
}



void digital_control_onboard_leds(led_color_t led_color, led_action_t led_action){

	/**
	 * Control LEDs based on specified LED(s), then action
	 */
	switch(led_color){
	case red:
		switch(led_action){
		case digital_clear:
			DIGITAL_CLEAR_RED_LED();
			break;
		case digital_set:
			DIGITAL_SET_RED_LED();
			break;
		case digital_toggle:
			DIGITAL_TOGGLE_RED_LED();
			break;
		default:
			break;
		}
		break;
	case green:
		switch(led_action){
		case digital_clear:
			DIGITAL_CLEAR_GREEN_LED();
			break;
		case digital_set:
			DIGITAL_SET_GREEN_LED();
			break;
		case digital_toggle:
			DIGITAL_TOGGLE_GREEN_LED();
			break;
		default:
			break;
		}
		break;
	case blue:
		switch(led_action){
		case digital_clear:
			DIGITAL_CLEAR_BLUE_LED();
			break;
		case digital_set:
			DIGITAL_SET_BLUE_LED();
			break;
		case digital_toggle:
			DIGITAL_TOGGLE_BLUE_LED();
			break;
		default:
			break;
		}
		break;
	case cyan:
		switch(led_action){
		case digital_clear:
			DIGITAL_CLEAR_GREEN_LED();
			DIGITAL_CLEAR_BLUE_LED();
			break;
		case digital_set:
			DIGITAL_SET_GREEN_LED();
			DIGITAL_SET_BLUE_LED();
			break;
		case digital_toggle:
			DIGITAL_TOGGLE_GREEN_LED();
			DIGITAL_TOGGLE_BLUE_LED();
			break;
		default:
			break;
		}
		break;
	case magenta:
		switch(led_action){
		case digital_clear:
			DIGITAL_CLEAR_RED_LED();
			DIGITAL_CLEAR_BLUE_LED();
			break;
		case digital_set:
			DIGITAL_SET_RED_LED();
			DIGITAL_SET_BLUE_LED();
			break;
		case digital_toggle:
			DIGITAL_TOGGLE_RED_LED();
			DIGITAL_TOGGLE_BLUE_LED();
			break;
		default:
			break;
		}
		break;
	case yellow:
		switch(led_action){
		case digital_clear:
			DIGITAL_CLEAR_RED_LED();
			DIGITAL_CLEAR_GREEN_LED();
			break;
		case digital_set:
			DIGITAL_SET_RED_LED();
			DIGITAL_SET_GREEN_LED();
			break;
		case digital_toggle:
			DIGITAL_TOGGLE_RED_LED();
			DIGITAL_TOGGLE_GREEN_LED();
			break;
		default:
			break;
		}
		break;
	case white:
		switch(led_action){
		case digital_clear:
			DIGITAL_CLEAR_RED_LED();
			DIGITAL_CLEAR_GREEN_LED();
			DIGITAL_CLEAR_BLUE_LED();
			break;
		case digital_set:
			DIGITAL_SET_RED_LED();
			DIGITAL_SET_GREEN_LED();
			DIGITAL_SET_BLUE_LED();
			break;
		case digital_toggle:
			DIGITAL_TOGGLE_RED_LED();
			DIGITAL_TOGGLE_GREEN_LED();
			DIGITAL_TOGGLE_BLUE_LED();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

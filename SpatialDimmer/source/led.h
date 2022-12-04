/**
 * @file    led.h
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief   Macros and function headers for on-board LEDs
 */



#ifndef LED_H_
#define LED_H_



/**
 * @brief	Used to refer to aspect of on-board RGB LED
 */
typedef enum led_color_e{
	red,
	green,
	blue,
	cyan,
	magenta,
	yellow,
	white
} led_color_t;



/**
 * @brief	Used to refer to set on-board RGB LED output type
 */
typedef enum led_type_e{
	digital,
	analog
} led_type_t;



/**
 * @brief	Possible actions to be performed on on-board LEDs when treating
 * 			on-board LEDs as digital or analog outputs
 */
typedef enum led_action_e{
	digital_clear,
	digital_set,
	digital_toggle,
	analog_clear,
	analog_set} led_action_t;



/**
 * @brief	Initialize red on-board LED as GPIO output and turn it off
 * @param	led_type - Indicates whether LED will be digital or analog
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 2 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers).
 * 		Also referenced operations from https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_2/Source/main.c
 *
 * 		SIM:	System Integration Module is a peripheral containing many
 * 				control registers, including SCGC5
 * 		SCGC5:	System Clock Gating Control Register 5 is a register containing
 * 				different controls, including clock gating for Ports A-E
 * 				(controlled with bits 9:13 respectively)
 * 		PORTB:	Port containing 32 pins, including red on-board LED
 * 		PCR:	Pin Control Register, though in this function PCR is an array
 * 				of 32 PCR registers (where each Port A-E has its own array of
 * 				PCR registers). All 32 pins within each Port A-E have their own
 * 				PCR register (e.g. pin 1 on Port A has its own 32-bit PCR)
 * 		PTB:	Points to all GPIO Control Registers for Port B, including PDDR
 * 		PDDR:	Pin Data Direction Register is a register allowing configuration
 * 				of a pin on the respective port A-E (where 0 is input, 1 is output)
 */
void init_red_onboard_led(led_type_t led_type);



/**
 * @brief	Initialize red on-board LED as GPIO output and turn it off
 * @param	led_type - Indicates whether LED will be digital or analog
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 2 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers).
 * 		Also referenced operations from https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_2/Source/main.c
 *
 * 		SIM:	System Integration Module is a peripheral containing many
 * 				control registers, including SCGC5
 * 		SCGC5:	System Clock Gating Control Register 5 is a register containing
 * 				different controls, including clock gating for Ports A-E
 * 				(controlled with bits 9:13 respectively)
 * 		PORTB:	Port containing 32 pins, including red on-board LED
 * 		PCR:	Pin Control Register, though in this function PCR is an array
 * 				of 32 PCR registers (where each Port A-E has its own array of
 * 				PCR registers). All 32 pins within each Port A-E have their own
 * 				PCR register (e.g. pin 1 on Port A has its own 32-bit PCR)
 * 		PTB:	Points to all GPIO Control Registers for Port B, including PDDR
 * 		PDDR:	Pin Data Direction Register is a register allowing configuration
 * 				of a pin on the respective port A-E (where 0 is input, 1 is output)
 */
void init_green_onboard_led(led_type_t led_type);



/**
 * @brief	Initialize red on-board LED as GPIO output and turn it off
 * @param	led_type - Indicates whether LED will be digital or analog
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 2 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers).
 * 		Also referenced operations from https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_2/Source/main.c
 *
 * 		SIM:	System Integration Module is a peripheral containing many
 * 				control registers, including SCGC5
 * 		SCGC5:	System Clock Gating Control Register 5 is a register containing
 * 				different controls, including clock gating for Ports A-E
 * 				(controlled with bits 9:13 respectively)
 * 		PORTB:	Port containing 32 pins, including red on-board LED
 * 		PCR:	Pin Control Register, though in this function PCR is an array
 * 				of 32 PCR registers (where each Port A-E has its own array of
 * 				PCR registers). All 32 pins within each Port A-E have their own
 * 				PCR register (e.g. pin 1 on Port A has its own 32-bit PCR)
 * 		PTD:	Points to all GPIO Control Registers for Port D, including PDDR
 * 		PDDR:	Pin Data Direction Register is a register allowing configuration
 * 				of a pin on the respective port A-E (where 0 is input, 1 is output)
 */
void init_blue_onboard_led(led_type_t led_type);



/**
 * @brief   Control digital signals of on-board LED for specific color(s)
 * @param	led_color - The LED color(s) to perform the action on
 * @param	led_action - The action to perform on LED color(s)
 * @detail
 * 		Many operations were referenced from Alexander G Dean (Chapter 2 of
 * 		Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 *
 * 		PCOR:	Port Clear Output Register is a register used to clear some bits
 * 				on the respective port A-E (e.g. writing 0x00000070 to PTA->PCOR
 * 				will clear bits 6:4 on Port A)
 * 		PSOR:	Port Set Output Register is a register used to set some bits on
 * 				the respective port A-E (e.g. writing 0x00000070 to PTA->PSOR will
 * 				set bits 6:4 on Port A)
 *		PTOR:	Port Toggle Output Register is a register used to toggle some bits
 *				on the respective port A-E (e.g. writing 0x00000070 to PTA-PTOR will
 *				toggle bits 6:4 on Port A)
 */
void digital_control_onboard_leds(led_color_t led_color, led_action_t led_action);



#endif /* LED_H_ */

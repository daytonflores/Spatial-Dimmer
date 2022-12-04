/**
 * @file    bitops.h
 * @author	Dayton Flores (dafl2542@colorado.edu)
 * @date	12/11/2022
 * @brief   Macros and function headers for bit operations
 */



#ifndef BITOPS_H_
#define BITOPS_H_



/**
 * @brief	Takes user input as bit position and returns a 1-bit mask.
 * 			For example, giving this macro 18 will return 32-bit value
 * 			of all 0s except the 18th bit will be 1
 * @param	x - The amount of bits 1 should be shifted to the left
 * @detail
 * 		Referenced from Chapter 2 of Alexander G Dean's Embedded Systems
 * 		Fundamentals with ARM Cortex-M Based Microcontrollers: Embedded Systems
 */
#define MASK(x)\
	((uint32_t)(1UL << (x)))



#endif /* BITOPS_H_ */

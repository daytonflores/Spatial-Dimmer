# PES-Final-Project
 Code for Final Project for PRES, ECEN 5813-001B, Fall 2022

# Completed Functionality

- GPIO Output with on-board LEDs
- I2C with on-board MMA8451Q accelerometer
- PWM to control dimming/brightening of on-board LEDs

# Unfinished Functionality

- GPIO Input with external pushbutton

# Usage

- Once uploaded to FRDM25KLZ board, you may view the calculated XYZ + RGB values via serial terminal (baud 115200, 0 parity, 8-bit data)

# Test Cases

- Kept board level and observed XYZ values constant with constantly mapped RGB values
- Rotated board along X axis and observed:
	- X value being displayed increasing/decreasing
	- Red LED dimming and brightening
- Rotated board along Y axis and observed:
	- Y value being displayed increasing/decreasing
	- Green LED dimming and brightening
- Rotated board along Z axis and observed:
	- Z value being displayed increasing/decreasing
	- Blue LED dimming and brightening

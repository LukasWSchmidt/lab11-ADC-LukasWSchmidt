/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 2 Match Me
 * File: led_bar.h
 * Dependencies: stdint, stdbool
 */

#ifndef LED_BAR_H_
#define LED_BAR_H_

#include <stdint.h>
#include <stdbool.h>

#define RCC_AHB1_ENR 0x40023830
#define GPIOB_EN 1



//offset 0
#define GPIOB 0x40020400
//offset 0x14
#define GPIOB_ODR 0x40020414
//offset 0x18
#define GPIOB_BSRR 0x40020418


/**
 * LedBarInit()
 * Description:
 * 		Configures PB5-PB10,PB12-PB15 as outputs
 * Inputs: None
 * Outputs: None
 */
void LedBarInit();

/**
 * LedBarOn()
 * Description:
 * 		Turns on the LED at the index
 * Inputs:
 * 		index - 0-9 the index of light to be activated
 * 			Note: out of range args should have no effect.
 * Outputs: none
 *
 */
void LedBarOn(uint8_t index);

/**
 * LedBarOff()
 * Description:
 * 		Turns off the LED at the index
 * Inputs:
 * 		index - 0-9 the index of light to be activated
 * 			Note: out of range args should have no effect.
 * Outputs: none
 *
 */
void LedBarOff(uint8_t index);

/**
 * LedBarToggle()
 * Description:
 * 		Reverses the state the LED at the index
 * Inputs:
 * 		index - 0-9 the index of light to be activated
 * 			Note: out of range args should have no effect.
 * Outputs: none
 *
 */
void LedBarToggle(uint8_t index);

/**
 * LedBarWrite()
 * Description:
 * 		Write a pattern to the Led Bar
 * 	Inputs:
 * 		pattern - the 10 least significant bit are
 * 						written to the lights
 * 	Outputs: None
 */
void LedBarWrite(uint16_t pattern);

/**
 * LedBarRead()
 * Description:
 * 		Read the current from Led Bar
 * 	Inputs: None
 * 	Return: pattern - the 10 least significant bits are
 * 						current light patterns
 */
uint16_t LedBarRead();

/**
 * LedBarRollRight()
 * Description:
 * 		Shift the pattern one bit the right.
 * 		LSBit becomes MSBit.
 * 	Inputs: None
 * 	Outputs: None
 */
void LedBarRollRight();

/**
 * LedBarRollRight()
 * Description:
 * 		Shift the pattern one bit the left.
 * 		MSBit becomes LSBit.
 * 	Inputs: None
 * 	Outputs: None
 */
void LedBarRollLeft();




#endif /* LED_BAR_H_ */

/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 8 Keypad Interrupt
 * File: keypad.c
 * Dependencies: keypad.h, gpio.h, systick.h, interrupt.h, tim.h
 */

#include "stdint.h"
#include <stdbool.h>

#ifndef KEYPAD_H_
#define KEYPAD_H_

/**
  * @brief  Initializes the keypad
  * @param  None
  * @retval None
  */
void keypadInit(void);

void buttonPressed(uint8_t col, uint32_t passedIDR, bool *updateScreen, uint16_t *keyPassed);

#endif /* KEYPAD_H_ */

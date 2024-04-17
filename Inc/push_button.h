/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 3 Buttons
 * File: push_button.h
 * Dependencies: stdint
 */

#ifndef PUSH_BUTTON_H_
#define PUSH_BUTTON_H_

#include <stdint.h>
#include "gpio.h"

void GPIOInit();

uint8_t is_button_pressed(GPIO_t *gpio, int pin);

uint8_t is_encode_button_pressed();

uint8_t is_user_button_pressed();

uint8_t is_stick_button_pressed();

void InitEncodeButton();

void InitUserButton();

void InitStickButton();

#endif /* PUSH_BUTTON_H_ */

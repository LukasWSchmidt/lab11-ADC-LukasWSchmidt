/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 10 - Led Matrix
 * File: led_array.h
 * Dependencies: led_array, stdint
 * Usage: Functions used for displaying on the LED matrix
 */

#ifndef LED_ARRAY_H_
#define LED_ARRAY_H_

#include <stdint.h>

typedef struct frame_buffer_t {
	uint8_t row1;
	uint8_t row2;
	uint8_t row3;
	uint8_t row4;
	uint8_t row5;
	uint8_t row6;
	uint8_t row7;
	uint8_t row8;
} frame_buffer_t;

void LedArrayInit();
void LedArraySetPixel(uint16_t x,uint16_t y,uint8_t val);
void LedArrayClear();
void LedArrayUpdate();
void setFace(frame_buffer_t* face);

#endif /* LED_ARRAY_H_ */

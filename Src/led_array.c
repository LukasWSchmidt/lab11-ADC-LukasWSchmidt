/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 10 - Led Matrix
 * File: led_array.c
 * Dependencies: led_array, stdint, spi
 * Usage: Functions used for displaying on the LED matrix
 */

#include "../Inc/led_array.h"
#include <stdint.h>
#include "../Inc/spi.h"


static frame_buffer_t* frame_buffer;

void LedArrayInit(){
	//go into normal mode
  uint16_t 	reg = 0x0C01;
	transmitData(reg);

	reg = 0x0F00;
	transmitData(reg);

	//allow digits to light
	reg = 0x0B07;
	transmitData(reg);

	frame_buffer = (frame_buffer_t*)malloc(sizeof(frame_buffer_t));

}

void LedArraySetPixel(uint16_t x,uint16_t y,uint8_t val){
	//check if the pixel is in the frame
	if((x >= 0 && x <= 7) && (y >= 0 && y <= 7) && (val == 0 || val == 1)) {
		switch(y){
			case 0:
				frame_buffer->row1 &= ~(0b1 << x);
				frame_buffer->row1 |= (val << x);
				break;
			case 1:
				frame_buffer->row2 &= ~(0b1 << x);
				frame_buffer->row2 |= (val << x);
				break;
			case 2:
				frame_buffer->row3 &= ~(0b1 << x);
				frame_buffer->row3 |= (val << x);
				break;
			case 3:
				frame_buffer->row4 &= ~(0b1 << x);
				frame_buffer->row4 |= (val << x);
				break;
			case 4:
				frame_buffer->row5 &= ~(0b1 << x);
				frame_buffer->row5 |= (val << x);
				break;
			case 5:
				frame_buffer->row6 &= ~(0b1 << x);
				frame_buffer->row6 |= (val << x);
				break;
			case 6:
				frame_buffer->row7 &= ~(0b1 << x);
				frame_buffer->row7 |= (val << x);
				break;
			case 7:
				frame_buffer->row8 &= ~(0b1 << x);
				frame_buffer->row8 |= (val << x);
				break;
				}
	}
}
void LedArrayClear(){
	//set whole fb to 0's
	//update
	frame_buffer->row1 = 0;
	frame_buffer->row2 = 0;
	frame_buffer->row3 = 0;
	frame_buffer->row4 = 0;
	frame_buffer->row5 = 0;
	frame_buffer->row6 = 0;
	frame_buffer->row7 = 0;
	frame_buffer->row8 = 0;
	LedArrayUpdate();
}
void LedArrayUpdate(){
	//transmit buffer row by row
	for(int i = 0; i < 8; i++) {
		uint16_t reg = 0x0000;
		uint16_t desired_line = 0;
		switch(i){
			case 0:
				reg = 0x0100;
				desired_line = frame_buffer->row1;
				break;
			case 1:
				reg = 0x0200;
				desired_line = frame_buffer->row2;
				break;
			case 2:
				reg = 0x0300;
				desired_line = frame_buffer->row3;
				break;
			case 3:
				reg = 0x0400;
				desired_line = frame_buffer->row4;
				break;
			case 4:
				reg = 0x0500;
				desired_line = frame_buffer->row5;
				break;
			case 5:
				reg = 0x0600;
				desired_line = frame_buffer->row6;
				break;
			case 6:
				reg = 0x0700;
				desired_line = frame_buffer->row7;
				break;
			case 7:
				reg = 0x0800;
				desired_line = frame_buffer->row8;
				break;
		}
		//put values of x's into reg
		reg |= desired_line;

		//send out to matrix
		transmitData(reg); 
	}
}

void setFace(frame_buffer_t* face){
	frame_buffer = face;
	LedArrayUpdate();
}

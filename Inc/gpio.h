/*
 * gpio.h
 *
 *  Created on: Feb 7, 2024
 *      Author: schmidtlw
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

#define RCC_AHB1_ENR 0x40023830
#define GPIOA_BASE 0x40020000
#define GPIOB_BASE 0x40020400
#define GPIOC_BASE 0x40020800
#define GPIOD_BASE 0x40020C00
#define GPIOE_BASE 0x40021000
#define GPIOA_EN 0
#define GPIOB_EN 1
#define GPIOC_EN 2
#define INPUT_MODE 0b00
#define OUTPUT_MODE 0b01
#define ALTFUNC_MODE 0b10
#define ANALOG_MODE 0b11
#define CLEAR_MODE 0b11

typedef struct GPIO_t {
	uint32_t moder;
	uint32_t otyper;
	uint32_t ospeedr;
	uint32_t pupdr;
	uint32_t idr;
	uint32_t odr;
	uint32_t bsrr;
	uint32_t lckr;
	uint32_t afrl;
	uint32_t afrh;

} GPIO_t;



#endif /* GPIO_H_ */

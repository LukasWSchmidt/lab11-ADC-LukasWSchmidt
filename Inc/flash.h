/*
 * flash.h
 *
 *  Created on: Jan 23, 2024
 *      Author: livingston
 */

#ifndef FLASH_H_
#define FLASH_H_

#define RCC_AHB1ENR 0x40023830
#define RCC_GPIOA_EN 0

#define GPIOA_MODER 0x40020000
#define GPIOA_ODR 0x40020014
#define GPIOA_USER 5

void UserLedInit();

void UserLedOn();

void UserLedOff();


#endif /* FLASH_H_ */

/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 4 LCD Display
 * File: lcd.h
 * Dependencies:
 * 		gpio.h - gpio structs
 * Usage:
 */
#include "stdint.h"
#include "led_bar.h"
#include "gpio.h"

#define PB5 5
#define PB6 6
#define PB7 7
#define PB8 8
#define PB9 9
#define PB10 10
#define PB12 12
#define PB13 13
#define PB14 14
#define PB15 15

GPIO_t* gpio  = (GPIO_t *)GPIOB;

/**
 * LedBarInit()
 * Description:
 * 		Configures PB5-PB10,PB12-PB15 as outputs
 * Inputs: None
 * Outputs: None
 */
void LedBarInit(){
	volatile uint32_t* moder = &(gpio->moder);

	volatile uint32_t* rcc = (uint32_t*) RCC_AHB1_ENR;
	*rcc |= (1<<GPIOB_EN);



	*moder &= ~(0b11<<(2*PB5));
	*moder |= 0b01<<(2*PB5);

	*moder &= ~(0b11<<(2*PB6));
	*moder |= 0b01<<(2*PB6);

	*moder &= ~(0b11<<(2*PB7));
	*moder |= 0b01<<(2*PB7);

	*moder &= ~(0b11<<(2*PB8));
	*moder |= 0b01<<(2*PB8);

	*moder &= ~(0b11<<(2*PB9));
	*moder |= 0b01<<(2*PB9);

	*moder &= ~(0b11<<(2*PB10));
	*moder |= 0b01<<(2*PB10);

	*moder &= ~(0b11<<(2*PB12));
	*moder |= 0b01<<(2*PB12);

	*moder &= ~(0b11<<(2*PB13));
	*moder |= 0b01<<(2*PB13);

	*moder &= ~(0b11<<(2*PB14));
	*moder |= 0b01<<(2*PB14);

	*moder &= ~(0b11<<(2*PB15));
	*moder |= 0b01<<(2*PB15);

}

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
void LedBarOn(uint8_t index){
	//index 0 is PB5...continues normally
	//index 5 = PB10
	//index 6 = PB12
	//index 9 = PB15

	uint8_t realIndex = PB5;

	switch(index){
		case 0:
			realIndex = PB5;
			break;
		case 1:
			realIndex = PB6;
			break;
		case 2:
			realIndex = PB7;
			break;
		case 3:
			realIndex = PB8;
			break;
		case 4:
			realIndex = PB9;
			break;
		case 5:
			realIndex = PB10;
			break;
		case 6:
			realIndex = PB12;
			break;
		case 7:
			realIndex = PB13;
			break;
		case 8:
			realIndex = PB14;
			break;
		case 9:
			realIndex = PB15;
			break;
	}

	volatile uint32_t* bsrr = &(gpio->bsrr);

	*bsrr |= 1<<realIndex;
}

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
void LedBarOff(uint8_t index){
		uint8_t realIndex = PB5;

		switch(index){
			case 0:
				realIndex = PB5;
				break;
			case 1:
				realIndex = PB6;
				break;
			case 2:
				realIndex = PB7;
				break;
			case 3:
				realIndex = PB8;
				break;
			case 4:
				realIndex = PB9;
				break;
			case 5:
				realIndex = PB10;
				break;
			case 6:
				realIndex = PB12;
				break;
			case 7:
				realIndex = PB13;
				break;
			case 8:
				realIndex = PB14;
				break;
			case 9:
				realIndex = PB15;
				break;
		}
		realIndex += 16;

		volatile uint32_t* bsrr = &(gpio->bsrr);

		*bsrr |= 1<<realIndex;
}

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
void LedBarToggle(uint8_t index) {
		uint8_t realIndex = PB5;

		switch(index){
			case 0:
				realIndex = PB5;
				break;
			case 1:
				realIndex = PB6;
				break;
			case 2:
				realIndex = PB7;
				break;
			case 3:
				realIndex = PB8;
				break;
			case 4:
				realIndex = PB9;
				break;
			case 5:
				realIndex = PB10;
				break;
			case 6:
				realIndex = PB12;
				break;
			case 7:
				realIndex = PB13;
				break;
			case 8:
				realIndex = PB14;
				break;
			case 9:
				realIndex = PB15;
				break;
		}
		volatile uint32_t* odr = &(gpio->odr);

		*odr ^= 1<<realIndex;
}

/**
 * LedBarWrite()
 * Description:
 * 		Write a pattern to the Led Bar
 * 	Inputs:
 * 		pattern - the 10 least significant bit are
 * 						written to the lights
 * 	Outputs: None
 */
void LedBarWrite(uint16_t pattern) {
	//clear everything on bar
	volatile uint32_t* odr = &(gpio->odr);
	//clear ODR
	*odr &= ~(0x7BF<<5);

	//Take bottom 6 bits
	uint16_t temp = pattern & 0x3F;
	*odr |= (temp<<5);

	//Take the last 4 upper bits
	temp = pattern & 0x3C0;
	*odr |= (temp<<6);
}

/**
 * LedBarRead()
 * Description:
 * 		Read the current from Led Bar
 * 	Inputs: None
 * 	Return: pattern - the 10 least significant bits are
 * 						current light patterns
 */
uint16_t LedBarRead() {

	volatile uint32_t* odr = &(gpio->odr);

	//copy of ODR
	uint16_t copy = *odr;


	//bottom 6 bits of pattern
	uint16_t bottom = copy & (0x3F<<5);

	//Upper 4 Bits
	//must avoid bit 11 - reserved
	uint16_t upper = *odr & (0xF<<12);

	uint16_t built = bottom>>5;
	upper = upper>>6;

	built = built | upper;

	return built;

}

/**
 * LedBarRollRight()
 * Description:
 * 		Shift the pattern one bit the right.
 * 		LSBit becomes MSBit.
 * 	Inputs: None
 * 	Outputs: None
 */
void LedBarRollRight() {
	//pattern is 10 bits
	
	uint16_t pattern = LedBarRead();
	
	uint16_t lsb = pattern & 0x1;

	pattern = pattern>>1;

	//clear spot for msb
	pattern &= ~(0x1<<9);

	pattern |= (lsb<<9);

	LedBarWrite(pattern);

}

/**
 * LedBarRollLeft()
 * Description:
 * 		Shift the pattern one bit the left.
 * 		MSBit becomes LSBit.
 * 	Inputs: None
 * 	Outputs: None
 */
void LedBarRollLeft(){
    //pattern is 10 bits
    uint16_t pattern = LedBarRead();

    uint16_t msb = pattern & 0x200;

    pattern = pattern<<1;

    //clear spot for lsb
    pattern &= ~(0x1);

    pattern |= (msb>>9);

    LedBarWrite(pattern);
}




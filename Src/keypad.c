/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 8 Keypad Interrupt
 * File: keypad.c
 * Dependencies: keypad.h, gpio.h, systick.h, interrupt.h, tim.h
 */

#include "keypad.h"
#include "gpio.h"
#include "systick.h"
#include "stdint.h"
#include "interrupt.h"
#include "tim.h"
#include <stdbool.h>
#define RCC_APB1_ENR 0x40023840

char hexValues[] = "123A456B789C*0#D";
uint32_t tones[] = { 175, 185, 196, 208, 220, 233, 247, 261, 277, 294, 311, 330, 350, 370, 392, 415 };

static volatile EXTI_t *exti = (volatile EXTI_t *)EXTI_BASE;
static volatile GPIO_t *gpioc = (GPIO_t *)GPIOC_BASE;
static volatile TIMx_t *tim2 = (volatile TIMx_t *)TIM2_BASE;
uint8_t debounce = 0;
uint8_t firedCol = 0;
uint32_t old_idr = 0;
volatile uint8_t prevRising = 0;
volatile uint8_t rising = 0;
char prevKey = ' ';


/**
 * @brief  Initializes the keypad
 * @param  None
 * @retval None
 */
void keypadInit(void){
	GPIO_t *gpioc = (GPIO_t *)GPIOC_BASE;
	//set up the GPIO pins
	volatile uint32_t *rcc = (volatile uint32_t *)RCC_AHB1_ENR;

	*rcc |= GPIOC_EN;

	uint32_t reg = 0;

	//set all COLS and ROWS to be pull up resistor (active low)
	reg = gpioc->pupdr;
	reg &= ~(0xFFFF);
	reg |= 0x5555;
	gpioc->pupdr = reg;


	//cols are input, rows are output
	//PC0-3 are cols, PC4-7 are rows
	//01 -output, 00 - input
	reg = gpioc->moder;
	reg &= ~(0xFFFF);
	reg |= 0x5500;
	gpioc->moder = reg;

	//set all rows to 0
	gpioc->odr &= ~(0xF0);

	//sys config setup
	rcc = (volatile uint32_t *)RCC_APB2ENR;
	*rcc = (1 << SYSCFGEN);

	volatile SYSCFG_t *syscfg = (volatile SYSCFG_t *)SYSCFG_BASE;
	//EXT 0..3 all point to port C
	uint32_t temp = syscfg->extcr1;
	temp &= ~(0xFFFF);      //clear bits for ext 3..0
	temp |= 0x2222;         //set bits for ext 3..0 (port C)
	syscfg->extcr1 = temp;

	//EXTI setup
	volatile EXTI_t *exti = (volatile EXTI_t *)EXTI_BASE;
	//Activate on falling and rising edges
	exti->rtsr |= 0xF;      //rising edge
	exti->ftsr |= 0xF;      //falling edge
	//Enabled in IMR (3..0)
	exti->imr |= 0xF;       //all 4 enabled

	//NVIC setup
	volatile uint32_t *nvic_iser = (volatile uint32_t *)0xE000E100;
	*nvic_iser = (0xF << 6);  //EXTI0 is position 6 in ISER0, up to EXTI3 at pos. 9

	//Tim2 used for debounce(5ms)
	//Enable clock for TIM2
	rcc = (volatile uint32_t *)RCC_APB1_ENR;
	*rcc |= (1 << TIM2_EN);

	//Set up TIM2
	TIMx_t *tim2 = (TIMx_t *)TIM2_BASE;
	tim2->CR1 = 0;          //clear control register
	tim2->CR2 = 0;          //clear control register 2

	//prescalar and arr for 10ms
	tim2->PSC = 16000 - 1;  //prescalar
	tim2->ARR = 5 - 1;      //auto reload register

	tim2->DIER = 1;         //enable interrupt
	tim2->SR = 0;           //clear status register

	//TIM2 NVIC setup
	//position 28 in ISER1
	*nvic_iser |= (1 << 28);

	//timer is ready for when interrupt uses it
}


//change note method
void displayNote(uint16_t keyDec) {

	//write to bottom line of LCD based on keyDec
	LcdSetPosition(1, 0);
	LcdWriteStr("       ");
	LcdSetPosition(1, 0);

	//Note String
	char noteString[2];
	//case statement for note
	switch (keyDec) {
	case 0:
		sprintf(noteString, "F");
		break;
	case 1:
		sprintf(noteString, "F#");
		break;
	case 2:
		sprintf(noteString, "G");
		break;
	case 3:
		sprintf(noteString, "G#");
		break;
	case 4:
		sprintf(noteString, "A");
		break;
	case 5:
		sprintf(noteString, "A#");
		break;
	case 6:
		sprintf(noteString, "B");
		break;
	case 7:
		sprintf(noteString, "C");
		break;
	case 8:
		sprintf(noteString, "C#");
		break;
	case 9:
		sprintf(noteString, "D");
		break;
	case 10:
		sprintf(noteString, "D#");
		break;
	case 11:
		sprintf(noteString, "E");
		break;
	case 12:
		sprintf(noteString, "F");
		break;
	case 13:
		sprintf(noteString, "F#");
		break;
	case 14:
		sprintf(noteString, "G");
		break;
	case 15:
		sprintf(noteString, "G#");
		break;
	}

	LcdWriteStr(noteString);
	//write freq after note value using sprintf
	char freq[5];
	sprintf(freq, " %d", tones[keyDec]);
	LcdWriteStr(freq);


}


void buttonPressed(uint8_t col, uint32_t passedIDR, bool *updateScreen, uint16_t *keyPassed) {
	//this func is called by ISR when button is pressed

	//turn off exti interrupts
	exti->imr &= ~(0xF);
	//clear pending
	exti->pr |= 1 << col;

	if (debounce == 0) {
		debounce = 1;

		old_idr = 0;
		old_idr = passedIDR & 0xF;

		
		//prevRising is used for checking later on
		prevRising = rising;

		if (rising) {
			rising = false;
		}

		if (old_idr == 0xF) {
			rising = false;
		} else {
			if (!rising)
				rising = true;
		}

		//Now debounce
		//start timer
		tim2->CR1 |= 1;


		firedCol = col;
	} else if (debounce == 1) {
		debounce = 0;

		//flip inputs to outputs and outputs to inputs
		//copy old IDR to ODR
		uint32_t temp_odr = gpioc->odr;
		temp_odr &= ~(0xF);
		temp_odr |= old_idr;
		gpioc->odr = temp_odr;


		uint32_t temp_moder = gpioc->moder;
		temp_moder &= ~(0xFFFF);
		temp_moder |= 0x55;
		gpioc->moder = temp_moder;

		BusyMilli(1);

		//read IDR for row
		uint32_t curr_idr = gpioc->idr & 0xF0;
		uint32_t row = (curr_idr >> 4);

		uint32_t reg = gpioc->moder;
		reg &= ~(0xFFFF);
		reg |= 0x5500;
		gpioc->moder = reg;

		BusyMilli(2);

		//set all rows to 0
		gpioc->odr &= ~(0xF0);

		uint8_t firedRow = 0;

		//find which row based on position 1 in row var
		if (rising) {
			//find which row based on position 1 in row var
			for (int i = 0; i < 4; i++) {
				if ((row & (1 << i)) == 0) {
					firedRow = i;
					break;
				}
			}
		}

		uint16_t keyDec = (firedRow * 4) + firedCol;
		char keyChar = ' ';
		if (rising) {
			keyChar = hexValues[keyDec];
			prevKey = keyChar;
		} else {
			keyChar = prevKey;
		}
		prevKey = keyChar;


		//play tone if rising with keyDec (change freq first)
		if (rising) {
			changeFreq(tones[keyDec]);
			playTone();
			*keyPassed = keyDec;
			*updateScreen = true;
		} else {
			pauseTone();
		}

		//Sometimes a rising tone may lose its falling tone because of how the button was pushed weird
		//or something so this clean those scenarios
		if (prevRising == rising)
			pauseTone();

	}

	//after either branch, re-enable interupts
	exti->imr |= 0xF;
	//clear pending
	exti->pr |= 0xF;

}

/**
 * Name:
 * Course: CPE2610
 * Section:
 * Assignment: Lab11
 * File: lab11.c
 * Dependencies:
 * Usage: Prints position of control stick to the LCD Display
*/


#include <stdbool.h>
#include "../Inc/adc.h"
#include "../Inc/gpio.h"
#include "../Inc/uart_driver.h"
#include <stdio.h>
#define PC0 0
#define PC1 1

#define F_CPU 16000000UL


static volatile GPIO_t* gpioc = (GPIO_t*) GPIOC_BASE;
static volatile ADC_t* adc1 = (ADC_t*) ADC1_BASE;


int main(void){
  init_usart2(57600, F_CPU);
	///ADC USAGE Steps

	///Step 1 - Enable ADC1 and GPIOC in RCC
	volatile uint32_t *adc_rcc = (volatile uint32_t *)RCC_APB2ENR;
	*adc_rcc |= (1 << ADC1EN);

	volatile uint32_t *gpio_rcc = (volatile uint32_t *)RCC_AHB1_ENR;
	*adc_rcc |= (1 << GPIOC_EN);

	///Step 2 - Set PC0 and PC1 to Analog Mode
	uint32_t reg = gpioc->moder;
	gpioc->moder = ~(CLEAR_MODE << (2 * PC0));
	gpioc->moder = (ANALOG_MODE << (2 * PC0));
	gpioc->moder = ~(CLEAR_MODE << (2 * PC1));
	gpioc->moder = (ANALOG_MODE << (2 * PC1));


	//maybe use pin 11 instead of 15!!!!!! for click button

	///Step 3 - Set number of samples(L) in SQR1
	adc1->sqr[0] |= (0b1 << SAMPLES);

	//Step 4 - Set Channel(s) in SQRN registers
	//PC0 - Channel 10
  //PC1 - Channel 11
	adc1->sqr[2] |= ((10 << SQ1) | (11 << SQ2));
	
	///Step 5 - Configure ADC
	///  CR1
	///     EOCIE - Are you using interrupts?
	///		SCAN - Are you scanning channels?
	uint32_t controlReg = adc1->cr1;
	controlReg |= (0b1 << EOCIE);
	adc1->cr1 = controlReg;

	///  CR2
	///		CONT - Are you continuously scanning?
	adc1->cr2 |= (1 << CONT);
	///		ADON - Requires step to start
	adc1->cr2 |= (1 << ADON);


	///Step 6
	/// When ready to sample
	///		Write 1 -> SWSTART
	///	   Then wait for EOC flag to go to 1 (if not using interrupts)
	///		Read DR register (Note this clears the flag)
	adc1->cr2 |= (1 << SWSTART);

	volatile uint32_t* nvic_iser = (volatile uint32_t*) 0xE000E100;
	*nvic_iser = (0b1 << 18);  //18th position in vector table

	//never return loop (safety)
	while(1);

	return 0;
}

//ISR for ADC
void ADC_IRQHandler(void) {
	uint32_t data = adc1->dr;

	printf("value: %d", data);

	//turn off ADC
	adc1->cr2 &= ~(1 << ADON);
}







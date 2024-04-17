/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 6 Hex Game(with time!)
 * File: tim.h
 * Dependencies: stdint
 */

#include "piezo.h"
#include "tim.h"
#include "gpio.h"
#include "systick.h"
#include <stdint.h>
#define SYSTICK_BASE 0xE000E010
#define AHB8_CLOCK 2000000
#define CC1S_OFFSET 0
#define OC1M_OFFSET 4
#define TOGGLE_MODE 0b011
#define CC1E 0
#define CEN 0

static volatile systick_t *systick = (volatile systick_t *)SYSTICK_BASE;
static volatile TIMx_t *tim3 = (volatile TIMx_t *)TIM3_BASE;

void PiezoInit(void){
//Pin PB4 is peizo buzzer
//general purpose timers run at 16MHz
//Use TIM3 to generate pulse purpose
//Use SysTick for length of note

	//Enable clock for GPIOB
	volatile uint32_t *rcc = (uint32_t *)RCC_AHB1_ENR;

	*rcc |= (1 << GPIOB_EN);

	//Enable clock for TIM3
	volatile uint32_t *rcc2 = (uint32_t *)RCC_APB1_ENR;
	*rcc2 |= (1 << TIM3_EN);

	//Set PB4 is AF2 so it can take the output of TIM3 ch1
	GPIO_t *gpiob = (GPIO_t*) GPIOB_BASE;
	gpiob->moder &= ~(0b11<<8);
	gpiob->moder |= (0b10<<8);
	gpiob->afrl &= ~(0b1111<<16);
	gpiob->afrl |= (0b0010<<16);

	//default freq is 440Hz
	changeFreq(440);
}

void playTone(void){
	//Turn off for setup
	tim3->CR1 = 0;

	//Set the prescalar
	//Find Prescalar to get 1 uS per tick
	//16MHz -> 62.5ns
	// 1uS/62.5ns = 16
	tim3->PSC = 15;
	tim3->EGR |= (1 << UG);

	//Enable the interrupt
	tim3->DIER |= (1 << UIE);

	//clear flag
	tim3->SR = ~(1 << UIF);

	//tim3->ARR = 1910;
	tim3->CNT = 0;

	uint32_t temp = tim3->CCMR1;
	//CC1S = 00 Output Compare Mode
	temp &= ~(0b11<<CC1S_OFFSET);
	//OC1M = 011 Toggle Mode
	temp &= ~(0b111<<OC1M_OFFSET);
	temp |= TOGGLE_MODE<<OC1M_OFFSET;
	tim3->CCMR1 = temp;

	//Enable Output Compare Function
	tim3->CCER |= 1<<CC1E;

	//Turn on Counter
	tim3->CR1 = 1<<CEN;


	//Turn on counting continuously while updating

	tim3->CR1 = ((1 << CEN) | (0 << UDIS) | (0 << DIR) | (0 << OPM));
}

void pauseTone(void){
	//Turn off timer

	tim3->CR1 = 0;
}

changeFreq(uint32_t newFreq){
	float freqFloat = (float)newFreq;
	//printf("note float: %f\n", freqFloat);
	float note = (1 / freqFloat) / 2;

	//printf("note1: %f\n", note);
	note = note * 1000000;
	//printf("note2: %f\n", note);
	note--;
	//printf("note3: %f\n", note);
	note = (int)note;
	//printf("note4: %f\n", note);
	tim3->ARR = note;

	//tim3->ARR = 1910;
	tim3->CNT = 0;
}

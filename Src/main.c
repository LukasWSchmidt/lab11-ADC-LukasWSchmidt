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
#include "../Inc/systick.h"
#include "../Inc/interrupt.h"
#include "../Inc/lcd.h"
#include <string.h>
#include <stdio.h>
#define PC0 0
#define PC1 1
#define PC11 11
#define EXTI11 11

#define F_CPU 16000000UL


static volatile GPIO_t* gpioc = (GPIO_t*) GPIOC_BASE;
static volatile ADC_t* adc1 = (ADC_t*) ADC1_BASE;
volatile uint32_t* nvic_iser0 = (volatile uint32_t*) 0xE000E100;
volatile uint32_t* nvic_iser1 = (volatile uint32_t*) 0xE000E104;
volatile EXTI_t *exti = (volatile EXTI_t *)EXTI_BASE;

static uint8_t channel_num = 0;
int center_value = 2048;
uint16_t x_data = 0;
uint16_t y_data = 0;
uint8_t converted = 0;
bool stickPressed = false;
bool updateDisplay = true;

void calculate_percentage_and_direction(int data, char *result, bool isYVal);

int main(void){
  init_usart2(57600, F_CPU);
	///ADC USAGE Steps

	//LCD setup & Init
	LcdSetup();
	DelaySysMilliSec(100);
	LcdInit();
	LcdClear();
	LcdSetPosition(0, 0);


	///Step 1 - Enable ADC1 and GPIOC in RCC
	volatile uint32_t *adc_rcc = (volatile uint32_t *)RCC_APB2ENR;
	*adc_rcc |= (1 << ADC1EN);

	volatile uint32_t *gpio_rcc = (volatile uint32_t *)RCC_AHB1_ENR;
	*gpio_rcc |= (1 << GPIOC_EN);

	///Step 2 - Set PC0 and PC1 to Analog Mode
	uint32_t reg = gpioc->moder;
	reg &= ~(CLEAR_MODE << (2 * PC0));
	reg |= (ANALOG_MODE << (2 * PC0));
	reg &= ~(CLEAR_MODE << (2 * PC1));
	reg |= (ANALOG_MODE << (2 * PC1));
	gpioc->moder = reg;


	//maybe use pin 11 instead of 15!!!!!! for click button

	///Step 3 - Set number of samples(L) in SQR1
	adc1->sqr[0] |= (0b0 << SAMPLES);

	//Step 4 - Set Channel(s) in SQRN registers
	//PC0 - Channel 10
  //PC1 - Channel 11
	adc1->sqr[2] |= (10 << SQ1);
	
	///Step 5 - Configure ADC
	///  CR1
	///     EOCIE - Are you using interrupts?
	///		SCAN - Are you scanning channels?
	uint32_t controlReg = adc1->cr1;
	controlReg |= (0b1 << EOCIE);
	adc1->cr1 = controlReg;

	///  CR2
	///		CONT - Are you continuously scanning?
	//adc1->cr2 |= (1 << CONT);
	///		ADON - Requires step to start
	adc1->cr2 |= (1 << ADON);




	//configure push button
	reg = gpioc->moder;
	reg &= ~(CLEAR_MODE << (2 * PC11));
	reg |= (INPUT_MODE << (2 * PC11));
	gpioc->moder = reg;
	gpioc->pupdr &= ~(0b11 << (2 * PC11));
	gpioc->pupdr |= (0b01 << (2 * PC11));

	//configure interrupt for push
	//sys config setup
	volatile uint32_t * syscfg_rcc = (volatile uint32_t *)RCC_APB2ENR;
	*syscfg_rcc |= (1 << SYSCFGEN);

	volatile SYSCFG_t *syscfg = (volatile SYSCFG_t *)SYSCFG_BASE;
	//EXT 0..3 all point to port C
	uint32_t temp = syscfg->extcr3;
	temp &= ~(0xF << 12);
	temp |= (0b0010 << 12);         //set bits for port C
	syscfg->extcr3 = temp;

	//EXTI setup
	
	//Activate on rising edges
	exti->rtsr |= (0b1 << EXTI11);      //rising edge

	//Enabled in IMR (3..0)
	exti->imr |= (0b1 << EXTI11);       //enabled

	exti->pr |= (1 << EXTI11);
	//enable in table
	*nvic_iser1 = (0b1 << (40-32));  //40th position in vector table
	



	///Step 6
	/// When ready to sample
	///		Write 1 -> SWSTART
	///	   Then wait for EOC flag to go to 1 (if not using interrupts)
	///		Read DR register (Note this clears the flag)
	adc1->cr2 |= (1 << SWSTART);

	//printf("connected");

	*nvic_iser0 = (0b1 << 18);  //18th position in vector table




	while(1){
		if(stickPressed) {
				stickPressed = 0;
				updateDisplay = !updateDisplay;
  			printf("pressed button\n");
  			//wait debounce and then reenable
  			DelaySysMilliSec(125);
				exti->imr |= (0b1 << EXTI11);       //enabled
		}

		if(converted == 1 && updateDisplay){

			char x_result[16];
    		char y_result[16];

    		calculate_percentage_and_direction(x_data, x_result, false);
    		calculate_percentage_and_direction(y_data, y_result, true);

			LcdSetPosition(0,0);
			LcdWriteStr(x_result);
			LcdSetPosition(1, 0);
			LcdWriteStr(y_result);
			converted = 0;
			DelaySysMilliSec(50);
			adc1->cr2 |= (1 << SWSTART);
		}
	}

	return 0;
}

// ISR for ADC
void ADC_IRQHandler(void) {
//disable the interrupt
adc1->cr1 &= ~(1 << EOCIE);

//printf("interrupt Fired\n");


adc1->sr &= ~(1 << STRT);
adc1->sr &= ~(1 << EOC);

// Read the data register
uint32_t data = adc1->dr;

// printf("data: %d\n", data);

 channel_num++;
   if (channel_num > 1) {
       channel_num = 0;
   }

//switch channel

//put data into x_data or y_data
if(channel_num == 0){
	x_data = data;
	//printf("x data: %d\n", x_data);

	adc1->sqr[2] &= ~(11 << SQ1);
	adc1->sqr[2] |= (10 << SQ1); //Channel 11

	converted = 1;
} else{
	y_data = data;
	//printf("y data: %d\n", y_data);

	adc1->sqr[2] &= ~(11 << SQ1);
	adc1->sqr[2] |= (11 << SQ1); //Channel 10

	adc1->cr2 |= (1 << SWSTART);
}

//enable the interrupt & start conversions
adc1->cr1 |= (1 << EOCIE);



}

void EXTI15_10_IRQHandler(void) {
	//clear pending
	//turn off exti interrupts
	if (exti->pr & (1 << EXTI11)) {
	       exti->imr &= ~(0xF << EXTI11);
		// Clear pending bit for pin 11
	        exti->pr |= (1 << EXTI11);
	        printf("Pin 11 went high\n");

	        stickPressed = true;
	    }
}


// Function to calculate percentage difference and direction
void calculate_percentage_and_direction(int data, char *result, bool isYVal) {
    double percent_diff = ((double)abs(data - center_value) / center_value) * 100;
    sprintf(result, "%d%% ", (int)percent_diff);

	if(!isYVal){
		if (data > center_value) {
			strcat(result, "Right   ");
		} else {
        strcat(result, "Left   ");
		}
	} else {
		if (data > center_value) {
		strcat(result, "Up   ");
		} else {
		strcat(result, "Down   ");
		}
	}
}




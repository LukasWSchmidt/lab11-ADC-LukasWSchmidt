/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 3 Buttons
 * File: push_button.c
 * Dependencies: stdint, gpio, push_button
 */
#include "push_button.h"
#include <stdint.h>
#include "gpio.h"

//#define RCC_AHB1_ENR 0x40023830
//#define GPIOA_EN 0
//#define GPIOB_EN 1
//#define GPIOC_EN 2
#define GPIOA 0x40020000
#define GPIOB 0x40020400
#define GPIOC 0x40020800
#define ENCODE_BUTTON_PIN 12
#define USER_BUTTON_PIN 13
#define STICK_BUTTON_PIN 11


//GPIOs
GPIO_t* gpioa  = (GPIO_t *)GPIOA;
GPIO_t* gpiob  = (GPIO_t *)GPIOB;
GPIO_t* gpioc  = (GPIO_t *)GPIOC;

void GPIOInit() {
    //allow the clock to be enabled
    volatile uint32_t* rcc = (uint32_t*) RCC_AHB1_ENR;
    *rcc |= (1<<GPIOA_EN);
    *rcc |= (1<<GPIOB_EN);
    *rcc |= (1<<GPIOC_EN);
}

// Function to check if a button is pressed
uint8_t is_button_pressed(GPIO_t *gpio, int pin) {
    // Read the input data register and check the specified pin
    return (gpio->idr & (1 << pin)) == 0; //Actuve low
}

// Function to check if the encoder button is pressed
uint8_t is_encode_button_pressed() {
    return is_button_pressed(gpioa, ENCODE_BUTTON_PIN);
}

// Function to check if the user button is pressed
uint8_t is_user_button_pressed() {
    return is_button_pressed(gpioc, USER_BUTTON_PIN);
}

// Function to check if the stick button is pressed
uint8_t is_stick_button_pressed() {
    return is_button_pressed(gpioc, STICK_BUTTON_PIN);
}

//Separate function declarations to initialize the Encode, User, and Control buttons

// Function to initialize the encoder button
void InitEncodeButton() {
    //Encoder push button is Pin A12
    // Set the mode of the pin to input
    gpioa->moder &= ~(0b11 << (2 * ENCODE_BUTTON_PIN));

    // Set the pull-up resistor
    gpioa->pupdr &= ~(0b11 << (2 * ENCODE_BUTTON_PIN));
    gpioa->pupdr |= 0b01 << (2 * ENCODE_BUTTON_PIN);
}

// Function to initialize the user button
void InitUserButton() {
   gpioc->moder &= ~(0b11 << (2 * USER_BUTTON_PIN));
    gpioc->pupdr &= ~(0b11 << (2 * USER_BUTTON_PIN));
    gpioc->pupdr |= 0b01 << (2 * USER_BUTTON_PIN);
}

// Function to initialize the stick button
void InitStickButton() {
    //C11
    gpioc->moder &= ~(0b11 << (2 * STICK_BUTTON_PIN));
    gpioc->pupdr &= ~(0b11 << (2 * STICK_BUTTON_PIN));
    gpioc->pupdr |= 0b01 << (2 * STICK_BUTTON_PIN);
    
}



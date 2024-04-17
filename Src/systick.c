/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 5 Hex Game
 * File: systick.c
 * Dependencies: stdint, systick
 */
#include "systick.h"
#include "stdint.h"

systick_t* systick = (systick_t *)SYSTICK_BASE;

/**
  * @brief  Initializes the systick timer
  * @param  None
  * @retval None
  */
void systick_init(void) {
    //enable the systick timer
    systick->CTRL = 0b1<<ENABLE;
}

/**
  * @brief  Delays the program by a specified number of microseconds
  * @param  N: The number of microseconds to delay the program
  * @retval None
  */
void DelaySysMicroSec(uint32_t N) {
     // Calculate the number of ticks needed
    uint32_t ticks = N * (DEFAULT_CLOCK / 1000000);

    // Load the number of ticks into the SysTick reload value register
    systick->LOAD = ticks;

    // Enable the SysTick timer
    systick->CTRL |= 1<<ENABLE | 1<<CLKSOURCE;

    // Reset the SysTick timer value
    systick->VAL = 0;

    // Wait until the COUNTFLAG is set (indicating the timer has counted down to 0)
    while ((systick->CTRL & 1<<COUNTFLAG) == 0);

    // Disable the SysTick timer
    systick->CTRL |= 0<<ENABLE;
}

/**
  * @brief  Delays the program by a specified number of milliseconds
  * @param  N: The number of milliseconds to delay the program
  * @retval None
  */
void DelaySysMilliSec(uint32_t N) {
    //use ahb8 clock
    // Calculate the number of ticks needed
    uint32_t ticks = N * (AHB8_CLOCK / 1000);

    systick->LOAD = ticks;

    // Enable the SysTick timer
    systick->CTRL |= (1<<ENABLE | 0<<CLKSOURCE);

    systick->VAL = 0;

    while ((systick->CTRL & 1<<COUNTFLAG) == 0);

    systick->CTRL |= 0<<ENABLE;

}

/**
  * @brief  Delays the program by a specified number of seconds
  * @param  N: The number of seconds to delay the program
  * @retval None
  */
void DelaySysSec(uint32_t N) {
  //call milli sec delay(1000) N times in loop
  for (int i = 0; i < N; i++) {
    DelaySysMilliSec(1000);
  }
}

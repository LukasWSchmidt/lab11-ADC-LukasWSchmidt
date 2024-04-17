/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 5 Hex Game
 * File: sysTick.h
 * Dependencies: stdint, stdbool, gpio
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#define SYSTICK_BASE 0xE000E010
#define ENABLE 0
#define COUNTFLAG 16
#define CLKSOURCE 2
#define DEFAULT_CLOCK 16000000
#define AHB8_CLOCK 2000000

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

typedef struct systick_t {
    uint32_t CTRL;
    uint32_t LOAD;
    uint32_t VAL;
    uint32_t CALIB;
} systick_t;

/**
  * @brief  Initializes the systick timer
  * @param  None
  * @retval None
  */
void systick_init(void);

/**
  * @brief  Delays the program by a specified number of microseconds
  * @param  N: The number of microseconds to delay the program
  * @retval None
  */
void DelaySysMicroSec(uint32_t N);

/**
  * @brief  Delays the program by a specified number of milliseconds
  * @param  N: The number of milliseconds to delay the program
  * @retval None
  */
void DelaySysMilliSec(uint32_t N);

/**
  * @brief  Delays the program by a specified number of seconds
  * @param  N: The number of seconds to delay the program
  * @retval None
  */
void DelaySysSec(uint32_t N);

#endif /* LCD_H_ */

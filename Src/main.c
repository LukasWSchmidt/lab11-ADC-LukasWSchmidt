/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 11 - ADC With Controller Stick
 * File: main.c
 * Dependencies:
 * purpose of program:
 */
#include "../Inc/led_array.h"
#include "../Inc/spi.h"
#include "../Inc/systick.h"
#include "../Inc/uart_driver.h"
#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F_CPU 16000000UL

int main(void) {
   init_usart2(57600, F_CPU);

}

  // never return
  for (;;) {
  }

  return (0);
}

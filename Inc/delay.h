/**
  ******************************************************************************
  * @file    delay.h
  * @author  Lukas Schmidt
  * @version V1.0
  * @Usage   Used to reach delay api from main
  * @dependencies stdint.h
  ******************************************************************************
*/

#ifndef DELAY_H_
#define DELAY_H_

#include "stdint.h"

/*
 * Name:BusyMicro
 * Purpose: To delay the program for a specific amount of microseconds
 * Input: uint32_t number that specifies how many microseconds to delay for
 * Output: Void
 */
void BusyMicro(uint32_t N);

/*
 * Name:BusyMilli
 * Purpose: To delay the program for a specific amount of milliseconds
 * Input: uint32_t number that specifies how many milliseconds to delay for
 * Output: Void
 */
void BusyMilli(uint32_t N);

/*
 * Name:BusySec
 * Purpose: To delay the program for a specific amount of seconds
 * Input: uint32_t number that specifies how many seconds to delay for
 * Output: Void
 */
void BusySec(uint32_t N);




#endif /* DELAY_H_ */

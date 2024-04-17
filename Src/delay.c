/**
  ******************************************************************************
  * @file    delay.c
  * @author  Lukas Schmidt
  * @version V1.0
  * @Usage   Used to delay the program by doing NO-OP for certain amount of time
  * @dependencies stdint.h
  ******************************************************************************
*/
#include "delay.h"
#include "stdint.h"


/*
 * Name:BusyMicro
 * Purpose: To delay the program for a specific amount of microseconds
 * Input: uint32_t number that specifies how many microseconds to delay for
 * Output: Void
 */
void BusyMicro(uint32_t N){
	for(uint32_t i = N; i > 0; i--) {
	}
}

/*
 * Name:BusyMilli
 * Purpose: To delay the program for a specific amount of milliseconds
 * Input: uint32_t number that specifies how many milliseconds to delay for
 * Output: Void
 */
void BusyMilli(uint32_t N){
	uint32_t delay = 1605 * N;
	for(uint32_t i = delay; i > 0; i--) {}
}

/*
 * Name:BusySec
 * Purpose: To delay the program for a specific amount of seconds
 * Input: uint32_t number that specifies how many seconds to delay for
 * Output: Void
 */
void BusySec(uint32_t N){
	uint32_t delay = 800000 * N;
	for(uint32_t i = delay; i > 0; i--) {}
}


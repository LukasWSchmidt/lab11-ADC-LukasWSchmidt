/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 6 Hex Game(with time!)
 * File: piezo.h
 * Dependencies: stdint
 */

#ifndef PIEZO_H_
#define PIEZO_H_
#define RCC_APB1_ENR 0x40023840
#define UG 0
#define CEN 0
#define UDIS 1
#define DIR 4
#define OPM 3
#define UIE 0
#define UIF 0
#define PIEZO 4

#include <stdint.h>


/**I am leaving the api open but a method which takes the frequncy(Hz)
 and length of time(ms) to play a tone is probably a good starting point.
An initialization method is probably a good idea as well.*/

void PiezoInit(void);

void playTone();

void pauseTone();

void changeFreq(uint32_t newFreq);


#endif /* PIEZO_H_ */

/*
 * interrupt.h
 *
 *  Created on: Apr 5, 2024
 *      Author: lukasschmidt
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_
#define PORTA 0x0
#define PORTB 0x1
#define PORTC 0x2
#define PORTD 0x3
#define PORTE 0x4

#define SYSCFG_BASE 0x40013800
#define EXTI_BASE 0x40013C00
#define SYSCFGEN 14
#define RCC_APB2ENR 0x40023844

typedef struct SYSCFG_t {
	uint32_t memrmp;
	uint32_t pmc;
	uint32_t extcr1;
	uint32_t extcr2;
	uint32_t extcr3;
	uint32_t extcr4;
	uint32_t cmpcr;
} SYSCFG_t;

typedef struct EXTI_t{
	uint32_t imr;
	uint32_t emr;
	uint32_t rtsr;
	uint32_t ftsr;
	uint32_t swier;
	uint32_t pr;
} EXTI_t;




#endif /* INTERRUPT_H_ */

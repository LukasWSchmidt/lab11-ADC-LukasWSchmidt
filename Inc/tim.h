/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 6 Hex Game(with time!)
 * File: tim.h
 * Dependencies: stdint
 */

#ifndef TIM_H_
#define TIM_H_

#define TIM2_BASE 0x40000000
#define TIM3_BASE 0x40000400
#define TIM4_BASE 0x40000800
#define TIM5_BASE 0x40000C00
//for rcc enabling in APB1
#define TIM2_EN 0
#define TIM3_EN 1
#define TIM4_EN 2
#define TIM5_EN 3

#include <stdint.h>

typedef struct TIMx_t {
    uint32_t CR1;
    uint32_t CR2;
    uint32_t SMCR;
    uint32_t DIER;
    uint32_t SR;
    uint32_t EGR;
    uint32_t CCMR1;
    uint32_t CCMR2;
    uint32_t CCER;
    uint32_t CNT;
    uint32_t PSC;
    uint32_t ARR;
    uint32_t RESERVED0;
    uint32_t CCR1;
    uint32_t CCR2;
    uint32_t CCR3;
    uint32_t CCR4;
    uint32_t RESERVED1;
    uint32_t DCR;
    uint32_t DMAR;
    uint32_t OR;
} TIMx_t;

#endif

/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 10 - Led Matrix
 * File: spi.h
 * Dependencies: gpio, stdint, spi
 * Usage: Used for making SPI enabled for SPI2
 */
#ifndef SPI_H_
#define SPI_H_


#define RCC_SPI1_EN 1<<12
#define RCC_SPI2_EN 1<<14
#define RCC_SPI3_EN 1<<15
#define RCC_SPI4_EN 1<<13
#define SPI1_BASE 0x40013000
#define SPI2_BASE 0x40003800
#define SPI3_BASE 0x40003C00 
#define SPI4_BASE 0x40013400
#define RCC_APB1ENR 0x40023840
#define RCC_APB2ENR 0x40023844
#define BR 3
#define DFF 11
#define MSB 7
#define CPOL 1
#define CPHA 0
#define MSTR 2
#define SSM 9
#define SSI 8
#define SSOE 2
#define SPE 6
#include <stdint.h>

//APB1 RCC enables SPI2 & SPI3,
//APB2 enables SPI1 & SPI4

typedef struct SPI_t {
	uint32_t cr1;
	uint32_t cr2;
	uint32_t sr;
	uint16_t dr;
	uint16_t res1;
	uint32_t crcpr;
	uint32_t rxcrcr;
	uint32_t txcrcr;
	uint32_t i2scfgr;
	uint32_t i2spr;
} SPI_t;

 void SPIInit(void);

 void transmitData(uint16_t data);

#endif /* SPI_H_ */
